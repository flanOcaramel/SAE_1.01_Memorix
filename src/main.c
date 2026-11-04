/* ==========================================================================
 *   - Le plateau contient NB_CASES cases (4 lignes x 5 colonnes = 20).
 *   - Chaque case cache un chiffre compris entre 1 et NB_PAIRES (1 a 10).
 *   - Chaque chiffre est present exactement DEUX fois (10 paires).
 *   - La valeur 0 n'est jamais utilisee.
 * ========================================================================== */

#include <stdio.h>
#include <stdlib.h>  /* rand, srand                               */
#include <time.h>    /* time                                      */

#ifdef _WIN32
#include <windows.h> /* GetStdHandle, GetConsoleMode, SetConsoleMode */
#include <io.h>      /* _isatty, _fileno (test "est-ce un terminal ?") */
#else
#endif

/* --------------------------------------------------------------------------
 * Dimensions du plateau.
 * On les met en constantes symboliques (#define) pour pouvoir changer la
 * taille du plateau sans toucher au reste du code : NB_CASES et NB_PAIRES
 * -------------------------------------------------------------------------- */
#define NB_LIGNES 4
#define NB_COLONNES 5

/* Nombre total de cases */
#define NB_CASES (NB_LIGNES * NB_COLONNES)

/* Nombre de paires : la moitie des cases*/
#define NB_PAIRES (NB_CASES / 2)

/* --------------------------------------------------------------------------
 * Codes couleur ANSI.
 *   ESC[1;32m : texte en vert gras
 *   ESC[1;31m : texte en rouge gras
 *   ESC[0m    : remet le style par defaut
 * "\033" est le caractere ECHAP (code ASCII 27).
 * -------------------------------------------------------------------------- */
#define CODE_VERT "\033[1;32m"
#define CODE_ROUGE "\033[1;31m"
#define CODE_RAZ "\033[0m"

/* Booleen global (1 = couleurs disponibles, 0 = sortie sans couleur).*/
static int couleursActives = 0;

/* ==========================================================================
 * couleurVerte - Renvoie le code ANSI du vert, ou une chaine vide.
 * Parametres : aucun.
 * Retour      : "ESC[1;32m" si les couleurs sont actives, "" sinon.
 * ========================================================================== */
const char *couleurVerte(void)
{
    return couleursActives ? CODE_VERT : "";
}

/* ==========================================================================
 * couleurRouge - Renvoie le code ANSI du rouge, ou une chaine vide.
 *
 * Parametres : aucun.
 * Retour      : "ESC[1;31m" si les couleurs sont actives, "" sinon.
 * ========================================================================== */
const char *couleurRouge(void)
{
    return couleursActives ? CODE_ROUGE : "";
}

/* ==========================================================================
 * couleurRaz - Renvoie le code ANSI de remise a zero du style.
 * Parametres : aucun.
 * Retour      : "ESC[0m" si les couleurs sont actives, "" sinon.
 * ========================================================================== */
const char *couleurRaz(void)
{
    return couleursActives ? CODE_RAZ : "";
}

/* ==========================================================================
 * activerCouleurs - Autorise les couleurs ANSI sur la sortie standard.
 *
 * Parametres : aucun.
 * Retour      : aucun (modifie la variable globale couleursActives).
 *
 * Sur Windows, la console historique ne comprend pas les codes ANSI par
 * defaut : il faut activer l'option ENABLE_VIRTUAL_TERMINAL_PROCESSING
 * ========================================================================== */
void activerCouleurs(void)
{
#ifdef _WIN32
    HANDLE sortie;   /* handle (identifiant systeme) de la sortie standard */
    DWORD mode = 0;  /* DWORD = entier non signe 32 bits impose par l'API Windows */

    /* _fileno(stdout) donne le descripteur numerique de stdout (1 en general),
       et _isatty teste si ce descripteur est relie a un vrai terminal. */
    if (!_isatty(_fileno(stdout))) {
        return; /* sortie redirigee : pas de couleur, on quitte la fonction */
    }

    /* Recupere le handle associe a la sortie standard du processus. */
    sortie = GetStdHandle(STD_OUTPUT_HANDLE);

    /* INVALID_HANDLE_VALUE signifie "pas de console" ; GetConsoleMode lit
       ensuite les options actuelles de la console dans "mode". */
    if (sortie != INVALID_HANDLE_VALUE && GetConsoleMode(sortie, &mode)) {
        /* On ajoute (OU binaire "|") l'option VT a la configuration existante
           pour ne pas ecraser les reglages deja presents. */
        if (SetConsoleMode(sortie, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING)) {
            couleursActives = 1; /* succes : les codes ANSI seront compris */
        }
    }
#else
    /* Sur les systemes de type Unix : stdout est un terminal ?
       isatty renvoie 1 si oui, 0 si la sortie est redirigee. */
    couleursActives = isatty(fileno(stdout));
#endif
}

