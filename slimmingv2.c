#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <stdlib.h>

#include "PNM.h"
#include "slimming.h"
#include "PriorityQueue.h"

typedef struct POSITION{
  size_t x;
  size_t y;

}POSITION;


typedef struct sillon_t SILLON;

struct sillon_t{
  POSITION pos;
  SILLON *next;
  int energie;
  unsigned int cout_cumule;

  PNMPixel pixel;
};


inline static SILLON *min(SILLON *x, SILLON *y){
  if(y == NULL)
    return x;

  if(x == NULL)
    return y;

  if(x == NULL && y == NULL)
    return NULL;

  return  x->cout_cumule < y->cout_cumule ? x : y; // minimum de deux valeurs
}

static int energie_aux(size_t i, size_t j, char c, const PNMImage* image);

static int energie(size_t i, size_t j, const PNMImage* image);

static SILLON *create_sillon(size_t i, size_t j, const PNMImage* image);

static PNMImage *sillion_vers_image(SILLON **tab_sillon, size_t height, size_t width, size_t k);

static void libere_tab_sillon(SILLON **tab_sillon, const PNMImage *image);

static int energie_aux(size_t i, size_t j, char c, const PNMImage* image){

  int couleur_im_j = -1;
  int couleur_ip_j = -1;

  int couleur_i_jm = -1;
  int couleur_i_jp = -1;

  switch(c){
    case 'r':
      if(i == 0)
        couleur_im_j = image->data[i*image->width +j].red;

      else
        couleur_im_j = image->data[(i-1)*image->width +j].red;


      if(i+1 == image->height)
        couleur_im_j = image->data[i*image->width +j].red;

      else
        couleur_ip_j = image->data[(i+1)*image->width +j].red;

      if(j == 0)
        couleur_im_j = image->data[i*image->width +j].red;

      else
        couleur_i_jm = image->data[i*image->width + j-1].red;


      if(j+1 == image->width)
        couleur_im_j = image->data[i*image->width +j].red;

      else
        couleur_i_jp = image->data[i*image->width + j+1].red;
      break;

    case 'g':
      if(i == 0)
        couleur_im_j = image->data[i*image->width +j].green;

      else
        couleur_im_j = image->data[(i-1)*image->width +j].green;


      if(i+1 == image->height)
        couleur_im_j = image->data[i*image->width +j].green;

      else
        couleur_ip_j = image->data[(i+1)*image->width +j].green;

      if(j == 0)
        couleur_im_j = image->data[i*image->width +j].green;

      else
        couleur_i_jm = image->data[i*image->width + j-1].green;


      if(j+1 == image->width)
        couleur_im_j = image->data[i*image->width +j].green;

      else
        couleur_i_jp = image->data[i*image->width + j+1].green;

      break;

    case 'b':
      if(i == 0)
        couleur_im_j = image->data[i*image->width +j].blue;

      else
        couleur_im_j = image->data[(i-1)*image->width +j].blue;


      if(i+1 == image->height)
        couleur_im_j = image->data[i*image->width +j].blue;

      else
        couleur_ip_j = image->data[(i+1)*image->width +j].blue;

      if(j == 0)
        couleur_im_j = image->data[i*image->width +j].blue;

      else
        couleur_i_jm = image->data[i*image->width + j-1].blue;


      if(j+1 == image->width)
        couleur_im_j = image->data[i*image->width +j].blue;

      else
        couleur_i_jp = image->data[i*image->width + j+1].blue;

      break;

      default:
        return -1; // ERREUR
        break;
  }

  return abs(couleur_im_j - couleur_ip_j)/2 + abs(couleur_i_jm - couleur_i_jp)/2;
}

static int energie(size_t i, size_t j, const PNMImage* image){
  return energie_aux(i, j, 'r', image) + energie_aux(i, j, 'g', image) + energie_aux(i, j, 'b', image);
}

