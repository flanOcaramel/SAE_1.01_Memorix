#include <stdlib.h> /* srand */
#include <time.h>   /* time */

#include "../include/jeu.h"

/* --------------------------------------------------------------------------
 * main - Point d'entree du programme.
 *
 * Il reste volontairement tres court : main initialise le hasard puis
 * laisse le module jeu derouler la partie.
 * -------------------------------------------------------------------------- */
int main(void)
{
    /* Sans srand, rand() renverrait toujours la meme suite de nombres.
       On utilise l'heure courante pour changer le melange a chaque partie. */
    srand((unsigned)time(NULL));

    jouerPartie();

    return 0;
}
