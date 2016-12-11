#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>

#include "instrlist.h"
#include "string.h"
#include "scanner.h"
#include "error_codes.h"

FILE *source;
int token; 
string attr; 

typedef struct token_t {
    int token;
    char *buffer;
    struct token_t *next;
} token_t;

token_t *token_buffer;

void setSourceFile(FILE *f)
{
  source = f;
}

#ifdef DEBUG
void print_token(int symbol) {
    switch (symbol) {
        case PLUS:
            printf("+"); break;
        case MINUS:
            printf("-"); break;
        case MUL:
            printf("*"); break;
       case DIV:
            printf("/"); break;
        case LESS:
            printf("'<'"); break;
        case GREAT:
            printf("'>'"); break;
        case LESS_EQ:
            printf("'<='"); break;
        case GREAT_EQ:
            printf("'>='"); break;
        case EQUAL:
            printf("=="); break;
        case N_EQUAL:
            printf("!="); break;
        case LEFT_BRACKET:
            printf("("); break;
        case RIGHT_BRACKET:
            printf(")"); break;
        case LEFT_VINCULUM:
            printf("{"); break;
        case RIGHT_VINCULUM:
            printf("}"); break;
        case COMMA:
            printf(","); break;
        case ASSIGN:
            printf("="); break;
        //case :
        //    printf(""); break;
        case INT_LITERAL:
            printf("int literal"); break;
        case DOUBLE_LITERAL:
            printf("double literal"); break;
        case STRING_LITERAL:
            printf("str literal"); break;
        case ID:
            printf("id"); break;
        case SEMICOLON:
            printf(";"); break;
        case END_OF_FILE:
            printf("$"); break;
        default:
            printf("%d", symbol); break;
    }
}
#endif

void return_token(int token, char *buffer) {
    token_t *temp = malloc(sizeof(token_t));

    if (temp == NULL)
        return;

    temp->token = token;
    temp->buffer = buffer;
    temp->next = token_buffer;

    token_buffer = temp;

#ifdef DEBUG
    printf("SCANNER: returning token ");
    print_token(token);
    printf(", %s\n", buffer);
#endif
}

int lexer(string *buffer);

int get_next_token(char **buffer) {
    int result;

    if (token_buffer == NULL) {
        string str;
        strInit(&str);
        result = lexer(&str);
        *buffer = str.str;

#ifdef DEBUG
        printf("SCANNER: sending token ");
        print_token(result);
        printf(", %s\n", *buffer);
#endif

        return result;
    }

    result = token_buffer->token;
    *buffer = token_buffer->buffer;

    token_t *temp = token_buffer;
    token_buffer = token_buffer->next;
    free(temp);

#ifdef DEBUG
    printf("SCANNER: sending token ");
    print_token(result);
    printf(", %s\n", *buffer);
#endif

    return result;
}

