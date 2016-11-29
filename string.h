#ifndef _MY_STRING_H
#define _MY_STRING_H

// prevzaty z ukazkoveho projektu "Zjednodušená implementace interpretu jednoduchého jazyka", zatim beze zmeny

typedef struct string_struct
{
  char* str;		// misto pro dany retezec ukonceny znakem '\0'
  int length;		// skutecna delka retezce
  int allocSize;	// velikost alokovane pameti
} string;

int strInit(string *s);
void strFree(string *s);

void strClear(string *s);
int strAddChar(string *s1, char c);
int strCopyString(string *s1, string *s2);
int strCmpString(string *s1, string *s2);
int strCmpConstStr(string *s1, char *s2);

char *strGetStr(string *s);
int strGetLength(string *s);

// ====== funkce ifj2016 =======
//funkce 'find' a 'sort' jsou v souboru ial.c
int length(char *s);
char *substr(char *s, int i, int n);
int compare(char *s1, char *s2);

int readInt();
double readDouble();
char *readString();

#endif
