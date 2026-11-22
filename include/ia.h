#ifndef IA_H
#define IA_H

#include "jeu.h"

/* L'IA ne lit jamais valeurs[] directement : elle utilise seulement vu[],
   la memoire des cartes deja montrees a l'ecran. */
void iaNoterCaseVue(Partie *partie, int index);
int choisirCaseOrdinateur(const Partie *partie, int premiere);

#endif /* IA_H */
