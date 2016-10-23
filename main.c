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

#include "instrlist.h"
#include "parser.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        return 99;
    }

    FILE *source;

    if ((source = fopen(argv[1], "r")) == NULL) {
        return 99;
    }

    // initialize table of symbols

    tListOfInstr ilist;
    listInit(&ilist);

    // table of symbols == NULL (no table implemented yet)
    int parse_result = parse(&ilist, NULL, source);

    // check error code

    // interpret code

    // free table of symbols
    // free instruction list
    fclose(source);

    return 0;
}
