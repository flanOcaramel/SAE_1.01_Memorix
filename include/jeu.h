#ifndef JEU_H
#define JEU_H

#define NB_LIGNES 3
#define NB_COLONNES 7
/* 3 * 7 = 21 cases : 10 paires + le Joker. */
#define NB_CASES (NB_LIGNES * NB_COLONNES)
/* On retire le Joker avant de diviser par 2. */
#define NB_PAIRES ((NB_CASES - 1) / 2)
#define NB_JOUEURS_MAX 4

/* Le Joker vaut 0 : les chiffres des paires vont de 1 a NB_PAIRES. */
#define VALEUR_JOKER 0

typedef struct {
    int valeurs[NB_CASES];             /* chiffre cache dans chaque case */
    int revelees[NB_CASES];            /* 1 = paire deja trouvee */
    int score;                         /* paires trouvees au total */
    int nbJoueurs;                     /* 1 a NB_JOUEURS_MAX */
    int joueurCourant;                 /* indice de 0 a nbJoueurs - 1 */
    int scores[NB_JOUEURS_MAX];        /* points de chaque joueur */
    int estOrdinateur[NB_JOUEURS_MAX]; /* 1 = IA, 0 = humain */
    int vu[NB_CASES];                  /* -1 = inconnue, sinon le chiffre vu */
} Partie;

void jouerPartie(void);

#endif /* JEU_H */
