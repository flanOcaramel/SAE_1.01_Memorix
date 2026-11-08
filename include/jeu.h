#ifndef JEU_H
#define JEU_H

/* --------------------------------------------------------------------------
 * Dimensions du plateau : 4 lignes x 5 colonnes = 20 cases.
 * On utilise des constantes symboliques pour pouvoir changer la taille
 * du plateau sans toucher au reste du code.
 * -------------------------------------------------------------------------- */
#define NB_LIGNES 4
#define NB_COLONNES 5
#define NB_CASES (NB_LIGNES * NB_COLONNES)

#define NB_PAIRES (NB_CASES / 2)

/* --------------------------------------------------------------------------
 * Partie : tout l'etat d'une partie, regroupe au meme endroit.
 *
 *   valeurs  : le chiffre cache dans chaque case (de 1 a NB_PAIRES).
 *   revelees : 1 si la case fait partie d'une paire deja trouvee, 0 sinon.
 *   score    : nombre de paires deja trouvees.
 * -------------------------------------------------------------------------- */
typedef struct {
    int valeurs[NB_CASES];
    int revelees[NB_CASES];
    int score;
} Partie;

void jouerPartie(void);

#endif /* JEU_H */
