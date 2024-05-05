#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "lexer.h"
#include "lexeme_list.h"

// === FONCTIONS UTILITAIRES ===

/*
    Transforme un buffer en une string correcte.

    Arguments:
        - char* buffer: Le buffer contenant les caractères à "normaliser".
        - int taille_buffer: La taille du buffer.

    Renvoie:
        - char*: Une string correcte.
*/
char* cree_arg(char* buffer, const int taille_buffer)
{
    char* arg = malloc((taille_buffer+1) * sizeof(char));
    for (int i = 0; i < taille_buffer; i++)
    {
        arg[i] = buffer[i];
    }
    arg[taille_buffer] = '\0'; // Caractère de fin de string.
    return arg;
}

/*
    Vérifie si un caractères est dans un tableau.

    Arguments:
        - char c: Le caractère à tester.
        - char tab[]: Le tableau où on cherche.
        - int len: La taille du tableau où on cherche.

    Renvoie:
        - bool: true si le caractère est présent.
*/
bool is_char_in(char c, const char tab[], const int len)
{
    for(int i = 0; i < len; i++)
    {
        if (tab[i] == c) { return true; }
    }
    return false;
}

/*
    Vérifie si une string est dans un tableau.

    Arguments:
        - char* s: La string à tester.
        - char tab[]: Le tableau où on cherche.
        - int len: La taille du tableau où on cherche.

    Renvoie:
        - bool: true si le caractère est présent.
*/
bool is_string_in(char* s, const char* tab[], const int len)
{
    for(int i = 0; i < len; i++)
    {
        if (!strcmp(s, tab[i])) { return true; }
    }
    return false;
}

// === LEXEUR ===

// Listes des lexèmes qu'on souhaite détecter.
const char ponctuation[] = {'(', ')', '{', '}', ';'};
const int len_ponctuation = 5;

const char* type[] = {"bool", "int", "void"};
const int len_type = 3; 

const char* motcle[] = {"while"};
const int len_motcle = 1;

const char operateurs_simples[] = {'+', '-', '/', '*', '%'};
const int len_ops = 5;

    const char operateurs_doubles[] = {'=', '|', '&','<','>','!'};
const int len_opd = 6;

/*
    Crée une liste de lexèmes à partir d'un fichier.

    Arguments:
        - FILE* fichier: Le fichier source.

    Renvoie:
        - maillon*: La liste de lexèmes issue du fichier.
*/
maillon* lexeur(FILE* fichier)
{
    // Crée le maillon de début.
    maillon* debut = malloc(sizeof(maillon));
    debut->argument = cree_arg("DEBUT", 5); // Marque le début des lexèmes.
    debut->lexeme = 'D'; 
    debut->suivant = NULL;
    maillon* fin = debut; // Pour ajouter de nouveaux maillons.

    // Buffer permettant de lire les arguments.
    char buffer[100];
    int taille_buffer;

    char c = fgetc(fichier);
    while (c != EOF)
    {
        taille_buffer = 0; // On vide le buffer.

        if ('0' <= c && c <= '9')
        { // Cas 1 : Entier
            while ('0' <= c && c <= '9')
            {
                buffer[taille_buffer] = c;
                taille_buffer += 1;
                c = fgetc(fichier);
            }
            ajoute_maillon_fin(&fin, '0', cree_arg(buffer, taille_buffer));
        } 
        else if (is_char_in(c, ponctuation, len_ponctuation))
        { // Cas 2 : Ponctuation
            char* arg = cree_arg(&c, 1);
            ajoute_maillon_fin (&fin, 'P', arg); // 'P' -> ponctuation

            c = fgetc(fichier);
        } 
        else if (is_char_in(c, operateurs_simples, len_ops))
        { // Cas 3 : Opérateur simple
            char* arg = cree_arg(&c, 1);
            ajoute_maillon_fin (&fin, 'O', arg); // O -> Opérateur
            
            c = fgetc(fichier);
        }
        else if (is_char_in(c, operateurs_doubles, len_opd))
        { // Cas 4 : Opérateur double
            char d = c;
            c = fgetc(fichier);

            if(is_char_in(c, operateurs_doubles, len_opd))
            {
                buffer[0] = d;
                buffer[1] = c;
                taille_buffer = 2;

                ajoute_maillon_fin(
                    &fin, 'B', // B -> Opérateur binaire
                    cree_arg(buffer, taille_buffer)
                );

                c = fgetc(fichier);
            }
            else if (d == '=')
            {
                char* arg = cree_arg(&d, 1);
                ajoute_maillon_fin (&fin, 'E', arg); // E -> Affectation
            }
            else
            {
                char* arg = cree_arg(&d, 1);
                ajoute_maillon_fin(&fin, 'B', arg); // Devrait être O ?
            }
        } 
        else if (c == '"')
        { // Cas 5 : Chaîne de caractères
            c = fgetc(fichier);
            while (c != '"'){
                buffer[taille_buffer] = c;
                taille_buffer++;
                c = fgetc(fichier);
            }
            
            c = fgetc(fichier);
            ajoute_maillon_fin (&fin, 'S', cree_arg(buffer, taille_buffer));

        } 
        else if (('a' <= c && c <='z') || ('A' <= c && c <='Z'))
        { // Cas 6 : Mots-clés
            // Lis l'entiereté du mot-clé
            while (('a' <= c && c <='z') || ('A' <= c && c <='Z'))
            {
                buffer[taille_buffer] = c;
                taille_buffer++;
                c = fgetc(fichier);
            }
            char* chaine = cree_arg(buffer, taille_buffer);

            if (is_string_in(chaine, type, len_type))
            {
                ajoute_maillon_fin(&fin, 'T', chaine); // T -> Type
            }
            else if (is_string_in(chaine, motcle, len_motcle))
            {
                ajoute_maillon_fin(&fin, 'M', chaine); // M -> Mot-clé
            }
            else
            {
                ajoute_maillon_fin(&fin, 'V', chaine); // V -> Variable
            }
        } 
        else if (c == ' ' || c == '\n')
        { // Cas 7 : Ignore les espaces blancs
            c = fgetc(fichier);
        }
        else
        {
            fprintf(stderr, "Le charactère %c de numéro %d n'a pas été reconnu.", c, (int) c);
            exit(1);
        }
    }

    ajoute_maillon_fin(&fin, 'F', cree_arg("FIN", 3)); // F -> EOF

    return debut;
}