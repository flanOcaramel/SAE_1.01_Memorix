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

/* Nombre maximum de joueurs autorises. Le joueur choisit au debut de la
   partie entre 1 et NB_JOUEURS_MAX (donc 1 a 4). Les tableaux de scores sont
   dimensionnes avec cette constante : elle borne la taille max du tableau. */
#define NB_JOUEURS_MAX 4

/* --------------------------------------------------------------------------
 * Partie : tout l'etat d'une partie, regroupe au meme endroit.
 *
 * Attention : il n'y a qu'UNE SEULE structure Partie, donc un seul plateau,
 * partage par tous les joueurs. Les chiffres caches et les cases deja
 * trouvees sont communs ; seuls les points sont propres a chaque joueur.
 *
 *   valeurs       : le chiffre cache dans chaque case (de 1 a NB_PAIRES).
 *   revelees      : 1 si la case fait partie d'une paire deja trouvee, 0 sinon.
 *   score         : total des paires trouvees (tous joueurs confondus).
 *                   Sert a savoir quand la partie est terminee : il vaut
 *                   NB_PAIRES quand le plateau est vide.
 *   nbJoueurs     : nombre de joueurs choisi au debut (1 a NB_JOUEURS_MAX).
 *   joueurCourant : indice du joueur qui doit jouer (0 a nbJoueurs-1).
 *                   On affiche joueurCourant + 1 au joueur, car les indices
 *                   C commencent a 0 alors que les joueurs sont numerotes
 *                   de 1 a nbJoueurs.
 *   scores        : points de chaque joueur. scores[k] = nombre de paires
 *                   trouvees par le joueur d'indice k. La somme des scores
 *                   est toujours egale a "score" (une paire = un point).
 * -------------------------------------------------------------------------- */
typedef struct {
    int valeurs[NB_CASES];
    int revelees[NB_CASES];
    int score;
    int nbJoueurs;
    int joueurCourant;
    int scores[NB_JOUEURS_MAX];
} Partie;

void jouerPartie(void);

#endif /* JEU_H */
