#import "enssat.typ":*
#show: doc => conf(
  titre: [ Projet "Labyrinthe aveugle" ],
  auteurs: (
    (nom: "Pierre Alain", email:"alain@enssat.fr"),
    (nom: "X Y", email:"xy@enssat.fr"),
    (nom: "A B", email:"ab@enssat.fr"),
  ),
  annee: 2023,
  promotion: "Fisa Informatique 2",
  module: "Contenu Multimédia",
  doc,
)

= Contexte du projet

Nous souhaitons développer une application de jeu dans lequel le joueur doit trouver la sortie d’un labyrinthe (en 2 dimensions, mais vous pouvez réfléchir à une version en 3 dimensions) qui n’est pas éclairé (il n’est pas utile de dessiner les murs, l'interface peut rester noire pendant le jeu). Il est fait appel à vous pour développer l’intégralité de ce jeu.

Une précédente application est donnée en exemple (sur moodle) permettant d’utiliser le son spatialisé. Cette application permet le déplacement libre à la souris et au clavier avec pour but la recherche des canards jouant des sons. Lorsque la caméra est suffisament proche d’un canard, le canard s’affiche et arrête la lecture du son. Le but est de trouver les deux canards présents dans la scène.

= Travail demandé

L’objectif de ce projet est de pouvoir charger un fichier décrivant le labyrinthe et de donner la possibilité au le joueur d’utiliser l’écho-location pour avancer dans le labyrinthe.

== Mise en place technique
Vous êtes libre sur les spécifications de format concernant ce fichier, vous pouvez par exemple utiliser un fichier texte avec des caractères définissant les murs, la position de départ et la position d’arrivée. Vous pouvez également choisir un format structuré (json) plus complexe ou écrire un générateur de labyrinthe à votre convenance~:
- par exemple #link("https://weblog.jamisbuck.org/2011/2/3/maze-generation-sidewinder-algorithm") ou une variante décrite sur le site,
- ou en 3D #link("https://github.com/conorpp/3d-maze-generator") qui utilise une des variantes présentée sur le site précédent (algorithme de Prim).

Il est possible d’imaginer, dans le but d’aider le joueur, qu’un son spécifique puisse indiquer la direction générale de la sortie.

La documentation d’openAL et le guide du programmeur sont disponibles aux adresses suivantes~:
- #link("http://openal.org/documentation/openal-1.1-specification.pdf")
- #link("http://openal.org/documentation/OpenAL_Programmers_Guide.pdf").

== Objectifs
Pour ce projet des niveaux d'avancement sont définis ainsi~:
+ *Echo-location simple :* le joueur doit appuyer sur des touches pour savoir s’il y a un mur à droite/gauche/devant _(/en haut/en bas pour la version en 3dimensions)_.
+ *Echo-location avancée :* le joueur obtient automatiquement un retour en fonction de son environnement.
+ *Ajout d’un mode spectateur :* pour des raisons de debug, vous pouvez avoir besoin d’afficher le labyrinthe dans une fenêtre et la rendre visible aux spectateurs.

Dans ce projet, le niveau 1 est le niveau minimal à atteindre. Les niveaux 2 et 3 seront chacun valorisés dans la note du projet. L’expérience utilisateur n’est pas notée dans ce projet, mais la prendre en compte sera un plus de votre réalisation.

= Rendus

Le travail demandé est l'implémentation du jeu. Le projet se soldera par la remise de deux livrables. Une note sera attribuée pour chacun de ces éléments, la combinaison des notes produira la note globale de projet.
- *Vos programmes commentés et documentés.* Le code source sera fourni sous la forme d'une archive intitulée `code_projet_contenu_mutimedia_FISA2_N1_N2_N3.zip` où les `Ni` sont à remplacer par les *noms* des membres du groupe. L’ensemble de la procédure de compilation, de lancement et de test doit être décrite dans un fichier `README.txt`. Si vous avez atteint différents niveaux, ne rendez que le niveau fonctionnel le plus avancé et précisez-le dans le compte rendu.
- *Un compte rendu de 2 pages maximum (de contenu, sans la page de garde, sans les éventuelles annexes)* expliquant de manière concise l'organisation mise en œuvre et vos choix d'implémentation. Il s'agira de montrer~:
 - comment vous avez mis en œuvre le système (fonctions de gestion du son, architecture logicielle...) ;
 - comment vous avez testé votre code (méthode automatisée, sur quel scénario, avec quelle intensité...).
La forme du rapport devra *suivre ou imiter le modèle de cet énoncé* (le type et la taille de la police, marges...). Le rapport remis sera un fichier PDF nommé `rapport_contenu_multimedia_FISA2_N1_N2_N3.pdf` où les `Ni` sont à remplacer par les noms des membres du groupe. Ajustez en fonction de la taille de votre groupe.

= Remarques
Sauf cas exceptionnel, *tous les membres d'un même groupe auront la même note de projet*. Les groupes doivent s'assurer eux-mêmes de la répartition et de la régularité du travail pour leur projet. 

*Les pages, au delà de la taille maximale demandée, ne seront pas lues.* Il est demandé dans votre compte rendu de hiérarchiser les informations en privilégiant les plus importantes pour la bonne appréciation de votre travail.

Étant donné la faible longueur du compte rendu, une attention particulière sera accordée à l'*orthographe*, la *clarté* du document et au *respect du format demandé*.
