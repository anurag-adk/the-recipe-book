#ifndef STORE_H
#define STORE_H
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "common.h"

#define FILE1_NAME "Recipe-Details.csv"
#define FILE2_NAME "Recipe-Ingredients.csv"
#define FILE3_NAME "Recipe-Steps.csv"
#define TEMP_FILE "Temp--Record.csv"

FILE *fp1, *fp2, *fp3, *tempfp; // Globally declaring the pointers to avoid naming conflict

// To write data in file
void saveData(struct recipeBook recipe)
{
    fp1 = fopen(FILE1_NAME, "a");
    fp2 = fopen(FILE2_NAME, "a");
    fp3 = fopen(FILE3_NAME, "a");

    if (fp1 == NULL || fp2 == NULL || fp3 == NULL)
    {
        printf("\n\t \033[1;31m SOMETHING WENT WRONG! ERROR OPENING FILE...\033[0;m\n");
        exit(1);
    }

    // recipe details
    fprintf(fp1, "%s,%s,%s,%d,%d,%s", recipe.id, recipe.name, recipe.type, recipe.ingredientNumber, recipe.stepNumber, recipe.notes);
    // fprintf(fp1,"\n");

    // ingredient details
    fprintf(fp2,"%s,", recipe.id);

    for (int i = 0; i < recipe.ingredientNumber; i++)
    {
        char *ingredient = recipe.ingredients[i];
        for (int j = 0; ingredient[j] != '\0'; j++) 
        {
            if(i<recipe.ingredientNumber-1) // Don't replace the last ingredient newline
                if (ingredient[j] == '\n') 
                    ingredient[j] = ','; // Replace newline with comma
        }
        fprintf(fp2, "%s", ingredient);
    }
    if(recipe.ingredientNumber==0)
        fprintf(fp2, "\n");

    // instruction details
    fprintf(fp3,"%s,", recipe.id);

    for (int i = 0; i < recipe.stepNumber; i++)
    {
        char *instruction = recipe.instructions[i];
        for (int j = 0; instruction[j] != '\0'; j++) 
        {
            if(i<recipe.stepNumber-1) // Don't replace the last ingredient newline
            {
                if (instruction[j] == '\n') 
                    instruction[j] = ','; // Replace newline with comma
            }
        }
        fprintf(fp3, "%s", instruction);
    }
    if(recipe.stepNumber==0)
        fprintf(fp2, "\n");

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
}

// To display ingredients from the file-2
void displayIngredients(int recipeCount)
{
    char line[2048];
    char token[] = ",";

    printf("\n\t Ingredients:\n");
    int currentLine = 1;
    rewind(fp2); // resetting the file pointer to beginning;
    while (1)
    {
        fgets(line, sizeof(line), fp2);
        if (feof(fp2))
            break;
        else if (currentLine == recipeCount)
        {
            // printf("%s",line);
            char *portion = strtok(line, token); // extracting data from each line
            int flag = 0;
            for (int i = 0; portion != NULL; i++)
            {
                if (flag)
                {
                    printf("\t Ingredient %d: %s\n", i, portion);
                }
                portion = strtok(NULL, token);
                flag = 1;
            }
            break;
        }
        currentLine++;
    }
}

// To display instructions from the file-3
void displayInstructions(int recipeCount)
{
    char line[2048];
    char token[] = ",";

    printf("\t Instructions:\n");
    int currentLine = 1;
    rewind(fp3); // resetting the file pointer to beginning;
    while (1)
    {
        fgets(line, sizeof(line), fp3);
        if (feof(fp3))
            break;
        else if (currentLine == recipeCount)
        {
            // printf("%s",line);
            char *portion = strtok(line, token); // extracting data from each line
            int flag = 0;
            for (int i = 0; portion != NULL; i++)
            {
                if (flag)
                {
                    printf("\t Step %d: %s\n", i, portion);
                }
                portion = strtok(NULL, token);
                flag = 1;
            }
            break;
        }
        currentLine++;
    }
}

