/*
Author: EMMANUEL EJAKPOMEWHE 33961412
Date: 19/11/20
Purpose: reads amount of coin from a file and finds the most efficient set of coins to give, this is then printed on to a csv file
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int SIZE = 10;

typedef struct item
{
    char name[50];
    int coinAmt;
    int fifties;
    int twenties;
    int tens;
    int fives;
} itemType;

int coinCount(int userInput, int coinValue)
{
    int coinCounter = 0;

    while(userInput >= coinValue)
    {
        userInput = userInput - coinValue;
        coinCounter = coinCounter +1;
    }
    return coinCounter;
}

int coinSubtract(int userInput, int coinValue)
{
    int coinCounter = 0;

    while(userInput >= coinValue)
    {
        userInput = userInput - coinValue;
        coinCounter = coinCounter + 1;
    }

    //printf("Number is: %d\n", userInput);
    return userInput;
}

int nullCheck(FILE *userFile)
{
    //Checks if the file has successfully been opened
    if (userFile == NULL)
    {
        printf("File opening failed\n");
        return -1;
    }
    else
    {
        printf("File opening successful\n");
        return 0;
    }
    
    
}

FILE* readFile(char *x[])
{
    //Opens the file which is being read from
    FILE *userFile;
    userFile = fopen(x[1], "r");
    return (userFile);

}

FILE* writeFile(char *x[])
{
    //Opens the file which is being written to
    FILE *userFile;
    userFile = fopen(x[2], "w");
    return (userFile);

}

/*void printLine (FILE *userFile)
{
    //Prints the content of the file
    char line[SIZE];
    int age = 0;
    float wage = 0.0;
    int count = 0;
    while (fscanf(userFile, "%s %d %f" , line, &age, &wage) != EOF)
    {
        printf("%s %d %.1f\n", line, age , wage);
        count++;
    }
    printf("\nThe number of lines is: %d", count);
    
}*/

void arrayBin(itemType bins[SIZE], int *numBins, FILE *userFile)
{
    //stores the content of the file which it has read from into an array of structs   
    char line[SIZE];
    int coinAmt = 0;
    int count = 0;
    int countOne = 0;
    int countTwo = 0;
    while (fscanf(userFile, "%s %d" , line, &coinAmt) != EOF)
    {
        //bins[count].name= (char*) calloc(SIZE, sizeof(char));
        for(int i=0; i<=strlen(line); i++)
        {
            if(line[i] >= 65 && line[i] <= 90)
            {
                line[i] = line[i] + 32;
            }
        }
   
        strcpy(bins[count].name, line);
        bins[count].coinAmt = coinAmt;
        count++;
    }
    
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = i + 1; j < SIZE; j++)
        {
            int res = strcmp(bins[i].name, bins[j].name);
            //printf("res is :%d", res);
            if(res == 0)
            {
                bins[i].coinAmt = bins[i].coinAmt + bins[j].coinAmt;
                countTwo = j + 1;
                break;
            }
        }
        countOne++;
        
    }

    if(countTwo < 0 || countTwo > SIZE)
    {
        printf("Invalid position!");
    }
    else
    {
        for(int i = countTwo - 1; i < SIZE; i++)
        {
            strcpy(bins[i].name, bins[i + 1].name);
            bins[i].coinAmt = bins[i + 1].coinAmt;

        }
    }
    *numBins = count - 1;
    //printf("%s", bins[0].name);
    
}

void writeRecord(itemType bins[], FILE *userFile, int x)
{
    //writes the content of the array struct onto an output file using pointers
    int count = 0;
    for (int i = 0; i < x; i++)
    {
        fprintf(userFile, "%s,%d,%d,%d,%d,%d\n", bins[count].name, bins[count].coinAmt, bins[count].fifties, bins[count].twenties, bins[count].tens, bins[count].fives);
        count++;
    }
    
}

char getString(char userInput[])
{
    //Asks the user for an input
    userInput[SIZE];
    printf("Please enter a name: ");
    scanf("%s%*c", userInput);
    for(int i = 0; i <=strlen(userInput); i++)
    {
        if(userInput[i]>=65&&userInput[i]<=90)
        {
            userInput[i]=userInput[i]+32;
        }
   }

}

void arraySearch(itemType bins[], char userInput[], int x)
{
    //Searches for user input string in struct array
    char line[SIZE];
    int res;
    for (int i = 0; i < x; i++)
    {
        res = strcmp(userInput, bins[i].name);
        if (res == 0)
        {
            printf("Customer: \n");
            printf("%s %d cents\n", bins[i].name, bins[i].coinAmt);

            printf("%d coin(s) x 50 cents\n", bins[i].fifties);
            printf("%d coin(s) x 20 cents\n", bins[i].twenties);
            printf("%d coin(s) x 10 cents\n", bins[i].tens);
            printf("%d coin(s) x 5 cents\n", bins[i].fives);
            break;
        }       
    }
    if (res != 0)
    {
        printf("Name not found\n");
        return -1;
    }
    
}

int coinAdder(itemType bins[])
{
    int coinRemainder = 0;
    int x;
    for (int i = 0; i < SIZE; i++)
    {
        x = bins[i].coinAmt;
        bins[i].fifties = coinCount(x, 50);
        coinRemainder = coinSubtract(x, 50);
        bins[i].twenties = coinCount(coinRemainder, 20);
        coinRemainder = coinSubtract(coinRemainder, 20);
        bins[i].tens = coinCount(coinRemainder, 10);
        coinRemainder = coinSubtract(coinRemainder, 10);
        bins[i].fives = coinCount(coinRemainder, 5);
    }
}

int main (int argc, char *argv[])
{
    FILE *srcFile;
    FILE *dstFile;
    itemType bins[SIZE];
    char userString[50];
    int recordSIZE;
    int x;
    int numPick; 
    //Checks if a command line arguement has been given
    if (argc == 1)
    {
        printf("No command line arguments given!\n");
    }
    else
    {
        srcFile = readFile(argv);
        dstFile = writeFile(argv);

        if (nullCheck(dstFile) == -1 || nullCheck(srcFile) == -1)
        {
            exit(0);
        }
        arrayBin(bins, &recordSIZE, srcFile);
        coinAdder(bins);
        writeRecord(bins, dstFile, recordSIZE);
        fclose(srcFile);

        printf("1. Enter a name:\n");
        printf("2. Exit:\n");
        printf("Pick option 1 or 2: ");
        scanf("%d%*c", &numPick);
        
        do
        {

            if (numPick == 1)
            {              
                getString(userString);
                arraySearch(bins, userString, recordSIZE);

                printf("\nPress 1 to continue or 2 to quit: ");
                scanf("%d%*c", &numPick);
            } 
            if (numPick != 1 && numPick != 2)
            {
                printf("Please try again: ");
                scanf("%d%*c", &numPick);
            }         
            

        } while (numPick != 2);
        

        /*printf("Plese enter a name: ");
        getString(userString);
        arraySearch(bins, userString, recordSIZE);*/

    }

}