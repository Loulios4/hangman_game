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

int letter_in_string(const char letter, const char string[])
{
    for (int i = 0; i < strlen(string); i++)
    {
        if (letter == string[i])
        {
            return 1;
        }
    }
    return 0;
}

void hangman(const char secret[])
{
    int attempts = 8;
    char letters_guessed[30] = "";
    char available_letters[30] = "";
    int lg_index = 0;
    char guessed_word[20] = "";

    printf("Welcome to the game, Hangman!\n");
    printf("I am thinking of a word that is %ld letters long.\n", strlen(secret));

    while (!is_word_guessed(secret, letters_guessed) && attempts > 0)
    {
        printf("\n");
        printf("-------------\n");
        printf("You have %d guesses left.\n", attempts);
        get_available_letters(letters_guessed, available_letters);
        printf("Available letters: %s\n", available_letters);
        printf("Please guess a letter: ");

        char tmp[30];
        fgets(tmp, 30, stdin);
        int counter = 0;

        if (tmp[1] == '\n')
        {
            char letter = '\0';

            if (tmp[0] >= 97 && tmp[0] <= 122)
            {
                letter = tmp[0];
            }
            else if (tmp[0] >= 65 && tmp[0] <= 90)
            {
                letter = tmp[0] + 32;
            }
            else
            {
                printf("Oops! '%c' is not a valid letter", tmp[0]);
            }

            if (letter_in_string(letter, letters_guessed))
            {
                counter = 1;
            }
            else
            {
                letters_guessed[lg_index] = letter;
                lg_index++;
            }

            get_guessed_word(secret, letters_guessed, guessed_word);

            if (counter == 1)
            {
                printf("Oops! You've already guessed that letter: ");
                for (int i = 0; i < strlen(guessed_word); i++)
                {
                    printf("%c ", guessed_word[i]);
                }
                printf("\n");
            }
            else
            {
                if (letter_in_string(letter, secret))
                {

                    printf("Good guess: ");
                }
                else
                {
                    printf("Oops! That letter is not in my word: ");
                    attempts--;
                }

                for (int i = 0; i < strlen(guessed_word); i++)
                {
                    printf("%c ", guessed_word[i]);
                }
                printf("\n");
            }
        }
        else if (strlen(tmp) - 1 == strlen(secret))
        {
            for (int i = 0; i < strlen(tmp) - 1; i++)
            {
                if (tmp[i] >= 65 && tmp[i] <= 90)
                {
                    tmp[i] += 32;
                }
            }

            for (int i = 0; i < strlen(tmp) - 1; i++)
            {
                if (tmp[i] != secret[i])
                {
                    printf("Sorry, bad guess. The word was %s.\n", secret);
                    attempts = -1;
                    break;
                }
                attempts = -2;
            }
        }
        else
        {
            attempts = 0;
        }
    }

    if (attempts > 0 || attempts == -2)
    {
        printf("Congratulations, you won!\n");
    }
    else if (attempts == 0)
    {
        printf("Sorry, you ran out of guesses. The word was undeserved.\n");
    }
}