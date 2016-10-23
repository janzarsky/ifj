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

int main(int argc, char** argv) {
    if (argc != 2) {
        return 99;
    }

    FILE *source_file;

    if ((source_file = fopen(argv[1], "r")) == NULL) {
        return 99;
    }

    // set source file

    // initialize table of symbols

    tListOfInstr list_of_instr;
    listInit(&list_of_instr);

    // parse file
    // check error code

    // interpret program

    // free table of symbols
    // free instruction list
    fclose(source_file);

    return 0;
}
