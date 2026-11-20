#include "../include/ia.h"

#include <stdlib.h>

static int estJouable(const Partie *partie, int index, int premiere)
{
    return !partie->revelees[index] && index != premiere;
}

static int choisirAuHasard(const Partie *partie, int premiere,
                           int seulementInconnues)
{
    int possibles[NB_CASES];
    int nbPossibles = 0;
    int i;

    /* On construit la liste des cases possibles avant de tirer : cela evite
       une boucle qui retente au hasard et peut tourner longtemps. */
    for (i = 0; i < NB_CASES; i++) {
        if (!estJouable(partie, i, premiere)) {
            continue;
        }
        if (seulementInconnues && partie->vu[i] != -1) {
            continue;
        }
        possibles[nbPossibles] = i;
        nbPossibles++;
    }

    /* Si toutes les cases jouables sont deja connues, on accepte n'importe
       laquelle. */
    if (nbPossibles == 0) {
        for (i = 0; i < NB_CASES; i++) {
            if (estJouable(partie, i, premiere)) {
                possibles[nbPossibles] = i;
                nbPossibles++;
            }
        }
    }

    return possibles[rand() % nbPossibles];
}

void iaNoterCaseVue(Partie *partie, int index)
{
    partie->vu[index] = partie->valeurs[index];
}

int choisirCaseOrdinateur(const Partie *partie, int premiere)
{
    int i;
    int j;

    /* Si la premiere case est connue, on joue son partenaire s'il est connu. */
    if (premiere != -1 && partie->vu[premiere] != -1) {
        for (j = 0; j < NB_CASES; j++) {
            if (estJouable(partie, j, premiere)
                && partie->vu[j] == partie->vu[premiere]) {
                return j;
            }
        }
    }

    /* Au premier choix, on cherche deux cases connues identiques. */
    if (premiere == -1) {
        for (i = 0; i < NB_CASES; i++) {
            if (!estJouable(partie, i, -1) || partie->vu[i] == -1) {
                continue;
            }
            for (j = i + 1; j < NB_CASES; j++) {
                if (estJouable(partie, j, -1)
                    && partie->vu[j] == partie->vu[i]) {
                    return i;
                }
            }
        }
    }

    /* Sinon, on retourne une case encore inconnue. */
    return choisirAuHasard(partie, premiere, 1);
}
