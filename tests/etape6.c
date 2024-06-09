// Fonction pour calculer la factorielle d'un nombre
int factorielle(int n) {
    if (n <= 1) {
        return 1;
    } else {
        return n * factorielle(n - 1);
    }
}

int main() {
    int resultat = factorielle(5);
    printf("Le résultat est %d\n", resultat);
}