int lexer(string *buffer) {
    int state = 0; // stav automatu
    int c; // promenna pro znak
    char a [4]; // promenna pomocna pro zadani retezce pomoci hex cisla
    a[3] = '\0';

    int num_count = 0;
    int E_count = 0;
    int id_count = 0;

    int dbl_dot_count = 0; // promenna pro kontrolu tecky u doublu
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

        state = 4; // preskoc do stavu pro zpracovani retezce
	 }

	 else if (isdigit(c)){ // jedna se o int nebo double

        strAddChar(buffer, c);
        state = 5;
	 }                    // OSTATNI ZNAKY

	 else if (c == '{') {  return LEFT_VINCULUM; }
	 else if (c == '}') {  return RIGHT_VINCULUM; }
	 else if (c == '(') {  return LEFT_BRACKET; }
	 else if (c == ')') {  return RIGHT_BRACKET; }
	 else if (c == ';') {  return SEMICOLON; }
	 else if (c == ',') {  return COMMA; }

     else if (c == '*' && quote_count == 0) { return MUL; }
	 else if (c == '+') { plus_count = 1; state = 8;}
	 else if (c == '-') { minus_count = 1; state = 8;}
	 else if (c == '<') { less_count = 1; state = 8;}
	 else if (c == '>') { great_count = 1; state = 8;}
	 else if (c == '=') { eq_count = 1; state = 8;}
	 else if (c == '!') { excl_count = 1; state = 8;}
	 else return ER_LEX;
     break;

    case 1: // RADKOVY KOMENTAR

        if (c != '\n') state = 1;  // jednoradkovy komentar, zustan tu a ignoruj ho

        if (c == '\n') state = 0; // konec radku a teda i komentare ???

        break;

    case 2: // BLOKOVY KOMENTAR

        if (c != '*' && c!= '/') { state = 2;} // uvnitr blokoveho komentare, ignoruj

        else if (c == '*') { star_count = 1; state = 2;} // eviduj hvezdicku a zustan tady

        else if (c == '/' && star_count == 1) { star_count = 0; state = 0;} // konec blokoveho komentare, vrat se na zacatek do nuly

        else if (c == EOF) {return ER_LEX;} // Testuj neukonceny komentar???!!

        break;

    case 3: // IDENTIFIKATORY, KLICOVA SLOVA

        if (isalnum(c) || c == '_' || c == '$' || c == '.'){ // pridana tecka jako znak pouzivany pro plne kvalifikovany identifikator

        if (c == '.' && dot_count == 0) { dot_count++; // evidujeme nalezenou tecku
					 
	if (strCmpConstStr(buffer, "boolean") == 0 || strCmpConstStr(buffer, "break") == 0 || strCmpConstStr(buffer, "class") == 0 || strCmpConstStr(buffer, "continue") == 0
              || strCmpConstStr(buffer, "do") == 0 || strCmpConstStr(buffer, "double") == 0 || strCmpConstStr(buffer, "else") == 0 || strCmpConstStr(buffer, "false") == 0
              || strCmpConstStr(buffer, "for") == 0 || strCmpConstStr(buffer, "if") == 0 || strCmpConstStr(buffer, "int") == 0 || strCmpConstStr(buffer, "return") == 0
              || strCmpConstStr(buffer, "String") == 0 || strCmpConstStr(buffer, "static") == 0 || strCmpConstStr(buffer, "true") == 0 || strCmpConstStr(buffer, "void") == 0
              || strCmpConstStr(buffer, "while") == 0 || strCmpConstStr(buffer, "Main") == 0 || strCmpConstStr(buffer, "run") == 0 )

        {return ER_LEX; break;} // otestuj jestli prvni cast plne kvalifikovaneho identifikatoru neni klicove slovo

        }				 

        else if (c == '.' && dot_count > 0 ) {return ER_LEX; break;} // pokud se objevi dalsi tecka jedna se o neplatny identifikator,
        
	else if (c != '.' && dot_count > 0) {

            id_count = id_count + 1;

        }
        
	strAddChar(buffer, c); // dokud se jedna o identifikator nebo klicove slovo, naplnuj strukturu

        state = 3; // zustan tady a res identifikatory a klicova slova

        }

	    else {// struktura naplnena, nasleduje prazdne misto nebo nepovoleny znak nebo zacatek zavorky

        if (!isspace(c) && !isalnum(c) && c != '_' && c != '$' && c != '.' && c != '(' && c!= ')' && c!= '{' && c!= '}' && c!= '=' && c!= '+' && c!= '-' && c!= '*' && c!= '/' && c!= '<' && c!= '>' && c!='!' && c!= ';' && c != ',') { return ER_LEX; break; } // pokud se neobjevi prazdne misto nebo zavorky nebo operatory ale nejaky nepovoleny znak je to error

        ungetc(c, source); // POZOR! Je potreba vratit posledni nacteny znak
		    
	 if (dot_count > 0) { // jednalo se o plne kvalifikovany identifikator, potreba zkontrolovat, jestli v jeho druhe casti neni klicove slovo

                int delka = buffer->length +1;
                int delka2 = delka - (id_count +1);
                char kontrola [id_count +1];

                for (int i = 0; i < id_count +1; i++){

                    kontrola[i] = buffer->str[delka2];
                    delka2 = delka2 + 1;

                }

                kontrola[id_count +1] = '\0';

                if (strcmp(kontrola, "boolean") == 0 || strcmp(kontrola, "break") == 0 || strcmp(kontrola, "class") == 0 || strcmp(kontrola, "continue") == 0
                    || strcmp(kontrola, "do") == 0 || strcmp(kontrola, "double") == 0 || strcmp(kontrola, "else") == 0 || strcmp(kontrola, "false") == 0
                    || strcmp(kontrola, "for") == 0 || strcmp(kontrola, "if") == 0 || strcmp(kontrola, "int") == 0 || strcmp(kontrola, "return") == 0
                    || strcmp(kontrola, "String") == 0 || strcmp(kontrola, "static") == 0 || strcmp(kontrola, "true") == 0 || strcmp(kontrola, "void") == 0
                    || strcmp(kontrola, "while") == 0 || strcmp(kontrola, "Main") == 0 || strcmp(kontrola, "run") == 0 ) return ER_LEX;

        }	    

        // kontrola, zda se nejedna o klicove slovo nebo treba povinnou vestavenou fci

	    if (strCmpConstStr(buffer, "boolean") == 0) { return BOOLEAN;}
   else if (strCmpConstStr(buffer, "break") == 0) { return BREAK; }
   else if (strCmpConstStr(buffer, "class") == 0) { return CLASS;}
   else if (strCmpConstStr(buffer, "continue") == 0) { return CONTINUE;}
   else if (strCmpConstStr(buffer, "do") == 0) { return DO; }
   else if (strCmpConstStr(buffer, "double") == 0) { return DOUBLE;}
   else if (strCmpConstStr(buffer, "else") == 0) { return ELSE;}
   else if (strCmpConstStr(buffer, "false") == 0) {return FALSE;}
   else if (strCmpConstStr(buffer, "for") == 0) {return FOR;}
   else if (strCmpConstStr(buffer, "if") == 0) {return IF;}
   else if (strCmpConstStr(buffer, "int") == 0) {return INT;}
   else if (strCmpConstStr(buffer, "return") == 0) {return RETURN;}
   else if (strCmpConstStr(buffer, "String") == 0) {return STRING;}
   else if (strCmpConstStr(buffer, "static") == 0) {return STATIC;}
   else if (strCmpConstStr(buffer, "true") == 0) {return TRUE;}
   else if (strCmpConstStr(buffer, "void") == 0) {return VOID;}
   else if (strCmpConstStr(buffer, "while") == 0) {return WHILE;}
   else if (strCmpConstStr(buffer, "Main") == 0) { return MAIN;}
   else if (strCmpConstStr(buffer, "run") == 0) {return RUN; }

	    else {return ID;}
        } break;

    case 4: // RETEZCOVY LITERAL

         if (c != '"' && c!= '\x5C' && quote_count == 0 && c != '\n'){ // dokud sme v retezci a nejsou pouzity specialni znaky jako \n \" a nema nasledovat neco za spec znakem

         strAddChar(buffer, c); // tak normalne naplnuj strukturu

         //quote_count = 0; // radsi furt nuluj quote_count jelikoz se nejedna o specialni znak

         state = 4; // a zustan tady
        }

        else if (c == '\x5C' && quote_count == 0){ // bude nasledovat specialni znak

         quote_count = 1; // signalizuj ze se bude jedna o nejaky spec. znak

         state = 4; // a zustan tady

        }

        else if (c == 'n' && quote_count == 1) { // \n preved na new line

        c = '\x0A';

        strAddChar(buffer, c);

        quote_count = 0;

		state = 4;
	}

        else if (c == '"' && quote_count == 1){ // jedna se o uvozovky uvnitr stringu NE signalizujici jeho konec

         strAddChar(buffer, c); // hod je do struktury

         quote_count = 0;

         state = 4; // a zustan tady

        }

        else if ( c == 't' && quote_count == 1){ // jedna se o tab

        c = '\x09';

        strAddChar(buffer, c);

        quote_count = 0;

		state = 4;

        }

        else if ( c == '\x5C' && quote_count == 1){ // jedna se o  \ uvnitr stringu

        strAddChar(buffer, c);

        quote_count = 0;

		state = 4;

        }

	else if (c == '\n' && quote_count == 0){

            return ER_LEX; break;
        }

       else if (!isdigit(c) && c != '\x5C' && c != 't' && c != '"' && c != 'n' && quote_count == 1){ // cokoliv jineho  \a, \b atd bude zamitnuto

        return ER_LEX; break;

        }

        else if (isdigit(c) && quote_count == 1) { // znak zadany pomoci \xxx

	a[0] = c;

	quote_count = 0;

	num_count = 1; // prislo prvni cislo

	state = 9;

	}

        else if (c == '"' && quote_count == 0){ // sme na konci retezce

         return STRING_LITERAL; // vrat retezcovy literal
        } break;

    case 5: // CELOCISELNY LITERAL

        if (isdigit(c)){ // pokud prichazi cislo

         strAddChar(buffer, c); // pln strukturu

         state = 5; // a zustan tady
        }

        else if (!isdigit(c) && c != ';' && c != '.' && c != 'e' && c != 'E' && !isspace(c) && c != ')' && c != '+' && c != '-' && c != '/' && c != '*' && c != '!' && c != ',' && c != '=' && c != '<' && c != '>'){ // pokud nasleduje znak jiny nez ; ktery signalizuje konec zadavani literalu nebo tecka signalizujici des. cislo nebo exponent, je to error

          return ER_LEX; break;
        }

        else if (isspace(c) || c == ')' || c == ',' || c == ';' || c == '+' || c == '-' || c == '/' || c == '*' || c == '!' || c == '=' || c == '<' || c == '>'){

          ungetc(c, source); // konec celeho cisla, vracime ; nebo volny zpatky, zpracujem pak
		
	  long int int_control = atoi(buffer->str);

          if (int_control > INT_MAX) {return ER_LEX; break;} 

          return INT_LITERAL; // a vrati se celociselny literal

        }



        else if(c == '.') { // bude se jednat o desetinny literal ve kterem se nachazi desetinna cast

         dbl_dot_count = 1;
         strAddChar(buffer, c);
         state = 6;

        }

        else if(c == 'e' || c == 'E') { // bude se jednat o desetinny literal ve kterem NENI desetinna cast ale pouze exponent

         strAddChar(buffer, c);

         E_count = 1;

         state = 7;

        }
        break;

    case 6: // DESETINNY LITERAL

        if (isdigit(c) && dbl_dot_count == 1){ // pokud prichazi cislo

         dbl_dot_count = 0;

         strAddChar(buffer, c); // pln strukturu

         state = 6; // a zustan tady
        }

        else if (isdigit(c) && dbl_dot_count == 0){

         strAddChar(buffer, c); // pln strukturu

         state = 6; // a zustan tady
        }

        else if (dbl_dot_count == 1 && !isdigit(c)){ // po tecce musi nasledovat cislo

            return ER_LEX;
        }

        else if (c == 'e' || c == 'E'){ // nasleduje exponent

         strAddChar(buffer, c);

         E_count = 1;

         state = 7;
        }

        else if (!isdigit(c) && c != ';' && c != 'e' && c != 'E' && !isspace(c) && c != ')' && c != '+' && c != '-' && c != '/' && c != '*' && c != '!' && c != ',' && c != '=' && c != '<' && c != '>'){ // pokud nasleduje znak jiny nez ; ktery signalizuje konec zadavani literalu nebo tecka signalizujici des. cislo nebo exponent, je to error

          return ER_LEX; break;
        }

        else if (isspace(c) || c == ')' || c == ',' || c == ';' || c == '+' || c == '-' || c == '/' || c == '*' || c == '!' || c == '=' || c == '<' || c == '>'){

          ungetc(c, source); // konec celeho cisla, vracime ; nebo volny zpatky, zpracujem pak

          return DOUBLE_LITERAL; // a vrati se celociselny literal

        }

        break;

    case 7: // DESETINNY LITERAL S EXPONENTEM

        if ((c == '+' || c == '-') && E_count == 1 ){ // nasleduje nepovinne znamenko

         E_count = 0;

         sign_count = 1; // bylo pouzito nepovinne znamenko

         strAddChar(buffer, c); // uloz ho do struktury

         state = 7; // a zustan tady
        }

        else if (!isdigit(c) && sign_count == 1){

           return ER_LEX; break; // za nepovinnym znamenkem nenasledovala neprazdna posloupnost cislic
        }

        else if (isdigit(c) && sign_count == 1){

         sign_count = 0; // bylo pouzito nepovinne znamenko

         strAddChar(buffer, c); // uloz ho do struktury

         state = 7; // a zustan tady
        }

        else if ((c != '+' || c != '-') && !isdigit(c) && E_count == 1){ // hned po exponentu nenasleduje cislo nebo nepovinne znamenko

        return ER_LEX; break;

        }

        else if (isdigit(c) && E_count == 1 ) { // nebylo vyuzito nepovinne znamenko

         strAddChar(buffer, c); // uloz ho do struktury

         E_count = 0;

         state = 7; // a zustan tady
        }

        else if (isdigit(c) && E_count == 0 ) { // neprazdna posloupnost cislic

         strAddChar(buffer, c); // uloz ho do struktury

         state = 7; // a zustan tady
        }

        else if (!isdigit(c) && c != ';' && !isspace(c) && c != ')' && c != '+' && c != '-' && c != '/' && c != '*' && c != '!' && c != ',' && c != '=' && c != '<' && c != '>'){ // pokud nasleduje znak jiny nez ; ktery signalizuje konec zadavani literalu nebo tecka signalizujici des. cislo nebo exponent, je to error

          return ER_LEX; break;
        }

        else if (isspace(c) || c == ')' || c == ',' || c == ';' || c == '+' || c == '-' || c == '/' || c == '*' || c == '!' || c == '=' || c == '<' || c == '>'){

          ungetc(c, source); // konec celeho cisla, vracime ; nebo volny zpatky, zpracujem pak

          return DOUBLE_LITERAL; // a vrati se celociselny literal

        } break;

    case 8: // OPERATORY, SLOZENE OPERATORY, ROZPOZNAVANI ZACATKU KOMENTARU

          if (plus_count == 1 && c == '+')  { return INC; }
     else if (plus_count == 1 && c != '+')  {ungetc(c, source); return PLUS;} // vrat neplatny znak, je to plus
     else if (minus_count == 1 && c == '-') { return DEC; }
     else if (minus_count == 1 && c != '-') {ungetc(c, source);  return MINUS;} // vrat neplatny znak, je to minus
     else if (less_count == 1 && c == '=')  { return LESS_EQ;}
     else if (less_count == 1 && c != '=')  {ungetc(c, source); return LESS; } // vrat neplatny znak, je to mensi nez
     else if (great_count == 1 && c == '=') {return GREAT_EQ;}
     else if (great_count == 1 && c != '=') {ungetc(c, source); return GREAT; } // vrat neplatny znak, je to vetsi nez
     else if (excl_count == 1 && c == '=')  {return N_EQUAL;}
     else if (eq_count == 1 && c != '=')    {ungetc(c, source); return ASSIGN; } // vrat neplatny znak, je to rovnitko
     else if (eq_count == 1 && c == '=')    {return EQUAL;} // vrat operator ==
     else if (quote_count == 1 && c != '/' && c != '*') {ungetc(c, source); return DIV; } // nejedna se o komentar ale o operator deleni
     else if (quote_count == 1 && c == '/') state = 1; // jedna se o jednoradkovy komentar
     else if (quote_count == 1 && c == '*') {state = 2;} // jedna se o blokovy komentar
     else return ER_LEX;
     break;

   case 9: // RETEZCE DODATEK

      if (!isdigit(c) && num_count < 3) { return ER_LEX; } // error typu \01abc

      else if ( isdigit(c) && num_count == 1 ) {

	 a[1] = c;
	 num_count = 2;

	 state = 9;
      }
      else if ( isdigit(c) && num_count == 2 ) {

      a[2] = c;
      num_count = 3; //naplneno mame vsechny cisla

      errno = 0;
      char *endptr;

      int helpmepls = strtoul(a, &endptr, 8);

      if (errno != 0)
          return ER_LEX;

      if (*endptr != '\0')
          return ER_LEX;

      if ( helpmepls > 0377 || helpmepls < 01 ){ return ER_LEX; } else {

      strAddChar(buffer, helpmepls);

      num_count = 0;

      state = 4;

      }

      }

    break;

  } // konec switche
 } // konec while
} // konec funkce



