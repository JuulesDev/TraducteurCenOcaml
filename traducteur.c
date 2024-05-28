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
    int compteur_parenthese = -1; // Nombre de parenthèses après une fonction 

    // Pile indiquant le bloc dans lequel on se trouve : 0 -> while/for, 1 -> if
    int* pile_bloc = malloc(sizeof(int)*20);
    int indice_pile_bloc = 0;

    while (lex->type != LxmEnd)
    {    
        lex = lex->next;
        
        // printf("-> %d,%s\n",lex->type,lex->content);
        if (lex->type == LxmType){ //Traduction des Types
            if (strcmp(lex->next->content, "main") == 0) { //cas pour int main ou on skip tout
                lex = lex->next->next->next->next;
            }
            else 
            {
                indentation(indice_pile_bloc);
                printf("let "); //cas pour création d'une variable
            }
            
        }
        else if (lex->type == LxmVariable)
        { //Traduction des variables
            if (strcmp(lex->next->content, "=") == 0)
            { //écriture definition variable
                indentation(indice_pile_bloc);
                printf("%s", lex->content);
            } else {
                printf("!%s", lex->content); //écriture valeur variable avec !
            }
            
        }
        else if (lex->type == LxmAffectation)
        { //Traduction affectation de variable
            printf(" = ref ");
        }
        else if (lex->type == LxmInt)
        { //Traduction variable entier
            printf("%s", lex->content);
        }
        else if (lex->type == LxmPunctuation)
        { // Traduction de fin de ligne
            if (strcmp(lex->content, ";") == 0)
            {
                if (indice_pile_bloc > 0)
                {
                    printf(";\n"); // Dans un bloc
                } else {
                    printf(";;\n"); // Hors d'un bloc
                }
            } else if (strcmp(lex->content,"(") == 0 )
            {
                if (compteur_parenthese>0) { 
                    compteur_parenthese += 1; //augmente le compteur des parenthèse pour les arguments d'une fonction
                }
                printf("(");
            } else if (strcmp(lex->content,")") == 0)
            {
                if (compteur_parenthese == 0)
                {
                    compteur_parenthese = -1; //cas de la dernière parenthèse que l'on affiche pas à la fin des arguments d'une fonction 
                } else {
                    if (compteur_parenthese>0)
                    {
                        compteur_parenthese -= 1; //diminue le compteur des parenthèse en argument
                    }
                    printf(")");
                }
            } else if (strcmp(lex->content,"{") == 0)
            { 
                if (pile_bloc[indice_pile_bloc] == 0) { // si la dernière fonction utilisée est une boucle 
                    printf("do \n");
                    indice_pile_bloc += 1; //augmente le nombre d'itérateurs
                } else { // si la dernière fonction utilisée est un condtion 
                    printf(" (\n");
                    indice_pile_bloc += 1; //augmente le nombre d'itérateurs
                }
            } else if (strcmp(lex->content,"}")==0)
            {
                if (indice_pile_bloc>0){
                    if (pile_bloc[indice_pile_bloc-1]==0){ // si la dernière fonction utilisée est une boucle 
                        printf("done\n");
                        indice_pile_bloc-=1; //diminue le nombre d'itérateurs
                    } else { // si la dernière fonction utilisée est un condtion
                        indice_pile_bloc -= 1; //diminue le nombre d'itérateurs
                        indentation(indice_pile_bloc); 
                        printf(")\n");
                    }
                }
            }
        }
        else if (lex->type == LxmOperator) { //cas pour les opérateurs 
            printf("%s", lex->content);
        }
        else if(lex->type == LxmComment) { //cas pour les commentaires
            indentation(indice_pile_bloc);
            printf("(*");
            printf("%s", lex->content);
            printf("*)\n");
        }
        else if(lex->type == LxmString) {
            printf("\"%s\" ", lex->content);
        }
        else if(lex->type == LxmKeyWord)
        {
            if (strcmp(lex->content, "printf") == 0)
            { //cas pour printf
                indentation(indice_pile_bloc);
                printf("Printf.printf ");
                lex=lex->next; //on passe pour passer la parenthèse
                compteur_parenthese=0; //initialisation du compteur de parenthèse après une fonction à 0
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
}

int main(int argc, char* argv[])
{
    if (argc == 1)
    { // On execute notre fichier de test.
        FILE* source_file = fopen("./tests/etape4.c", "r");
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
