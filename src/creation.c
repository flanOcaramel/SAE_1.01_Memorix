#include "../include/creation.h"

#include <stdlib.h> /* rand */

/* --------------------------------------------------------------------------
 * melanger - Melange un tableau de NB_CASES entiers.
 *
 * Algorithme de Fisher-Yates : on parcourt le tableau de la fin vers le
 * debut, et on echange chaque case avec une case tiree au hasard avant elle.
 * Chaque ordre possible a ainsi la meme chance d'apparaitre.
 * -------------------------------------------------------------------------- */
static void melanger(int valeurs[NB_CASES])
{
    int i;

    for (i = NB_CASES - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temporaire = valeurs[i];

        valeurs[i] = valeurs[j];
        valeurs[j] = temporaire;
    }
}

/* --------------------------------------------------------------------------
 * creerPartie - Prepare une nouvelle partie complete.
 *
 * Parametres :
 *   partie    : pointeur vers la partie a remplir (modifiee sur place).
 *   nbJoueurs : nombre de joueurs (deja valide par le module ui).
 * Retour : aucun.
 * -------------------------------------------------------------------------- */
void creerPartie(Partie *partie, int nbJoueurs)
{
    int i;

    /* Etape 1 : deux exemplaires de chaque chiffre de 1 a NB_PAIRES.
       La division entiere i / 2 groupe les indices par deux :
       i = 0,1 -> 1 ; i = 2,3 -> 2 ; etc.
       Le plateau etant unique, cette etape n'est faite qu'une seule fois,
       quel que soit le nombre de joueurs. */
    for (i = 0; i < NB_CASES; i++) {
        partie->valeurs[i] = i / 2 + 1;
        partie->revelees[i] = 0;
    }

    /* Etape 2 : initialisation de la partie multi-joueurs. */
    partie->score = 0;             /* aucune paire trouvee au depart */
    partie->nbJoueurs = nbJoueurs; /* on garde le choix de l'utilisateur */
    partie->joueurCourant = 0;     /* indice 0 = premier joueur affiche */

    /* On met TOUS les scores a 0, pas seulement les nbJoueurs premiers :
       ainsi aucune case du tableau ne reste avec une valeur indeterminee. */
    for (i = 0; i < NB_JOUEURS_MAX; i++) {
        partie->scores[i] = 0;
    }

    /* Etape 3 : on melange pour que la position des paires soit aleatoire. */
    melanger(partie->valeurs);
}
