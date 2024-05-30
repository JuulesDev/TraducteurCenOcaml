#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "./lexer/lexer.h"

/*
    Affiche l'indentation du code.

    Argument:
        int depth: La profondeur de l'indentation.
*/
void indentation(int depth)
{
    for (int i = 0; i < depth; i += 1)
    {
        printf("    ");
    }
}

void traducteur(lexeme_list* lexemes)
{
    lexeme_list* lex = lexemes; // Le lexeme actuel (passe le premier).
    int compteur_parenthese = 0; // Nombre de parenthèses après une fonction 

    // Pile indiquant le bloc dans lequel on se trouve : 0 -> while/for, 1 -> if
    int* pile_bloc = malloc(sizeof(int)*20);
    int indice_pile_bloc = 0;

    while (lex->type != LxmEnd)
    {    
        lex = lex->next;
        // printf("-> %d,%s\n",lex->type,lex->content);
        
        if (lex->type == LxmType){
            if (strcmp(lex->next->content, "main") == 0) {
                // On écrit le contenu du main dans le fichier directement
                lex = lex->next->next->next->next; 
            } else {
                // Cas de première assignation d'une variable, on gère le cas jusqu'au =
                // Pour que le cas de base soit une réassignation
                lex = lex->next;
                char* var_name = lex->content;
                lex = lex->next;

                indentation(indice_pile_bloc);
                printf("let %s = ref ", var_name); // ATTENTION: CAS AVEC OPERATION BUG 
            }            
        } else if (lex->type == LxmVariable)
        {
            if (strcmp(lex->next->content, "=") == 0)
            {
                // Cas de ré-assignation de variable
                indentation(indice_pile_bloc);
                printf("%s", lex->content);
            } else {
                // On veut la valeur d'une variable, toutes sont des ref
                printf("!%s", lex->content);
            }            
        }
        else if (lex->type == LxmAffectation)
        {
            // Forcèment une réassignation (autre cas déjà gérée)
            printf(" := ");
        }
        else if (lex->type == LxmInt)
        {
            printf("%s", lex->content);
        }
        else if(lex->type == LxmString) {
            printf("\"%s\"", lex->content);
        }
        else if (lex->type == LxmPunctuation)
        { // Traduction de fin de ligne
            if (strcmp(lex->content, ",") == 0)
            {
                // Entre les arguments d'une fonction
                printf(" ");
            } else if (strcmp(lex->content, ";") == 0)
            {
                if (indice_pile_bloc > 0)
                {
                    printf(";\n"); // Dans un bloc
                } else {
                    printf(";;\n"); // Hors d'un bloc
                }
            } else if (strcmp(lex->content,"(") == 0 )
            {
                compteur_parenthese += 1; //augmente le compteur des parenthèse pour les arguments d'une fonction
                printf("(");
            } else if (strcmp(lex->content,")") == 0)
            {
                if (compteur_parenthese != 0)
                {
                    compteur_parenthese -= 1; //diminue le compteur des parenthèse en argument
                    printf(")");
                }
            } else if (strcmp(lex->content,"{") == 0)
            { 
                if (pile_bloc[indice_pile_bloc] == 0) { // si la dernière fonction utilisée est une boucle 
                    printf("do \n");
                    indice_pile_bloc += 1;
                } else { // si la dernière fonction utilisée est un condition 
                    printf(" then begin\n");
                    indice_pile_bloc += 1;
                }
            } else if (strcmp(lex->content,"}")==0)
            {
                if (indice_pile_bloc>0){
                    if (pile_bloc[indice_pile_bloc-1]==0){ // si la dernière fonction utilisée est une boucle 
                        printf("done\n");
                        indice_pile_bloc -= 1;
                    } else { // si la dernière fonction utilisée est un condtion
                        indice_pile_bloc -= 1;
                        indentation(indice_pile_bloc); 
                        printf("end\n");
                    }
                }
            }
        }
        else if (lex->type == LxmOperator) {
            // Cas spéciaux si les opérateurs ne sont pas les mêmes
            if (!strcmp(lex->content, "=="))
            {
                printf(" = ");
            } else {
                // Cas de base
                printf(" %s ", lex->content);   
            }
        }
        else if(lex->type == LxmComment) { //cas pour les commentaires
            indentation(indice_pile_bloc);
            printf("(*");
            printf("%s", lex->content);
            printf("*)\n");
        }
        else if(lex->type == LxmKeyWord)
        {
            if (strcmp(lex->content, "printf") == 0)
            { //cas pour printf
                indentation(indice_pile_bloc);
                printf("Printf.printf ");
                lex=lex->next; //on passe pour passer la parenthèse
                compteur_parenthese = 0; //initialisation du compteur de parenthèse après une fonction à 0
            }
            else
            {
                indentation(indice_pile_bloc);
                printf("%s ",lex->content); //Cas des mots clés pour les boucles
                if (strcmp(lex->content, "while") == 0)
                {
                    pile_bloc[indice_pile_bloc] = 0; //ajout d'une boucle while dans la pile
                }
                if (strcmp(lex->content, "if") == 0)
                {
                    pile_bloc[indice_pile_bloc] = 1; //ajout d'une condition dans la pile 
                }
            }
        }
    }

    // Au pire ça change rien, au mieux on oublie pas le ;;, a changer
    printf(";;\n");
}

int main(int argc, char* argv[])
{
    if (argc == 1)
    { // On execute notre fichier de test.
        FILE* source_file = fopen("./tests/etape1.c", "r");
        lexeme_list* l = lexeur(source_file);

        printf("\n===\n");

        print_list(l);

        printf("\n= CODE TRADUIT =\n\n");

        traducteur(l);

        free_list(l);
    } else if (argc == 2)
    { // On execute le fichier spécifié.
        FILE* source_file = fopen(argv[1], "r");
        lexeme_list* l = lexeur(source_file);
        traducteur(l);
        free_list(l);
    } else {
        printf("Erreur: trop d'arguments. Il faut 0 ou 1 argument (le chemin du fichier à traduire).\n");
    }
}
