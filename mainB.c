/*
* Ethan Shealey
* 11/27/18
* ~/csc1710/prog5/shealey_HangMan_A.c
*
* The Classic game of hangman!
*
* My program works with full phrases up to 100 chars, includes ASCII art of each stage,
* and works with many data files filled with 30 random words and phrases!
*
* Max Grade: 108 :D
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>

struct hangman {
    char words[30][100];
    char chosenWord[100];
    char guessChar;
    char pos[100];
    char pool[100];
    bool dupe;
    int wordLengthWithoutSpaces;
    int fail;
    int wordLength;
};

struct hangman game;

void drawGame(int missed) // ASCII art
{
    int i;

    for(i = 0; i < 100; i++)
        fprintf(stdout, "\n");
    for(i = 0; i < 20; i++)
        fprintf(stdout, "~");
    fprintf(stdout, " Hangman ");
    for(i = 0; i < 20; i++)
        fprintf(stdout, "~");
    fprintf(stdout, "\n");

    if(missed == 0)
        fprintf(stdout, "\n____\n|  ‖\n|\n|\n|\n|\n");
    else if(missed == 1)
        fprintf(stdout, "Latest Part Drawn: Head\n____\n|  ‖\n|(   )\n|\n|\n|\n");
    else if(missed == 2)
        fprintf(stdout, "Latest Part Drawn: Torso\n____\n|  ‖\n|(   )\n|  |\n|\n|\n");
    else if(missed == 3)
        fprintf(stdout, "Latest Part Drawn: Left Arm\n____\n|  ‖\n|(   )\n| /|\n|\n|\n");
    else if(missed == 4)
        fprintf(stdout, "Latest Part Drawn: Right Arm\n____\n|  ‖\n|(   )\n| /|\\\n|\n|\n");
    else if(missed == 5)
        fprintf(stdout, "Latest Part Drawn: Left Leg\n____\n|  ‖\n|(   )\n| /|\\\n| /\n|\n");
    else if(missed == 6)
        fprintf(stdout, "Latest Part Drawn: Right Leg\n____\n|  ‖\n|(   )\n| /|\\\n| / \\\n|\n");
    else if(missed == 7)
        fprintf(stdout, "Latest Part Drawn: Left Eye\n____\n|  ‖\n|(˚  )\n| /|\\\n| / \\\n|\n");
    else if(missed == 8)
        fprintf(stdout, "Latest Part Drawn: Right Eye\n____\n|  ‖\n|(˚ ˚)\n| /|\\\n| / \\\n|\n");
    else if(missed == 9)
        fprintf(stdout, "Latest Part Drawn: Nose\n____\n|  ‖\n|(˚‘˚)\n| /|\\\n| / \\\n|\n");
    else if(missed == 10)
        fprintf(stdout, "Latest Part Drawn: Frown\n____\n|  ‖\n|(˚¬˚)\n| /|\\\n| / \\\n|\n");
}
int loadArray() // Function to load the words in the data file
{
    int count = 0;
    char filename[100], newLine;

    FILE *wordfile;
    fprintf(stdout, "Please enter data file name: ");  //
    fscanf(stdin, "%s", filename);            // Opens the file user request
    wordfile = fopen(filename, "r");          //
    while (fscanf(wordfile, "%[^\n]%c", game.words[count], &newLine) != EOF) // Scans in all words
        count++;

    if(wordfile == NULL)
    {
        fprintf(stderr, "Data File Could Not Be Loaded\n");
        return 1;
    }

    fclose(wordfile);

    return count; // return word count
}
void getWordAndLength() // Function to get the random word/phrase and its length
{
    int j = 0, r, count = loadArray();

    srand(time(0));     // Funds a random number
    r = rand() % count; // to correspond with a word

    strcpy(game.chosenWord, game.words[r]);

    while(game.chosenWord[j] != '\0') // finds the length of the chosen word
    {
        game.wordLength++;
        game.chosenWord[j] = tolower(game.chosenWord[j]); // sets the chosen word to lowercase
        if(game.chosenWord[j] >= 97 && game.chosenWord[j] <= 122)
            game.wordLengthWithoutSpaces++; // If not a special character, increment
        j++;
    }

    fprintf(stdout, "%s\n", game.chosenWord);
}
void printPuzzle() // Prints the puzzle in its current state
{
    int i;

    for (i = 0; i < game.wordLength; i++) {
        if (game.pos[i] >= 97 && game.pos[i] <= 122) // if alphabetical character ->
            fprintf(stdout, "%c", game.pos[i]);
        else if(game.chosenWord[i] == ' ') // if a space ->
            fprintf(stdout, " ");
        else if(game.chosenWord[i] < 97 || game.chosenWord[i] > 122) // if not a letter ->
            fprintf(stdout, "%c", game.chosenWord[i]);
        else
            fprintf(stdout, "~"); // Else print missing char
    }

    fprintf(stdout, "\n");
}
void guess() // Gets the users guess
{
    if(!game.dupe)
        fprintf(stdout, "Please Guess a Letter: ");
    else
        fprintf(stdout, "Duplicate char entered! Please try again: ");
    fscanf(stdin, " %c", &game.guessChar);
    game.guessChar = tolower(game.guessChar); // forces guess to lowercase
    while(game.guessChar < 97 || game.guessChar > 122) // idiot proofing
    {
        fprintf(stdout, "Invalid Character! Please try again!\n");
        fscanf(stdin, " %c", &game.guessChar);
        game.guessChar = tolower(game.guessChar);
    }
}
void hangmanGame() // Loops until game is over
{
    int correct = 0, i, failStore = 0, e;
    bool didUserMiss = true;

    // While game isn't over -->
    while (game.fail <= 9 && correct != game.wordLengthWithoutSpaces) {
        // Checking for Dupes
        for (i = 0; i < game.wordLength; i++) {
            while(game.guessChar == game.pos[i] || game.guessChar == game.pool[i]) {
                // If a dupe, loop until fixed
                game.dupe = true;
                guess();
                i = 0;
            }
        }
        game.dupe = false;

        // Finding if user guessed char matches the word
        for (i = 0; i < game.wordLength; i++) {
            if (game.guessChar == game.chosenWord[i]) {
                didUserMiss = false;
                game.pos[i] = game.guessChar; // Sets aside the correct letters
                correct++;
            }
        }
        // Gives the user 10 chances to mess up
        if (didUserMiss) {
            game.fail++;
            game.pool[failStore++] = game.guessChar; // Stores incorrect letters
        }
        didUserMiss = true; // Resets bool

        drawGame(game.fail);  // Prints ASCII art and puzzle
        printPuzzle();

        fprintf(stdout, "\nAttempts Left: %i\n", 10 - game.fail); // Prints how many attempts are left
        fprintf(stdout, "Letters Guessed: ");      //
        for (e = 0; e < game.fail; e++)   // Prints the
            fprintf(stdout, "%c ", game.pool[e]);  // Missed Characters
        fprintf(stdout, "\n");                     //

        if (correct != game.wordLengthWithoutSpaces && game.fail <= 9)  // Ask for another letter if game is not over
            guess();
    }
}
void winOrLose()
{
    if (game.fail > 9) { // if they lost, print appropriate response
        drawGame(10);
        printPuzzle();
        fprintf(stdout, "You Lost! The Phrase was: '%s'\n", game.chosenWord);
    }
    else { // if they won, print appropriate response
        drawGame(game.fail);
        printPuzzle();
        fprintf(stdout, "Congratulations! You Won!\n");
    }
}
int main() {

    game.fail = 0;                     //
    game.wordLengthWithoutSpaces = 0;  // Initializes needed variables
    game.wordLength = 0;               //
    game.dupe = false;                 //

    // Gets the random word or phrase
    getWordAndLength();

    //~~~~~~~~~~~~~~~~~~~ Begin Game ~~~~~~~~~~~~~~~~~~~//
    drawGame(0);   // Prints the first
    fprintf(stdout, "Attempts Left: 10\n");
    printPuzzle(); // empty puzzle and gets user's guess
    guess();       //

    hangmanGame(); // Loops through the rest of the game
    winOrLose();   // Prints appropriate Win/Loss message

    return 0;

}
