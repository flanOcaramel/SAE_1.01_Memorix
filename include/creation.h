#ifndef CREATION_H
#define CREATION_H

#include "jeu.h"

/* Prepare une nouvelle partie :
 *   - deux exemplaires de chaque chiffre de 1 a NB_PAIRES,
 *   - melange aleatoire des cases,
 *   - toutes les cases cachees et score a 0.
 *
 * Parametre : partie a initialiser (modifiee sur place).
 * Retour    : aucun.
 */
void creerPartie(Partie *partie);

#endif /* CREATION_H */