// To update the details part of the recipe
void updateDetail(struct recipeBook recipe, struct recipeBook updatedRecipe, int recipeCount)
{
    fp1 = fopen(FILE1_NAME, "r+");
    tempfp = fopen(TEMP_FILE, "w+");

    if (fp1 == NULL || tempfp == NULL)
    {
        printf("\n\t \033[1;31m SOMETHING WENT WRONG! ERROR OPENING FILE...\033[0;m\n");
        exit(1);
    }

    int currentLine = 1;
    rewind(fp1); // resetting the file pointer to beginning;
    while (fscanf(fp1, "%[^,],%[^,],%[^,],%d,%d,%[^\n]\n", recipe.id, recipe.name, recipe.type, &recipe.ingredientNumber, &recipe.stepNumber, recipe.notes)!=EOF)
    {
        if (currentLine == recipeCount)
        {
            // replacing data with new one and putting in temporary file
            fprintf(tempfp, "%s,%s,%s,%d,%d,%s", recipe.id, recipe.name, updatedRecipe.type, updatedRecipe.ingredientNumber, updatedRecipe.stepNumber, updatedRecipe.notes);
        }
        else
        {
            // placing old data with no changes in temporary file
            fprintf(tempfp, "%s,%s,%s,%d,%d,%s\n", recipe.id, recipe.name, recipe.type, recipe.ingredientNumber, recipe.stepNumber, recipe.notes); 
        }
        currentLine++;
    }
    fclose(fp1);
    fclose(tempfp);
    remove (FILE1_NAME);
    rename (TEMP_FILE, FILE1_NAME);
}

// To update the ingredients of the recipe
void updateIngredients(struct recipeBook recipe, struct recipeBook updatedRecipe, int recipeCount)
{
    fp2 = fopen(FILE2_NAME, "r+");
    tempfp = fopen(TEMP_FILE, "w+");

    if (fp2 == NULL || tempfp == NULL)
    {
        printf("\n\t \033[1;31m SOMETHING WENT WRONG! ERROR OPENING FILE...\033[0;m\n");
        exit(1);
    }

    char line[2048];
    int currentLine = 1;
    rewind(fp2); // resetting the file pointer to beginning;
    while (1)
    {
        fgets(line, sizeof(line), fp2);
        if (feof(fp2))
            break;
        else if (currentLine == recipeCount)
        {
            // replacing data with new one and putting in temporary file
            fprintf(tempfp, "%s,", recipe.id);

            for (int i = 0; i < updatedRecipe.ingredientNumber; i++)
            {
                char *ingredient = updatedRecipe.ingredients[i];
                for (int j = 0; ingredient[j] != '\0'; j++)
                {
                    if (i < updatedRecipe.ingredientNumber - 1) // Don't replace the last ingredient newline
                    {
                        if (ingredient[j] == '\n')
                            ingredient[j] = ','; // Replace newline with comma
                    }
                }
                fprintf(tempfp, "%s", ingredient);
            }
        }
        else
        {
            // placing old data with no changes in temporary file
            fputs(line,tempfp);
        }
        currentLine++;
    }
    fclose(fp2);
    fclose(tempfp);
    remove (FILE2_NAME);
    rename (TEMP_FILE, FILE2_NAME);
}

// To update the instructions of the recipe
void updateInstructions(struct recipeBook recipe, struct recipeBook updatedRecipe, int recipeCount)
{
    fp3 = fopen(FILE3_NAME, "r+");
    tempfp = fopen(TEMP_FILE, "w+");

    if (fp3 == NULL || tempfp == NULL)
    {
        printf("\n\t \033[1;31m SOMETHING WENT WRONG! ERROR OPENING FILE...\033[0;m\n");
        exit(1);
    }

    char line[2048];
    int currentLine = 1;
    rewind(fp3); // resetting the file pointer to beginning;
    while (1)
    {
        fgets(line, sizeof(line), fp3);
        if (feof(fp3))
            break;
        else if (currentLine == recipeCount)
        {
            // replacing data with new one and putting in temporary file
            fprintf(tempfp, "%s,", recipe.id);

            for (int i = 0; i < updatedRecipe.stepNumber; i++)
            {
                char *instruction = updatedRecipe.instructions[i];
                for (int j = 0; instruction[j] != '\0'; j++)
                {
                    if (i < updatedRecipe.stepNumber - 1) // Don't replace the last ingredient newline
                    {
                        if (instruction[j] == '\n')
                            instruction[j] = ','; // Replace newline with comma
                    }
                }
                fprintf(tempfp, "%s", instruction);
            }
        }
        else
        {
            // placing old data with no changes in temporary file
            fputs(line,tempfp);
        }
        currentLine++;
    }
    fclose(fp3);
    fclose(tempfp);
    remove (FILE3_NAME);
    rename (TEMP_FILE, FILE3_NAME);
}

