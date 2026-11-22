#include "../include/jeu.h"

#include "../include/creation.h"
#include "../include/ia.h"
#include "../include/ui.h"

/* 3 secondes entre les cartes retournees par l'ordinateur.
   La valeur peut etre remplacee a la compilation avec -DPAUSE_IA=... */
#ifndef PAUSE_IA
#define PAUSE_IA 3000
#endif

void jouerPartie(void)
{
    Partie partie;
    int nbJoueurs;
    int estOrdinateur[NB_JOUEURS_MAX];
    int case1;
    int case2;

    uiInitialiser();

    nbJoueurs = uiDemanderNombreJoueurs();
    if (nbJoueurs < 0) {
        uiAfficherInterruption();
        return;
    }
    if (uiDemanderTypeJoueurs(nbJoueurs, estOrdinateur) < 0) {
        uiAfficherInterruption();
        return;
    }

    creerPartie(&partie, nbJoueurs, estOrdinateur);

    while (partie.score < NB_PAIRES) {
        int tourIA = partie.estOrdinateur[partie.joueurCourant];

        uiEffacerEcran();
        uiAfficherPartie(&partie, -1, -1);

        if (tourIA) {
            case1 = choisirCaseOrdinateur(&partie, -1);
        } else {
            case1 = uiDemanderCase(&partie, -1, 1);
        }
        if (case1 < 0) {
            uiAfficherInterruption();
            return;
        }

        /* Tous les joueurs voient la carte : l'IA la memorise aussi. */
        iaNoterCaseVue(&partie, case1);

        uiEffacerEcran();
        uiAfficherPlateau(&partie, case1, -1);
        if (tourIA) {
            uiAfficherChoixOrdinateur(case1 + 1);
            uiPause(PAUSE_IA);
        }

        if (tourIA) {
            case2 = choisirCaseOrdinateur(&partie, case1);
        } else {
            case2 = uiDemanderCase(&partie, case1, 2);
        }
        if (case2 < 0) {
            uiAfficherInterruption();
            return;
        }

        iaNoterCaseVue(&partie, case2);

        uiEffacerEcran();
        uiAfficherPlateau(&partie, case1, case2);
        if (tourIA) {
            uiAfficherChoixOrdinateur(case2 + 1);
            uiPause(PAUSE_IA);
        }

        if (partie.valeurs[case1] == partie.valeurs[case2]) {
            partie.revelees[case1] = 1;
            partie.revelees[case2] = 1;
            partie.score++;
            partie.scores[partie.joueurCourant]++;

            /* Paire trouvee : le joueur rejoue (joueurCourant ne change pas). */
            uiEffacerEcran();
            uiAfficherPartie(&partie, -1, -1);
            uiAfficherPaireTrouvee(partie.valeurs[case1], partie.valeurs[case2]);
        } else {
            uiAfficherPaireRatee(partie.valeurs[case1], partie.valeurs[case2]);
            uiAttendreEntree("Appuie sur Entree pour retourner les cases...");

            /* Paire ratee : joueur suivant. Le modulo revient au joueur 0
               apres le dernier. */
            partie.joueurCourant = (partie.joueurCourant + 1) % partie.nbJoueurs;
        }
    }

    uiEffacerEcran();
    uiAfficherPartie(&partie, -1, -1);
    uiAfficherVictoire(&partie);
}
