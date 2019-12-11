#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <stdlib.h>

#include "PNM.h"
#include "slimming.h"

typedef struct POSITION{
  size_t x;
  size_t y;

}POSITION;

typedef struct sillon_t SILLON;

struct sillon_t{
  POSITION pos; // à supprimer ( sert pour le debug )
  SILLON *next;
  int energie;
  unsigned int cout_cumule;

  PNMPixel pixel;
};

/* ------------------------------------------------------------------------- *
 * Calcule l'énergie du pixel au points i, j
 *
 * PARAMETERS
 * i           i-eme ligne
 * j           j-eme colonne
 * c           couleurs, {r,g,b}
 * tab_sillon  Tableau de sillon
 * image       image donnée en entrée au format pnm
 * k           nombres de pixels à enlever
 *
 * RETURN
 *  l'energie du pixels
 * ------------------------------------------------------------------------- */
static int energie_aux(size_t i, size_t j, char c, SILLON **tab_sillon, const PNMImage *image, size_t k);

/* ------------------------------------------------------------------------- *
 * Lanceur de energie_aux
 *
 * PARAMETERS
 * i           i-eme ligne
 * j           j-eme colonne
 * c           couleurs, {r,g,b}
 * tab_sillon  Tableau de sillon
 * image       image donnée en entrée au format pnm
 * k           nombres de pixels à enlever
 *
 * RETURN
 *  l'energie du pixels
 * ------------------------------------------------------------------------- */
inline static int energie(size_t i, size_t j, SILLON** tab_sillon, const PNMImage *image, size_t k);

/* ------------------------------------------------------------------------- *
 * crée un sillon
 *
 * PARAMETERS
 * i           i-eme ligne
 * j           j-eme colonne
 * c           couleurs, {r,g,b}
 * image       image donnée en entrée au format pnm
 *
 * RETURN
 *  Un sillon
 * ------------------------------------------------------------------------- */
static SILLON *create_sillon(size_t i, size_t j, const PNMImage* image);

/* ------------------------------------------------------------------------- *
 * Crée une image pnm à partir du tableau de sillon
 *
 * PARAMETERS
 * tab_sillon  Tableau de sillon
 * height      Hauteur de l'image
 * width       largeur de l'image
 * k           nombres de pixels à enlever
 *
 * RETURN
 *  image pnm
 * ------------------------------------------------------------------------- */
static PNMImage *sillion_vers_image(SILLON **tab_sillon, size_t height, size_t width, size_t k);

/* ------------------------------------------------------------------------- *
 * Libere le tableau de sillon case par case
 *
 * PARAMETERS
 * tab_sillon  Tableau de sillon
 * image       image donnée en entrée au format pnm
 * ------------------------------------------------------------------------- */
static void libere_tab_sillon(SILLON **tab_sillon, const PNMImage *image);

/* ------------------------------------------------------------------------- *
 * Supprime le sillon avec le cout total minimum
 *
 * PARAMETERS
 * tab_sillon  Tableau de sillon
 * image       image donnée en entrée au format pnm
 * k           nombres de pixels à enlever
 * ------------------------------------------------------------------------- */
static void reset_tabSillon(SILLON **tab_sillon, const PNMImage *image, size_t k);

/* ------------------------------------------------------------------------- *
 * Fonction récursvie pour calculer le coups total d'un sillon qui commence à la derniere ligne
 *
 * PARAMETERS
 * j           j-eme colonne
 * i           i-eme ligne
 * c           couleurs, {r,g,b}
 * tab_sillon  Tableau de sillon
 * image       image donnée en entrée au format pnm
 * k           nombres de pixels à enlever
 *
 * RETURN
 *  Un sillon
 * ------------------------------------------------------------------------- */
static SILLON *C(size_t j, size_t i, SILLON **tab_sillon, const PNMImage *image, size_t k);

/* ------------------------------------------------------------------------- *
 * renvois le sillon avec le coup cumulé minimum
 *
 * PARAMETERS
 * x      premier sillon
 * y      deuxieme sillon
 *
 * RETURN
 *  Un sillon
 * ------------------------------------------------------------------------- */
inline static SILLON *min(SILLON *x, SILLON *y);

inline static SILLON *min(SILLON *x, SILLON *y){

  if(x == NULL && y == NULL)
    return NULL;

  if(y == NULL)
    return x;

  if(x == NULL)
    return y;

  return  x->cout_cumule < y->cout_cumule ? x : y;
}

