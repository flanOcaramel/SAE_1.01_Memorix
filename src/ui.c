/* Necessaire pour nanosleep sur Linux/macOS en C17 strict. */
#if !defined(_WIN32)
#define _POSIX_C_SOURCE 199309L
#endif

#include "../include/ui.h"

#include <stdio.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

/* Codes ANSI : vert, rouge, puis remise a zero du style. */
#define CODE_VERT "\033[1;32m"
#define CODE_ROUGE "\033[1;31m"
#define CODE_RAZ "\033[0m"

void uiInitialiser(void)
{
#ifdef _WIN32
    HANDLE sortie;
    DWORD mode = 0;

    /* Active les codes ANSI dans la console Windows. */
    sortie = GetStdHandle(STD_OUTPUT_HANDLE);

    if (sortie != INVALID_HANDLE_VALUE && GetConsoleMode(sortie, &mode)) {
        SetConsoleMode(sortie, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
#endif
}

int uiDemanderNombreJoueurs(void)
{
    int nbJoueurs = 0;
    int resultat;
    int c;

    do {
        printf("Nombre de joueurs (1-%d) : ", NB_JOUEURS_MAX);
        fflush(stdout);

        resultat = scanf("%d", &nbJoueurs);

        /* scanf laisse le caractere Entree dans le clavier : on le vide pour
           ne pas fausser la saisie suivante. */
        while ((c = getchar()) != '\n' && c != EOF) {
        }

        if (resultat == EOF) {
            return -1;
        }
        if (resultat != 1) {
            printf("Entree invalide : tape un nombre entre 1 et %d.\n",
                   NB_JOUEURS_MAX);
            nbJoueurs = 0;
        }
    } while (nbJoueurs < 1 || nbJoueurs > NB_JOUEURS_MAX);

    return nbJoueurs;
}

int uiDemanderTypeJoueurs(int nbJoueurs, int estOrdinateur[NB_JOUEURS_MAX])
{
    int i;

    for (i = 0; i < nbJoueurs; i++) {
        char reponse;

        for (;;) {
            int c;

            printf("Le joueur %d est-il un ordinateur ? (o/n) : ", i + 1);
            fflush(stdout);

            if (scanf(" %c", &reponse) == EOF) {
                return -1;
            }
            while ((c = getchar()) != '\n' && c != EOF) {
            }

            if (reponse == 'o' || reponse == 'O') {
                estOrdinateur[i] = 1;
                break;
            }
            if (reponse == 'n' || reponse == 'N') {
                estOrdinateur[i] = 0;
                break;
            }
            printf("Reponds par o (oui) ou n (non).\n");
        }
    }

    return 0;
}

void uiEffacerEcran(void)
{
    /* \033[2J efface l'ecran, \033[H replace le curseur en haut a gauche. */
    printf("\033[H\033[2J\033[3J");
    fflush(stdout);
}

void uiPause(int millisecondes)
{
#ifdef _WIN32
    Sleep((DWORD)millisecondes);
#else
    struct timespec duree;

    duree.tv_sec = millisecondes / 1000;
    duree.tv_nsec = (long)(millisecondes % 1000) * 1000000L;
    nanosleep(&duree, NULL);
#endif
}

static void afficherCase(int numero, int chiffre, int visible, int gagnee)
{
    /* Chaque case fait 8 caracteres pour garder les colonnes alignees. */
    if (!visible) {
        printf("[%2d]    ", numero);
    } else if (gagnee) {
        printf("%s  %2d    %s", CODE_VERT, chiffre, CODE_RAZ);
    } else {
        printf("  %2d    ", chiffre);
    }
}

void uiAfficherPlateau(const Partie *partie, int case1, int case2)
{
    int ligne;
    int colonne;

    for (ligne = 0; ligne < NB_LIGNES; ligne++) {
        printf("  ");

        for (colonne = 0; colonne < NB_COLONNES; colonne++) {
            /* Coordonnees 2D -> indice 1D. */
            int i = ligne * NB_COLONNES + colonne;
            int visible = partie->revelees[i] || i == case1 || i == case2;

            afficherCase(i + 1, partie->valeurs[i], visible, partie->revelees[i]);
        }

        printf("\n");
    }
}

void uiAfficherPartie(const Partie *partie, int case1, int case2)
{
    int j;

    printf("Memory : %d cases numerotees, %d paires a trouver\n",
           NB_CASES, NB_PAIRES);

    /* +1 : les joueurs sont affiches de 1 a nbJoueurs, alors que l'indice
       joueurCourant commence a 0. */
    printf("Tour du joueur %d / %d%s\n\n",
           partie->joueurCourant + 1, partie->nbJoueurs,
           partie->estOrdinateur[partie->joueurCourant] ? " (ordinateur)" : "");

    uiAfficherPlateau(partie, case1, case2);

    printf("\nScore total : %d / %d paires\n", partie->score, NB_PAIRES);

    for (j = 0; j < partie->nbJoueurs; j++) {
        printf("  Joueur %d : %d paire(s)\n", j + 1, partie->scores[j]);
    }
    printf("\n");
}

void uiAfficherChoixOrdinateur(int numeroCase)
{
    printf("%sL'ordinateur choisit la case %d.%s\n",
           CODE_VERT, numeroCase, CODE_RAZ);
}

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
    int ordre[NB_JOUEURS_MAX];
    int i;
    int j;

    printf("\n%sBravo ! Toutes les paires sont trouvees.%s\n", CODE_VERT, CODE_RAZ);
    printf("Score final : %d / %d\n", partie->score, NB_PAIRES);

    if (partie->nbJoueurs == 1) {
        printf("Joueur 1 : %d paire(s) trouvee(s)\n", partie->scores[0]);
        return;
    }

    for (i = 0; i < partie->nbJoueurs; i++) {
        ordre[i] = i;
    }

    /* Tri par selection : ordre[] contient les indices des joueurs, du
       meilleur score au moins bon. */
    for (i = 0; i < partie->nbJoueurs - 1; i++) {
        for (j = i + 1; j < partie->nbJoueurs; j++) {
            if (partie->scores[ordre[j]] > partie->scores[ordre[i]]) {
                int temporaire = ordre[i];
                ordre[i] = ordre[j];
                ordre[j] = temporaire;
            }
        }
    }

    printf("\nClassement :\n");
    for (i = 0; i < partie->nbJoueurs; i++) {
        int indice = ordre[i];
        printf("  %d. Joueur %d : %d paire(s)%s\n",
               i + 1, indice + 1, partie->scores[indice],
               partie->estOrdinateur[indice] ? " (ordinateur)" : "");
    }
}

void uiAfficherInterruption(void)
{
    printf("\nPartie interrompue.\n");
}

void uiAttendreEntree(const char *message)
{
    char ligne[64];

    printf("%s", message);
    fflush(stdout);

    /* fgets consomme aussi le caractere Entree reste dans le clavier. */
    (void)fgets(ligne, sizeof ligne, stdin);
}

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

            /* "%d %c" ne renvoie 1 que si la ligne contient un seul nombre :
               "1 2" ou "3abc" sont refuses. */
            if (sscanf(ligne, "%d %c", &valeur, &fin) == 1
                && valeur >= 1 && valeur <= NB_CASES) {
                return valeur - 1;
            }
        }

        printf("Entree invalide : tape un nombre entre 1 et %d.\n", NB_CASES);
    }
}

int uiDemanderCase(const Partie *partie, int premiere, int numeroChoix)
{
    for (;;) {
        int choix = lireChoix(numeroChoix);

        if (choix < 0) {
            return -1;
        }
        if (partie->revelees[choix] || choix == premiere) {
            printf("Choisis une autre case.\n");
            continue;
        }

        return choix;
    }
}