/* ==========================================================================
 * effacerEcran - Efface la console et replace le curseur en haut a gauche.
 *
 * Parametres : aucun.
 * Retour      : aucun.
 * ========================================================================== */
void effacerEcran(void)
{
    if (couleursActives) {
        printf("\033[2J\033[H");
    }
}

/* ==========================================================================
 * attendreEntree - Affiche un message puis attend que le joueur valide.
 *
 * Parametres :
 *   message : texte affiche avant l'attente (ne doit pas etre NULL).
 * Retour : aucun.
 *
 * On lit une ligne complete avec fgets (et non scanf) pour consommer aussi
 * le caractere Entree.
 * ========================================================================== */
void attendreEntree(const char *message)
{
    char ligne[64]; /* zone temporaire : le contenu lu est volontairement ignore */

    printf("%s", message);
    /* fflush force l'affichage immediat : sans ca, le message pourrait
       rester dans le tampon et n'apparaitre qu'apres la saisie. */
    fflush(stdout);

    if (fgets(ligne, sizeof ligne, stdin) == NULL) {
    }
}

/* ==========================================================================
 * afficherEntete - Affiche le titre et le rappel des regles en haut du jeu.
 *
 * Parametres : aucun.
 * Retour      : aucun.
 * ========================================================================== */
void afficherEntete(void)
{
    /* %d est remplace par un int (ici les deux constantes NB_CASES/NB_PAIRES).
    sauter une ligne pour separer l'en-tete du plateau. */
    printf("Memory : %d cases numerotees, %d paires a trouver\n\n",
           NB_CASES, NB_PAIRES);
}

/* ==========================================================================
 * remplirTableau - Remplit les cases puis melange les chiffres.
 *
 * Parametres :
 *   valeurs : tableau de NB_CASES entiers a remplir (modifie sur place).
 * Retour : aucun.
 *
 * Etape 1 : on ecrit deux fois chaque chiffre de 1 a NB_PAIRES.
 *           i = 0,1 -> chiffre 1 ; i = 2,3 -> chiffre 2 ; etc.
 * Etape 2 : on melange le tableau (algorithme de Fisher-Yates)
 * ========================================================================== */
void remplirTableau(int valeurs[NB_CASES])
{
    int i;

    /* --- Etape 1 : deux exemplaires de chaque chiffre --- */
    for (i = 0; i < NB_CASES; i++) {
        /* La division entiere groupe les indices par deux :
           i / 2 vaut 0 pour i = 0 et 1, 1 pour i = 2 et 3, ...
           On ajoute 1 pour obtenir des chiffres de 1 a NB_PAIRES (jamais 0). */
        valeurs[i] = i / 2 + 1;
    }

    /* --- Etape 2 : melange de Fisher-Yates --- */
    for (i = NB_CASES - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temporaire = valeurs[i]; /* sauvegarde pour l'echange */

        valeurs[i] = valeurs[j]; /* la case i recoit la valeur de la case j */
        valeurs[j] = temporaire; /* la case j recoit l'ancienne valeur de i */
    }
}

/* ==========================================================================
 * afficherCase - Affiche une seule case du plateau.
 *
 * Parametres :
 *   numero  : numero de la case, de 1 a NB_CASES (affiche si elle est cachee).
 *   chiffre : valeur cachee dans la case, de 1 a NB_PAIRES.
 *   visible : 1 si le contenu doit etre montre, 0 si la case est cachee.
 *   gagnee  : 1 si la paire a deja ete trouvee (affichage vert permanent).
 * Retour : aucun.
 *
 *   cachee  -> "[ 7]    "  (le numero entre crochets)
 *   revelee -> "   4    "  (le chiffre seul)
 *   gagnee  -> "   4    "  (le meme chiffre, mais en vert)
 * ========================================================================== */
void afficherCase(int numero, int chiffre, int visible, int gagnee)
{
    if (!visible) {
        /* "!visible" signifie visible == 0 : la case est cachee.
           %2d affiche le numero sur 2 caracteres, aligne a droite. */
        printf("[%2d]    ", numero);
    } else if (gagnee) {
        /* Case trouvee : on encadre le chiffre des codes couleur.
           Les %s sont remplaces par "" si les couleurs sont desactivees,
           ce qui donne exactement le meme texte, sans code d'echappement. */
        printf("%s  %2d    %s", couleurVerte(), chiffre, couleurRaz());
    } else {
        /* Case simplement retournee pour le tour en cours (pas encore gagnee) :
           chiffre affiche sans couleur. */
        printf("  %2d    ", chiffre);
    }
}

