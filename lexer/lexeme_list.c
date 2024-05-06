#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "lexer.h"

/*
    Ajoute un maillon à la fin de la liste chaînée.

    Arguments:
        - maillon** pfin: Un pointeur vers le dernier maillon de la liste.
        - char lex: Le type de lexème du nouveau maillon.
        - char* arg: L'argument du nouveau maillon.
*/
void ajoute_maillon_fin(maillon** pfin, char lex, char* arg)
{
    // printf(">> Nouveau maillon (lex:%c) (arg:%s)\n", lex, arg);
    // Création du nouveau maillon
    maillon* nv_maillon = malloc(sizeof(maillon));
    nv_maillon->lexeme = lex;
    nv_maillon->argument = arg;
    nv_maillon->suivant = NULL;
    // Ajoute le maillon à la fin de la liste
    (*pfin)->suivant = nv_maillon;
    *pfin = nv_maillon;
}

/*
    Affiche une liste chaînée de lexèmes.

    Arguments:
        - maillon* debut: Le premier maillon de la liste chaînée à afficher.
*/
void affiche_liste(maillon* debut)
{
    maillon* m = debut;
    printf("Liste de lexèmes : (\n");
    while (m != NULL)
    {
        printf("  [%c, '%s']\n", m->lexeme, m->argument);
        m = m->suivant;
    }
    printf(")\n");
}

/*
    Libère l'espace alloué à une liste chaînée.

    Arguments:
        - maillon* debut: Le premier maillon de la liste chaînée à libérer.
*/
void libere_liste(maillon* debut)
{
    if (debut != NULL)
    {
        // Libère récursivement la liste chaînée.
        libere_liste(debut->suivant);
        // Libère l'argument s'il a été alloué avec un malloc.
        if (debut->argument != NULL){
            free(debut->argument);
        }
        free(debut);
    }
}