#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "./lexer/lexer.h"
void afficher_liste( const char* tab[], const int len) //fonction de débugage pour afficher une liste
{
    for(int i = 0; i < len; i++)
    {
        printf("%s",tab[i]);
    }
}
/*
    Affiche l'indentation du code.

    Argument:
        int depth: La profondeur de l'indentation.
*/
void indentation(int depth, FILE* target)
{
    for (int i = 0; i < depth; i += 1)
    {
        fprintf(target, "    ");
    }
}

void traducteur(lexeme_list* lexemes, FILE* target)
{
    lexeme_list* lex = lexemes; // Le lexeme actuel (passe le premier).
    int compteur_parenthese = 0; // Nombre de parenthèses après une fonction 

    // Pile indiquant le bloc dans lequel on se trouve : 0 -> while/for, 1 -> if, 2 -> fonction, 3->fonction avec return, 4->boucle for
    int* pile_bloc = malloc(sizeof(int)*20);
    int indice_pile_bloc = 0;
    // Si une parenthèse a été ouverte pour une assignation.
    bool open_par_assignation = false;
    bool retour =false;
    //liste des noms des fonctions qui ont été assignée
    const char *noms_fonctions[100];
    int len_nf = 0;

    const char *noms_reference[100];
    int len_ref = 0;


    while (lex->type != LxmEnd)
    {    
        lex = lex->next;
        //printf("-> %d,%s\n",lex->type,lex->content);
        
        if (lex->type == LxmType){
            if (strcmp(lex->next->content, "main") == 0) {
                // On écrit le contenu du main dans le fichier directement
                lex = lex->next->next->next->next; 
            } 
           
            else if (strcmp(lex->next->next->content,"=")==0){
                // Cas de première assignation d'une variable, on gère le cas jusqu'au =
                // Pour que le cas de base soit une réassignation
                lex = lex->next;
                char* var_name = lex->content;
                lex = lex->next;

                indentation(indice_pile_bloc, target);
                // On met des parenthèses
                fprintf(target, "let %s = ref (", var_name);
                open_par_assignation = true;
                noms_reference[len_ref]=var_name;
                len_ref+=1;
            }  
            else {
                //cas de création d'une fonction
                lex = lex->next;
                char* var_name = lex->content;
                lex = lex->next;
                indentation(indice_pile_bloc, target);
                fprintf(target,"let %s ",var_name);
                compteur_parenthese+=1;
                pile_bloc[indice_pile_bloc] = 2; //ajout de la fonction dans la pile
                noms_fonctions[len_nf]=var_name; //ajout du nom de la fonction dans la liste
                len_nf+=1;
                //afficher_liste(noms_fonctions,len_nf);
                while (compteur_parenthese!=0) //boucle pour afficher tous les arguments
                {
                    //printf("%d,%s\n",lex->type,lex->content);
                    lex=lex->next;
                    if (lex->type==LxmVariable){
                        fprintf(target,"%s",lex->content);
                    }
                    else if (lex->type==LxmType){
                        fprintf(target," ");
                    }
                    else if(lex->type=LxmPunctuation){
                        if (strcmp(lex->content,")") == 0){
                            compteur_parenthese=0;
                        }
                    }
                }
                
            }            
        } else if (lex->type == LxmVariable)
        {
            if (strcmp(lex->next->content, "=") == 0)
            {
                // Cas de ré-assignation de variable
                indentation(indice_pile_bloc, target);
                fprintf(target, "%s", lex->content);
            } 
            else if(is_string_in( lex->content ,noms_fonctions, len_nf)){
                fprintf(target, "%s", lex->content); //cas des noms de fonction que l'on a crée
            }
            else {
                // On veut la valeur d'une variable, toutes sont des ref
                if (is_string_in(lex->content,noms_reference,len_ref)==true){
                    fprintf(target, "!%s", lex->content); //cas des noms de fonction que l'on a crée
                }
                else{
                    fprintf(target, "%s", lex->content);
                }
            }            
        }
        else if (lex->type == LxmAffectation)
        {
            // Forcèment une réassignation (autre cas déjà gérée)
            fprintf(target, " := ");
        }
        else if (lex->type == LxmInt)
        {
            fprintf(target, "%s", lex->content);
        }
        else if(lex->type == LxmString) {
            fprintf(target, "\"%s\"", lex->content);
        }
        else if (lex->type == LxmPunctuation)
        { // Traduction de fin de ligne
            if (strcmp(lex->content, ",") == 0)
            {
                // Entre les arguments d'une fonction
                fprintf(target, " ");
            } else if (strcmp(lex->content, ";") == 0)
            {
                // On ferme une parenthèse potentielle.
               
                if (indice_pile_bloc > 0 && open_par_assignation )
                {
                    fprintf(target, ") in\n");
                    open_par_assignation = false;
                    if (retour==true){
                        indentation(indice_pile_bloc,target);
                        fprintf(target,"!res\n");
                        retour=false;
                    }
                } 
                else if (open_par_assignation){
                    fprintf(target, ");;\n");
                    open_par_assignation =false;
                }
                else if (indice_pile_bloc > 0) {
                    fprintf(target, ";\n");
                } else {
                    fprintf(target, ";;\n");
                }
            } else if (strcmp(lex->content,"(") == 0 )
            {
                compteur_parenthese += 1; //augmente le compteur des parenthèse pour les arguments d'une fonction
                fprintf(target, " ");
            } else if (strcmp(lex->content,")") == 0)
            {
                if (compteur_parenthese != 0)
                {
                    compteur_parenthese -= 1; //diminue le compteur des parenthèse en argument
                    fprintf(target, " ");
                }
            } else if (strcmp(lex->content,"{") == 0)
            { 
                if (pile_bloc[indice_pile_bloc] == 0) { // si la dernière fonction utilisée est une boucle 
                    fprintf(target, "do \n");
                    indice_pile_bloc += 1;
                } 
                else if (pile_bloc[indice_pile_bloc] == 1){ // si la dernière fonction utilisée est un condition 
                    fprintf(target, " then begin\n");
                    indice_pile_bloc += 1;
                }
                else if (pile_bloc[indice_pile_bloc] == 5){ // si la dernière fonction utilisée est un condition 
                    fprintf(target, " begin \n");
                    indice_pile_bloc += 1;
                }
                else if (pile_bloc[indice_pile_bloc]==4){//si la dernière fonction utilisée est une boucle for 
                        fprintf(target, "do \n");
                        indice_pile_bloc +=1;
                    }
                else{ //si la dernière fonction utilisée est une fonction 
                    fprintf(target, " =\n");
                    indice_pile_bloc += 1;
                }
            } else if (strcmp(lex->content,"}")==0)
            {
                printf("%d\n",pile_bloc[indice_pile_bloc-1]);
                if (indice_pile_bloc>0){
                    if (pile_bloc[indice_pile_bloc-1]==0){ // si la dernière fonction utilisée est une boucle while
                        indentation(indice_pile_bloc, target);
                        fprintf(target, "done;\n");
                        indice_pile_bloc -= 1;
                    } 
                    else if (pile_bloc[indice_pile_bloc-1]==3){// si la dernière fonction utlisée est une fonction avec une valeur de retour
                        indentation(indice_pile_bloc, target);
                        fprintf(target,"!res\n"); //affiche le résultat de res
                        indice_pile_bloc -= 1;
                    } 
                    else if (pile_bloc[indice_pile_bloc-1]==2){// si la dernière fonction utlisée est une fonction sans valeur de retour
                        indentation(indice_pile_bloc, target);
                        indice_pile_bloc -= 1;}
                    else if (pile_bloc[indice_pile_bloc-1]==4){//si la dernière fonction utilisée est une boucle for 
                        indentation(indice_pile_bloc,target);
                        fprintf(target, "done;\n");
                        indice_pile_bloc -=1;
                    }
                    else
                    { // si la dernière fonction utilisée est une condtion
                        indentation(indice_pile_bloc, target); 
                        fprintf(target, "end\n");
                        indice_pile_bloc -= 1;
                    }
                    // Dans tous les cas, si on sort de tous les blocs, on ;;
                    if (indice_pile_bloc == 0)
                    {
                        fprintf(target, ";;\n");
                    }
                }
            }
        }
        else if (lex->type == LxmOperator) {
            // Cas spéciaux si les opérateurs ne sont pas les mêmes
            if (!strcmp(lex->content, "=="))
            {
                fprintf(target, " = ");
            } else {
                // Cas de base
                fprintf(target, " %s ", lex->content);   
            }
        }
        else if(lex->type == LxmComment) { //cas pour les commentaires
            indentation(indice_pile_bloc, target);
            fprintf(target, "(*");
            fprintf(target, "%s", lex->content);
            fprintf(target, "*)\n");
        }
        else if(lex->type == LxmKeyWord)
        {
            if (strcmp(lex->content, "printf") == 0)
            { //cas pour printf
                indentation(indice_pile_bloc, target);
                fprintf(target, "Printf.printf ");
                lex=lex->next; //on passe pour passer la parenthèse
                compteur_parenthese = 0; //initialisation du compteur de parenthèse après une fonction à 0
            }
            else if (strcmp(lex->content, "return")==0){ //défini une variable pour le return
                indentation(indice_pile_bloc, target);
                fprintf(target, "let res = ref (");
                open_par_assignation=true;
                retour=true;
            
            }
            else
            {    if (strcmp(lex->content,"for")==0){
                lex=lex->next->next->next; // skip parenthèse + type
                indentation(indice_pile_bloc, target);
                fprintf(target,"let %s = ref (",lex->content);
                noms_reference[len_ref]=lex->content;
                len_ref+=1;
                lex=lex->next->next; //skip =
                if (lex->type=LxmInt){
                    fprintf(target,"%s-%s ) in \n",lex->content,lex->next->next->next->next->next->next->next->next->next->content); //affiche le nombre
                }
                else{
                    fprintf(target,"!%s-%s ) in \n",lex->content,lex->next->next->next->next->next->next->next->next->next->content); //affiche la valeur de la variable
                }
                lex=lex->next;
                indentation(indice_pile_bloc,target);
                fprintf(target,"while (");
                pile_bloc[indice_pile_bloc] = 0; //ajout d'une boucle for dans la pile
                indice_pile_bloc+=1;
                lex=lex->next; //skip le ;
                fprintf(target,"!%s ",lex->content);//print variable
                lex=lex->next;
                fprintf(target,"%s ",lex->content);//print condition
                lex=lex->next;
                fprintf(target,"%s ) do\n ",lex->content);//print valeur + fin
                lex=lex->next->next;
                indentation(indice_pile_bloc,target);
                fprintf(target,"%s := !%s %s %s;\n",lex->content,lex->content,lex->next->content,lex->next->next->next->content);
                lex=lex->next->next->next->next->next;
            }
            else{
                indentation(indice_pile_bloc, target);
                fprintf(target, "%s ",lex->content); //Cas des mots clés pour les boucles
                if (strcmp(lex->content, "while") == 0)
                {
                    pile_bloc[indice_pile_bloc] = 0; //ajout d'une boucle while dans la pile
                }
                if (strcmp(lex->content, "if") == 0)
                {
                    pile_bloc[indice_pile_bloc] = 1; //ajout d'une condition dans la pile 
                }
                if (strcmp(lex->content, "else")==0)
                {
                    pile_bloc[indice_pile_bloc] = 5; //ajout d'une condition dans la pile 
                }
            }
        }
    }
    }
    free(pile_bloc);
}

