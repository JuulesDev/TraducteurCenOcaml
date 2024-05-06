#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "parser.h"


// PARSER

syntax_tree* parse_math(maillon** lex, char* closing_lexeme)
{ // Ne fonctionne presque pas du tout :)
    if ((*lex)->suivant->lexeme == 'P' && !strcmp((*lex)->suivant->argument, closing_lexeme))
    {
        syntax_tree* constant = create_node((*lex)->argument, NULL, NULL);
        (*lex) = (*lex)->suivant->suivant;
        return constant;
    }
    else
    {
        syntax_tree* leftop = create_node((*lex)->argument, NULL, NULL);
        (*lex) = (*lex)->suivant;
        char* operator = (*lex)->argument;
        (*lex) = (*lex)->suivant;
        printf("----> %s %s %s\n", leftop->value, operator, (*lex)->argument);

        if (!strcmp((*lex)->argument, "("))
        {
            (*lex) = (*lex)->suivant;
            return create_node(operator, leftop, parse_math(lex, ")"));
        }
        else
        {
            printf("WTF\n");
            return NULL;
        }
    }
}


syntax_tree* parse(maillon* lexemes)
{
    syntax_tree* ast_root = create_node("ROOT", NULL, NULL);
    syntax_tree* ast_end = ast_root; // Derniere feuille de l'AST.

    maillon* lex = lexemes->suivant; // Le lexeme actuel (passe le premier).
    while (lex->lexeme != 'F')
    {
        switch (lex->lexeme)
        {
        case 'T': // /!\ Aucune verification de la validite des lexemes
            char* type = lex->argument;
            lex = lex->suivant;
            syntax_tree* name_node = create_node(lex->argument, NULL, NULL);
            lex = lex->suivant;

            if (lex->lexeme == 'E')
            { // Variable
                lex = lex->suivant;
                syntax_tree* value_node = parse_math(&lex, ";");
                syntax_tree* assignation_node = create_node("ASSIGN", name_node, value_node);

                ast_end->next = assignation_node;
                ast_end = assignation_node;
            }
            else if (lex->argument == "(")
            { // Fonction

            }
            break;
        
        default:
            printf("Lexeme non reconnu ('%c' : '%s').\n", lex->lexeme, lex->argument);
            lex->lexeme = 'F';
            break;
        }
    }

    return ast_root;
}
