// Fonction pour calculer la somme des entiers de 1 à n
int sommejusquan(int n) {
    int somme = 0;
    for (int i = 1; i <= n; i+=1) {
        somme = somme + i;
    }
    return somme;
}

int main() {
    int n = 5;
    int resultat = sommejusquan(n);
    printf("La somme des entiers de 1 à %d est : %d\n", n, resultat);
}