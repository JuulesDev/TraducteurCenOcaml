#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

int main()
{
    // Cree la liste de lexemes du fichier
    FILE* fichier_source = fopen("./tests/etape1/etape1.c", "r");
    maillon* l = lexeur(fichier_source);

    affiche_liste(l);
    libere_liste(l);
}
