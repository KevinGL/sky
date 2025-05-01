Une fois la lib ajoutée à votre projet ajouter un fichier "Path.ini" dans votre répertoire de travail indiquant le chemin relatif vers la lib.

Deux fonctions principales :

-  Init() avec pour paramètres :
  - La date
  - La latitude
  - La couleur à l'horizon
  - La couleur au zenith
  - Paramètre optionnel : Un multiple de la vitesse d'écoulement du temps

- Draw() avec pour paramètres :
    - La position de la caméra (vec3)
    - La matrice de modèle (mat4)
    - La matrice de vue (mat4)
    - La matrice de projection (mat4)

  /!\ Faire le rendu du ciel avant le reste
