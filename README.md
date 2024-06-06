# Traducteur de C en OCaml

Le but du projet est de creer un traducteur qui, a partir d'un fichier C, cree un fichier OCaml executable.

## Utilisation

Pour compiler il suffit d'executer le fichier `compile.sh` qui contient l'instruction gcc avec tous les fichiers necessaire a la compilation.

Un fichier `Traducteur` sera alors creer et il vous suffira de l'exécuter.

```
$ ./compile
>> Compilation terminée.

$ ./Traducteur <NomDuFichier>.c
>> Le fichier <NomDuFichier>.ml a été créé.
```

## Architecture du projet

- Le code source en C avec leurs fichiers headers respectifs.
    - `traducteur.c` : Le coeur du projet.
    - `./lexer` : Tous les fichiers relatifs au lexer et à la liste chaînée de lexèmes.
- `compile.sh` : Facilite l'etape de compilation.
- `./tests` : Contient les exemples utilises pour les tests.
    - Etape 1 : Fonction main, variables et int.
    - Etape 2 : Commentaires.
    - Etape 3 : fonction printf.
