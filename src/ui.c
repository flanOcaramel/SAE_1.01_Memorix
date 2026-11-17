#include "../include/ui.h"

#include <stdio.h>

#ifdef _WIN32
#include <windows.h> /* GetStdHandle, GetConsoleMode, SetConsoleMode */
#else
#endif

/* --------------------------------------------------------------------------
 * Codes couleur ANSI.
 *   ESC[1;32m : vert gras
 *   ESC[1;31m : rouge gras
 *   ESC[0m    : style par defaut
 * -------------------------------------------------------------------------- */
#define CODE_VERT "\033[1;32m"
#define CODE_ROUGE "\033[1;31m"
#define CODE_RAZ "\033[0m"

/* --------------------------------------------------------------------------
 * uiInitialiser - Prepare le terminal pour les couleurs ANSI.
 *
 * On active les couleurs sans tester isatty : certains terminaux (ou
 * lanceurs de type IDE) ne sont pas reconnus comme des terminaux alors
 * qu'ils comprennent parfaitement les codes ANSI.
 * -------------------------------------------------------------------------- */
void uiInitialiser(void)
{
#ifdef _WIN32
    HANDLE sortie;
    DWORD mode = 0;

    /* Sur Windows, on demande a la console de comprendre les codes ANSI.
       Si cela echoue, les codes seront simplement affiches tels quels. */
    sortie = GetStdHandle(STD_OUTPUT_HANDLE);

    if (sortie != INVALID_HANDLE_VALUE && GetConsoleMode(sortie, &mode)) {
        SetConsoleMode(sortie, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
#endif
}

/* --------------------------------------------------------------------------
 * uiDemanderNombreJoueurs - Demande combien de joueurs vont participer.
 *
 * Parametres : aucun.
 * Retour : 1, 2, 3 ou 4 ; -1 si l'entree est terminee (fin de fichier).
 *
 * Version simple pour debutants : on utilise scanf("%d") dans une boucle
 * do...while. Tant que le nombre n'est pas entre 1 et NB_JOUEURS_MAX, on
 * redemande.
 * -------------------------------------------------------------------------- */
int uiDemanderNombreJoueurs(void)
{
    int nbJoueurs = 0; /* 0 n'est pas valide : ca force le 1er passage */
    int resultat;      /* scanf renvoie le nombre de valeurs lues (1 ici) */
    int c;             /* sert a vider la fin de la ligne apres scanf */

    do {
        printf("Nombre de joueurs (1-%d) : ", NB_JOUEURS_MAX);

        /* scanf attend un nombre et le range dans nbJoueurs. */
        resultat = scanf("%d", &nbJoueurs);

        /* Attention : scanf laisse le "Entree" dans le clavier. On lit les
           caracteres restants jusqu'au '\n' pour ne pas les laisser au
           prochain scanf/fgets (sinon la saisie suivante serait faussee). */
        while ((c = getchar()) != '\n' && c != EOF) {
            /* on ne fait rien : on jette juste les caracteres lus */
        }

        if (resultat == EOF) {
            return -1; /* plus d'entree : on arrete proprement */
        }
        if (resultat != 1) {
            /* L'utilisateur a tape du texte au lieu d'un nombre. */
            printf("Entree invalide : tape un nombre entre 1 et %d.\n",
                   NB_JOUEURS_MAX);
            nbJoueurs = 0; /* valeur impossible : la boucle redemandera */
        }
    } while (nbJoueurs < 1 || nbJoueurs > NB_JOUEURS_MAX);

    return nbJoueurs;
}

void uiEffacerEcran(void)
{
    printf("\033[H\033[2J\033[3J");
    fflush(stdout); /* on force l'effacement immediat */
}

/* --------------------------------------------------------------------------
 * afficherCase - Affiche une case du plateau.
 *
 *   cachee  -> "[ 7]    "
 *   revelee -> "   4    "
 *   gagnee  -> "   4    " (en vert)
 * -------------------------------------------------------------------------- */
static void afficherCase(int numero, int chiffre, int visible, int gagnee)
{
    if (!visible) {
        printf("[%2d]    ", numero);
    } else if (gagnee) {
        printf("%s  %2d    %s", CODE_VERT, chiffre, CODE_RAZ);
    } else {
        printf("  %2d    ", chiffre);
    }
}

/* --------------------------------------------------------------------------
 * uiAfficherPlateau - Affiche les NB_LIGNES x NB_COLONNES cases.
 * -------------------------------------------------------------------------- */
void uiAfficherPlateau(const Partie *partie, int case1, int case2)
{
    int ligne;
    int colonne;

    for (ligne = 0; ligne < NB_LIGNES; ligne++) {
        printf("  ");

        for (colonne = 0; colonne < NB_COLONNES; colonne++) {
            /* Coordonnees 2D -> indice 1D : i = ligne * NB_COLONNES + colonne. */
            int i = ligne * NB_COLONNES + colonne;

            /* Visible si la paire est deja trouvee, ou si c'est l'une des
               deux cases retournees pendant ce tour. */
            int visible = partie->revelees[i] || i == case1 || i == case2;

            /* +1 : le joueur voit des numeros de 1 a NB_CASES. */
            afficherCase(i + 1, partie->valeurs[i], visible, partie->revelees[i]);
        }

        printf("\n");
    }
}

/* --------------------------------------------------------------------------
 * uiAfficherPartie - Entete + plateau + score.
 * -------------------------------------------------------------------------- */
void uiAfficherPartie(const Partie *partie, int case1, int case2)
{
    int j; /* parcourt les joueurs pour afficher leurs scores */

    printf("Memory : %d cases numerotees, %d paires a trouver\n",
           NB_CASES, NB_PAIRES);

    /* +1 : joueurCourant est un indice (0, 1, 2...), alors qu'on affiche
       des joueurs numerotes de 1 a nbJoueurs. */
    printf("Tour du joueur %d / %d\n\n",
           partie->joueurCourant + 1, partie->nbJoueurs);

    /* Un seul plateau pour tout le monde : les cases trouvees par un
       joueur restent visibles pour les suivants. */
    uiAfficherPlateau(partie, case1, case2);

    printf("\nScore total : %d / %d paires\n", partie->score, NB_PAIRES);

    /* Un score par joueur, pour savoir qui a trouve quelles paires.
       La boucle s'arrete a nbJoueurs : les cases inutilisees du tableau
       scores[] ne sont pas affichees. */
    for (j = 0; j < partie->nbJoueurs; j++) {
        printf("  Joueur %d : %d paire(s)\n", j + 1, partie->scores[j]);
    }
    printf("\n");
}

/* --------------------------------------------------------------------------
 * Messages de jeu.
 * -------------------------------------------------------------------------- */
void uiAfficherPaireTrouvee(int a, int b)
{
    printf("%sPaire trouvee : %d et %d -- +1 point%s\n", CODE_VERT, a, b, CODE_RAZ);
}

void uiAfficherPaireRatee(int a, int b)
{
    printf("\n%sCe n'est pas une paire : %d et %d%s\n", CODE_ROUGE, a, b, CODE_RAZ);
}

void uiAfficherVictoire(const Partie *partie)
{
    /* ordre[i] contiendra l'indice du joueur arrive a la i-eme place.
       On trie ces indices au lieu de trier les scores : la partie reste
       intacte et les joueurs gardent leur numero d'origine. */
    int ordre[NB_JOUEURS_MAX];
    int i;
    int j;

    printf("\n%sBravo ! Toutes les paires sont trouvees.%s\n", CODE_VERT, CODE_RAZ);
    printf("Score final : %d / %d\n", partie->score, NB_PAIRES);

    /* A un seul joueur, un classement ne dirait rien de plus que le score. */
    if (partie->nbJoueurs == 1) {
        printf("Joueur 1 : %d paire(s) trouvee(s)\n", partie->scores[0]);
        return;
    }

    /* Etape 1 : ordre = {0, 1, 2, ...} (l'ordre de depart des joueurs). */
    for (i = 0; i < partie->nbJoueurs; i++) {
        ordre[i] = i;
    }

    /* Etape 2 : tri par selection, du score le plus eleve au plus bas. */
    for (i = 0; i < partie->nbJoueurs - 1; i++) {
        for (j = i + 1; j < partie->nbJoueurs; j++) {
            /* On echange seulement si le score suivant est STRICTEMENT
               plus grand : en cas d'egalite, le joueur au plus petit
               numero reste devant (classement stable). */
            if (partie->scores[ordre[j]] > partie->scores[ordre[i]]) {
                int temporaire = ordre[i];
                ordre[i] = ordre[j];
                ordre[j] = temporaire;
            }
        }
    }

    /* Etape 3 : affichage du classement. */
    printf("\nClassement :\n");
    for (i = 0; i < partie->nbJoueurs; i++) {
        int indice = ordre[i]; /* +1 pour afficher un joueur 1, 2, 3 ou 4 */
        printf("  %d. Joueur %d : %d paire(s)\n",
               i + 1, indice + 1, partie->scores[indice]);
    }
}

void uiAfficherInterruption(void)
{
    printf("\nPartie interrompue.\n");
}

/* --------------------------------------------------------------------------
 * uiAttendreEntree - Affiche un message puis attend une ligne au clavier.
 * -------------------------------------------------------------------------- */
void uiAttendreEntree(const char *message)
{
    char ligne[64];

    printf("%s", message);
    fflush(stdout); /* affiche l'invite avant d'attendre la saisie */

    /* On lit une ligne entiere pour consommer aussi le caractere Entree. */
    (void)fgets(ligne, sizeof ligne, stdin);
}

/* --------------------------------------------------------------------------
 * lireChoix - Lit un numero de case valide.
 * Retour : indice 0..NB_CASES-1, ou -1 si l'entree est terminee.
 * -------------------------------------------------------------------------- */
static int lireChoix(int numeroChoix)
{
    char ligne[64];

    for (;;) {
        printf("Case %d (1-%d) : ", numeroChoix, NB_CASES);
        fflush(stdout);

        if (fgets(ligne, sizeof ligne, stdin) == NULL) {
            return -1;
        }

        {
            int valeur;
            char fin;

            /* sscanf analyse la ligne lue :
                 %d -> un entier dans "valeur"
                 %c -> un caractere supplementaire dans "fin"
               Le retour vaut 1 seulement s'il y a exactement un nombre. */
            if (sscanf(ligne, "%d %c", &valeur, &fin) == 1
                && valeur >= 1 && valeur <= NB_CASES) {
                return valeur - 1; /* numero affiche (1..N) -> indice (0..N-1) */
            }
        }

        printf("Entree invalide : tape un nombre entre 1 et %d.\n", NB_CASES);
    }
}

/* --------------------------------------------------------------------------
 * uiDemanderCase - Demande une case reellement jouable.
 * -------------------------------------------------------------------------- */
int uiDemanderCase(const Partie *partie, int premiere, int numeroChoix)
{
    for (;;) {
        int choix = lireChoix(numeroChoix);

        if (choix < 0) {
            return -1;
        }

        /* Une case deja gagnee reste visible : inutile de la rejouer. */
        if (partie->revelees[choix]) {
            printf("Cette case est deja retournee, choisis-en une autre.\n");
            continue;
        }

        /* Les deux cases d'un tour doivent etre differentes. */
        if (choix == premiere) {
            printf("Choisis une autre case que la premiere.\n");
            continue;
        }

        return choix;
    }
}
