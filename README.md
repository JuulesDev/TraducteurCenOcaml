# Traducteur de C en OCaml

Le but du projet est de creer un traducteur qui, a partir d'un fichier C, cree un fichier OCaml executable.

## Utilisation

Pour compiler il suffit d'executer le fichier `compile.sh` qui contient l'instruction gcc avec tous les fichiers necessaire a la compilation.

Un fichier `Traducteur` sera alors creer et il vous suffira de l'executer.

## Architecture du projet

- Le code source en C avec leurs fichiers headers respectifs.
    - `traducteur.c` : Le coeur du projet.
    - `lexeme_list.c` (et `.h`) : Definit la liste chainee de lexemes et ses fonctions associees.
    - `lexer.c` (et `.h`) : Permet de creer une liste de lexemes a partir d'un fichier.
- `compile.sh` : Facilite l'etape de compilation.
- `./tests` : Contient les exemples utilises pour les tests.
    - Etape 1 : Fonction main, variables et int.
    - Etape 2 : Commentaires.
