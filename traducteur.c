#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "./lexer/lexer.h"

void traducteur(lexeme_list* lexemes)
{
    lexeme_list* lex = lexemes->next; // Le lexeme actuel (passe le premier).
    while (lex->type != LxmEnd)
    {
        lex = lex->next;
    }
}

int main()
{
    // Cree la liste de lexemes du fichier
    FILE* source_file = fopen("./test.c", "r");
    // FILE* source_file = fopen("./tests/etape1/etape1.c", "r");
    lexeme_list* l = lexeur(source_file);

    printf("\n===\n");

    print_list(l);
    traducteur(l);

    free_list(l);
}