static int energie_aux(size_t i, size_t j, char c, SILLON **tab_sillon, const PNMImage *image, size_t k){
  int couleur_im_j = -1;
  int couleur_ip_j = -1;

  int couleur_i_jm = -1;
  int couleur_i_jp = -1;

  switch(c){
    case 'r':

      couleur_im_j = tab_sillon[i*image->width +j]->pixel.red;
      couleur_ip_j = tab_sillon[i*image->width +j]->pixel.red;

      couleur_i_jm = tab_sillon[i*image->width +j]->pixel.red;
      couleur_i_jp = tab_sillon[i*image->width +j]->pixel.red;

      if(i > 0) // si borne
        couleur_im_j = tab_sillon[(i-1)*image->width +j]->pixel.red;

      if(i < image->height -1)
        couleur_ip_j = tab_sillon[(i+1)*image->width +j]->pixel.red;

      if(j > 0)
        couleur_i_jm = tab_sillon[i*image->width + j-1]->pixel.red;

      if(j < image->width -1 -k)
        couleur_i_jp = tab_sillon[i*image->width + j+1]->pixel.red;

      break;

    case 'g':

      couleur_im_j = tab_sillon[i*image->width +j]->pixel.green;
      couleur_ip_j = tab_sillon[i*image->width +j]->pixel.green;

      couleur_i_jm = tab_sillon[i*image->width +j]->pixel.green;
      couleur_i_jp = tab_sillon[i*image->width +j]->pixel.green;

      if(i > 0)
        couleur_im_j = tab_sillon[(i-1)*image->width +j]->pixel.green;

      if(i < image->height -1)
        couleur_ip_j = tab_sillon[(i+1)*image->width +j]->pixel.green;

      if(j > 0)
        couleur_i_jm = tab_sillon[i*image->width + j-1]->pixel.green;

      if(j < image->width -1 -k)
        couleur_i_jp = tab_sillon[i*image->width + j+1]->pixel.green;

      break;

    case 'b':

      couleur_im_j = tab_sillon[i*image->width +j]->pixel.blue;
      couleur_ip_j = tab_sillon[i*image->width +j]->pixel.blue;

      couleur_i_jm = tab_sillon[i*image->width +j]->pixel.blue;
      couleur_i_jp = tab_sillon[i*image->width +j]->pixel.blue;

      if(i > 0)
        couleur_im_j = tab_sillon[(i-1)*image->width +j]->pixel.blue;

      if(i < image->height -1)
        couleur_ip_j = tab_sillon[(i+1)*image->width +j]->pixel.blue;

      if(j > 0)
        couleur_i_jm = tab_sillon[i*image->width + j-1]->pixel.blue;

      if(j < image->width -1 -k)
      couleur_i_jp = tab_sillon[i*image->width + j+1]->pixel.blue;

      break;

    default:
      return -1; // ERREUR
      break;
  }

  return abs(couleur_im_j - couleur_ip_j)/2 + abs(couleur_i_jm - couleur_i_jp)/2;
}

inline static int energie(size_t i, size_t j, SILLON** tab_sillon, const PNMImage *image, size_t k){
  return energie_aux(i, j, 'r', tab_sillon, image, k) + energie_aux(i, j, 'g', tab_sillon, image, k) + energie_aux(i, j, 'b', tab_sillon, image, k);
}

static SILLON *create_sillon(size_t i, size_t j, const PNMImage* image){
  SILLON *new_sillon = malloc(sizeof(SILLON));
  if(new_sillon == NULL)
    return NULL;

  new_sillon->pos.x = i; // à supprimer
  new_sillon->pos.y = j; // à supprimer

  new_sillon->energie = -1; // {-1 = l'energie doit etre calculer, -2 = sillon à supprimer, -3 = élément à deplacer}

  new_sillon->cout_cumule = 0;
  new_sillon->next = NULL;

  new_sillon->pixel.red = image->data[i*image->width +j].red;
  new_sillon->pixel.blue = image->data[i*image->width +j].blue ;
  new_sillon->pixel.green = image->data[i*image->width +j].green;
  return new_sillon;
}

static void libere_tab_sillon(SILLON **tab_sillon, const PNMImage *image){
  for(size_t i = 0; i < image->height; i++){
    for(size_t j = 0; j < image->width; j++)
      free(tab_sillon[i*image->width + j]);
    }

  free(tab_sillon);
}

static PNMImage *sillion_vers_image(SILLON **tab_sillon, size_t height, size_t width, size_t k){

  PNMImage *image = createPNM(width-k, height);

  for(size_t i = 0; i < height; i++){
    for(size_t j = 0; j < width-k; j++)
      image->data[i*(width-k) +j] = tab_sillon[i*width +j]->pixel;
  }

  return image;
}