// Removing general detail inside the file-1
void removeDetail(int recipeCount)
{    
    fp1 = fopen(FILE1_NAME, "r+");
    tempfp = fopen(TEMP_FILE, "w+");

    if (fp1 == NULL || tempfp == NULL)
    {
        printf("\n\t \033[1;31m SOMETHING WENT WRONG! ERROR OPENING FILE...\033[0;m\n");
        exit(1);
    }

    int currentLine = 1;
    rewind(fp1); // resetting the file pointer to beginning;
    while (fscanf(fp1, "%[^,],%[^,],%[^,],%d,%d,%[^\n]\n", recipe.id, recipe.name, recipe.type, &recipe.ingredientNumber, &recipe.stepNumber, recipe.notes)!=EOF)
    {
        if (currentLine != recipeCount)
        {
            // Storing all the data in temporary file excluding the matching data
            fprintf(tempfp, "%s,%s,%s,%d,%d,%s\n", recipe.id, recipe.name, recipe.type, recipe.ingredientNumber, recipe.stepNumber, recipe.notes); 
        }
        currentLine++;
    }
    fclose(fp1);
    fclose(tempfp);
    remove (FILE1_NAME);
    rename (TEMP_FILE, FILE1_NAME);
}

// Removing ingredients inside the file-2
void removeIngredients(int recipeCount)
{
    fp2 = fopen(FILE2_NAME, "r+");
    tempfp = fopen(TEMP_FILE, "w+");

    if (fp2 == NULL || tempfp == NULL)
    {
        printf("\n\t \033[1;31m SOMETHING WENT WRONG! ERROR OPENING FILE...\033[0;m\n");
        exit(1);
    }

    char line[2048];
    int currentLine = 1;
    rewind(fp2); // resetting the file pointer to beginning;
    while (1)
    {
        fgets(line, sizeof(line), fp2);
        if (feof(fp2))
            break;
        else if (currentLine != recipeCount)
        {
            // Storing all the data in temporary file excluding the matching data
            fputs(line,tempfp);
        }
        currentLine++;
    }
    fclose(fp2);
    fclose(tempfp);
    remove (FILE2_NAME);
    rename (TEMP_FILE, FILE2_NAME);
}

// Removing instructions inside the file-3
void removeInstructions(int recipeCount)
{
    fp3 = fopen(FILE3_NAME, "r+");
    tempfp = fopen(TEMP_FILE, "w+");

    if (fp3 == NULL || tempfp == NULL)
    {
        printf("\n\t \033[1;31m SOMETHING WENT WRONG! ERROR OPENING FILE...\033[0;m\n");
        exit(1);
    }

    char line[2048];
    int currentLine = 1;
    rewind(fp3); // resetting the file pointer to beginning;
    while (1)
    {
        fgets(line, sizeof(line), fp3);
        if (feof(fp3))
            break;
        else if (currentLine != recipeCount)
        {
            // Storing all the data in temporary file excluding the matching data
            fputs(line,tempfp);
        }
        currentLine++;
    }
    fclose(fp3);
    fclose(tempfp);
    remove (FILE3_NAME);
    rename (TEMP_FILE, FILE3_NAME);
}

// To calculate number of recipe
int numberOfRecipe()
{
    int numRecipe=0;
    fp1 = fopen(FILE1_NAME, "r");

    if (fp1 == NULL)
    {
        printf("\n\t \033[1;31m SOMETHING WENT WRONG! ERROR OPENING FILE...\033[0;m\n");
        exit(1);
    }

    while (fscanf(fp1, "%[^,],%[^,],%[^,],%d,%d,%[^\n]\n", recipe.id, recipe.name, recipe.type, &recipe.ingredientNumber, &recipe.stepNumber, recipe.notes) != EOF)
    {
        numRecipe++;
    }
    // printf("%d",numRecipe);
    fclose(fp1);
    return numRecipe;
}

#endif