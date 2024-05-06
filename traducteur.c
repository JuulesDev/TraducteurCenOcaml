#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "./lexer/lexer.h"
#include "./parser/parser.h"

int main()
{
    // Cree la liste de lexemes du fichier
    FILE* source_file = fopen("./test.c", "r");
    // FILE* source_file = fopen("./tests/etape1/etape1.c", "r");
    lexeme_list* l = lexeur(source_file);


    ast* ast = parse(l);

    printf("\n===\n");

    print_list(l);
    print_tree(*ast);

    free_list(l);
}
