#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "./lexer/lexer.h"

void traducteur(lexeme_list* lexemes)
{
    lexeme_list* lex = lexemes; // Le lexeme actuel (passe le premier).
    int parenthese_fonction=-1;
    while (lex->type != LxmEnd)
    {
        
        lex = lex->next; 
        //printf("%d,%s\n",lex->type,lex->content);
        if (lex->type == LxmType){ //Traduction des Types
            if (strcmp(lex->next->content,"main") == 0) { //cas pour int main ou on skip tout
                lex =lex->next->next->next->next;
            }
            else 
            { 
                printf("let ");
            }
            
        }
        else if (lex->type == LxmVariable) { //Traduction des variables
            if (strcmp(lex->content,"printf") == 0) { //cas pour printf
                printf("Printf.printf ");
                lex=lex->next;
                parenthese_fonction=1;
            }
            else{
                if (strcmp(lex->next->content,"=") == 0) { //écriture definition variable
                printf("%s",lex->content);
                }
                else{
                printf("!%s",lex->content); //écriture valeur variable avec !
                }
            }
            
        }
        else if (lex->type == LxmAffectation) { //Traduction affectation de variable
            printf("%s ref ",lex->content);
        }
        else if (lex->type == LxmInt) { //Traduction variable entier
            printf("%s",lex->content);
        }
        else if (lex->type == LxmPunctuation){ //Traduction de fin de ligne
            if (strcmp(lex->content, ";") == 0) {
                printf(";;\n"); //double point virgule en fin de ligne 
            }
            if (strcmp(lex->content,"(") == 0 ) {
                if (parenthese_fonction>0) { 
                    parenthese_fonction=parenthese_fonction+1; //augmente le compteur des parenthèse pour les arguments d'une fonction
                }
                printf("(");
            }
            if (strcmp(lex->content,")") == 0) {
                if (parenthese_fonction == 1) {
                    parenthese_fonction = -1; //cas de la dernière parenthèse que l'on affiche pas à la fin des arguments d'une fonction 
                }
                else{
                    if (parenthese_fonction>0) {
                    parenthese_fonction=parenthese_fonction-1; //diminue le compteur des parenthèse en argument
                    }
                    printf(")");
                }
            }

        }
        else if (lex->type == LxmOperator) { 
            printf("%s",lex->content );
        }
        else if(lex->type == LxmComment) {
            if (strcmp(lex->content,"//") == 0 || strcmp(lex->content,"/*") == 0) { //cas parenthèse ouvrante
                printf("(* ");
            }
            else{
                printf("*) \n"); //cas parenthèse fermante
            }
        }
        else if(lex->type == LxmString) {
            printf("%s ",lex->content);
        }
        /*else if(lex->type == LxmFunction) {

        }*/
    }
}

int main(int argc, char* argv[])
{
<<<<<<< HEAD
    // Cree la liste de lexemes du fichier
    //FILE* source_file = fopen("./test.c", "r");
    FILE* source_file = fopen("./tests/etape2/etape2.c", "r");
    lexeme_list* l = lexeur(source_file);
=======
    if (argc == 1)
    { // On execute notre fichier de test.
        FILE* source_file = fopen("./test.c", "r");
        lexeme_list* l = lexeur(source_file);
>>>>>>> dd0c636 (Ajout d'une base pour la sélection du fichier à traduire.)

        printf("\n===\n");

        print_list(l);
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
