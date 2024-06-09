Projet : traducteur C vers Ocaml

Objectif :
Ce projet a pour objectif de traduire un programme en C, en OCaml.
Le projet comprend 4 fichiers principaux : `traducteur.c`, `lexeme_list.c`,  `lexer.c`et `lexer.h`.

### `Traducteur.c` :
Le fichier `traducteur.c` contient la fonction principale pour la traduction.

**Pile de Blocs** :
Une pile est utilisée pour suivre les différents blocs de code (condition et boucles).
Elle permet de garder la même structure que le code C d'origine.

**Indentation** :
La fonction `indentation` permet de gérer l'indentation du code OCAML en fonction de la profondeur du bloc de code.
Cela permet au code OCAML d'être bien indenté.

**Gestion des Lexèmes** :
La fonction `traducteur` parcourt la liste des lexèmes pour générer le code OCaml.
Chaque type de lexeme est traité dans plusieurs cas différents.


#### Fichier `lexeme_list.c`:
Le fichier `lexeme_list.c` permet de gérer les listes de lexèmes :

**Structure des Lexèmes** : Définition de la structure `lexeme` qui contient le type du lexème , son contenu , et un pointeur vers le lexème suivant .

**Ajout de Lexèmes** : La fonction `add_list` permet d'ajouter des lexèmes à la liste.
Elle prend en compte le type du lexème et son contenu pour créer une nouvelle entrée dans la liste.

**Création d'Arguments** : La fonction `create_arg` permet de créer des chaînes de caractères pour les lexèmes.

#### Fichier `lexer.c`:
Le fichier `lexer.c` contient le lexer qui analyse le code source en caractères individuels pour les transformer en lexèmes.

**Lecture et Classification** : La fonction `lexer` lit le fichier source caractère par caractère.
Elle utilise des buffers pour accumuler les caractères qui forment des lexèmes.

**Types de Lexèmes** : Le lexer reconnaît différents types de lexèmes, y compris les types , les mots-clés , les variables , les opérateurs , et les chaînes de caractères .


#### Choix Techniques :
**Gestion des Parenthèses et Blocs** : L'utilisation d'une pile pour suivre les blocs de code et permet une bonne traduction.

**Références et Mutabilité** : En OCaml: les variables sont gérées avec des références.