# SAE 1.01 - Memorix

Jeu de Memory en C : un plateau de 4 lignes x 5 colonnes cache 10 paires de
chiffres. Le joueur retourne deux cases par tour et doit retrouver toutes
les paires.

## Joueurs

Au debut de la partie, on choisit le nombre de joueurs (**1 a 4**), puis
pour chacun on repond `o` (ordinateur) ou `n` (humain). On peut donc jouer
seul, a plusieurs, contre l'ordinateur, ou avec n'importe quel melange.

- **Un seul plateau** est partage par tout le monde : les paires deja
  trouvees restent visibles pour tous les joueurs.
- Le programme affiche a chaque tour **le joueur actif** (avec la mention
  `(ordinateur)` si c'est une IA) et le score de chacun.
- **Regle de rotation** : si le joueur trouve une paire, il **garde la main**
  et rejoue ; s'il rate, le tour passe au joueur suivant (avec boucle
  apres le dernier joueur).
- Chaque paire trouvee rapporte **1 point au joueur actif** et 1 point au
  score total. A la fin, un classement est affiche.

## Joueur ordinateur

L'IA ne triche pas : elle ne lit jamais les chiffres caches directement.
Elle memorise seulement les cartes deja montrees a l'ecran (les siennes et
celles des humains) grace au tableau `vu[]` de la structure `Partie`.

Chaque carte retournee par l'ordinateur reste affichee **3 secondes**, pour
que le joueur humain puisse suivre ce qui se passe.

Sa strategie, dans `src/ia.c` :

1. si elle connait deux cases deja vues ayant le meme chiffre, elle les joue ;
2. sinon, elle retourne une case encore inconnue pour apprendre un chiffre ;
3. les tirages au hasard se font sur une liste de cases jouables, jamais
   avec une boucle qui pourrait tourner a l'infini.

## Organisation du projet

```
.
├── include/          # Les en-tetes (.h) : ce que chaque module expose
│   ├── jeu.h         # Modele de donnees (Partie) + constantes du plateau
│   ├── creation.h    # Creation / melange du plateau
│   ├── ia.h          # Decision des joueurs ordinateur
│   └── ui.h          # Affichage et saisie clavier
├── src/              # Le code des modules (.c)
│   ├── main.c        # Point d'entree : initialise le hasard, lance le jeu
│   ├── jeu.c         # Regles du jeu : boucle de partie, verification des paires
│   ├── creation.c    # Remplissage et melange (Fisher-Yates)
│   ├── ia.c          # Choix des cases par l'ordinateur (sans tricher)
│   └── ui.c          # Couleurs, affichage du plateau, lecture des choix
├── build/            # Fichiers compiles (ignore par git)
├── data/             # Documents de remplissage
└── Makefile
```

## Les trois modules

- **`creation`** : construit une nouvelle partie. Il place deux exemplaires de
  chaque chiffre puis melange le tout. Il ne fait aucun affichage.
- **`jeu`** : contient les regles et la boucle de la partie. Il ne fait aucun
  `printf` ni `scanf` : il demande au module `ui` d'afficher et de lire.
- **`ui`** : tout ce que voit et tape le joueur (plateau, score, messages,
  couleurs, saisie au clavier).

Le fichier `jeu.h` sert de lien commun : il definit la structure `Partie` et
les constantes `NB_LIGNES`, `NB_COLONNES`, `NB_CASES` et `NB_PAIRES`.

## Compiler et lancer

```sh
make        # construit ./memorix a la racine du projet
./memorix
```

Pour nettoyer les fichiers compiles :

```sh
make clean
```
