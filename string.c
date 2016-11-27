//KNIHOVNA PRO ZPRACOVANI RETEZCU

// prevzata z ukazkoveho projektu "Zjednodušená implementace interpretu jednoduchého jazyka", zatim beze zmeny

#include <string.h>
#include <malloc.h>
#include "string.h"

#define STR_LEN_INC 8
// konstanta STR_LEN_INC udava, na kolik bytu provedeme pocatecni alokaci pameti
// pokud nacitame retezec znak po znaku, pamet se postupne bude alkokovat na
// nasobky tohoto cisla

#define STR_ERROR  -1
#define STR_SUCCESS 0

// funkce vytvori novy retezec

int strInit(string *s){
   if ((s->str = (char*) malloc(STR_LEN_INC)) == NULL)
      return STR_ERROR;
   s->str[0] = '\0';
   s->length = 0;
   s->allocSize = STR_LEN_INC;
   return STR_SUCCESS;
}

// funkce uvolni retezec z pameti

void strFree(string *s){
   free(s->str);
}

// funkce vymaze obsah retezce

void strClear(string *s){
   s->str[0] = '\0';
   s->length = 0;
}

// prida na konec retezce jeden znak

int strAddChar(string *s1, char c){
   if (s1->length + 1 >= s1->allocSize)
   {
      // pamet nestaci, je potreba provest realokaci
      if ((s1->str = (char*) realloc(s1->str, s1->length + STR_LEN_INC)) == NULL)
         return STR_ERROR;
      s1->allocSize = s1->length + STR_LEN_INC;
   }
   s1->str[s1->length] = c;
   s1->length++;
   s1->str[s1->length] = '\0';
   return STR_SUCCESS;
}

int strCopyString(string *s1, string *s2)
// prekopiruje retezec s2 do s1
{
   int newLength = s2->length;
   if (newLength >= s1->allocSize)
   {
      // pamet nestaci, je potreba provest realokaci
      if ((s1->str = (char*) realloc(s1->str, newLength + 1)) == NULL)
         return STR_ERROR;
      s1->allocSize = newLength + 1;
   }
   strcpy(s1->str, s2->str);
   s1->length = newLength;
   return STR_SUCCESS;
}

int strCmpString(string *s1, string *s2)
// porovna oba retezce a vrati vysledek
{
   return strcmp(s1->str, s2->str);
}

int strCmpConstStr(string *s1, char* s2)
// porovna nas retezec s konstantnim retezcem
{
   return strcmp(s1->str, s2);
}

char *strGetStr(string *s)
// vrati textovou cast retezce
{
   return s->str;
}

int strGetLength(string *s)
// vrati delku daneho retezce
{
   return s->length;
}

//Vrati delku retezce. (napr:  ifj16.length("x\nz") == 3 )
int length(char *s)
{
  return (int) strlen(s);
}

//vrati podretezec zadaneho retezce 's'
//parametr 'i' udava zacatek pozadovaného podretezce (pocitano od nuly)
//parametr 'n' urcuje delku podretezce
//V okrajovych pripadech simulujte metodu substring tridy String z jazyka Java a vracejte chybu 10.
char *substr(char *s, int i, int n)
{
  char *sub;

  //pokud zacatek podretezce 'i' nepresahuje velikost pole,
  //a zaroven pocet pozadovanych znaku 'n' nepresahuje konec puvodniho retezce,
  //anebo nejsou zaporne
  if ((i < (int) strlen(s)) && (n <= (int) (strlen(s) - i)) && i >= 0 && n >= 0) {

    //vytvareni podretezce
    int need_size = (n / STR_LEN_INC) + 1;  //pomocna promenna, aby byl alokovany prostor nasobek osmi
    if ((sub = (char*) malloc(STR_LEN_INC * need_size)) == NULL)
      printf("CHYBA pri realokaci\n"); //error

    sub = strncpy(sub, s + i, n);
    sub[n] = '\0';

  } //konec if ((i < s.length) && (n <= ... ) ... )
  else {
    //error, num. 10
    printf("CHYBA, mimo rozsah.\n");

  }
  return sub;
  //zkontrolovat s Java 8?
}

//Lexikograficky porovna dva retezce.
//0 pokud jsou stejne, 1 pokud s1 > s2, -1 pro ostatni
//Z hlediska funkcnosti simulujte metodu 'compareTo' tridy String jazyka Java
int compare(char *s1, char *s2)
{
  return strcmp(s1, s2);
  //zkontrolovat s Java 8?
}
