#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef LEXER_H
#define LEXER_H

// -> lexemes_list.c

/*
    Un type de lexème.
*/
typedef enum {
    LxmStart,
    LxmEnd,
    LxmInt,
    LxmString,
    LxmPunctuation,
    LxmOperator,
    LxmAffectation,
    LxmType,
    LxmKeyWord,
    LxmVariable,
    LxmComment,
    LxmFunction
} LexemeType;

/*
    Une liste de chainees classique de lexemes.
*/
struct lexeme_list {
    LexemeType type; // Le type de lexème.
    char* content; // La valeur du lexeme.
    struct lexeme_list* next;
};
typedef struct lexeme_list lexeme_list;

void add_list(lexeme_list** pfin, LexemeType type, char* content);

void print_list(lexeme_list* lex);

void free_list(lexeme_list* lex);

// -> lexer.c

char* create_arg( char* buffer, const int len);

bool is_char_in( char c, const char tab[], const int len);

bool is_string_in ( char* c, const char* tab[], const int len);

lexeme_list* lexeur (FILE* fichier);

#endif