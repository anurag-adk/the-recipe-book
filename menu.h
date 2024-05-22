#ifndef MENU_H
#define MENU_H
#include <stdio.h>
#include <stdlib.h>

void addRecipe();
void viewRecipe();
void searchRecipe();
void updateRecipe();
void deleteRecipe();
void exitProgram();

void displayMainMenu()
{
    int choice;  // user choice

    // headline("The Recipe Book");
    system("cls");

    printf("\t\033[1;36m------------------------------\n");
    printf("\t| Welcome to The Recipe Book |\n");
    printf("\t------------------------------\033[0;m\n\n");

    printf("\t 1. Add New Recipe\n");
    printf("\t 2. View All Recipe\n");
    printf("\t 3. Search For A Recipe\n");
    printf("\t 4. Update Existing Recipe\n");
    printf("\t 5. Delete Existing Recipe\n");
    printf("\t 6. Surprise Cooking\n");
    printf("\t 7. Exit\n\n");
    printf("\t-Enter your choice (1-7)...");

    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
        addRecipe();
        break;
    case 2:
        viewRecipe();
        break;
    case 3:
        searchRecipe();
        break;
    case 4:
        updateRecipe();
        break;
    case 5:
        deleteRecipe();
        break;
    case 6:
        surpriseCooking();
        break;
    case 7:
        exitProgram();
        break;
    default:
        printf("\t %s\n",ERRORMESSAGE);
        printf("\t Press any key to continue...");
        _getch();
        fflush(stdin);
        displayMainMenu();
    }
}

void surpriseCooking()
{
    int choice;

    headline("Surprise Cooking");

    printf("\t Challenge yourself with:\n");
    printf("\t 1. A Vegan Surprise Cooking\n");
    printf("\t 2. A Non Vegan Surprise Cooking\n\n");

    printf("\t-Select Vegan or Non Vegan Option (1-2)...");
    fflush(stdout);

    scanf("%d", &choice);

    if (choice == 1)
    {
        veganSurprise();
    }
    else if (choice == 2)
    {
        nonVeganSurprise();
    }
    else
    {
        printf("\t %s\n",ERRORMESSAGE);
        printf("\t Press any key to continue...");
        _getch();
        fflush(stdin);
        surpriseCooking();
    }
}
#endif