#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include "common.h"
#include "menu.h"
#include "store.h"

struct recipeBook recipe;
struct recipeBook updatedRecipe;

int main()
{
    displayMainMenu();
    return 0;
}

void addRecipe()
{
    int validInput = 0; // Checking validity
    char choice; // Checking Yes or No (Y/N)
    
    headline("Add New Recipe");
    clearInputBuffer();

    // generateID();
    // printf("Recipe Id: %s\n",recipe.id);
    printf("\t Enter the name of the Recipe: ");
    fgets(recipe.name, sizeof(recipe.name), stdin);
    recipe.name[strcspn(recipe.name, "\n")] = '\0'; // Removing newline character for formatted csv file

    while (!validInput)
    {
        printf("\t Is it a vegan recipe (Y/N)? ");
        scanf(" %c", &choice);

        if (choice == 'Y' || choice == 'y')
        {
            strcpy(recipe.type, "YES");
            validInput = 1;
        }
        else if (choice == 'N' || choice == 'n')
        {
            strcpy(recipe.type, "NO");
            validInput = 1;
        }
        else
            printf("%s\n", ERRORMESSAGE);
    }

    validInput = 0;
    while (!validInput)
    {
        printf("\t How many main ingredients does this dish require? ");
        scanf("%d", &recipe.ingredientNumber);
        clearInputBuffer();
        if (recipe.ingredientNumber >= 0 && recipe.ingredientNumber <= 9)
            validInput = 1;
        else
            printf("%s\n", ERRORMESSAGE);
    }
    for (int i = 0; i < recipe.ingredientNumber; i++)
    {
        printf("\t Ingredient %d: ", i + 1);
        fgets(recipe.ingredients[i], sizeof(recipe.ingredients[i]), stdin);
    }

    validInput = 0;
    while (!validInput)
    {
        printf("\t How many steps does this dish require? ");
        scanf("%d", &recipe.stepNumber);
        clearInputBuffer();
        if (recipe.stepNumber >= 0 && recipe.stepNumber <= 9)
            validInput = 1;
        else
            printf("%s\n", ERRORMESSAGE);
    }
    for (int i = 0; i < recipe.stepNumber; i++)
    {
        printf("\t Step %d: ", i + 1);
        fgets(recipe.instructions[i], sizeof(recipe.instructions[i]), stdin);
    }

    validInput = 0;
    // temp=0;
    while (!validInput)
    {
        printf("\t Do you have any note or comment (Y/N)? ");
        scanf(" %c", &choice);
        clearInputBuffer();
        if (choice == 'Y' || choice == 'y')
        {
            printf("\t Enter a messsage: ");
            fgets(recipe.notes, sizeof(recipe.notes), stdin);
            validInput = 1;
        }
        else if (choice == 'N' || choice == 'n')
        {
            strcpy(recipe.notes,"Bon Appetit!");
            validInput = 1;
        }
        else
            printf("%s\n", ERRORMESSAGE);
    }

    // Generating recipe ID
    generateID();

    // Writing in File
    saveData(recipe);
    
    printf("\t %s\n", pattern());
    printf("\n\t \033[1;32mRECIPE ADDED SUCCESSFULLY!\n\033[0;m");
    printf ("\t Press any key to continue...");
    _getch();
    displayMainMenu();
}

