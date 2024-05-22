#ifndef COMMON_H
#define COMMON_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define ID_LENGTH 6
#define MAX_NAME_LENGTH 20
#define MAX_TYPE_LENGTH 3   // yes or no
#define MAX_INGREDIENTS 15   // max no. of ingredients
#define MAX_INSTRUCTION 20   // max no. of steps
#define MAX_WORD_LENGTH 100   // ingredient or instruction
#define MAX_NOTES_LENGTH 100 // additional tips and tricks

// int number_of_recipe = 0;
// static int id=0;

#define ERRORMESSAGE "\n\t \033[1;31m ERROR 400: INVALID ENTRY PLEASE TRY AGAIN...\033[0;m\n"

// Recipe Book Structure
struct recipeBook
{
    char id[ID_LENGTH];
    char name[MAX_NAME_LENGTH];
    char type[MAX_TYPE_LENGTH];
    int ingredientNumber;
    int stepNumber;
    char ingredients[MAX_INGREDIENTS][MAX_WORD_LENGTH];
    char instructions[MAX_INSTRUCTION][MAX_WORD_LENGTH];
    char notes[MAX_NOTES_LENGTH];
};

extern struct recipeBook recipe;
extern struct recipeBook updatedRecipe;
// To display a divider
char *pattern()
{
    static char ch[51];

    for (int i = 0; i < 50; i++)
        ch[i] = '-';

    ch[50] = '\0';

    return ch;
}

// To print header
void headline(char *text)
{
    system("cls");
    // system("clear");

    printf("\t \033[1;33m%s\033[0;m\n",text);
    printf("\t %s\n",pattern());
}

// To generate ID
void generateID()
{ 
    srand(time(NULL));
    for (int i = 0; i < ID_LENGTH-1; i++)
    {
        recipe.id[i] = '0' + rand() % 10;
    }
    recipe.id[ID_LENGTH-1] = '\0'; // Null-terminate the string
}

// To generate random Index
int randomIndex()
{
    int upper=numberOfRecipe();
    int lower=1;
    int index = (rand() % (upper - lower + 1)) + lower;
    return index;
}

// To clear input buffer
void clearInputBuffer() 
{
    int flag; // Checking buffer
    while ((flag = getchar()) != '\n' && flag != EOF){}; // To clear input buffer
}

#endif