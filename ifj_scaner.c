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

    c = getc(source); // nacteni znaku ze souboru

    switch (state){ // automat pro zpracovani znaku ze vstupu

    case 0: // POCATECNI STAV

      if (isspace(c)) state = 0; // ignoruj bila mista

      else if (c == '/') { quote_count = 1; state = 8; }// bude bud komentar a nebo se jedna o operator deleni, nejdriv zkontrolovat jestli je to deleni ve stavu 8

      else if (isalpha(c) || c == '_' || c == '$'){ // jedna se o indentifikator nebo klicove slovo

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

	 else if (c == '{') return LEFT_VINCULUM;
	 else if (c == '}') return RIGHT_VINCULUM;
	 else if (c == '(') return LEFT_BRACKET;
	 else if (c == ')') return RIGHT_BRACKET;
	 else if (c == ';') return SEMICOLON;
         else if (c == ',') return COMMA; 

     else if (c == '*') return MUL;
	 else if (c == '+') { plus_count = 1; state = 8;}
	 else if (c == '-') { minus_count = 1; state = 8;}
	 else if (c == '<') { less_count = 1; state = 8;}
	 else if (c == '>') { great_count = 1; state = 8;}
         else if (c == '=') { eq_count = 1; state = 8;} 
	 else return LEX_ERROR;
     break;

    case 1: // RADKOVY KOMENTAR

        if (c != '\n') state = 1;  // jednoradkovy komentar, zustan tu a ignoruj ho

        if (c == '\n') state = 0; // konec radku a teda i komentare ???

    case 2: // BLOKOVY KOMENTAR

        if (c != '*') state = 2; // uvnitr blokoveho komentare, ignoruj

        else if (c == '*') {star_count = 1; state = 2;} // eviduj hvezdicku a zustan tady

        else if (c == '/' && star_count == 1) state = 0; // konec blokoveho komentare, vrat se na zacatek do nuly

        else if (c == EOF) return LEX_ERROR; break; // Testuj neukonceny komentar???!!

    case 3: // IDENTIFIKATORY, KLICOVA SLOVA

        if (isalnum(c) || c == '_' || c == '$' || c == '.'){ // pridana tecka jako znak pouzivany pro plne kvalifikovany identifikator

        state = 3; // zustan tady a res identifikatory a klicova slova

        if (c == '.' && dot_count == 0) dot_count++; // evidujeme nalezenou tecku

        else if (c == '.' && dot_count > 0 ) return LEX_ERROR; break; // pokud se objevi dalsi tecka jedna se o neplatny identifikator,

        strAddChar(buffer, c); // dokud se jedna o identifikator nebo klicove slovo, naplnuj strukturu
        }

	    else {// struktura naplnena, nasleduje prazdne misto nebo nepovoleny znak nebo zacatek zavorky

        if (!isspace(c) && !isalnum(c) && c != '_' && c != '$' && c != '.' && c != '(' && c!= ')' && c!= '{' && c!= '}' && c!= '=' && c!= '+' && c!= '-' && c!= '*' && c!= '/' && c!= '<' && c!= '>' && c!='!' && c!= ';') return LEX_ERROR; break; // pokud se neobjevi prazdne misto nebo zavorky nebo operatory ale nejaky nepovoleny znak je to error

        ungetc(c, source); // POZOR! Je potreba vratit posledni nacteny znak

        // kontrola, zda se nejedna o klicove slovo

	    if (strCmpConstStr(buffer, "boolean") == 0) return BOOLEAN;
   else if (strCmpConstStr(buffer, "break") == 0) return BREAK;
   else if (strCmpConstStr(buffer, "class") == 0) return CLASS;
   else if (strCmpConstStr(buffer, "continue") == 0) return CONTINUE;
   else if (strCmpConstStr(buffer, "do") == 0) return DO;
   else if (strCmpConstStr(buffer, "double") == 0) return DOUBLE;
   else if (strCmpConstStr(buffer, "else") == 0) return ELSE;
   else if (strCmpConstStr(buffer, "false") == 0) return FALSE;
   else if (strCmpConstStr(buffer, "for") == 0) return FOR;
   else if (strCmpConstStr(buffer, "if") == 0) return IF;
   else if (strCmpConstStr(buffer, "int") == 0) return INT;
   else if (strCmpConstStr(buffer, "return") == 0) return RETURN;
   else if (strCmpConstStr(buffer, "String") == 0) return STRING;
   else if (strCmpConstStr(buffer, "static") == 0) return STATIC;
   else if (strCmpConstStr(buffer, "true") == 0) return TRUE;
   else if (strCmpConstStr(buffer, "void") == 0) return VOID;
   else if (strCmpConstStr(buffer, "while") == 0) return WHILE;
   else if (strCmpConstStr(buffer, "Main") == 0) return MAIN;
   else if (strCmpConstStr(buffer, "run") == 0) return RUN;

	    else return ID;
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

         return STRING_LITERAL; // vrat retezcovy literal
        } break;

    case 5: // CELOCISELNY LITERAL

        if (isdigit(c)){ // pokud prichazi cislo

         strAddChar(buffer, c); // pln strukturu

         state = 5; // a zustan tady
        }

        else if (!isdigit(c) && c != ';' && c != '.' && c != 'e' && c != 'E'){ // pokud nasleduje znak jiny nez ; ktery signalizuje konec zadavani literalu nebo tecka signalizujici des. cislo nebo exponent, je to error

          return LEX_ERROR; break;
        }

        else if (c == ';'){

          ungetc(c, source); // konec celeho cisla, vracime ; zpatky, zpracujem pak

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

        else if (!isdigit(c) && c != ';' && c != 'e' && c != 'E'){ // pokud nasleduje znak jiny nez ; ktery signalizuje konec zadavani literalu a nebo exponent, je to error

          return LEX_ERROR; break;
        }

        else if (c == ';'){

          ungetc(c, source); // konec desetinneho cisla, vracime ; zpatky, zpracujem pak

          return DOUBLE_LITERAL; // a vrati se desetinny literal bez exponentu

        } break;

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

          return DOUBLE_LITERAL; // a vrati se desetinny literal bez exponentu

        } break;

    case 8: // OPERATORY, SLOZENE OPERATORY, ROZPOZNAVANI ZACATKU KOMENTARU

          if (plus_count == 1 && c == '+')  return INC;
     else if (plus_count == 1 && c != '+')  {ungetc(c, source); return PLUS;} // vrat neplatny znak, je to plus
     else if (minus_count == 1 && c == '-') return DEC;
     else if (minus_count == 1 && c != '-') {ungetc(c, source); return MINUS;} // vrat neplatny znak, je to minus
     else if (less_count == 1 && c == '=')  return LESS_EQ;
     else if (less_count == 1 && c != '=')  {ungetc(c, source); return LESS; } // vrat neplatny znak, je to mensi nez
     else if (great_count == 1 && c == '=') return GREAT_EQ;
     else if (great_count == 1 && c != '=') {ungetc(c, source); return GREAT; } // vrat neplatny znak, je to vetsi nez
     else if (excl_count == 1 && c == '=')  return N_EQUAL;
     else if (eq_count == 1 && c != '=')    {ungetc(c, source); return EQUAL; } // vrat neplatny znak, je to rovnitko
     else if (eq_count == 1 && c == '=')    return ASSIGN; // vrat operator == 
     else if (quote_count == 1 && (c != '/' || c != '*')) {ungetc(c, source); return DIV; } // nejedna se o komentar ale o operator deleni
     else if (quote_count == 1 && c == '/') state = 1; // jedna se o jednoradkovy komentar
     else if (quote_count == 1 && c == '*') state = 2; // jedna se o blokovy komentar
     else return LEX_ERROR; break;

  } // konec switche
 } // konec while
} // konec funkce