void viewRecipe()
{
    int recipeCount = 1;

    headline("View All Recipe");

    fp1 = fopen(FILE1_NAME, "r");
    fp2 = fopen(FILE2_NAME, "r");
    fp3 = fopen(FILE3_NAME, "r");

    if (fp1 == NULL || fp2 == NULL || fp3 == NULL)
    {
        printf("\n\t \033[1;31m SOMETHING WENT WRONG! ERROR OPENING FILE...\033[0;m\n");
        exit(1);
    }

    while (fscanf(fp1, "%[^,],%[^,],%[^,],%d,%d,%[^\n]\n", recipe.id, recipe.name, recipe.type, &recipe.ingredientNumber, &recipe.stepNumber, recipe.notes) != EOF)
    {
        system("cls");
        headline("View All Recipe"); // To Clear the above recipies

        printf("\t \033[1;36m%s\033[0;m\n", recipe.name);
        if (strcmp(recipe.type, "YES") == 0)
            printf("\t Type: Vegan\n");
        else
            printf("\t Type: Non Vegan\n");

        // printing ingredients
        displayIngredients(recipeCount);

        // printing instructions
        displayInstructions(recipeCount);

        // printing chef's comment
        printf("\t Chef's Comment: \n");
        printf("\t %s\n\n", recipe.notes);

        recipeCount++;

        printf("\t Press 'Esc' to return main menu or 'Enter' to display next recipe\n\n");
        printf("\t %s\n", pattern());

        // Checking the entered key to display each value
        while (!_kbhit()){};
        char ch = _getch();

        if (ch == 27)
            displayMainMenu();
    }

    printf("\t You've reached the end! Press any key to continue...");
    _getch();
    displayMainMenu();
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
}

void searchRecipe()
{
    int recipeCount=1;
    char temp[MAX_NAME_LENGTH];
    int found=0;

    headline("Search For A Recipe");
    clearInputBuffer();

    printf("\t Enter the name of a dish: ");
    fgets(temp,sizeof(temp),stdin);
    temp[strcspn(temp, "\n")] = '\0'; // removing newline character

    fp1 = fopen(FILE1_NAME, "r");
    fp2 = fopen(FILE2_NAME, "r");
    fp3 = fopen(FILE3_NAME, "r");

    if (fp1 == NULL || fp2 == NULL || fp3 == NULL)
    {
        printf("\n\t \033[1;31m SOMETHING WENT WRONG! ERROR OPENING FILE...\033[0;m\n");
        exit(1);
    }

    while (fscanf(fp1, "%[^,],%[^,],%[^,],%d,%d,%[^\n]\n", recipe.id, recipe.name, recipe.type, &recipe.ingredientNumber, &recipe.stepNumber, recipe.notes) != EOF)
    {
        if (strcmp(strupr(temp), strupr(recipe.name)) == 0)
        {
            printf("\n\033[1;36m\t %s\033[0;m\n", recipe.name);
            if (strcmp(recipe.type, "YES") == 0)
                printf("\t Type: Vegan\n");
            else
                printf("\t Type: Non Vegan\n");

            // for ingredients
            displayIngredients(recipeCount);

            // for instructions
            displayInstructions(recipeCount);

            // for chef's comment
            printf("\t Chef's Comment: \n");
            printf("\t %s\n\n", recipe.notes);
            printf("\t %s\n", pattern());

            found = 1;
            break;
        }
        recipeCount++;
    }
    if (found == 0)
        printf("\n\t \033[1;31mSOMETHING WENT WRONG! RECORD NOT FOUND...\033[0;m\n");

    printf ("\t Press any key to continue...");
    _getch();
    displayMainMenu();

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
}