/* ==========================================================================
 * afficherTableau - Affiche le plateau comple
 *
 * Parametres :
 *   valeurs    : les NB_CASES chiffres caches du plateau.
 *   revelees   : pour chaque case, 1 = paire deja trouvee, 0 = encore cachee.
 *   temporaire1: indice de la 1re case retournee pendant ce tour, ou -1.
 *   temporaire2: indice de la 2e case retournee pendant ce tour, ou -1.
 * Retour : aucun.
 *
 * Les indices des cases vont de 0 a NB_CASES-1, alors que
 * les numeros affiches au joueur vont de 1 a NB_CASES.
 * ========================================================================== */
void afficherTableau(const int valeurs[NB_CASES], const int revelees[NB_CASES],
                     int temporaire1, int temporaire2)
{
    int ligne;
    int colonne;

    for (ligne = 0; ligne < NB_LIGNES; ligne++) {
        /* Deux espace de marge a gauche, pour que le plateau ne colle pas
           au bord de la console. */
        printf("  ");

        for (colonne = 0; colonne < NB_COLONNES; colonne++) {
            /* Passage des coordonnnees 2D (ligne, colonne) a l'indice 1D du
               tableau : i = ligne * nombre_de_colonnes + colonne. */
            int i = ligne * NB_COLONNES + colonne;

            /* Une case est visible si elle est deja gagnee, ou si c'est l'une
               des deux cases retournees pendant le tour en cours. */
            int visible = revelees[i] || i == temporaire1 || i == temporaire2;

            /* +1 : le joueur voit des numeros de 1 a 20, pas de 0 a 19. */
            afficherCase(i + 1, valeurs[i], visible, revelees[i]);
        }

        /* Fin de ligne du plateau. */
        printf("\n");
    }
}

/* ==========================================================================
 * lireChoix - Lit au clavier un numero de case valide.
 *
 * Parametres :
 *   numeroChoix : 1 ou 2, sert seulement a afficher "Case 1" ou "Case 2".
 * Retour :
 *   l'indice de la case choisi (0 a NB_CASES-1),
 *   ou -1 si l'entree est terminee (fin de fichier / Ctrl+Z).
 *
 * ========================================================================== */
int lireChoix(int numeroChoix)
{
    char ligne[64]; /* tampon recevant la ligne tapee par le joueur */

    /* for (;;) = boucle infinie : on ne sort que par un return. */
    for (;;) {
        printf("Case %d (1-%d) : ", numeroChoix, NB_CASES);
        fflush(stdout); /* affiche l'invite avant d'attendre la saisie */

        /* fgets lit une ligne entiere (espaces compris) et renvoie NULL si
           l'entree est fermee : dans ce cas, on signale la fin par -1. */
        if (fgets(ligne, sizeof ligne, stdin) == NULL) {
            return -1;
        }
        {
            int valeur;
            char fin; /* */

            /* scanf analyse la ligne lue :
                 %d  -> un entier, range dans "valeur"
                 %c  -> UN caractere quelconque supplementaire, range dans "fin"
               Le " %c" ignore les espaces */
            if (scanf(ligne, "%d %c", &valeur, &fin) == 1
                && valeur >= 1 && valeur <= NB_CASES) {
                /* -1 : conversion du numero affiche (1..20) vers l'indice
                   de tableau (0..19). */
                return valeur - 1;
            }
        }

        /* Saisie incorrecte : on explique et on recommence la boucle. */
        printf("Entree invalide : tape un nombre entre 1 et %d.\n", NB_CASES);
    }
}

/* ==========================================================================
 * demanderCase - Demande une case reellement jouable.
 *
 * Parametres :
 *   revelees    : etat des cases (1 = deja gagnee).
 *   premiere    : indice de la 1re case du tour, pour interdire de la
 *                 reprendre ; mettre -1 au 1er choix (aucune case a eviter).
 *   numeroChoix : 1 ou 2, transmis a lireChoix pour l'affichage.
 * Retour :
 *   l'indice d'une case jouable, ou -1 si l'entree est terminee.
 * ========================================================================== */
