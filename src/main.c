#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NB_LIGNES 3
#define NB_COLONNES 7
#define NB_CASES (NB_LIGNES * NB_COLONNES)

/* Cherche si la paire (a, b) est deja dans le tableau.
   On considere que (4, 8) et (8, 4) sont la meme paire.
   Renvoie 1 si elle existe deja, 0 sinon. */
int paireDejaUtilisee(int paires[][2], int nbPaires, int a, int b)
{
    int i;

    for (i = 0; i < nbPaires; i++) {
        int x = paires[i][0];
        int y = paires[i][1];

        if ((x == a && y == b) || (x == b && y == a)) {
            return 1;
        }
    }

    return 0;
}

/* Affiche un tableau de 3 lignes et 7 colonnes.
   Chaque case contient une paire de nombres entre 1 et 10,
   et aucune paire n'apparait deux fois. */
void afficherTableau(void)
{
    int paires[NB_CASES][2]; /* memoire de toutes les paires deja affichees */
    int nbPaires = 0;        /* nombre de paires deja affichees */
    int ligne;
    int colonne;

    for (ligne = 0; ligne < NB_LIGNES; ligne++) {
        for (colonne = 0; colonne < NB_COLONNES; colonne++) {
            int a;
            int b;

            /* On retire une paire tant qu'elle existe deja. */
            do {
                a = rand() % 10 + 1;
                b = rand() % 10 + 1;
            } while (paireDejaUtilisee(paires, nbPaires, a, b));

            /* On garde la paire en memoire pour les cases suivantes. */
            paires[nbPaires][0] = a;
            paires[nbPaires][1] = b;
            nbPaires++;

            printf("(%2d, %2d) ", a, b);
        }
        printf("\n");
    }
}

int main(void)
{
    srand(time(NULL));

    afficherTableau();

    return 0;
}