void updateRecipe()
{
    int recipeCount=1;
    char temp[MAX_NAME_LENGTH];
    char choice;
    int validInput = 0;
    int found=0;

    headline("Update Recipe");
    clearInputBuffer();

    printf("\t Which recipe do you wish to update? ");
    fgets(temp,sizeof(temp),stdin);
    temp[strcspn(temp, "\n")] = '\0'; // removing newline character

    fp1 = fopen(FILE1_NAME, "r+");
    fp2 = fopen(FILE2_NAME, "r+");
    fp3 = fopen(FILE3_NAME, "r+");

    if (fp1 == NULL || fp2 == NULL || fp3 == NULL)
    {
        printf("\n\t \033[1;31m SOMETHING WENT WRONG! ERROR OPENING FILE...\033[0;m\n");
        exit(1);
    }

    // Displaying Old Details
    // printf("\t Old Details:\n");
    while (fscanf(fp1, "%[^,],%[^,],%[^,],%d,%d,%[^\n]\n", recipe.id, recipe.name, recipe.type, &recipe.ingredientNumber, &recipe.stepNumber, recipe.notes) != EOF)
    {
        if (strcmp(strupr(temp), strupr(recipe.name)) == 0)
        {
            printf("\033[1;36m\t %s\033[0;m\n", recipe.name);
            if (strcmp(recipe.type, "YES") == 0)
                printf("\t Type: Vegan\n");
            else
                printf("\t Type: Non Vegan\n");

            // for ingredients
            displayIngredients(recipeCount);

            // for instructions
            displayInstructions(recipeCount);

            // for chef's comment
            printf("\t Chef's Comment: \n");
            printf("\t %s\n\n", recipe.notes);
            printf("\t %s\n", pattern());

            found = 1;
            break;
        }

        recipeCount++;
    }
    if (found == 0)
    {
        printf("\n\t \033[1;31mSOMETHING WENT WRONG! RECORD NOT FOUND...\033[0;m\n");
        printf("\t Press any key to continue...");
        _getch();
        displayMainMenu();
    }

    // Adding New Details
    // printf("\t New Details:\n");
    printf("\033[1;36m\t %s\033[0;m\n", recipe.name);

    while (!validInput)
    {
        printf("\t Is it a vegan recipe (Y/N)? ");
        scanf(" %c", &choice);

        if (choice == 'Y' || choice == 'y')
        {
            strcpy(updatedRecipe.type, "YES");
            validInput = 1;
        }
        else if (choice == 'N' || choice == 'n')
        {
            strcpy(updatedRecipe.type, "NO");
            validInput = 1;
        }
        else
            printf("\t %s\n", ERRORMESSAGE);
    }

    validInput = 0;
    while (!validInput)
    {
        printf("\t How many main ingredients does this dish require? ");
        scanf("%d", &updatedRecipe.ingredientNumber);
        clearInputBuffer();
        if (updatedRecipe.ingredientNumber >= 0 && updatedRecipe.ingredientNumber <= 9)
            validInput = 1;
        else
            printf("\t %s\n", ERRORMESSAGE);
    }
    for (int i = 0; i < updatedRecipe.ingredientNumber; i++)
    {
        printf("\t Ingredient %d: ", i + 1);
        fgets(updatedRecipe.ingredients[i], sizeof(updatedRecipe.ingredients[i]), stdin);
    }

    validInput = 0;
    while (!validInput)
    {
        printf("\t How many steps does this dish require? ");
        scanf("%d", &updatedRecipe.stepNumber);
        clearInputBuffer();
        if (updatedRecipe.stepNumber >= 0 && updatedRecipe.stepNumber <= 9)
            validInput = 1;
        else
            printf("\t %s\n", ERRORMESSAGE);
    }
    for (int i = 0; i < updatedRecipe.stepNumber; i++)
    {
        printf("\t Step %d: ", i + 1);
        fgets(updatedRecipe.instructions[i], sizeof(updatedRecipe.instructions[i]), stdin);
    }

    validInput = 0;
    // temp=0;
    while (!validInput)
    {
        printf("\t Do you have any note or comment (Y/N)? ");
        scanf(" %c", &choice);
        clearInputBuffer();
        if (choice == 'Y' || choice == 'y')
        {
            printf("\t Enter a messsage: ");
            fgets(updatedRecipe.notes, sizeof(updatedRecipe.notes), stdin);
            validInput = 1;
        }
        else if (choice == 'N' || choice == 'n')
        {
            strcpy(updatedRecipe.notes,"Bon Appetit!");
            validInput = 1;
        }
        else
            printf("\t %s\n", ERRORMESSAGE);
    }

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);

    // Updating general detail inside the file
    updateDetail(recipe, updatedRecipe, recipeCount);

    // Updating ingredients inside the file
    updateIngredients(recipe, updatedRecipe, recipeCount);

    // Updating instructions inside the file
    updateInstructions(recipe, updatedRecipe, recipeCount);

    printf("\t %s\n", pattern());
    printf("\n\t \033[1;32mRECIPE UPDATED SUCCESSFULLY...\033[0;m\n");
    printf ("\t Press any key to continue...");
    _getch();
    displayMainMenu();
}

