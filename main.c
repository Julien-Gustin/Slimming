#include <stdio.h>
#include <math.h>
#include <stdlib.h>

inline static int min(int x, int y){
  return  x < y ? x : y; // minimum de deux valeurs
}
int main(){
  // printf("%d\n", min(1, 2, 1));

    printf("%d\n", min(min(3, 3), 4));
}
