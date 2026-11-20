#include "../include/jeu.h"

#include <stdlib.h>

#include "../include/creation.h"
#include "../include/ia.h"
#include "../include/ui.h"

/* 3 secondes entre les cartes retournees par l'ordinateur.
   La valeur peut etre remplacee a la compilation avec -DPAUSE_IA=... */
#ifndef PAUSE_IA
#define PAUSE_IA 2000
#endif

/* Echange le Joker avec une carte encore cachee tiree au hasard.
   La cible peut etre le Joker lui-meme : il reste alors sur place. */
static void permuterJoker(Partie *partie, int caseJoker)
{
    int cachees[NB_CASES];
    int nbCachees = 0;
    int i;
    int cible;

    for (i = 0; i < NB_CASES; i++) {
        if (!partie->revelees[i]) {
            cachees[nbCachees] = i;
            nbCachees++;
        }
    }

    /* Cible tiree parmi les cases cachees ; elle peut etre le Joker. */
    cible = cachees[rand() % nbCachees];

    if (cible != caseJoker) {
        int temporaire = partie->valeurs[caseJoker];

        partie->valeurs[caseJoker] = partie->valeurs[cible];
        partie->valeurs[cible] = temporaire;
        partie->vu[cible] = -1;
    }

    /* La permutation est invisible : meme si le Joker n'a pas bouge,
       les joueurs ne peuvent plus se fier a ce qu'ils ont memorise. */
    partie->vu[caseJoker] = -1;
}

static void joueurSuivant(Partie *partie)
{
    partie->joueurCourant = (partie->joueurCourant + 1) % partie->nbJoueurs;
}

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
        }

        /* Joker en premiere carte : le joueur passe son tour sans en
           retourner une deuxieme. */
        if (partie.valeurs[case1] == VALEUR_JOKER) {
            uiAfficherJoker();
            if (tourIA) {
                uiPause(PAUSE_IA);
            } else {
                uiAttendreEntree("Appuie sur Entree pour continuer...");
            }
            permuterJoker(&partie, case1);
            joueurSuivant(&partie);
            /* Tour termine : on repart au debut de la boucle sans
               demander de deuxieme case. */
            continue;
        }

        if (tourIA) {
            uiPause(PAUSE_IA);
        }

        if (tourIA) {
            case2 = choisirCaseOrdinateur(&partie, case1);
        } else {
            case2 = uiDemanderCase(&partie, case1, 2);
        }
        /* Valeur negative : saisie interrompue (EOF). On arrete la partie
           avant d'utiliser case2 comme indice du tableau. */
        if (case2 < 0) {
            uiAfficherInterruption();
            return;
        }

        iaNoterCaseVue(&partie, case2);

        uiEffacerEcran();
        uiAfficherPlateau(&partie, case1, case2);
        if (tourIA) {
            uiAfficherChoixOrdinateur(case2 + 1);
        }

        /* Joker en deuxieme carte : ce n'est pas une paire, le joueur
           passe son tour. */
        if (partie.valeurs[case2] == VALEUR_JOKER) {
            uiAfficherJoker();
            if (tourIA) {
                uiPause(PAUSE_IA);
            } else {
                uiAttendreEntree("Appuie sur Entree pour continuer...");
            }
            permuterJoker(&partie, case2);
            joueurSuivant(&partie);
            continue;
        }

        if (tourIA) {
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
            joueurSuivant(&partie);
        }
    }

    uiEffacerEcran();
    uiAfficherPartie(&partie, -1, -1);
    uiAfficherVictoire(&partie);
}
