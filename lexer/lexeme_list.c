#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "lexer.h"

/*
    Ajoute un maillon à la fin de la liste chaînée.

    Arguments:
        - lexeme_list** last_elem: Un pointeur vers le dernier maillon de la liste.
        - LexemeType type: Le type de lexème du nouveau maillon.
        - char* content: L'argument du nouveau maillon.
*/
void add_list(lexeme_list** last_elem, LexemeType type, char* content)
{
    // printf(">> Nouveau maillon (lex:%c) (arg:%s)\n", lex, arg);
    // Création du nouveau maillon
    lexeme_list* new_elem = malloc(sizeof(lexeme_list));
    new_elem->type = type;
    new_elem->content = content;
    new_elem->next = NULL;
    // Ajoute le maillon à la fin de la liste
    (*last_elem)->next = new_elem;
    *last_elem = new_elem;
}

/*
    Affiche une liste chaînée de lexèmes.

    Arguments:
        - lexeme_list* lex: Le premier maillon de la liste chaînée à afficher.
*/
void print_list(lexeme_list* lex)
{
    lexeme_list* m = lex;
    printf("Liste de lexèmes : (\n");
    while (m != NULL)
    {
        printf("  [%c, '%s']\n", m->type, m->content);
        m = m->next;
    }
    printf(")\n");
}

/*
    Libère l'espace alloué à une liste chaînée.

    Arguments:
        - lexeme_list* lex: Le premier maillon de la liste chaînée à libérer.
*/
void free_list(lexeme_list* lex)
{
    if (lex != NULL)
    {
        // Libère récursivement la liste chaînée.
        free_list(lex->next);
        // Libère l'argument s'il a été alloué avec un malloc.
        if (lex->content != NULL){
            free(lex->content);
        }
        free(lex);
    }
}