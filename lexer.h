#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "lexeme_list.h"

#ifndef LEXER_H
#define LEXER_H

bool char_in ( char c, const char tab[], const int len);

bool string_in ( char* c, const char* tab[], const int len);

char* cree_arg( char* buffer, const int len);

maillon* lexeur (FILE* fichier);

#endif