int main(int argc, char* argv[])
{
    if (argc == 1)
    { // On traduit nos fichiers de test.
        char source_file_path[17] = "./tests/etapeN.c";
        char target_file_path[18] = "./tests/etapeN.ml";
        for (int i = 1; i < 7; i += 1)
        {
            // Modifie le numéro du test
            source_file_path[13] = i + '0';
            target_file_path[13] = i + '0';
            FILE* source_file = fopen(source_file_path, "r");
            FILE* target_file = fopen(target_file_path, "w");
            lexeme_list* l = lexeur(source_file);

            traducteur(l, target_file);
            printf(">> Fichier 'etape%d.c' traduit.\n", i);
            free_list(l);
        }
    } else if (argc == 2)
    { // On execute le fichier spécifié.
        char* source_path = argv[1];
        int str_length = strlen(source_path);
        // Récupère le nom du fichier.
        char* target_path = malloc((str_length + 1) * sizeof(char));
        for (int i = 0; i < str_length - 1; i += 1)
        {
            target_path[i] = source_path[i];
        }
        target_path[str_length - 1] = 'm';
        target_path[str_length] = 'l';
        target_path[str_length + 1] = '\0';

        FILE* source_file = fopen(source_path, "r");
        FILE* target_file = fopen(target_path, "w");

        lexeme_list* l = lexeur(source_file);
        traducteur(l, target_file);
        printf(">> Le fichier %s a été créé.", target_path);
        
        free_list(l);        
    } else {
        printf("Erreur: trop d'arguments. Il faut 0 ou 1 argument (le chemin du fichier à traduire).\n");
    }
}