void deleteRecipe()
{
    int recipeCount=1;
    char temp[MAX_NAME_LENGTH];
    char choice;
    int validInput = 0;
    int found=0;

    headline("Delete Recipe");
    clearInputBuffer();

    printf("\t Which recipe do you wish to delete? ");
    fgets(temp,sizeof(temp),stdin);
    temp[strcspn(temp, "\n")] = '\0'; // removing newline character

    fp1 = fopen(FILE1_NAME, "r+");
    fp2 = fopen(FILE2_NAME, "r+");
    fp3 = fopen(FILE3_NAME, "r+");

    if (fp1 == NULL || fp2 == NULL || fp3 == NULL)
    {
        printf("\n\t \033[1;31m SOMETHING WENT WRONG! ERROR OPENING FILE...\033[0;m\n");
        exit(1);
    }

    // Displaying Recipe Details For Verification
    printf("\t Recipe Details:\n");
    while (fscanf(fp1, "%[^,],%[^,],%[^,],%d,%d,%[^\n]\n", recipe.id, recipe.name, recipe.type, &recipe.ingredientNumber, &recipe.stepNumber, recipe.notes) != EOF)
    {
        if (strcmp(strupr(temp), strupr(recipe.name)) == 0)
        {
            printf("\033[1;36m\t %s\033[0;m\n", recipe.name);
            if (strcmp(recipe.type, "YES") == 0)
                printf("\t Type: Vegan\n");
            else
                printf("\t Type: Non Vegan\n");

            // for ingredients
            displayIngredients(recipeCount);

            // for instructions
            displayInstructions(recipeCount);

            // for chef's comment
            printf("\t Chef's Comment: \n");
            printf("\t %s\n\n", recipe.notes);
            printf("\t %s\n", pattern());
            
            found = 1;
            break;
        }

        recipeCount++;
    }
    if (found == 0)
    {
        printf("\n\t \033[1;31mSOMETHING WENT WRONG! RECORD NOT FOUND...\033[0;m\n");
        printf("\t Press any key to continue...");
        _getch();
        displayMainMenu();
    }

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);

    while (!validInput)
    {
        printf("\t Are You Sure You Want To Delete This Recipe (Y/N)? ");
        scanf(" %c", &choice);

        if (choice == 'Y' || choice == 'y')
        {
            // Removing general detail inside the file
            removeDetail(recipeCount);

            // Removing ingredients inside the file
            removeIngredients(recipeCount);

            // Removing instructions inside the file
            removeInstructions(recipeCount);

            printf("\t %s\n", pattern());
            printf("\n\t \033[1;32mRECIPE DELETED SUCCESSFULLY!\n\033[0;m");
            printf("\t Press any key to continue...");
            _getch();
            displayMainMenu();
            validInput = 1;
            break;
        }
        else if (choice == 'N' || choice == 'n')
        {
            // Process terminated display main menu using sleep
            printf("\n\t \033[1;31mTHE PROCESS IS TERMINATED!\n\033[0;m");
            printf("\t Press any key to continue...");
            _getch();
            displayMainMenu();
            validInput = 1;
            break;
        }
        else
            printf("\t %s\n", ERRORMESSAGE);
    }
}

