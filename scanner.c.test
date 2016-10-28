#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "scanner.h"
#include "instrlist.h"
#include "symtab.h"

#include "string.h"

FILE *source;
int token; 
string attr; 

void setSourceFile(FILE *f)
{
  source = f;
}

int lexer(string *buffer){

    int state = 0; // stav automatu
    int c; // promenna pro znak

    int dot_count = 0; // promenna pro kontrolu spravnosti plne kvalifikovaneho identifikatoru
    int quote_count = 0; // promenna pro signalizaci pouziti znaku " uvnitr stringu
    int sign_count = 0; // promenna pro signalizaci pouziti nepovinneho znamenka u desetinneho exponentu
    int star_count = 0; // promenna pro signalizaci spravneho ukonceni blokoveho komentare

    int plus_count = 0; // kontrola jestli je na vstupu +
    int minus_count = 0; // kontrola jestli je na vstupu -
    int less_count = 0; // kontrolla jestli je na vstupu <
    int great_count = 0; // kontrola jestli je na vstupu >
    int excl_count = 0;  // kontrola jestli je na vstupu !
    int eq_count   = 0; // kontrola jestli je na vstupu =

    strClear(buffer); // vymazat soucasny obsah stringu

    while(1){ // cyklus nacitani znaku

   if( (c = getc(source)) == EOF ) {return END_OF_FILE; break;} // nacteni znaku ze souboru

    switch (state){ // automat pro zpracovani znaku ze vstupu

    case 0: // POCATECNI STAV

      if (isspace(c)) state = 0; // ignoruj bila mista

      else if (c == '/') { quote_count = 1; state = 8; }// bude bud komentar a nebo se jedna o operator deleni, nejdriv zkontrolovat jestli je to deleni ve stavu 8

      else if (isalpha(c) || c == '_' || c == '$'){ // jedna se o indentifikator nebo klicove slovo

       // printf("jetoznak jdu do 3\n"); // TEST

	    strAddChar(buffer, c); // nahraj znak do struktury
	    state = 3; // preskoc do casu kde se budou resit identifikatory atd.
	 }

	 else if (c == '"'){ // jedna se o zacatek retezce

        strAddChar(buffer, c); // nahraj znak do struktury
        state = 4; // preskoc do stavu pro zpracovani retezce
	 }

	 else if (isdigit(c)){ // jedna se o int nebo double

        strAddChar(buffer, c);
        state = 5;
	 }                    // OSTATNI ZNAKY

	 else if (c == '{') { printf("token {"); return LEFT_VINCULUM; }
	 else if (c == '}') { printf("token }"); return RIGHT_VINCULUM; }
	 else if (c == '(') {  printf("token ("); return LEFT_BRACKET; }
	 else if (c == ')') {  printf("token )");return RIGHT_BRACKET; }
	 else if (c == ';') {  printf("token ;");return SEMICOLON; }
	 else if (c == ',') {   printf("token ,");return COMMA; }

     else if (c == '*' && quote_count == 0) { printf("token *"); return MUL; }
	 else if (c == '+') { plus_count = 1; state = 8;}
	 else if (c == '-') { minus_count = 1; state = 8;}
	 else if (c == '<') { less_count = 1; state = 8;}
	 else if (c == '>') { great_count = 1; state = 8;}
	 else if (c == '=') { eq_count = 1; state = 8;}
	 else if (c == '!') { excl_count = 1; state = 8;}
	 else return LEX_ERROR;
     break;

    case 1: // RADKOVY KOMENTAR

        if (c != '\n') state = 1;  // jednoradkovy komentar, zustan tu a ignoruj ho

        if (c == '\n') state = 0; // konec radku a teda i komentare ???

        break;

    case 2: // BLOKOVY KOMENTAR

        if (c != '*' && c!= '/') {  /*printf("%c", (char)c);*/  state = 2;} // uvnitr blokoveho komentare, ignoruj

        else if (c == '*') { star_count = 1; state = 2;} // eviduj hvezdicku a zustan tady

        else if (c == '/' && star_count == 1) { star_count = 0; state = 0;} // konec blokoveho komentare, vrat se na zacatek do nuly

        else if (c == EOF) {printf("nasel eof"); return LEX_ERROR;} // Testuj neukonceny komentar???!!

        break;

    case 3: // IDENTIFIKATORY, KLICOVA SLOVA

        if (isalnum(c) || c == '_' || c == '$' || c == '.'){ // pridana tecka jako znak pouzivany pro plne kvalifikovany identifikator

        if (c == '.' && dot_count == 0) dot_count++; // evidujeme nalezenou tecku

        else if (c == '.' && dot_count > 0 ) {return LEX_ERROR; break;} // pokud se objevi dalsi tecka jedna se o neplatny identifikator,

        strAddChar(buffer, c); // dokud se jedna o identifikator nebo klicove slovo, naplnuj strukturu

        state = 3; // zustan tady a res identifikatory a klicova slova

        }

	    else {// struktura naplnena, nasleduje prazdne misto nebo nepovoleny znak nebo zacatek zavorky

        if (!isspace(c) && !isalnum(c) && c != '_' && c != '$' && c != '.' && c != '(' && c!= ')' && c!= '{' && c!= '}' && c!= '=' && c!= '+' && c!= '-' && c!= '*' && c!= '/' && c!= '<' && c!= '>' && c!='!' && c!= ';' && c != ',') { return LEX_ERROR; break; } // pokud se neobjevi prazdne misto nebo zavorky nebo operatory ale nejaky nepovoleny znak je to error

        ungetc(c, source); // POZOR! Je potreba vratit posledni nacteny znak

        // kontrola, zda se nejedna o klicove slovo nebo treba povinnou vestavenou fci

	    if (strCmpConstStr(buffer, "boolean") == 0) {printf("token Boolean"); return BOOLEAN;}
   else if (strCmpConstStr(buffer, "break") == 0) {printf("token Break"); return BREAK; }
   else if (strCmpConstStr(buffer, "class") == 0) {printf("token Class"); return CLASS;}
   else if (strCmpConstStr(buffer, "continue") == 0) {printf("token Continue"); return CONTINUE;}
   else if (strCmpConstStr(buffer, "do") == 0) {printf("token Do"); return DO; }
   else if (strCmpConstStr(buffer, "double") == 0) {printf("token Double"); return DOUBLE;}
   else if (strCmpConstStr(buffer, "else") == 0) {printf("token Else"); return ELSE;}
   else if (strCmpConstStr(buffer, "false") == 0) {printf("token False");return FALSE;}
   else if (strCmpConstStr(buffer, "for") == 0) {printf("token For");return FOR;}
   else if (strCmpConstStr(buffer, "if") == 0) {printf("token If");return IF;}
   else if (strCmpConstStr(buffer, "int") == 0) {printf("token Int");return INT;}
   else if (strCmpConstStr(buffer, "return") == 0) {printf("token Return"); return RETURN;}
   else if (strCmpConstStr(buffer, "String") == 0) {printf("token String");return STRING;}
   else if (strCmpConstStr(buffer, "static") == 0) {printf("token Static");return STATIC;}
   else if (strCmpConstStr(buffer, "true") == 0) {printf("token True");return TRUE;}
   else if (strCmpConstStr(buffer, "void") == 0) {printf("token Void");return VOID;}
   else if (strCmpConstStr(buffer, "while") == 0) {printf("token While");return WHILE;}
   else if (strCmpConstStr(buffer, "Main") == 0) {printf("token Main"); return MAIN;}
   else if (strCmpConstStr(buffer, "run") == 0) {printf("token Run");return RUN; }

	    else {printf("token ID");return ID;}
        } break;

    case 4: // RETEZCOVY LITERAL

        if (c != '"' && c!= '\x5C'){ // dokud sme v retezci a nejsou pouzity specialni znaky jako \n \"

         strAddChar(buffer, c); // tak normalne naplnuj strukturu

         quote_count = 0; // radsi furt nuluj quote_count jelikoz se nejedna o specialni znak

         state = 4; // a zustan tady
        }

        else if (c == '\x5C'){ // bude nasledovat specialni znak

         strAddChar(buffer, c); // nahraj lomitko do struktury

         quote_count = 1; // signalizuj ze se bude jedna o nejaky spec. znak

         state = 4; // a zustan tady

        }

        else if (c == '"' && quote_count == 1){ // jedna se o uvozovky uvnitr stringu NE signalizujici jeho konec

         strAddChar(buffer, c); // hod je do struktury

         state = 4; // a zustan tady

        }

        else if (c == '"' && quote_count == 0){ // sme na konci retezce

         strAddChar(buffer, c); // nahraj koncove uvozovky do struktury

         printf("token String Literal");

         return STRING_LITERAL; // vrat retezcovy literal
        } break;

    case 5: // CELOCISELNY LITERAL

        if (isdigit(c)){ // pokud prichazi cislo

         strAddChar(buffer, c); // pln strukturu

         state = 5; // a zustan tady
        }

        else if (!isdigit(c) && c != ';' && c != '.' && c != 'e' && c != 'E' && !isspace(c) && c != ')' && c != '+' && c != '-' && c != '/' && c != '*' && c != '!'){ // pokud nasleduje znak jiny nez ; ktery signalizuje konec zadavani literalu nebo tecka signalizujici des. cislo nebo exponent, je to error

          return LEX_ERROR; break;
        }

        else if (c == ';'){

          ungetc(c, source); // konec celeho cisla, vracime ; nebo volny zpatky, zpracujem pak

          printf("token Int Literal");

          return INT_LITERAL; // a vrati se celociselny literal

        }

        else if (isspace(c) || c == ')'){ // nasleduje volne misto, konec zadavani cisla, cislo muze byt zprava v zavorce! DOPLNIT DO FLOATU!!!!

          ungetc(c, source);

          printf("token Int Literal");

          return INT_LITERAL; // a vrati se celociselny literal
        }

        else if(c == '.') { // bude se jednat o desetinny literal ve kterem se nachazi desetinna cast

         strAddChar(buffer, c);
         state = 6;

        }

        else if(c == 'e' || c == 'E') { // bude se jednat o desetinny literal ve kterem NENI desetinna cast ale pouze exponent

         strAddChar(buffer, c);
         state = 7;

        }
        break;

    case 6: // DESETINNY LITERAL

        if (isdigit(c)){ // pokud prichazi cislo

         strAddChar(buffer, c); // pln strukturu

         state = 6; // a zustan tady
        }

        else if (c == 'e' || c == 'E'){ // nasleduje exponent

         strAddChar(buffer, c);

         state = 7;
        }

        else if (!isdigit(c) && c != ';' && c != 'e' && c != 'E' && !isspace(c) && c != ')' && c != '+' && c != '-' && c != '/' && c != '*' && c != '!'){ // pokud nasleduje znak jiny nez ; ktery signalizuje konec zadavani literalu a nebo exponent, je to error

          return LEX_ERROR; break;
        }

        else if (c == ';'){

          ungetc(c, source); // konec desetinneho cisla, vracime ; zpatky, zpracujem pak

          printf("token Double Literal");

          return DOUBLE_LITERAL; // a vrati se desetinny literal bez exponentu

        }

        else if (isspace(c) || c == ')'){

        ungetc(c, source);

        printf("token Double Literal");

        return DOUBLE_LITERAL; // a vrati se celociselny literal

        }

        break;

    case 7: // DESETINNY LITERAL S EXPONENTEM

        if ((c == '+' || c == '-') && sign_count == 0){ // nasleduje nepovinne znamenko

         sign_count = 1; // bylo pouzito nepovinne znamenko

         strAddChar(buffer, c); // uloz ho do struktury

         state = 7; // a zustan tady
        }

        else if (isdigit(c)) { // nasleduje neprazdna posloupnost cislic

         strAddChar(buffer, c); // uloz ho do struktury

         state = 7; // a zustan tady
        }

        else if (!isdigit(c) && c != ';' && c != '+' && c != '-') {// nic jineho nez cisla za exponentem nebo semicolon tu byt nemuze

         return LEX_ERROR; break;
        }

        else if (c == '+' || (c == '-' && sign_count == 1)) { // pokus o pouziti dalsiho znamenka, nepovoleno

         return LEX_ERROR; break;
        }

        else if (c == ';'){

          ungetc(c, source); // konec desetinneho cisla s exponentem, vracime ; zpatky, zpracujem pak

          printf("token Double Literal");

          return DOUBLE_LITERAL; // a vrati se desetinny literal bez exponentu

        } break;

    case 8: // OPERATORY, SLOZENE OPERATORY, ROZPOZNAVANI ZACATKU KOMENTARU

          if (plus_count == 1 && c == '+')  {printf("token ++"); return INC; }
     else if (plus_count == 1 && c != '+')  {ungetc(c, source); printf("token +");return PLUS;} // vrat neplatny znak, je to plus
     else if (minus_count == 1 && c == '-') {printf("token --"); return DEC; }
     else if (minus_count == 1 && c != '-') {ungetc(c, source); printf("token -"); return MINUS;} // vrat neplatny znak, je to minus
     else if (less_count == 1 && c == '=')  {printf("token <="); return LESS_EQ;}
     else if (less_count == 1 && c != '=')  {ungetc(c, source); printf("token <");return LESS; } // vrat neplatny znak, je to mensi nez
     else if (great_count == 1 && c == '=') {printf("token >=");return GREAT_EQ;}
     else if (great_count == 1 && c != '=') {ungetc(c, source); printf("token >");return GREAT; } // vrat neplatny znak, je to vetsi nez
     else if (excl_count == 1 && c == '=')  {printf("token !=");return N_EQUAL;}
     else if (eq_count == 1 && c != '=')    {ungetc(c, source); printf("token =");return EQUAL; } // vrat neplatny znak, je to rovnitko
     else if (eq_count == 1 && c == '=')    {printf("token ==");return ASSIGN;} // vrat operator ==
     else if (quote_count == 1 && c != '/' && c != '*') {ungetc(c, source);printf("token /"); return DIV; } // nejedna se o komentar ale o operator deleni
     else if (quote_count == 1 && c == '/') state = 1; // jedna se o jednoradkovy komentar
     else if (quote_count == 1 && c == '*') {state = 2;} // jedna se o blokovy komentar
     else return LEX_ERROR; break;

  } // konec switche
 } // konec while
} // konec funkce

int main(int argc, char** argv) {

    if (argc != 2) return 99;

    if ((source = fopen(argv[1], "r")) == NULL) return 91;

    setSourceFile(source);

    string *x = malloc(sizeof *x);

    strInit(x);

    int vraceni;

    while (vraceni != END_OF_FILE){

    vraceni = lexer(x);

    printf(" - %d\n", vraceni);

    }

    return 0;
}
