#include <stdio.h>

#include "parser.h"
#include "string.h"

// PARSER PRO POVINNOU TRIDU MAIN A POVINNOU FUNKCI RUN (WIP)

int parse(tListOfInstr * ilist, symtab_t * symtab , FILE* source) {
    fprintf(stderr, "DEBUG: Function parse\n");

    int token;
    string attr;
    strInit(&attr);
    
    do {
        token = get_next_token(&attr);
    
        fprintf(stderr, "DEBUG: got token %d, %s\n", token, attr.str);
    } while (token != END_OF_FILE && token != LEX_ERROR);
    
    return 42;
}
/*
int class_main(){ // parser pro povinnou tridu main

     token = get_next_token(&attr);

     switch (token){

     case CLASS:

         if ((token = get_next_token(&attr)) == LEX_ERROR) return LEX_ERROR;
         if (token != MAIN) return SYNTAX_ERROR; else                      // vstupnim bodem programu je povinna trida Main
         {
             if ((token = get_next_token(&attr)) == LEX_ERROR) return LEX_ERROR;
             if (token != LEFT_VINCULUM) return SYNTAX_ERROR; else        // jsme uvnitr mainu "class Main{"
             {
                 if ((token = get_next_token(&attr)) == LEX_ERROR) return LEX_ERROR;
                 if (token != STATIC) return SYNTAX_ERROR; else       // musi nasledovat funkce run - > static void run
                 {
                     if ((token = get_next_token(&attr)) == LEX_ERROR) return LEX_ERROR;
                     if (token != VOID) return SYNTAX_ERROR; else
                     {
                         if ((token = get_next_token(&attr)) == LEX_ERROR) return LEX_ERROR;
                         if (token != RUN) return SYNTAX_ERROR; else
                         {
                            if ((token = get_next_token(&attr)) == LEX_ERROR) return LEX_ERROR;
                            if (token != LEFT_BRACKET) return SYNTAX_ERROR; else
                            {
                                if ((token = get_next_token(&attr)) == LEX_ERROR) return LEX_ERROR;
                                if (token != RIGHT_BRACKET) return SYNTAX_ERROR; else
                                {
                                    if ((token = get_next_token(&attr)) == LEX_ERROR) return LEX_ERROR;
                                    if (token != LEFT_VINCULUM) return SYNTAX_ERROR; else       // jsme uvnitr fce run -> " class Main{ static void run(){ " muze zacit zbytek programu
                                    {
                                        main_body();
                                        main_end();
                                    }

                                }
                            }
                         }
                     }
                 }
             }
         }
     }
     return SYNTAX_ERROR; // pokud program nezacina klicovym slovem class je to syntakticky error
}


// PARSER PRO TELO TRIDY (WIP)

int class_body(){

token = get_next_token(&attr);

switch (token){

case CLASS: return SYNTAX_ERROR; // pokus o deklarovani tridy uvnitr tridy

case INT: // nasleduje deklarovani promenne typu int

    if ((token = get_next_token(&attr)) == LEX_ERROR) return LEX_ERROR;
    if (token != ID) return SYNTAX_ERROR; else
    {
       tableInsert(blablabla); // vloz identifikator do tabulky symbolu a podivej se, jestli uz takovy v ramci jedne tridy/fce neexistuje, DOPLNIT!!!!!

       if ((token = get_next_token(&attr)) == LEX_ERROR) return LEX_ERROR;
       if (token != SEMICOLON && token != EQUAL) return SYNTAX_ERROR; else

       {
           if (token == SEMICOLON) return class_body(); else // deklarace neinicializovaneho intu intu uvnitr tridy/fce probehla v poradku, rekurzivne volame fci
           {
           
           // pokud to nebyl semicolon, jedna se o inicializaci/prirazeni intu 
           
            int_declare_parser(); // volani parseru, ktery overuje syntax inicializace/prirazeni promenne int
        }
       }
    }
break; // konec case INT

case STRING: // podobne jako u int

    if ((token = get_next_token(&attr)) == LEX_ERROR) return LEX_ERROR;
    if (token != ID) return SYNTAX_ERROR; else
    {
       tableInsert(blablabla); // vloz identifikator do tabulky symbolu a podivej se, jestli uz takovy v ramci jedne tridy/fce neexistuje, DOPLNIT!!!!!

       if ((token = get_next_token(&attr)) == LEX_ERROR) return LEX_ERROR;
       if (token != SEMICOLON && token != EQUAL) return SYNTAX_ERROR; else

       {
           if (token == SEMICOLON) return class_body(); else // deklarace stringu uvnitr tridy/fce probehla v poradku, rekurzivne volame fci
           {

           string_declare_parser(); // jedna se o incializaci retezce pri deklaraci, volej parsovaci funkci
           }
       }
    }
break; // konec case STRING

case DOUBLE:

    if ((token = get_next_token(&attr)) == LEX_ERROR) return LEX_ERROR;
    if (token != ID) return SYNTAX_ERROR; else
    {
       tableInsert(blablabla); // vloz identifikator do tabulky symbolu a podivej se, jestli uz takovy v ramci jedne tridy/fce neexistuje, DOPLNIT!!!!!

       if ((token = get_next_token(&attr)) == LEX_ERROR) return LEX_ERROR;
       if (token != SEMICOLON && token != EQUAL) return SYNTAX_ERROR; else

       {
           if (token == SEMICOLON) return class_body(); else // deklarace stringu uvnitr tridy/fce probehla v poradku, rekurzivne volame fci
           {

           double_declare_parser(); // jedna se o incializaci retezce pri deklaraci, volej parsovaci funkci
           }

       }
    }
break; // konec case DOUBLE
           

// PARSER PRO INICIALIZACI CELOCISELNE PROMENNE (WIP)

int int_declare_parser(){ // parser ktery kontroluje spravny syntax inicializovania prirazovani hodnot celociselne promenne

    int sign_count = 0; // countery
    int id_count = 0;
    int literal_count = 0;
    int l_bracket_count = 0;
    int r_bracket_count = 0;

    switch (token){

    case SEMICOLON:

     if ( (id_count == 0 && literal_count == 0) || sign_count == 1) return SYNTAX_ERROR; // pokus o nasledujici deklaraci intu:  " int cislo = ; " nebo " int cislo = 1 + ;" je syntakticka chyba

     else if ( (id_count == 1 || literal_count == 1 ) && sign_count == 0) return SYNTAX_OK; break; // inicializace intu probehla v poradku

    case INT_LITERAL:

     literal_count = 1; return int_declare_parser(); break; // nasledovalo cele cislo, rekurzivne volej fci dale a parsuj

    case ID:

     id_count = 1;
     sign_count = 0;

     // NUTNO PODIVAT SE DO TABULKY SYMBOLU A NAJIT TAM DANY IDENTIFIKATOR POKUD SE JEDNA O ID INTU, MUZE SE JEDNAT I O ID FUNKCE KTERA SE NACHAZI ZA PROGRAMEM! DOPLNIT!!!

     tData polozka = tableSearch(blablabla); // podivej se do tabulky na ID, DOPLNIT!!!

     if (polozka.varType == INT_LITERAL) return int_declare_parser(); break; // pokud odpovida typ ID, rekurzivne parsuj dale, DOPLNIT!!!!!

     else return SYNTAX_ERROR; break; // pokud to neni celociselny literal, jedna se o syntakticky error

    case PLUS:
    case MINUS:
    case MUL:
    case DEC:

     sign_count = 1; // signalizuj znamenko

     if (id_count == 0 && literal_count == 0) return SYNTAX_ERROR; break; // pokud se nenachazi pred znamenkem id nebo celociselny literal, je to error

     id_count = 0;
     literal_count == 0;

     else return int_declare_parser(); break; // jinak parsuj dale

    case LEFT_BRACKET:

     l_bracket_count = 1;

     if (id_count == 1 && literal_count == 0 && sign_count == 0) blablabla // jedna se o funkci, zavolej parser parametru a konce funkci, DOPLNIT!!!

     else if (id_count == 0 && literal_count == 1 && sign_count == 0) return SYNTAX_ERROR; break; // jedna se o syntaktickou chybu typu " int cislo = 5( "

     else if (id_count == 0 && literal_count == 0 && sign_count == 1) // mozna platna deklarace uvidime, DOPLNIT!!!

    case RIGHT_BRACKET:

     r_bracket_count = 1;

     if (l_bracket_count == 1) l_bracket_count == 0; return int_declare_parser(); break;

     else return SYNTAX_ERROR; break;

     // DORESIT CO S ARGUMENTAMA VE FCI
    }
}
          
// PARSER PRO INICIALIZACI RETEZCOVE PROMENNE (WIP) skoro stejny jako int          
          
 int string_declare_parser(){

    int sign_count = 0; // countery
    int id_count = 0;
    int literal_count = 0;
    int l_bracket_count = 0;
    int r_bracket_count = 0;

    switch (token){

    case SEMICOLON:

     if ( (id_count == 0 && literal_count == 0) || sign_count == 1) return SYNTAX_ERROR;

     else if ( (id_count == 1 || literal_count == 1 ) && sign_count == 0) return SYNTAX_OK; break; // inicializace intu probehla v poradku

    case STRING_LITERAL:

     literal_count = 1; return string_declare_parser(); break;

    case ID:

     id_count = 1;
     sign_count = 0;

     // NUTNO PODIVAT SE DO TABULKY SYMBOLU A NAJIT TAM DANY IDENTIFIKATOR POKUD SE JEDNA O ID STRINGU, MUZE SE JEDNAT I O ID FUNKCE KTERA SE NACHAZI ZA PROGRAMEM! DOPLNIT!!!

     tData polozka = tableSearch(blablabla); // podivej se do tabulky na ID, DOPLNIT!!!

     if (polozka.varType == STRING_LITERAL) return string_declare_parser(); break; // pokud odpovida typ ID, rekurzivne parsuj dale, DOPLNIT!!!!!

     else if (polozka.varType != STRING_LITERAL && sign_count == 1 ) KONVERTUJ NA RETEZEC, DOPLNIT!!!!! return string_declare_parser(); break;

     else return SYNTAX_ERROR; break; // pokud to neni retezcovy literal, jedna se o syntakticky error

    case PLUS: // funguje jako konkatenace retezcu

     sign_count = 1; // signalizuj znamenko

     if (id_count == 0 && literal_count == 0) return SYNTAX_ERROR; break; // pokud se nenachazi pred znamenkem id nebo retezcovy literal, je to error

     else return string_declare_parser(); break; // jinak parsuj dale

    default: return SYNTAX_ERROR; break;
 }
}

// PARSER PRO INICIALIZACI DESETINNE PROMENNE (WIP) skoro stejny jako int           
          
int double_declare_parser(){ // parser ktery kontroluje spravny syntax inicializovania prirazovani hodnot desetinne promenne

    int sign_count = 0; // countery
    int id_count = 0;
    int literal_count = 0;
    int l_bracket_count = 0;
    int r_bracket_count = 0;

    switch (token){

    case SEMICOLON:

     if ( (id_count == 0 && literal_count == 0) || sign_count == 1) return SYNTAX_ERROR;

     else if ( (id_count == 1 || literal_count == 1 ) && sign_count == 0) return SYNTAX_OK; break;

    case DOUBLE_LITERAL:

     literal_count = 1; return double_declare_parser(); break; // nasledovalo cele cislo, rekurzivne volej fci dale a parsuj

    case ID:

     id_count = 1;
     literal_count = 0;
     l_bracket_count = 0;
     r_bracket_count = 0;
     sign_count = 0;

     // NUTNO PODIVAT SE DO TABULKY SYMBOLU A NAJIT TAM DANY IDENTIFIKATOR POKUD SE JEDNA O ID INTU, MUZE SE JEDNAT I O ID FUNKCE KTERA SE NACHAZI ZA PROGRAMEM! DOPLNIT!!!

     tData polozka = tableSearch(blablabla); // podivej se do tabulky na ID, DOPLNIT!!!

     if (polozka.varType == DOUBLE_LITERAL) return double_declare_parser(); break; // pokud odpovida typ ID, rekurzivne parsuj dale, DOPLNIT!!!!!

     else return SYNTAX_ERROR; break; // pokud to neni celociselny literal, jedna se o syntakticky error

    case PLUS:
    case MINUS:
    case MUL:
    case DEC:

     sign_count = 1; // signalizuj znamenko

     if (id_count == 0 && literal_count == 0) return SYNTAX_ERROR; break; // pokud se nenachazi pred znamenkem id nebo celociselny literal, je to error

     id_count = 0;
     literal_count == 0;

     else return double_declare_parser(); break; // jinak parsuj dale

    case LEFT_BRACKET:

     l_bracket_count = 1;

     if (id_count == 1 && literal_count == 0 && sign_count == 0) blablabla // jedna se o funkci, zavolej parser parametru a konce funkci, DOPLNIT!!!

     else if (id_count == 0 && literal_count == 1 && sign_count == 0) return SYNTAX_ERROR; break; // jedna se o syntaktickou chybu typu " int cislo = 5( "

     else if (id_count == 0 && literal_count == 0 && sign_count == 1) // mozna platna deklarace uvidime, DOPLNIT!!!

    case RIGHT_BRACKET:

     r_bracket_count = 1;

     if (l_bracket_count == 1) l_bracket_count == 0; return double_declare_parser(); break;

     else return SYNTAX_ERROR; break;

    default: return SYNTAX_ERROR; break;

    }         
*/  
