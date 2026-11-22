#ifndef UI_H
#define UI_H

#include "jeu.h"

void uiInitialiser(void);
int uiDemanderNombreJoueurs(void);
int uiDemanderTypeJoueurs(int nbJoueurs,
                          int estOrdinateur[NB_JOUEURS_MAX]);
void uiEffacerEcran(void);
void uiPause(int millisecondes);

/* case1 / case2 : cases retournees pendant le tour, ou -1. */
void uiAfficherPartie(const Partie *partie, int case1, int case2);
void uiAfficherPlateau(const Partie *partie, int case1, int case2);

void uiAfficherChoixOrdinateur(int numeroCase);
void uiAfficherPaireTrouvee(int a, int b);
void uiAfficherPaireRatee(int a, int b);
void uiAfficherVictoire(const Partie *partie);
void uiAfficherInterruption(void);
void uiAttendreEntree(const char *message);

/* premiere : case deja choisie au tour, ou -1. */
int uiDemanderCase(const Partie *partie, int premiere, int numeroChoix);

#endif /* UI_H */
