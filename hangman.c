#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "hangman.h"
#include <string.h>

int get_word(char secret[])
{
    // check if file exists first and is readable
    FILE *fp = fopen(WORDLIST_FILENAME, "rb");
    if (fp == NULL)
    {
        fprintf(stderr, "No such file or directory: %s\n", WORDLIST_FILENAME);
        return 1;
    }

    // get the filesize first
    struct stat st;
    stat(WORDLIST_FILENAME, &st);
    long int size = st.st_size;

    do
    {
        // generate random number between 0 and filesize
        long int random = (rand() % size) + 1;
        // seek to the random position of file
        fseek(fp, random, SEEK_SET);
        // get next word in row ;)
        int result = fscanf(fp, "%*s %20s", secret);
        if (result != EOF)
            break;
    } while (1);

    fclose(fp);

    return 0;
}

int is_word_guessed(const char secret[], const char letters_guessed[])
{
    for (int i = 0; i < strlen(secret); i++)
    {
        int findFlag = 0;
        for (int j = 0; j < strlen(letters_guessed); j++)
        {
            if (secret[i] == letters_guessed[j])
            {
                findFlag = 1;
                break;
            }
        }
        if (findFlag == 0)
        {
            return 0;
        }
    }
    return 1;
}

void get_guessed_word(const char secret[], const char letters_guessed[], char guessed_word[])
{
    for (int i = 0; i < strlen(secret); i++)
    {
        int findFlag = 0;
        for (int j = 0; j < strlen(letters_guessed); j++)
        {
            if (secret[i] == letters_guessed[j])
            {
                guessed_word[i] = secret[i];
                findFlag = 1;
                break;
            }
        }
        if (findFlag == 0)
        {
            guessed_word[i] = '_';
        }
    }
}

void get_available_letters(const char letters_guessed[], char available_letters[])
{
    char alphabet[27] = "abcdefghijklmnopqrstuvwxyz";
    int l = 0;
    for (int i = 0; i < strlen(alphabet); i++)
    {
        int counter = 0;
        for (int j = 0; j < strlen(letters_guessed); j++)
        {
            if (alphabet[i] == letters_guessed[j])
            {
                counter = 1;
                break;
            }
        }

        if (counter == 0)
        {
            available_letters[l++] = alphabet[i];
        }
    }

    available_letters[++l] = '\0';
}

void hangman(const char secret[])
{
    int guesses = 8;

    printf("Welcome to the game, Hangman!\n");
    printf("I am thinking of a word that is %d letters long.\n", strlen(secret));

    while (is_word_guessed() != 1)
    {
        if (guesses = 0)
        {
        }

        printf("-------------\n");
        printf("You have %d guesses left.", guesses);
        printf("Available letters: %s", get_available_letters());
        printf("Please guess a letter: ");
        scanf("%s", &letters_guessed[]);

        for (int i = 0; i < strlen(secret); i++)
        {

            if (letters_guessed[] == secret[i])
            {
                printf("Good guess: %s", get_guessed_word());
            }
            else
            {
                printf("Oops! That letter is not in my word: %s", get_guessed_word());
            }
        }

        guesses--;
    }

    printf("Congratulations, you won!");
}