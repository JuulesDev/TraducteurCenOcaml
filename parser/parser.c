#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "parser.h"

/*
    Crée un AST à partir d'une liste de lexèmes.

    Arguments:
        - lexeme_list* lexemes: La liste de lexèmes d'origine.

    Renvoie:
        - ast*: L'AST créé.
*/
ast* parse(lexeme_list* lexemes)
{
    ast* ast_tree = create_ast();

    lexeme_list* lex = lexemes->next; // Le lexeme actuel (passe le premier).
    while (lex->type != LxmEnd)
    {
        lex = lex->next;
    }

    return ast_tree;
}
