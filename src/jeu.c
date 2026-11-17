#include "../include/jeu.h"

#include "../include/creation.h"
#include "../include/ui.h"

/* --------------------------------------------------------------------------
 * jouerPartie - Deroule une partie complete de Memory (1 a 4 joueurs).
 *
 *
 * les paires deja trouvees par un joueur restent visibles pour tout le monde.
 * Seul le tour (joueurCourant) et les points (scores[]) changent d'un joueur
 * a l'autre.
 *
 * Regle de rotation choisie :
 *   - paire trouvee  -> le joueur GARDE LA MAIN (il rejoue tout de suite) ;
 *   - paire ratee    -> le tour passe au joueur suivant.
 * -------------------------------------------------------------------------- */
void jouerPartie(void)
{
    Partie partie;
    int nbJoueurs;
    int case1;
    int case2;

    uiInitialiser();

    /* On demande le nombre de joueurs avant de creer la partie, car cette
       information est stockee dans la structure Partie. */
    nbJoueurs = uiDemanderNombreJoueurs();
    if (nbJoueurs < 0) {
        /* L'utilisateur a ferme l'entree : on s'arrete proprement. */
        uiAfficherInterruption();
        return;
    }

    /* Une seule creation de plateau, commune a tous les joueurs. */
    creerPartie(&partie, nbJoueurs);

    /* On continue tant qu'il reste des paires a trouver. "score" est le
       total des paires trouvees par TOUS les joueurs confondus. */
    while (partie.score < NB_PAIRES) {
        /* --- Affichage du plateau et du joueur actif avant le 1er choix --- */
        uiEffacerEcran();
        uiAfficherPartie(&partie, -1, -1); /* -1 : aucune case ouverte */

        /* --- Premier choix --- */
        case1 = uiDemanderCase(&partie, -1, 1);
        if (case1 < 0) {
            uiAfficherInterruption();
            return;
        }

        /* On redessine le plateau en laissant la 1re case visible, pour que
           le joueur se souvienne de ce qu'il a vu. */
        uiEffacerEcran();
        uiAfficherPlateau(&partie, case1, -1);

        /* --- Second choix (different de case1 et non deja gagne) --- */
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
            /* Meme chiffre : la paire est gagnee et reste visible pour tous. */
            partie.revelees[case1] = 1;
            partie.revelees[case2] = 1;
            partie.score++;                          /* +1 au total general */
            partie.scores[partie.joueurCourant]++;   /* +1 au joueur actif */

            /* On ne touche PAS a joueurCourant : le joueur rejoue. */
            uiEffacerEcran();
            uiAfficherPartie(&partie, -1, -1);
            uiAfficherPaireTrouvee(partie.valeurs[case1], partie.valeurs[case2]);
        } else {
            /* Chiffres differents : on previent le joueur et on attend
               qu'il valide avant de cacher les cases au tour suivant. */
            uiAfficherPaireRatee(partie.valeurs[case1], partie.valeurs[case2]);
            uiAttendreEntree("Appuie sur Entree pour retourner les cases...");

            /* Paire ratee : on passe au joueur suivant.
               Le "% nbJoueurs" fait boucler apres le dernier joueur :
               indice 3 -> 0 quand il y a 4 joueurs, indice 1 -> 0 avec 2. */
            partie.joueurCourant = (partie.joueurCourant + 1) % partie.nbJoueurs;
        }
    }

    /* --- Fin de partie : toutes les paires ont ete trouvees --- */
    uiEffacerEcran();
    uiAfficherPartie(&partie, -1, -1);
    uiAfficherVictoire(&partie);
}
