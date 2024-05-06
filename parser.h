
#include "lexeme_list.h"

#ifndef PARSER_H
#define PARSER_H


struct syntax_tree {
    char* value;
    struct syntax_tree* leftop;
    struct syntax_tree* rightop;
    struct syntax_tree* next;
};
typedef struct syntax_tree syntax_tree;

void affiche_tree(syntax_tree* tree, int depth);

syntax_tree* parse(maillon* lexemes);

#endif