/*#include "ial.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//pomocna funkce sortu
void SiftDown(char *A, int Left, int Right)
{
  int i = Left;           //i = otec
  int j = (2 * i) +1;     //index leveho syna
  char Temp = A[i];       //ulozeni otce

  if (j < Right && A[j] < A[j + 1])
    j = j + 1;

  while (j <= Right && Temp < A[j]) {
    A[i] = A[j];          //nastav nejvetsiho jako otce
    i = j;                //syn se stane otcem pro pristi cyklus
    j = (2 * i) +1;       //pristi levy

    if (j < Right && A[j] < A[j + 1])
      j = j + 1;

  }
  A[i] = Temp;  //konecne umisteni proseteho uzlu
}

char *sort(char *s)
{
  //vytvoreni noveho stringu, ktery bude obsahovat serazene znaky
  char *sor;
  int need_size = ((strlen(s) + 1) / STR_LEN_INC) + 1;  //pomocna promenna, aby byl alokovany prostor nasobek osmi
  if ((sor = (char*) malloc(sizeof(char) * STR_LEN_INC * need_size)) == NULL)
    printf("CHYBA pri alokaci\n"); //error, num. 99

  strcpy(sor, s);

  int Left, Right;
  int N = strlen(sor);
  char *A = sor;

  //ustaveni hromady
  Left = (N -1) / 2;           //index nejpravejsiho nejspodnejsiho uzlu
  Right = N -1;
  for (int i = Left; i >= 0; i--)
    SiftDown(A, i, Right);

  //cyklus heap-sort
  for (int i = Right; i > 0; i--) {
    char Temp = A[0];         //vymena korene s aktualnim poslednim prvkem
    A[0] = A[i];
    A[i] = Temp;
    SiftDown(A, 0, i -1);     //znovuustaveni hromady

  }
  return sor;
}

int find(char *s, char *search)
{
  //... Knuth-Morris-Pratt

  return 5; //fixme tato funkce neni dokoncena!
}

*/
