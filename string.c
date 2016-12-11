/**
 * Implementace interpretu imperativniho jazyka IFJ16
 * 
 * xzarsk03   Jan Zarsky
 * xvlcek23   David Vlcek
 * xpelan04   Pelantova Lucie
 * xmrlik00   Vit Mrlik
 * xpapla00   Andrei Paplauski
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>

#include "ial.h"
#include "string.h"
#include "error_codes.h"

extern int ifj_errno;

// funkce inspirovany ukazkovym projektem "Zjednodusena implementace interpretu jednoducheho jazyka"

// funkce vytvori novy retezec
int strInit(string *s){
   s->str = (char*) malloc(STR_LEN_INC*sizeof(char));

   if (s->str == NULL)
      return ER_INTERN;

   s->str[0] = '\0';
   s->length = 0;
   s->allocSize = STR_LEN_INC;

   return ER_OK;
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
   if (s1->length + 1 >= s1->allocSize) {
      // pamet nestaci, je potreba provest realokaci
      s1->str = (char *) realloc(s1->str, (s1->length + STR_LEN_INC)*sizeof(char));

      if (s1->str == NULL)
         return ER_INTERN;

      s1->allocSize = s1->length + STR_LEN_INC;
   }

   s1->str[s1->length] = c;
   s1->length++;
   s1->str[s1->length] = '\0';

   return ER_OK;
}

// prekopiruje retezec s2 do s1
int strCopyString(string *s1, string *s2) {
   int newLength = s2->length;

   if (newLength >= s1->allocSize) {
      // pamet nestaci, je potreba provest realokaci
      s1->str = (char *) realloc(s1->str, (newLength + 1)*sizeof(char));

      if (s1->str == NULL)
         return ER_INTERN;

      s1->allocSize = newLength + 1;
   }

   strcpy(s1->str, s2->str);
   s1->length = newLength;

   return ER_OK;
}

// porovna oba retezce a vrati vysledek
int strCmpString(string *s1, string *s2) {
   return strcmp(s1->str, s2->str);
}

// porovna nas retezec s konstantnim retezcem
int strCmpConstStr(string *s1, char* s2) {
   return strcmp(s1->str, s2);
}

// vrati textovou cast retezce
char *strGetStr(string *s) {
   return s->str;
}

// vrati delku daneho retezce
int strGetLength(string *s) {
   return s->length;
}

// ZABUDOVANE FUNKCE

//Vrati delku retezce. (napr:  ifj16.length("x\nz") == 3 )
int length(char *s) {
  return (int) strlen(s);
}

//vrati podretezec zadaneho retezce 's'
//parametr 'i' udava zacatek pozadovaného podretezce (pocitano od nuly)
//parametr 'n' urcuje delku podretezce
//V okrajovych pripadech simulujte metodu substring tridy String z jazyka Java a vracejte chybu 10.
char *substr(char *s, int i, int n)
{
  char *sub = NULL;

  //pokud zacatek podretezce 'i' nepresahuje velikost pole,
  //a zaroven pocet pozadovanych znaku 'n' nepresahuje konec puvodniho retezce,
  //anebo nejsou zaporne
  if ((i < (int) strlen(s)) && (n <= (int) (strlen(s) - i)) && i >= 0 && n >= 0) {
    //vytvareni podretezce
    int need_size = (n / STR_LEN_INC) + 1;  //pomocna promenna, aby byl alokovany prostor nasobek osmi

    if ((sub = (char*) malloc(STR_LEN_INC * need_size)) == NULL) {
        ifj_errno = ER_INTERN;
        return NULL;
    }

    sub = strncpy(sub, s + i, n);
    sub[n] = '\0';
  }
  else {
      ifj_errno = ER_RUN_OTHER;
      return NULL;
  }

  return sub;
  //zkontrolovat s Java 8?
}

//Lexikograficky porovna dva retezce.
//0 pokud jsou stejne, 1 pokud s1 > s2, -1 pro ostatni
//Z hlediska funkcnosti simulujte metodu 'compareTo' tridy String jazyka Java
int compare(char *s1, char *s2)
{
  int result = strcmp(s1, s2);

  if (result > 0)
      return 1;
  
  if (result < 0)
      return -1;

  return 0;
  //zkontrolovat s Java 8?
}

//precte vstup a vrati jej jako integer
int readInt()
{
    //vytvoreni retezce do ktereho se budou ukladat znaky -ten se potom prevede na double
    char *readed = NULL;

    if ((readed = (char*) malloc(sizeof(char) * STR_LEN_INC)) == NULL) {
        ifj_errno = ER_INTERN;
        return -1;
    }

    int allocated = STR_LEN_INC;
    int length = 0;

    //fixme ... problem detected "inF" a "naN" ?
    //fixme ... prazdny vstup? znamenko minus?
    //ukladani znaku...
    char c = getchar();
    for (int i = 0; c >= '0' && c <= '9'; i++)  {

        if (length + 1 > allocated) {
            //pamet nestaci, je potreba provest realokaci
            if ((readed = (char*) realloc(readed, length + STR_LEN_INC)) == NULL) {
                ifj_errno = ER_INTERN;
                return -1;
            }
            allocated = length + STR_LEN_INC;
        }

        readed[i] = c;
        length++;
        c = getchar();
    }

    //pokud vstup obsahuje dale jine znaky nez cisla - chyba
    if (c != '\n' && c != EOF)  {
        ifj_errno = ER_RUN_READ;
        free(readed);
        return -1;  //error, nevim num. 7?
    }

    //pridani koncoveho znaku
    if (length + 1 > allocated) {  //pamet nestaci, je potreba provest realokaci
      if ((readed = (char*) realloc(readed, length + STR_LEN_INC)) == NULL) {
        ifj_errno = ER_RUN_READ;
        return -1;  //error, num. 99
      }
    }

    readed[strlen(readed)] = '\0';

    //prevod samotny...
    int number = atoi(readed);
    free(readed);
    return number;
}

//precte vstup a vrati jej jako double
double readDouble ()
{
    //vytvoreni retezce do ktereho se budou ukladat znaky -ten se potom prevede na double
    char *readed = NULL;

    if ((readed = (char*) malloc(sizeof(char) * STR_LEN_INC)) == NULL) {
        ifj_errno = ER_INTERN;
        return 0;
    }

    int allocated = STR_LEN_INC;
    int length = 0;

    //fixme ... problem detected "inF" a "naN" ?
    //viz http://en.cppreference.com/w/cpp/string/byte/atof
    //fixme ... prazdny vstup? znamenko minus?
    //ukladani znaku...
    char c = getchar();
    bool dot = false;
    for (int i = 0; (c >= '0' && c <= '9') || c == '.'; i++) {

        if (length + 1 > allocated) {
            //pamet nestaci, je potreba provest realokaci
            if ((readed = (char*) realloc(readed, length + STR_LEN_INC)) == NULL) {
                ifj_errno = ER_INTERN;
                return 0; 
            }
            allocated = length + STR_LEN_INC;
        }

        if (c == '.') {
            if (dot == false) //pokud jeste cislo nema des. tecku
                dot = true;
            else {            //pokud uz cislo ma des. tecku - chyba
                free(readed);
                return -1;    //error, nevim num. 7?
            }
        } //if (c == '.')

        readed[i] = c;
        length++;
        c = getchar();
    } //for

    //pokud vstup obsahuje dale jine znaky nez cisla nebo tecku - chyba
    if (c != '\n' && c != EOF) {
        ifj_errno = ER_RUN_READ;
        free(readed);
        return -1;    //error, nevim num. 7?
    }

    //pridani koncoveho znaku
    if (length + 1 > allocated) {  //pamet nestaci, je potreba provest realokaci
      if ((readed = (char*) realloc(readed, length + STR_LEN_INC)) == NULL) {
        ifj_errno = ER_INTERN;
        return 0;
      }
    }

    readed[strlen(readed)] = '\0';

    //prevod samotny
    double number = atof(readed);
    free(readed);
    return number;
}


char* readString()
{
    //vytvoreni retezce do ktereho se ulozi znaky
    char *readed = NULL;

    if ((readed = (char*) malloc(sizeof(char) * STR_LEN_INC)) == NULL) {
        ifj_errno = ER_INTERN;
        return NULL;
    }

    int allocated = STR_LEN_INC;
    int length = 0;

    //ukladani znaku...
    //fixme... prevod escape sekvenci??
    char c;
    c = getchar();
    
    if (c == '\n' || c == EOF)  //pro pripad nacitani prazdneho retezce, viz zadani
        readed[0] = '\0';
   
    for (int i = 0; c != '\n' && c != EOF; i++) {

        if (length + 1 > allocated) {
            //pamet nestaci, je potreba provest realokaci
            if ((readed = (char*) realloc(readed, length + STR_LEN_INC)) == NULL) {
                ifj_errno = ER_INTERN;
                return NULL;
            }

            allocated = length + STR_LEN_INC;
        }

        readed[i] = c;
        length++;
        c = getchar();

    }  //for

    //pridani koncoveho znaku
    if (length + 1 > allocated) {  //pamet nestaci, je potreba provest realokaci
      if ((readed = (char*) realloc(readed, length + STR_LEN_INC)) == NULL) {
          ifj_errno = ER_INTERN;
          return NULL;
      }
    }

    readed[strlen(readed)] = '\0';

    return readed;
}
