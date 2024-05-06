
#include "../lexer/lexer.h"

#ifndef PARSER_H
#define PARSER_H


// -> syntax_tree.c

typedef enum {
    CONST_VAL,
    SIMPLE_OP,
    DOUBLE_OP,
    DECLARATION,
} AstNodeType;

typedef struct {
    char* value;
} const_val;

typedef struct {
    char* type;
    ast_node* expr;
} simple_op;

typedef struct {
    char* type;
    ast_node* left_expr;
    ast_node* right_expr;
} double_op;

typedef struct {
    char* variable_name;
    ast_node* expr;
} declaration;

typedef struct  {
    AstNodeType type;
    union {
        const_val const_val_node;
        simple_op simple_op_node;
        double_op double_op_node;
        declaration declaration_node;
    };
} ast_node;

typedef struct {
    int n_instructions;
    ast_node** instructions;
} ast;

ast* create_node(char* value, ast* leftop, ast* rightop);

void affiche_tree(ast* tree, int depth);


// -> parser.c

ast* parse(maillon* lexemes);

#endif