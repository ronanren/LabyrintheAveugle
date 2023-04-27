# LabyrintheAveugle

## Bibliothèques

```bash
# for manjaro 
pamac install glfw-x11 freealut sdl2_image
# for debian
sudo apt install g++ mesa-utils libglew-dev libglfw3-dev libsdl2-dev libsdl2-image-dev libopenal-dev libalut-dev freeglut3-dev
```

## Développement

```bash
make cleanalllibs && make main && make run
```

## Rapport

Disponible dans /rapport/rapport_contenu_multimedia_FISA2_Renoux_Louboutin_Cordier.pdf

## Exemples

La joueur se retrouve en POV avec une vision (caché) comme ci-dessous :

<img src="https://raw.githubusercontent.com/ronanren/LabyrintheAveugle/main/rapport/visuelPOVSansEcranNoir.png?token=GHSAT0AAAAAAB35CIYYTDB4XPEJQBGFJCWYZCKGUCA" width="400px">

Le joueur peut avancer (touche Z), reculer (touche S) et tourner de 90° vers la droite (touche D) et vers la gauche (touche Q). De plus, il y a des commandes pour afficher le mode spectateur, et la gestion des modes du son comme indiqué dans la console.

<img src="https://raw.githubusercontent.com/ronanren/LabyrintheAveugle/main/rapport/commandesConsole.png?token=GHSAT0AAAAAAB35CIYZ5FKMYN2SPUCSRVD2ZCKGTDA" width="500px">

Le mode debug (spectateur) affiche le labyrinthe de haut avec l'arrivé en vert.

<img src="https://raw.githubusercontent.com/ronanren/LabyrintheAveugle/main/rapport/debugMode.png?token=GHSAT0AAAAAAB35CIYYWIBAFPSN7S5PHOZKZCKGTTA" width="500px">