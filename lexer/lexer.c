#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "lexer.h"

// === FONCTIONS UTILITAIRES ===

/*
    Transforme un buffer en une string correcte.

    Arguments:
        - char* buffer: Le buffer contenant les caractères à "normaliser".
        - int len_buffer: La taille du buffer.

    Renvoie:
        - char*: Une string correcte.
*/
char* create_arg(char* buffer, const int len_buffer)
{
    char* arg = malloc((len_buffer+1) * sizeof(char));
    for (int i = 0; i < len_buffer; i++)
    {
        arg[i] = buffer[i];
    }
    arg[len_buffer] = '\0'; // Caractère de fin de string.
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
const char ponctuation[] = {'(', ')', '{', '}', ';',','};
const int len_ponctuation = 6;

const char* type[] = {"bool", "int", "void"};
const int len_type = 3; 

const char* motcle[] = {"while","if", "for","printf","return","else"};
const int len_motcle = 6;

const char operateurs_simples[] = {'+', '-', '*', '%'};
const int len_ops = 4;

const char operateurs_doubles[] = {'=', '|', '&', '<', '>', '!', '/'};
const int len_opd = 7;

/*
    Crée une liste de lexèmes à partir d'un fichier.

    Arguments:
        - FILE* fichier: Le fichier source.

    Renvoie:
        - lexeme_list*: La liste de lexèmes issue du fichier.
*/
lexeme_list* lexeur(FILE* file)
{
    // Crée le maillon de début.
    lexeme_list* lex = malloc(sizeof(lexeme_list));
    lex->type = LxmStart; 
    lex->content = NULL;
    lexeme_list* fin = lex; // Pour ajouter de nouveaux maillons.

    // Buffer permettant de lire les arguments.
    char buffer[100];
    int len_buffer;

    char c = fgetc(file);
    while (c != EOF)
    {
        len_buffer = 0; // On vide le buffer.

        if ('0' <= c && c <= '9')
        { // Cas 1 : Entier
            while ('0' <= c && c <= '9')
            {
                buffer[len_buffer] = c;
                len_buffer += 1;
                c = fgetc(file);
            }
            add_list(&fin, LxmInt, create_arg(buffer, len_buffer));
        } 
        else if (is_char_in(c, ponctuation, len_ponctuation))
        { // Cas 2 : Ponctuation
            char* arg = create_arg(&c, 1);
            add_list(&fin, LxmPunctuation, arg);
            c=fgetc(file);
        }
        else if (is_char_in(c, operateurs_simples, len_ops))
        { // Cas 3 : Opérateur simple
            char* arg = create_arg(&c, 1);
            add_list(&fin, LxmOperator, arg);
            c = fgetc(file);
        }
        else if (is_char_in(c, operateurs_doubles, len_opd))
        { // Cas 4 : Opérateur double
            char d = c;
            c = fgetc(file);
            
            buffer[0] = d;
            buffer[1] = c;
            len_buffer = 2;           
            
            if (!strcmp(create_arg(buffer, len_buffer), "//") || !strcmp(create_arg(buffer, len_buffer), "/*"))
            { // Détection de commentaire
                bool inline_comment = !strcmp(create_arg(buffer, len_buffer), "//");

                d = fgetc(file); // Avant-dernier caractère
                c = fgetc(file); // Dernier caractére

                buffer[0] = d;
                len_buffer = 1;

                while ((inline_comment && c != '\n') || (!inline_comment && d != '*' && c != '/'))
                {
                    buffer[len_buffer] = c;
                    len_buffer += 1;
                    d = c;
                    c = fgetc(file);
                }
                
                if (!inline_comment) {
                    len_buffer -= 1; // Le dernier caractère ne doit pas être là
                    c = fgetc(file); // Skip le / de fin de commentaire.
                } 
                
                char* comment_content = create_arg(buffer, len_buffer);
                add_list(&fin, LxmComment, comment_content);
            }
            else if(is_char_in(c, operateurs_doubles, len_opd))
            { // Opérateur double

                add_list(
                    &fin, LxmOperator,
                    create_arg(buffer, len_buffer)
                );

                c = fgetc(file);
            }
            else if (d == '=')
            { // Affectation
                char* arg = create_arg(&d, 1);
                add_list(&fin, LxmAffectation, arg);
            }
            else
            { // Opérateur simple
                char* arg = create_arg(&d, 1);
                add_list(&fin, LxmOperator, arg);
            }
        } 
        else if (c == '"')
        { // Cas 5 : Chaîne de caractères
            c = fgetc(file);
            while (c != '"'){
                buffer[len_buffer] = c;
                len_buffer++;
                c = fgetc(file);
            }
            
            c = fgetc(file);
            add_list (&fin, LxmString, create_arg(buffer, len_buffer));

        } 
        else if (('a' <= c && c <='z') || ('A' <= c && c <='Z'))
        { // Cas 6 : Mots-clés
            // Lis l'entiereté du mot-clé
            while (('a' <= c && c <='z') || ('A' <= c && c <='Z'))
            {
                buffer[len_buffer] = c;
                len_buffer++;
                c = fgetc(file);
            }
            char* chaine = create_arg(buffer, len_buffer);

            if (is_string_in(chaine, type, len_type))
            {
                add_list(&fin, LxmType, chaine);
            }
            else if (is_string_in(chaine, motcle, len_motcle))
            {
                add_list(&fin, LxmKeyWord, chaine);
            }
            else
            {
                add_list(&fin, LxmVariable, chaine);
            }
        } 
        else if (c == ' ' || c == '\n')
        { // Cas 7 : Ignore les espaces blancs
            c = fgetc(file);
        }
        else
        {
            fprintf(stderr, "Le charactère %c de numéro %d n'a pas été reconnu.", c, (int) c);
            exit(1);
        }
    }

    add_list(&fin, LxmEnd, NULL);

    return lex;
}