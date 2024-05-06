#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "parser.h"

/*
    Crée un noeud d'AST vide.

    Arguments:
        - AstNodeType type: Le type du noeud.

    Renvoie:
        - ast_node*: Le noeud créé.
*/
ast_node* create_ast_node(AstNodeType type)
{
    ast_node* node = malloc(sizeof(ast_node));
    node->type = type;
    return node;
}

/*
    Libère récursivement la mémoire associée à un noeud.

    Arguments:
        - ast_node* node: Le noeud à libérer.
*/
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

/*
    Crée un AST vide.

    Renvoie:
        - ast*: L'AST créé.
*/
ast* create_ast()
{
    ast* tree = malloc(sizeof(ast));
    tree->n_instructions = 0;
    tree->instructions = malloc(sizeof(ast_node*) * 50); // Valeur arbitraire
    return tree;
}

/*
    Libère la mémoire associée à un AST.

    Arguments:
        - ast* tree: L'arbre à libérer.
*/
void free_ast(ast* tree)
{
    for (int i = 0; i < tree->n_instructions; i++)
    {
        free_ast_node(tree->instructions[i]);
    }
    free(tree->instructions);
    free(tree);
}

/*
    Print un alinéa. Utile pour afficher la profondeur dans un arbre.

    Arguments:
        - int n: La taille de l'alinéa.
*/
void n_tab(int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("  ");
    } 
}

/*
    Affiche un noeud d'AST et ses sous-noeuds récursivement.

    Arguments:
        - ast_node* node: Le noeud à afficher.
        - int depth: La profondeur du noeud (pour l'affichage, par défaut mettre 0).
*/
void print_node(ast_node* node, int depth)
{
    switch (node->type)
    {
    case CONST_VAL:
        n_tab(depth); printf("CONST{%s}", node->const_val.value);
        break;
    case SIMPLE_OP:
        n_tab(depth); printf("SIMPLE_OP{type:%s,\n", node->simple_op_node.type);
        n_tab(depth); printf("expr:\n");
        print_node(node->simple_op_node.expr, depth+1);
        n_tab(depth); printf("}\n");
        break;
    case DOUBLE_OP:
        n_tab(depth); printf("DOUBLE_OP{type:%s,\n", node->double_op_node.type);
        n_tab(depth); printf("left_expr:\n");
        print_node(node->double_op_node.left_expr, depth+1);
        n_tab(depth); printf("right_expr:\n");
        print_node(node->double_op_node.right_expr, depth+1);
        n_tab(depth); printf("}\n");
        break;
    case DECLARATION:
        printf("DECLARATION{name:%s", node->declaration_node.variable_name);
        n_tab(depth); printf("expr:\n");
        print_node(node->declaration_node.expr, depth+1);
        n_tab(depth); printf("}\n");
        break;
    }
}

/*
    Affiche un AST.

    Arguments:
        - ast tree: L'AST à afficher.
*/
void print_tree(ast tree) {
    printf("SYNTAX TREE : [\n");
    for (int i = 0; i < tree.n_instructions; i++)
    {
        ast_node* node = tree.instructions[i];
        print_node(node, 1);
        n_tab(1); printf("---\n");
    }
   printf("]\n");
}

