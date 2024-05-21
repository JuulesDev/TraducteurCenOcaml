#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "./lexer/lexer.h"

void traducteur(lexeme_list* lexemes)
{
    lexeme_list* lex = lexemes; // Le lexeme actuel (passe le premier).
    while (lex->type != LxmEnd)
    {
        
        lex = lex->next; 
        //printf("%d,%s,\n",lex->type,lex->content);
        if (lex -> type ==LxmType){
            if (strcmp(lex->next->content,"main")==0){
                lex =lex->next->next->next->next;
            }
            else{
                printf("let ");
            }
            
        }
        else if (lex->type ==LxmVariable){
            printf("%s ",lex->content);
        }
        else if (lex->type == LxmAffectation){
            printf("%s ref ",lex->content);
        }
        else if (lex->type ==LxmInt){
            printf("%s",lex->content);
        }
        else if (lex->type ==LxmPunctuation){
            if (strcmp(lex->content, ";")==0){
                printf(";\n");
            }
        }
        else if (lex->type ==LxmOperator){
            printf("%s",lex->content );
        }
        else if(lex->type ==LxmComment){
            if (strcmp(lex->content,"//")==0 || strcmp(lex->content,"/*")==0){
                printf("(* ");
            }
            else{
                printf("*) \n");
            }
        }
    }
}

int main()
{
    // Cree la liste de lexemes du fichier
    //FILE* source_file = fopen("./test.c", "r");
    FILE* source_file = fopen("./tests/etape2/etape2.c", "r");
    lexeme_list* l = lexeur(source_file);

    printf("\n===\n");

    print_list(l);
    traducteur(l);

    free_list(l);
}
