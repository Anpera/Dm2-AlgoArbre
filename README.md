<div style="text-align:center"> 
<h1>Devoir maison d'algorithmie des arbres n°2&nbsp;: QuadTree</h1>
<h2>Présenté par Amal ABDALLAH et Nicolas SEBAN</h2>
</div>

<h1>Sommaire</h1>

- [Instruction de compilation](#instruction-de-compilation)
- [Instruction d'utilisation](#instruction-dutilisation)
- [Répartition / Organisation du travail](#répartition--organisation-du-travail)
- [Améliorations possibles et bugs](#améliorations-possibles-et-bugs)


# Instructions de compilation

Pour compiler le programme, il suffit simplement de lancer une des commandes suivantes&nbsp;:

- ``make all``
- ``make quadtree``
- ``make``

Et voilà, vous vous retrouverez normalement avec un exécutable `quadtree` à la racine du dossier.

# Instructions d'utilisation

Pour lancer le programme, mettez vous à la racine du dossier et tapez la commande&nbsp;:

```shell
./quadtree
```

## Arguments

À noter que vous avez le choix d'ajouter une, ou plusieurs options, directement à la suite de la commande.<br>
L'ordre des options n'importe pas, mais attention, certaines options demandent un argument, faites donc attention à la syntaxe pour avoir le résultat que vous souhaitez.<br>
Voici donc les options que vous pouvez utiliser, et saisir selon la syntaxe suivante&nbsp;:

```md
-option_format_long=[parametre] / -o [parametre] 
```

Par exemple, le message `"-fenetre / -f [int]"` signifie que l'on peut soit utiliser le flag `-fenetre`, soit son raccourci `-f` suivi de l'entier.

`-help / -aled / -h`<br>
Permet d'afficher dans le terminal toutes les options 
ainsi que leur utilité.
À noter que cette option met fin à l'exécution du programme.

`-forme / -g [carre/cercle]`<br>
Active la génération de particules aléatoires et
définit la forme de la génération.
Seul "carre" et "cercle" sont acceptés.

`-rayon / -r [int]`<br>
Définit le rayon de génération aléatoire de particules.
L'entier int doit être strictement positif.

`-nbpoints / -n [int]`<br>
Définit le nombre de points que générera la génération
aléatoire.
L'entier int doit être strictement positif.

`-nbclicks / -s [int]`<br>
Définit le nombre de points que l'utilisateur peut générer
manuellement.
L'entier int doit être strictement positif.

`-concentration / -c [float]`<br>
Définit la concentration des points par rapport au centre
de la génération aléatoire. Une valeur plus élevée donnera
plus de points au centre.
Le nombre à virgule float peut être autant négatif
que compris entre 0 et 1, ou plus

`-tri / -t`<br>
trie les points de la génération aléatoire selon leur
proximité avec le centre de la fenêtre.

`-pas-a-pas / -p`<br>
Montre l'ajout des points de la génération aléatoire en
rafraîchissant l'affichage à chaque ajout.

`-velocite / -v [int]`<br>
Définit une vitesse de déplacement pour tous les points
ajoutés dans le programme, générés aléatoirement ou définis
par l'utilisateur.
L'entier [int] doit être un entier.

`-fenetre / -f [int]`<br>
Définit la taille de la fenêtre.
L'entier [int] doit être strictement positif et
être une puissance de 2.

`-maxpar / -k [int]`<br>
Définit le nombre maximum de particules dans un nœud.
L'entier [int] doit être strictement positif.

`-taillemin / -m [int]`<br>
Définit la taille minimale d'un nœud en pixels.
L'entier [int] doit être strictement positif.

Exemple d'exécution :

```shell
./quadtree -nbclicks 20 -velocite=2 -taillemin=8 -concentration=0.5 -n 500 -t -p
```

## Actions réalisables pendant l'exécution

- Pour créer un point, faites un clic gauche.<br>
- Pour déplacer un point, maintenez le clic droit sur un point et relâchez la souris là où vous voulez déposer le point.<br>
- Pour quitter le programme, appuyez sur la touche Échap.

# Répartition / Organisation du travail

- Commun : Implémentation du Quadtree

- SEBAN Nicolas
  - Génération aléatoire de points (Réutilisation du projet des enveloppes convexes)
  - Déplacement par clic droit des particules

- ABDALLAH Amal
  - Gestion des arguments avec ``getopt``, et documentation
  - Déplacement animé des particules selon une vélocité

# Remarques

Afin de pouvoir bénéficier d'une structure de liste générique, nous utilisons la bibliothèque ``<sys/queue.h>`` (``man 3 queue``), et son implémentation de liste chaînée ``STAILQ``.

# Améliorations possibles et bugs

- Implémenter le suivi du curseur

- Colorer les points selon la profondeur
