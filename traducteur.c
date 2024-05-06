#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "./lexer/lexer.h"
#include "./parser/parser.h"

int main()
{
    // Cree la liste de lexemes du fichier
    FILE* fichier_source = fopen("./test.c", "r");
    // FILE* fichier_source = fopen("./tests/etape1/etape1.c", "r");
    maillon* l = lexeur(fichier_source);

    affiche_liste(l);

    syntax_tree* ast = parse(l);

    affiche_tree(ast, 0);

    libere_liste(l);
}
