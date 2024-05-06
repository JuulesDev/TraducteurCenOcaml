
#include "../lexer/lexer.h"

#ifndef PARSER_H
#define PARSER_H


// -> syntax_tree.c

/*
    Un type de node d'un AST.
*/
typedef enum {
    CONST_VAL,
    SIMPLE_OP,
    DOUBLE_OP,
    DECLARATION,
} AstNodeType;

/*
    Une noeud d'un AST.
*/
struct ast_node {
    AstNodeType type;
    union {
        struct {
            char* value;
        } const_val;
        struct {
            char* type;
            struct ast_node* expr;
        } simple_op_node;
        struct {
            char* type;
            struct ast_node* left_expr;
            struct ast_node* right_expr;
        } double_op_node;
        struct {
            char* variable_name;
            struct ast_node* expr;
        } declaration_node;
    };
};
typedef struct ast_node ast_node;

/*
    Représente un AST, ou plutôt un tableau d'AST.
*/
typedef struct {
    int n_instructions;
    ast_node** instructions;
} ast;

ast_node* create_ast_node(AstNodeType type);

ast* create_ast();

void free_ast(ast* tree);

void print_tree(ast tree);


// -> parser.c

ast* parse(lexeme_list* lexemes);

#endif