int demanderCase(const int revelees[NB_CASES], int premiere, int numeroChoix)
{
    for (;;) {
        /* Lecture d'un numero : -1 remonte s'il n'y a plus d'entree. */
        int choix = lireChoix(numeroChoix);

        if (choix < 0) {
            return -1; /* fin de l'entree : on laisse main() arreter le jeu */
        }

        /* Une case deja gagnee reste visible en permanence : la rejouer
           n'aurait aucun sens. */
        if (revelees[choix]) {
            printf("Cette case est deja retournee, choisis-en une autre.\n");
            continue; /* "continue" saute a l'iteration suivante de la boucle */
        }

        /* Les deux cases d'un tour doivent etre differentes. */
        if (choix == premiere) {
            printf("Choisis une autre case que la premiere.\n");
            continue;
        }

        return choix; /* case valide */
    }
}

/* ==========================================================================
 * main - Point d'entree du programme : gere une partie complete.
 * ========================================================================== */
int main(void)
{
    int valeurs[NB_CASES];  /* le chiffre cache dans chaque case           */
    int revelees[NB_CASES]; /* 1 = paire deja trouvee, 0 = case cachee      */
    int score = 0;          /* nombre de paires trouvees (= points gagnes) */
    int i;

    /* Initialise le generateur aleatoire avec l'heure courante : sans ca,
       rand() renverrait toujours la meme suite de nombres a chaque partie.
       Le cast (unsigned) evite un avertissement de conversion de type. */
    srand((unsigned)time(NULL));

    activerCouleurs();

    remplirTableau(valeurs);

    for (i = 0; i < NB_CASES; i++) {
        revelees[i] = 0;
    }

    /* La partie continue tant que toutes les paires ne sont pas trouvees. */
    while (score < NB_PAIRES) {
        int case1; /* indice de la 1re case du tour */
        int case2; /* indice de la 2e case du tour  */

        /* --- Affichage du plateau avant le premier choix --- */
        effacerEcran();
        afficherEntete();
        afficherTableau(valeurs, revelees, -1, -1); /* -1 : aucune case ouverte */
        printf("\nScore : %d / %d paires\n\n", score, NB_PAIRES);

        /* --- Premier choix --- */
        case1 = demanderCase(revelees, -1, 1);
        if (case1 < 0) {
            /* L'utilisateur a ferme l'entree (Ctrl+Z) : on s'arrete sans planter. */
            printf("\nPartie interrompue.\n");
            return 0;
        }

        /* On redessine le plateau en laissant la 1re case visible, pour que
           le joueur se souvienne de ce qu'il a vu. */
        effacerEcran();
        afficherTableau(valeurs, revelees, case1, -1);

        /* --- Second choix (different de case1 et non deja gagne) --- */
        case2 = demanderCase(revelees, case1, 2);
        if (case2 < 0) {
            printf("\nPartie interrompue.\n");
            return 0;
        }

        /* On montre les deux cases retournees en meme temps. */
        effacerEcran();
        afficherTableau(valeurs, revelees, case1, case2);

        /* --- Verification de la paire --- */
        if (valeurs[case1] == valeurs[case2]) {
            /* Les deux chiffres sont identiques : la paire est gagnee. */
            revelees[case1] = 1; /* la 1re case restera retournee */
            revelees[case2] = 1; /* la 2e case restera retournee  */
            score++;             /* +1 point (donc +1 paire trouvee) */

            /* On redessine tout de suite le plateau : les deux cases
               gagnees apparaissent en vert et restent visibles. */
            effacerEcran();
            afficherEntete();
            afficherTableau(valeurs, revelees, -1, -1);
            printf("\nScore : %d / %d paires\n", score, NB_PAIRES);
            printf("%sPaire trouvee : %d et %d -- +1 point%s\n",
                   couleurVerte(), valeurs[case1], valeurs[case2], couleurRaz());
        } else {
            /* Chiffres differents : on affiche l'explication en rouge, puis
               on attend une validation. Les cases disparaitront au prochain
               affichage, puisqu'elles ne sont pas marquees dans "revelees". */
            printf("\n%sCe n'est pas une paire : %d et %d%s\n",
                   couleurRouge(),
                   valeurs[case1], valeurs[case2],
                   couleurRaz());
            attendreEntree("Appuie sur Entree pour retourner les cases...");
        }
    }

    /* --- Fin de partie : toutes les paires ont ete trouvees --- */
    effacerEcran();
    afficherEntete();
    afficherTableau(valeurs, revelees, -1, -1);
    printf("\n%sBravo ! Toutes les paires sont trouvees.%s\n", couleurVerte(), couleurRaz());
    printf("Score final : %d / %d\n", score, NB_PAIRES);

    return 0;
}
