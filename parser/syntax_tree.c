#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "parser.h"


ast_node* create_ast_node(AstNodeType type)
{
    ast_node* tree = malloc(sizeof(ast_node));
    tree->type = type;
    return tree;
}

void free_ast_node(ast_node* node)
{
    switch (node->type)
    {
    case CONST_VAL:
        break;
    case SIMPLE_OP:
        free(node->simple_op_node.expr);
        break;
    case DOUBLE_OP:
        free(node->double_op_node.left_expr);
        free(node->double_op_node.right_expr);
        break;
    case DECLARATION:
        free(node->declaration_node.expr);
        break;
    }
    free(node);
}

ast* create_ast()
{
    ast* tree = malloc(sizeof(ast));
    tree->n_instructions = 0;
    tree->instructions = (ast_node*)malloc(sizeof(ast_node) * 50); // Valeur arbitraire
    return tree;
}

void free_ast(ast* tree)
{
    for (int i = 0; i < tree->n_instructions; i++)
    {
        free_ast_node(tree->instructions[i]);
    }
    free(tree->instructions);
    free(tree);
}




void n_tab(int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("  ");
    } 
}

void affiche_node(ast_node node, int depth)
{
    switch (node.type)
    {
    case CONST_VAL:
        n_tab(depth); printf("CONST{%s}", node.const_val_node.value);
        break;
    case SIMPLE_OP:
        n_tab(depth); printf("SIMPLE_OP{type:%s,\n", node.simple_op_node.type);
        n_tab(depth); printf("expr:\n");
        affiche_node(*(node.simple_op_node.expr), depth+1);
        n_tab(depth); printf("}\n");
        break;
    case DOUBLE_OP:
        n_tab(depth); printf("DOUBLE_OP{type:%s,\n", node.double_op_node.type);
        n_tab(depth); printf("left_expr:\n");
        affiche_node(*(node.double_op_node.left_expr), depth+1);
        n_tab(depth); printf("right_expr:\n");
        affiche_node(*(node.double_op_node.right_expr), depth+1);
        n_tab(depth); printf("}\n");
        break;
    case DECLARATION:
        printf("DECLARATION{name:%s", node.declaration_node.variable_name);
        n_tab(depth); printf("expr:\n");
        affiche_node(*(node.declaration_node.expr), depth+1);
        n_tab(depth); printf("}\n");
        break;
    }
}

void affiche_tree(ast tree) {
    printf("SYNTAX TREE : [\n");
    for (int i = 0; i < tree.n_instructions; i++)
    {
        ast_node node = *(tree.instructions[i]);
        affiche_node(node, 1);
        n_tab(1); printf("---\n");
    }
   printf("]\n");
}

