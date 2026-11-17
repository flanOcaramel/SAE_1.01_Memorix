#ifndef CREATION_H
#define CREATION_H

#include "jeu.h"

/* Prepare une nouvelle partie :
 *   - deux exemplaires de chaque chiffre de 1 a NB_PAIRES,
 *   - melange aleatoire des cases,
 *   - toutes les cases cachees et scores a 0,
 *   - le joueur 0 commence (c'est-a-dire le "joueur 1" affiche).
 *
 * Parametres :
 *   partie    : la partie a initialiser (modifiee sur place).
 *   nbJoueurs : nombre de joueurs choisi au clavier (1 a NB_JOUEURS_MAX).
 *               Il est recopie dans partie->nbJoueurs pour que le reste du
 *               programme sache a qui appartient le tour et comment faire
 *               tourner les joueurs.
 * Retour    : aucun.
 */
void creerPartie(Partie *partie, int nbJoueurs);

#endif /* CREATION_H */