static SILLON *create_sillon(size_t i, size_t j, const PNMImage* image){
  SILLON *new_sillon = malloc(sizeof(SILLON));
  if(new_sillon == NULL)
    return NULL;

  new_sillon->pos.x = i;
  new_sillon->pos.y = j;

  new_sillon->energie = energie(i, j, image);
  //printf("%ld\n", new_sillon->energie);
  if(new_sillon->energie == -1)
    return NULL;

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
  k = 0;

  PNMImage *image = createPNM(width-k, height);

  for(size_t i = 0; i < height; i++){
    for(size_t j = 0; j < width; j++){
      if(tab_sillon[i*width +j]->energie != -2){
        image->data[i*(width-k) +j] = tab_sillon[i*width +j]->pixel;
        // l++;
        // if(l == width-k)
        //   l = 0;
      }
      else{
        image->data[i*(width)+j].red = 255;
        image->data[i*(width)+j].green = 255;
        image->data[i*(width)+j].blue = 255;
      }
    }
  }

  return image;
}
//
// static PNMImage *sillion_vers_image(SILLON **tab_sillon, size_t height, size_t width, size_t k){
//   k = 0;
//
//   PNMImage *image = createPNM(width-k, height);
//
//   for(size_t i = 0; i < height; i++){
//     for(size_t j = 0; j < width-k; j++)
//       image->data[i*(width-k) +j] = tab_sillon[i*width +j]->pixel;
//
// }
//
//   return image;
// }


static SILLON *C(size_t j, size_t i, SILLON **tab_sillon, const PNMImage *image){
  size_t actuel = i*image->width +j;

  if(i == 0){
    tab_sillon[actuel]->cout_cumule = tab_sillon[actuel]->energie;
    return tab_sillon[actuel];
  }

  if(tab_sillon[actuel]->next != NULL)
    return tab_sillon[actuel];

   SILLON *val1 = NULL;
   SILLON *val2 = NULL;
   SILLON *val3 = NULL;

  if(j > 0)
    val1 = C(j-1, i-1, tab_sillon, image);

  if(tab_sillon[(i-1)*image->width+j]->energie != -2)
    val2 = C(j, i-1, tab_sillon, image);

  if(j < image->width-1)
    val3 = C(j+1, i-1, tab_sillon, image);

  SILLON *s = min(min(val1, val2), val3);

  tab_sillon[actuel]->next = s;
  tab_sillon[actuel]->cout_cumule = tab_sillon[actuel]->energie + s->cout_cumule;

  return tab_sillon[actuel];
}

// static void calcul_cout(const PNMImage *image, size_t k, SILLON **tab_sillon){
//   PriorityQueue * pq = pqCreate(NULL, NULL, 0);
//
//   SILLON *tmp = NULL;
//   for(size_t j = 0; j < image->width; j++){
//     if(tab_sillon[(image->height-1)*image->width +j]->energie != -2){
//       tmp = C(j, image->height-1, tab_sillon, image);
//       pqInsert(pq, tmp, tmp->cout_cumule);
//     }
//   }
//
//   // for(size_t i = 0; i < k; i++){
//   SILLON *min2;
//
//   min2 = (SILLON *)pqExtractMin(pq);
//
//   while(min2 != NULL){
//     min2->energie = -2;
//     min2 = min2->next;
//   //}
// }
//
//   pqFree(pq);

// }
static void calcul_cout(const PNMImage *image, size_t k, SILLON **tab_sillon){
  PriorityQueue * pq = pqCreate(NULL, NULL, 0);

  SILLON *tmp = NULL;
  for(size_t j = 0; j < image->width; j++){
    if(tab_sillon[(image->height-1)*image->width +j]->energie != -2){
      tmp = C(j, image->height-1, tab_sillon, image);
      pqInsert(pq, tmp, tmp->cout_cumule);
    }
  }

  // for(size_t i = 0; i < k; i++){
  SILLON *min2;

  min2 = (SILLON *)pqExtractMin(pq);

  while(min2 != NULL){
    min2->energie = -2;
    min2 = min2->next;
  //}
}

  pqFree(pq);
}

PNMImage* reduceImageWidth(const PNMImage* image, size_t k){
  //k = 0; // à enlever

  SILLON **tab_sillon = malloc(sizeof(SILLON*)*image->width*image->height);

  for(size_t i = 0; i < image->height; i++){
    for(size_t j = 0; j < image->width; j++){
      tab_sillon[i*image->width + j] = create_sillon(i, j, image);
      if(tab_sillon[i*image->width + j] == NULL)
        return NULL;
      }
  }

  for(size_t i = 0; i < k; i++)
    calcul_cout(image, k, tab_sillon);

  PNMImage *image2 = sillion_vers_image(tab_sillon, image->height, image->width, k);

  libere_tab_sillon(tab_sillon, image);

  return image2;

}
