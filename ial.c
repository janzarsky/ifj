/*#include "ial.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

int readInt()
{
    string *instr;
    int i = 0;
    char c;
    instr = (string*)malloc(sizeof(string));
    if (instr == NULL)
        return -1;
    instr->str = (char*)malloc(sizeof(char)*STR_LEN_INC);
    if (instr->str == NULL)
    {
        free(instr);
        return -1;
    }
    instr->allocSize = 10;
    instr->length = 0;
    instr->str = NULL;
    c = getchar();
    while (c > '0' || c < '9')
    {
        if (instr->length + 1 > instr->allocSize)
        {
        // pamet nestaci, je potreba provest realokaci
            if ((instr->str = (char*) realloc(instr->str, instr->length + STR_LEN_INC)) == NULL)
                return -1;
            instr->allocSize = instr->length + STR_LEN_INC;
        }
        instr->str[i] = c;
        instr->length++;
        c = getchar();
        i++;
    }
    if (c != '\n' || c != EOF)
    {
        free(instr->str);
        free(instr);
        return -1;
    }
    i = atoi(instr->str);
    free(instr->str);
    free(instr);
    return i;
}

double readDouble ()
{
    string *instr;
    int i = 0;
    char c;
    bool dot = false;
    instr = (string*)malloc(sizeof(string));
    if (instr == NULL)
        return -1;
    instr->str = (char*)malloc(sizeof(char)*STR_LEN_INC);
    if (instr->str == NULL)
    {
        free(instr);
        return -1;
    }
    instr->allocSize = 10;
    instr->length = 0;
    instr->str = NULL;
    c = getchar();
    while ((c > '0' && c < '9') || c == '.')
    {
        if (instr->length + 1 > instr->allocSize)
        {
        // pamet nestaci, je potreba provest realokaci
            if ((instr->str = (char*) realloc(instr->str, instr->length + STR_LEN_INC)) == NULL)
                return -1;
            instr->allocSize = instr->length + STR_LEN_INC;
        }
        if (c == '.')
        {
            if (dot == false)
                dot = true;
            else
            {
                free(instr->str);
                free(instr);
                return -1;
            }
        }
        instr->str[i] = c;
        instr->length++;
        c = getchar();
        i++;
    }
    if (c != '\n' || c != EOF)
    {
        free(instr->str);
        free(instr);
        return -1;
    }
    i = atof(instr->str);
    free(instr->str);
    free(instr);
    return i;
}

char* readString()
{
    string *instr;
    int i = 0;
    char c;
    char *b;
    instr = (string*)malloc(sizeof(string));
    if (instr == NULL)
        return -1;
    instr->str = (char*)malloc(sizeof(char)*STR_LEN_INC);
    if (instr->str == NULL)
    {
        free(instr);
        return -1;
    }
    instr->allocSize = 10;
    instr->length = 0;
    instr->str = NULL;
    c = getchar();
    while (c != '\n' || c != EOF)
    {
        if (instr->length + 1 > instr->allocSize)
        {
        // pamet nestaci, je potreba provest realokaci
            if ((instr->str = (char*) realloc(instr->str, instr->length + STR_LEN_INC)) == NULL)
                return -1;
            instr->allocSize = instr->length + STR_LEN_INC;
        }
        instr->str[i] = c;
        instr->length++;
        c = getchar();
        i++;
    }
    free(instr->str);
    free(instr);
    //return string?
    return ;
}
*/
