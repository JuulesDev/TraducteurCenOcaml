#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef LEXEME_LIST_H
#define LEXEME_LIST_H

/*
    Une liste de chainees classique de lexemes.
*/
struct maillon {
    char lexeme; // Le nom du lexème.
    char* argument; // La valeur du lexeme.
    struct maillon* suivant;
};
typedef struct maillon maillon;

void ajoute_maillon_fin (maillon** pfin, char lex, char* arg);

void affiche_liste (maillon* debut);

void libere_liste (maillon* debut);;

#endif