void veganSurprise()
{
    system("cls");
    headline("Surprise Vegan Cooking");
    int found=0;

    srand(time(NULL)); //Seeding the random number generator
    while (!found) // Run the loop until the random recipe of type vegan is found
    {
        int index = randomIndex();
        // printf("%d",index);

        // the randomIndex() calls numberOfRecipe() function which opens fp1 and also closes it so using randomIndex() here i.e before opening the files in this particular function prevents file conflict which occured initially when I used the randomIndex() after opening files in this function. There were no syntax errors however, my while() loop was not being executed. After 6 hours of debugging I found out the reason was that, the numberOfRecipe() function closed the file which was open so the while loop could not check the EOF of fp1

        int recipeCount = 1;

        fp1 = fopen(FILE1_NAME, "r");
        fp2 = fopen(FILE2_NAME, "r");
        fp3 = fopen(FILE3_NAME, "r");

        if (fp1 == NULL || fp2 == NULL || fp3 == NULL)
        {
            printf("\n\t \033[1;31m SOMETHING WENT WRONG! ERROR OPENING FILE...\033[0;m\n");
            exit(1);
        }

        while (fscanf(fp1, "%[^,],%[^,],%[^,],%d,%d,%[^\n]\n", recipe.id, recipe.name, recipe.type, &recipe.ingredientNumber, &recipe.stepNumber, recipe.notes) != EOF)
        {
            if (recipeCount == index)
            {
                if (strcmp(recipe.type, "YES") == 0)
                {
                    printf("\033[1;36m\t %s\033[0;m\n", recipe.name);
                    printf("\t Type: Vegan\n");

                    // for ingredients
                    displayIngredients(recipeCount);

                    // for instructions
                    displayInstructions(recipeCount);

                    // for chef's comment
                    printf("\t Chef's Comment: \n");
                    printf("\t %s\n\n", recipe.notes);

                    found = 1;
                    break;
                }
            }

            recipeCount++;
        }
        fclose(fp1);
        fclose(fp2);
        fclose(fp3);
    }

        printf("\t Press 'Esc' to return main menu or 'Enter' to display new recipe\n\n");
        printf("\t %s\n", pattern());

        // Checking the entered key
        while (!_kbhit()){};
        char ch = _getch();
        if (ch == 27)
        displayMainMenu();
        else
        veganSurprise();
}

void nonVeganSurprise()
{
    system("cls");
    headline("Surprise Non-Vegan Cooking");
    int found=0;

    srand(time(NULL)); //Seeding the random number generator
    while (!found) // Run the loop until the random recipe of type vegan is found
    {
        int index = randomIndex();
        // printf("%d",index);

        // i did not repeate the same mistake again

        int recipeCount = 1;

        fp1 = fopen(FILE1_NAME, "r");
        fp2 = fopen(FILE2_NAME, "r");
        fp3 = fopen(FILE3_NAME, "r");

        if (fp1 == NULL || fp2 == NULL || fp3 == NULL)
        {
            printf("\n\t \033[1;31m SOMETHING WENT WRONG! ERROR OPENING FILE...\033[0;m\n");
            exit(1);
        }

        while (fscanf(fp1, "%[^,],%[^,],%[^,],%d,%d,%[^\n]\n", recipe.id, recipe.name, recipe.type, &recipe.ingredientNumber, &recipe.stepNumber, recipe.notes) != EOF)
        {
            if (recipeCount == index)
            {
                if (strcmp(recipe.type, "NO") == 0)
                {
                    printf("\033[1;36m\t %s\033[0;m\n", recipe.name);
                    printf("\t Type: Non-Vegan\n");

                    // for ingredients
                    displayIngredients(recipeCount);

                    // for instructions
                    displayInstructions(recipeCount);

                    // for chef's comment
                    printf("\t Chef's Comment: \n");
                    printf("\t %s\n\n", recipe.notes);

                    found = 1;
                    break;
                }
            }

            recipeCount++;
        }
        fclose(fp1);
        fclose(fp2);
        fclose(fp3);
    }

        printf("\t Press 'Esc' to return main menu or 'Enter' to display new recipe\n\n");
        printf("\t %s\n", pattern());

        // Checking the entered key
        while (!_kbhit()){};
        char ch = _getch();
        if (ch == 27)
        displayMainMenu();
        else
        nonVeganSurprise();
}

void exitProgram()
{
    printf("\n\t \033[1;31mEXITING THE RECIPE BOOK. GOODBYE!\n\n\033[0;m");
    exit(0);
}