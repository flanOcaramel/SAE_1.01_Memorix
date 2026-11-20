#include "../include/creation.h"

#include <stdlib.h>

static void melanger(int valeurs[NB_CASES])
{
    int i;

    /* Fisher-Yates : on echange chaque case avec une case tiree au hasard
       parmi celles qui la precedent. */
    for (i = NB_CASES - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temporaire = valeurs[i];

        valeurs[i] = valeurs[j];
        valeurs[j] = temporaire;
    }
}

void creerPartie(Partie *partie, int nbJoueurs,
                 const int estOrdinateur[NB_JOUEURS_MAX])
{
    int i;

    /* Deux exemplaires de chaque chiffre : i / 2 vaut 0 pour i = 0 et 1,
       1 pour i = 2 et 3, etc. Le +1 evite le chiffre 0. */
    for (i = 0; i < NB_CASES - 1; i++) {
        partie->valeurs[i] = i / 2 + 1;
        partie->revelees[i] = 0;
        partie->vu[i] = -1;
    }

    /* La derniere carte du paquet est le Joker. */
    partie->valeurs[NB_CASES - 1] = VALEUR_JOKER;
    partie->revelees[NB_CASES - 1] = 0;
    partie->vu[NB_CASES - 1] = -1;

    partie->score = 0;
    partie->nbJoueurs = nbJoueurs;
    partie->joueurCourant = 0;

    for (i = 0; i < NB_JOUEURS_MAX; i++) {
        partie->scores[i] = 0;
        partie->estOrdinateur[i] = estOrdinateur[i];
    }

    melanger(partie->valeurs);
}
