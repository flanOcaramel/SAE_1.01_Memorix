#ifndef UI_H
#define UI_H

#include "jeu.h"

/* Active les couleurs ANSI (a appeler une seule fois au debut). */
void uiInitialiser(void);

/* Demande au joueur combien de personnes vont jouer.
 * La saisie est redemandee tant qu'elle n'est pas un entier seul compris
 * entre 1 et NB_JOUEURS_MAX (donc 1 a 4).
 * Retour : le nombre de joueurs choisi, ou -1 si l'entree est terminee
 *          (Ctrl+D / Ctrl+Z). */
int uiDemanderNombreJoueurs(void);

/* Efface l'ecran visible ET l'historique de defilement (scrollback),
 * puis replace le curseur en haut a gauche. */
void uiEffacerEcran(void);

/* Affiche l'entete, le joueur dont c'est le tour, le plateau (commun a
 * tous), le score total puis le score de chaque joueur.
 * case1 / case2 : indices des cases retournees pendant le tour, ou -1. */
void uiAfficherPartie(const Partie *partie, int case1, int case2);

/* Affiche uniquement le plateau (sans l'entete ni le score). */
void uiAfficherPlateau(const Partie *partie, int case1, int case2);

/* Affiche un message vert quand une paire est trouvee. */
void uiAfficherPaireTrouvee(int a, int b);

/* Affiche un message rouge quand les deux cases ne forment pas une paire. */
void uiAfficherPaireRatee(int a, int b);

/* Affiche le message de fin de partie, le score total et le classement
 * des joueurs (trie par nombre de paires trouvees). */
void uiAfficherVictoire(const Partie *partie);

/* Affiche le message d'interruption (Ctrl+D / Ctrl+Z). */
void uiAfficherInterruption(void);

/* Affiche un message puis attend que le joueur appuie sur Entree. */
void uiAttendreEntree(const char *message);

/* Demande une case jouable au joueur.
 *   premiere    : case deja choisie dans ce tour (a interdire), ou -1.
 *   numeroChoix : 1 ou 2, pour afficher "Case 1" ou "Case 2".
 * Retour : l'indice de la case choisie (0 a NB_CASES-1), ou -1 si
 *          l'entree est terminee (Ctrl+D / Ctrl+Z). */
int uiDemanderCase(const Partie *partie, int premiere, int numeroChoix);

#endif /* UI_H */