static PNMImage *sillion_vers_imageDEBUG(SILLON **tab_sillon, size_t height, size_t width, size_t k){

  PNMImage *image = createPNM(width, height);

  for(size_t i = 0; i < height; i++){
    for(size_t j = 0; j < (width-k); j++){
      image->data[tab_sillon[i*width +j]->pos.x*width + tab_sillon[i*width +j]->pos.y] = tab_sillon[i*width +j]->pixel;
      image->test[tab_sillon[i*width +j]->pos.x*width + tab_sillon[i*width +j]->pos.y] = 1;
    }
  }

  for(size_t i = 0; i < height; i++){
    for(size_t j = 0; j < width; j++){
      if(image->test[i*width +j] == 0){
        image->data[i*width +j].red =255;
        image->data[i*width +j].blue =255;
        image->data[i*width +j].green =255;
      }
    }
  }

  return image;
}

static SILLON *C(size_t j, size_t i, SILLON **tab_sillon, const PNMImage *image, size_t k){
  size_t actuel = i*(image->width) +j;

  if(tab_sillon[actuel]->energie == -1)
    tab_sillon[actuel]->energie = energie(i, j, tab_sillon, image, k);

  if(i == 0){ // cas de base
    tab_sillon[actuel]->cout_cumule = tab_sillon[actuel]->energie;
    return tab_sillon[actuel];
  }

  if(tab_sillon[actuel]->next != NULL) // chemin déjà crée, pas besoin de le recalculer
    return tab_sillon[actuel];

   SILLON *val1 = NULL;
   SILLON *val2 = NULL;
   SILLON *val3 = NULL;

  if(j > 0)
    val1 = C(j-1, i-1, tab_sillon, image, k);

  val2 = C(j, i-1, tab_sillon, image, k);

  if(j < image->width-1-k)
    val3 = C(j+1, i-1, tab_sillon, image, k);


  SILLON *s = min(min(val1, val2), val3);

  tab_sillon[actuel]->next = s;
  tab_sillon[actuel]->cout_cumule = tab_sillon[actuel]->energie + s->cout_cumule;

  return tab_sillon[actuel];
}

static void calcul_cout(const PNMImage *image, size_t k, SILLON **tab_sillon){
  SILLON *tmp = NULL;
  SILLON *min = NULL;

  for(size_t j = 0; j < image->width-k; j++){
    tmp = C(j, image->height-1, tab_sillon, image, k);

    if(min == NULL || tmp->cout_cumule < min->cout_cumule)
      min = tmp;
    }

  while(min != NULL){
    min->energie = -2;
    min = min->next;
 }
}

static void reset_tabSillon(SILLON **tab_sillon, const PNMImage *image, size_t k){
  for(size_t i = 0; i < image->height; i++){
    for(size_t j = 0; j <= image->width - k; j++){

      if(tab_sillon[i*image->width + j]->energie != -2){
        tab_sillon[i*image->width + j]->next = NULL;
        tab_sillon[i*image->width + j]->cout_cumule = 0;

        if(j > 0 && tab_sillon[i*image->width + j-1]->energie == -3){
          tab_sillon[i*image->width + j-1]->next = NULL;
          // à supprimer
          tab_sillon[i*image->width + j-1]->pos.x = tab_sillon[i*image->width + j]->pos.x;
          // à supprimer
          tab_sillon[i*image->width + j-1]->pos.y = tab_sillon[i*image->width + j]->pos.y;

          tab_sillon[i*image->width + j-1]->energie = tab_sillon[i*image->width + j]->energie;

          tab_sillon[i*image->width + j-1]->cout_cumule = 0;

          tab_sillon[i*image->width + j-1]->pixel = tab_sillon[i*image->width + j]->pixel;

          tab_sillon[i*image->width + j]->energie = -3;
        }
      }

      else{
        tab_sillon[i*image->width + j]->energie = -3;

        if( j < image->width - k)
          tab_sillon[i*image->width + j+1]->energie = -1;

        if(j > 0)
          tab_sillon[i*image->width + j-1]->energie = -1;
      }
    }
  }
}

PNMImage* reduceImageWidth(const PNMImage* image, size_t k){

  SILLON **tab_sillon = malloc(sizeof(SILLON*)*image->width*image->height);

  for(size_t i = 0; i < image->height; i++){
    for(size_t j = 0; j < image->width; j++){
      tab_sillon[i*image->width + j] = create_sillon(i, j, image);
      if(tab_sillon[i*image->width + j] == NULL)
        return NULL;
      }
  }

  for(size_t i = 0; i < k; i++){
    calcul_cout(image, i, tab_sillon);
    reset_tabSillon(tab_sillon, image, i+1);
  }

  //PNMImage *image2 = sillion_vers_imageDEBUG(tab_sillon, image->height, image->width, k);

  PNMImage *image2 = sillion_vers_image(tab_sillon, image->height, image->width, k);

  free(image->test);
  free(image2->test); // à supprimer
  libere_tab_sillon(tab_sillon, image);

  return image2;

}
