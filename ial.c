/*#include "ial.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int readInt()
{
    string *instr;
    int i = 0;
    char c;
    instr = (string*)malloc(sizeof(string));
    if (instr == NULL)
        return -1;
    instr->str = (char*)malloc(sizeof(char)*STR_LEN_INC);
    if (instr->str == NULL)
    {
        free(instr);
        return -1;
    }
    instr->allocSize = 10;
    instr->length = 0;
    instr->str = NULL;
    c = getchar();
    while (c > '0' || c < '9')
    {
        if (instr->length + 1 > instr->allocSize)
        {
        // pamet nestaci, je potreba provest realokaci
            if ((instr->str = (char*) realloc(instr->str, instr->length + STR_LEN_INC)) == NULL)
                return -1;
            instr->allocSize = instr->length + STR_LEN_INC;
        }
        instr->str[i] = c;
        instr->length++;
        c = getchar();
        i++;
    }
    if (c != '\n' || c != EOF)
    {
        free(instr->str);
        free(instr);
        return -1;
    }
    i = atoi(instr->str);
    free(instr->str);
    free(instr);
    return i;
}

double readDouble ()
{
    string *instr;
    int i = 0;
    char c;
    bool dot = false;
    instr = (string*)malloc(sizeof(string));
    if (instr == NULL)
        return -1;
    instr->str = (char*)malloc(sizeof(char)*STR_LEN_INC);
    if (instr->str == NULL)
    {
        free(instr);
        return -1;
    }
    instr->allocSize = 10;
    instr->length = 0;
    instr->str = NULL;
    c = getchar();
    while ((c > '0' && c < '9') || c == '.')
    {
        if (instr->length + 1 > instr->allocSize)
        {
        // pamet nestaci, je potreba provest realokaci
            if ((instr->str = (char*) realloc(instr->str, instr->length + STR_LEN_INC)) == NULL)
                return -1;
            instr->allocSize = instr->length + STR_LEN_INC;
        }
        if (c == '.')
        {
            if (dot == false)
                dot = true;
            else
            {
                free(instr->str);
                free(instr);
                return -1;
            }
        }
        instr->str[i] = c;
        instr->length++;
        c = getchar();
        i++;
    }
    if (c != '\n' || c != EOF)
    {
        free(instr->str);
        free(instr);
        return -1;
    }
    i = atof(instr->str);
    free(instr->str);
    free(instr);
    return i;
}

char* readString()
{
    string *instr;
    int i = 0;
    char c;
    char *b;
    instr = (string*)malloc(sizeof(string));
    if (instr == NULL)
        return -1;
    instr->str = (char*)malloc(sizeof(char)*STR_LEN_INC);
    if (instr->str == NULL)
    {
        free(instr);
        return -1;
    }
    instr->allocSize = 10;
    instr->length = 0;
    instr->str = NULL;
    c = getchar();
    while (c != '\n' || c != EOF)
    {
        if (instr->length + 1 > instr->allocSize)
        {
        // pamet nestaci, je potreba provest realokaci
            if ((instr->str = (char*) realloc(instr->str, instr->length + STR_LEN_INC)) == NULL)
                return -1;
            instr->allocSize = instr->length + STR_LEN_INC;
        }
        instr->str[i] = c;
        instr->length++;
        c = getchar();
        i++;
    }
    free(instr->str);
    free(instr);
    //return string?
    return ;
}

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

//Seradi znaky v danem retezci 's' tak, aby znak s nizsi ordinalni hodnotou vzdy predchazel znaku s vyssi
// ordinalni hodnotou. Algoritmus: HEAP-SORT
char *sort(char *s)
{
  //vytvoreni noveho stringu, ktery bude obsahovat serazene znaky
  char *sor;
  if ((sor = (char*) malloc ( (strlen(s))+2 ) )== NULL)
    printf("CHYBA pri alokaci\n"); //error

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

int find(String s, String search)
{
  //... Knuth-Morris-Pratt

  return 5; //fixme tato funkce neni dokoncena!
}

*/
