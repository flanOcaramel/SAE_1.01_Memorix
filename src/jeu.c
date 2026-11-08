#include "../include/jeu.h"

#include "../include/creation.h"
#include "../include/ui.h"

/* --------------------------------------------------------------------------
 * jouerPartie - Deroule une partie complete de Memory.
 *
 * Le module jeu ne fait aucun affichage et ne lit rien au clavier :
 * il appelle le module ui pour communiquer avec le joueur. La logique
 * reste ainsi separee de la presentation.
 * -------------------------------------------------------------------------- */
void jouerPartie(void)
{
    Partie partie;
    int case1;
    int case2;

    uiInitialiser();
    creerPartie(&partie);

    /* On continue tant que toutes les paires ne sont pas trouvees. */
    while (partie.score < NB_PAIRES) {
        /* --- Affichage du plateau avant le premier choix --- */
        uiEffacerEcran();
        uiAfficherPartie(&partie, -1, -1); /* -1 : aucune case ouverte */

        /* --- Premier choix --- */
        case1 = uiDemanderCase(&partie, -1, 1);
        if (case1 < 0) {
            uiAfficherInterruption();
            return;
        }

        /* On redessine le plateau en laissant la 1re case visible. */
        uiEffacerEcran();
        uiAfficherPlateau(&partie, case1, -1);

        /* --- Second choix (different de case1 et non deja gagnee) --- */
        case2 = uiDemanderCase(&partie, case1, 2);
        if (case2 < 0) {
            uiAfficherInterruption();
            return;
        }

        /* On montre les deux cases retournees en meme temps. */
        uiEffacerEcran();
        uiAfficherPlateau(&partie, case1, case2);

        /* --- Verification de la paire --- */
        if (partie.valeurs[case1] == partie.valeurs[case2]) {
            /* Meme chiffre : la paire est gagnee et reste visible. */
            partie.revelees[case1] = 1;
            partie.revelees[case2] = 1;
            partie.score++;

            uiEffacerEcran();
            uiAfficherPartie(&partie, -1, -1);
            uiAfficherPaireTrouvee(partie.valeurs[case1], partie.valeurs[case2]);
        } else {
            /* Chiffres differents : on previent le joueur et on attend
               qu'il valide avant de cacher les cases au tour suivant. */
            uiAfficherPaireRatee(partie.valeurs[case1], partie.valeurs[case2]);
            uiAttendreEntree("Appuie sur Entree pour retourner les cases...");
        }
    }

    /* --- Fin de partie : toutes les paires ont ete trouvees --- */
    uiEffacerEcran();
    uiAfficherPartie(&partie, -1, -1);
    uiAfficherVictoire(&partie);
}
