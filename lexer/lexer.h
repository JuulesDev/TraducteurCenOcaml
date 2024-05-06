#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef LEXER_H
#define LEXER_H

// -> lexemes_list.c

/*
    Une liste de chainees classique de lexemes.
*/
struct maillon {
    char lexeme; // Le nom du lexème.
    char* argument; // La valeur du lexeme.
    struct maillon* suivant;
};
typedef struct maillon maillon;

void ajoute_maillon_fin(maillon** pfin, char lex, char* arg);

void affiche_liste(maillon* debut);

void libere_liste(maillon* debut);

// -> lexer.c

bool char_in ( char c, const char tab[], const int len);

bool string_in ( char* c, const char* tab[], const int len);

char* cree_arg( char* buffer, const int len);

maillon* lexeur (FILE* fichier);

#endif