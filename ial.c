#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ial.h"
#include "error_codes.h"

extern int ifj_errno;

//pomocna funkce sortu, opravi heap tak, aby platilo pravidlo heapu (vsech uzlech musi byt nejvyssi prvek)
void SiftDown(char *A, int Left, int Right)
{
  int i = Left;           //i = otec
  int j = (2 * i) +1;     //index leveho syna
  char Temp = A[i];       //ulozeni otce

  //do promenne j se nastavi index vetsiho syna
  if (j < Right && A[j] < A[j + 1])
    j = j + 1;

  //pokud je otec mensi nez mensi syn
  while (j <= Right && Temp < A[j]) {
    A[i] = A[j];          //nastav nejvetsiho jako otce
    i = j;                //syn se stane otcem pro pristi cyklus
    j = (2 * i) +1;       //pristi levy

    //do promenne j se nastavi index vetsiho syna
    if (j < Right && A[j] < A[j + 1])
      j = j + 1;
  } //while

  A[i] = Temp;  //konecne umisteni proseteho uzlu
}

//seradi prvky v retezci 's' podle ordinalni hodnoty, od nejnizsi po nejvyssi,
//vracen je retezec obsahujici serazene znaky
char *sort(char *s)
{
  //vytvoreni noveho stringu, ktery bude obsahovat serazene znaky
  char *sor = NULL;
  int need_size = ((strlen(s) + 1) / STR_LEN_INC) + 1;  //pomocna promenna, aby byl alokovany prostor nasobek osmi

  if ((sor = (char*) malloc(sizeof(char) * STR_LEN_INC * need_size)) == NULL) {
      ifj_errno = ER_INTERN;
      return NULL;
  }

  strcpy(sor, s);

  int Left, Right;
  int N = strlen(sor);
  char *A = sor;

  Left = (N -2) / 2;            //index nejpravejsiho nejspodnejsiho uzlu
  Right = N -1;			//index max.

  //ustaveni hromady
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

//pomocna funkce 'findu'
//skenuje retezec search a zapisuje do pole shody znaku, ktere budou pouzity jako dalsi pri neshode
void preKMP(char *search, int fail[])
{
  int search_len = strlen(search);
  int r;	//hlavne pro zapis indexu do pole fail

  fail[0] = -1;		//hodnota '-1' je "defaultni" stav promenne 'r'
  for (int i = 1; i < search_len; i++) {   //pocitadlo indexu...
    
    r = fail[i - 1];  //ulozeni predchozi hodnoty 'fail'

    //cyklus je navrzen tak, aby v prvnim pruchodu nedoslo ke cteni nechtene hodnoty z search[-1] a zaroven,
    //aby hlidal, zda-li neni porovnavany znak na indexu [i-1] shodny se znakem na indexu [r]
    //speciality: "AAcAAAx" a dalsi
    while (r > -1 && search[r] != search[i - 1])
      r = fail[r];  //do 'r' se ulozi cislo z pole 'fail', ktere je na pozici "konce shody"

  //pokud byla promenna 'r' v "defaultnim" stavu (r = -1), tak se ulozi hodnota 0
  //pokud byla promenna vetsi nez -1, tak se zacne navysovat, jelikoz doslo ke shode znaku v retezci
  fail[i] = r + 1;
  }
}

//funkce vrati pozici (pocitano od 0) na ktere zacina retezec 'search', pokud se nachazi v retezci 's'
//pokud 's' neobsahuje retezec 'search' funkce vraci hodnotu -1
//Algoritmus:  Knuth-Morris-Prattuv (KMP)
int find(char *s, char *search)
{
  int string_len = strlen(s);
  int search_len = strlen(search);

  //pro pripad hledani prazdneho retezce
  if (search_len == 0)
    return 0;

  //vytvoreni pomocneho pole
  int fail[search_len];
  preKMP(search, fail);

  //hledani
  int string_index = 0;
  int search_index = 0;

  while (string_index < string_len) {
    //shoda znaku
    if (search[search_index] == s[string_index]) {
      search_index++;
      string_index++;

      //nalezeno kompletne
      if (search_index == search_len)
        return string_index - search_index;
    }
    //neshoda znaku
    else {
      if (search_index != -1)
        search_index = fail[search_index];
      else {
        search_index = 0;
        string_index++;
      }
    }
  } //while

  //retezec nebyl nalezen
  return -1;
}
