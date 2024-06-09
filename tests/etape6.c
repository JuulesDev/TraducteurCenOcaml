// Fonction pour calculer la factorielle d'un nombre
int factorielle(int n) {
    if (n <= 1) {
        return 1;
    } else {
        return n * factorielle(n - 1);
    }
}

