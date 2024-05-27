#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "./lexer/lexer.h"

void indentation(int indice){ //fonction pour print l'indentation en fonction de while/ condition 
    for (int i=0; i< indice;i+=1){
            printf("   ");
        }
}
void traducteur(lexeme_list* lexemes)
{
    lexeme_list* lex = lexemes; // Le lexeme actuel (passe le premier).
    int parenthese_fonction=-1; //nombre de parenthèse après une fonction 
    int* liste_itérateur=malloc(sizeof(int)*20); // liste pour savoir l'ordre des boucles et conditions 0:boucle 1:condition
    int indice_liste_itérateur=0;
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
                printf("let "); //cas pour création d'une variable
            }
            
        }
        else if (lex->type == LxmVariable) { //Traduction des variables
                if (strcmp(lex->next->content,"=") == 0) { //écriture definition variable
                printf("%s",lex->content);
                }
                else{
                printf("!%s",lex->content); //écriture valeur variable avec !
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
                indentation(indice_liste_itérateur); //appel de l'indentation après \n
            }
            if (strcmp(lex->content,"(") == 0 ) {
                if (parenthese_fonction>0) { 
                    parenthese_fonction=parenthese_fonction+1; //augmente le compteur des parenthèse pour les arguments d'une fonction
                }
                printf("(");
            }
            if (strcmp(lex->content,")") == 0) {
                if (parenthese_fonction == 0) {
                    parenthese_fonction = -1; //cas de la dernière parenthèse que l'on affiche pas à la fin des arguments d'une fonction 
                }
                else{
                    if (parenthese_fonction>0) {
                    parenthese_fonction=parenthese_fonction-1; //diminue le compteur des parenthèse en argument
                    }
                    printf(")");
                }
            }
            if (strcmp(lex->content,"{")==0){ 
                if (liste_itérateur[indice_liste_itérateur]==0){ // si la dernière fonction utilisée est une boucle 
                    printf("do \n");
                    indice_liste_itérateur+=1; //augmente le nombre d'itérateurs
                    indentation(indice_liste_itérateur);
                    }
                else{ // si la dernière fonction utilisée est un condtion 
                    printf("begin \n");
                    indice_liste_itérateur+=1; //augmente le nombre d'itérateurs
                    indentation(indice_liste_itérateur);
                    }
                }
            if (strcmp(lex->content,"}")==0){
                if (indice_liste_itérateur>0){
                    if (liste_itérateur[indice_liste_itérateur-1]==0){ // si la dernière fonction utilisée est une boucle 
                        printf("done \n");
                        indice_liste_itérateur-=1; //diminue le nombre d'itérateurs
                        indentation(indice_liste_itérateur);
                        }
                    else{ // si la dernière fonction utilisée est un condtion 
                        printf("end \n");
                        indice_liste_itérateur-=1; //diminue le nombre d'itérateurs
                        indentation(indice_liste_itérateur);
                        }
                    }
            }
        }
        else if (lex->type == LxmOperator) { //cas pour les opérateurs 
            printf("%s",lex->content );
        }
        else if(lex->type == LxmComment) { //cas pour les commentaires
            printf("(*");
            printf("%s", lex->content);
            printf("*)\n");
            indentation(indice_liste_itérateur);
        }
        else if(lex->type == LxmString) {
            printf("%s ",lex->content);
        }
        else if(lex->type == LxmKeyWord) {
            if (strcmp(lex->content,"printf") == 0) { //cas pour printf
                printf("Printf.printf ");
                lex=lex->next; //on passe pour passer la parenthèse
                parenthese_fonction=0; //initialisation du compteur de parenthèse après une fonction à 0
            }
            else{
            printf("%s ",lex->content); //Cas des mots clés pour les boucles
            if (strcmp(lex->content,"while")==0 ){
                liste_itérateur[indice_liste_itérateur]=0; //ajout d'une boucle while dans la pile
            }
            if (strcmp(lex->content,"if")==0){
                liste_itérateur[indice_liste_itérateur]=1; //ajout d'une condition dans la pile 
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
