<div style="text-align:center"> 
<h1>Devoir maison d'algorithmie des arbres n°2&nbsp;: QuadTree</h1>
<h2>Présenté par Amal ABDALLAH et Nicolas SEBAN</h2>
</div>

<h1>Sommaire</h1>

- [Instruction de compilation](#instruction-de-compilation)
- [Instruction d'utilisation](#instruction-dutilisation)
- [Répartition / Organisation du travail](#répartition--organisation-du-travail)
- [Améliorations possibles et bugs](#améliorations-possibles-et-bugs)


# Instruction de compilation

Pour compiler le programme, il suffit simplement de lancer une des commandes suivantes&nbsp;:

- make all
- make quadtree
- make

Et voilà, vous vous retrouver normalement avec un exécutable `quadtree` à la racine du dossier.

# Instruction d'utilisation

Pour lancer le programme, mettez vous à la racine du dossier et tapez la commande&nbsp;:

```
./quadtree
```

À noter que vous avez le choix d'ajouter une ou plusieurs options de compilation directement à la suite de la commande.<br>
L'ordre des options n'importe pas, mais attention, certaines options demande un argument, faites donc attention à la syntaxe pour avoir le résultat que vous souhaiter.<br>
Voici donc les options que vous pouvez mettre, ils seront marqués selon la syntaxe suivante&nbsp;:

```
-option_format_long / -option_format_court [parametre]
```

Par exemple, le message 
"-fenetre / -f [int]" signifie que l'on peut soit
utilisé le flag -fenetre suivit de l'entier, soit le flag
-f suivit du même entier.

`-help / -aled / -h`<br>
Permet d'afficher dans le terminal toutes les options 
ainsi que leur utilité.
À noter que cette option met fin à l'exécution du programme.

`-forme / -g [carre/cercle]`<br>
Active la génération de particules aléatoires et
définie la forme de la génération.
Seul "carre" et "cercle" sont acceptés.

`-rayon / -r [int]`<br>
Définie le rayon de génération aléatoire de particules.
L'entier int doit être strictement positif.

`-nbpoints / -n [int]`<br>
Définie le nombre de points que générera la génération
aléatoire.
L'entier int doit être strictement positif.

`-nbclicks / -s [int]`<br>
Définie le nombre de points que l'utilisateur peut générer
manuellement.
L'entier int doit être strictement positif.

`-concentration / -c [float]`<br>
Définie la concentration des points par rapport au centre
de la génération aléatoire. Une valeur plus élevée donnera
plus de points au centre.
Le nombre à virgule float peut être autant négatif
que compris entre 0 et 1.

`-tri / -t`<br>
tri les points de la génération aléatoire selon leur
proximité avec le centre de la fenêtre.

`-pas-a-pas / -p`<br>
Montre l'ajout de points de la génération aléatoire et
rafraichit l'affichage à chaque point.

`-velocite / -v [int]`<br>
Définie une vitesse de déplacement pour tous les points
ajoutés dans le programme, générer aléatoirement ou définie
par l'utilisateur.
L'entier int doit simplement être un entier.

`-fenetre / -f [int]`<br>
Définie la taille de la fenêtre.
L'entier [int] doit être strictement positive et
être une puissance de 2.

`-maxpar / -k [int]`<br>
Définie le nombre maximum de particules dans un noeud.
L'entier [int] doit être strictement positive.

`-taillemin / -m [int]`<br>
Définie la taille minimale d'un noeud en pixels.
L'entier [int] doit être strictement positive.

Pour créer un point, faites un clic gauche.<br>
Pour déplacer un point, maintenez le clic droit sur un
point et relâchez la souris là où vous voulez déposer le point.<br>
Pour quitter le programme, appuyer sur la touche échap.


# Répartition / Organisation du travail
# Améliorations possibles et bugs