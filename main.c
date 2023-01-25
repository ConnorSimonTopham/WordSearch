/* AUTHOR:  Connor Simon-Topham
 * DATE:    03/04/19
 * PURPOSE: To create an interactive wordsearch
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

int WIDTH = 16, HEIGHT = 16;

char ** create2DArray() {
    /*  Creates and allocates space for a
     * 2d array of size 16 by 16
     * returns array
     */
    char ** newArray = malloc(sizeof(char*) * HEIGHT);
    int i, n;
    for (i = 0; i < 16; i++) {
        for (n = 0; n < 16; n++) {
            // Allocate space for the second dimension of the array
            newArray[i] = malloc(sizeof(char*) * WIDTH);;
        }
    }
    return newArray;
}

void printArray(char ** array) {
    /* iterates through an array
     * printing the values out
     * 2 spaces after each character
     * with new line after 16 chars
     */
    int i, n;
    for (i = 0; i < WIDTH; i++) {
        for (n = 0; n < HEIGHT; n++) {
            printf("%c  ", array[i][n]);
        }
        // Add a newline after each 2nd level array
        printf("\n");
    }
}

void generateArray(char **array) {
    /* Fills a 2d array (16 by 16)
     * with periods
     */
    int i, n;
    for (i = 0; i < WIDTH; i++) {
        for (n = 0; n < HEIGHT; n++) {
            array[i][n] = '.';
        }
    }
}

void copyArray(char ** array1, char ** array2) {
    /* iterates through an array
     * and copies the values into
     * another array
     */
    int i, n;
    for (i = 0; i < 16; i++) {
        for (n = 0; n < 16; n++) {
            array1[i][n] = array2[i][n];
        }
    }
}

void insertHorizontally(char * word, char ** array) {
    /* Inserts word into an array
     * horizontally randomly,
     * making sure there is no
     * overwriting of over words
     */
    int i;
    while (1) {
        // Creates a copy of the array
        char **tempArray = create2DArray();
        copyArray(tempArray, array);
        // If the passed word is too long throws error
        if (strlen(word) > WIDTH) {
            printf("ERROR: \"%s\" is too long to add to the array\n", word);
            break;
        } else {
            // Get a random X and Y. Avoid a div by 0 on the Y.
            int randX = rand() % HEIGHT;
            int randY = (WIDTH - strlen(word) == 0) ? 0 : rand() % (WIDTH - strlen(word));
            // Inserts the letters in the array
            for (i = 0; i < strlen(word); i++) {
                // If the spot isnt free will break out of the for loop and try again
                if (tempArray[randX][randY + i] != '.') {
                    break;
                }
                tempArray[randX][randY + i] = word[i];
            }
        }
        // Check if the loop is completed
        if (i == strlen(word)) {
            copyArray(array, tempArray);
            break;
        }
    }
}

void insertVertically(char * word, char ** array) {
    /* Inserts word into an array
     * vertically randomly,
     * making sure there is no
     * overwriting of over words
     */
    int i;
    while (1) {
        // Creates a copy of the array
        char ** tempArray = create2DArray();
        copyArray(tempArray, array);
        // If the passed word is too long throws error
        if (strlen(word) > HEIGHT) {
            printf("ERROR: \"%s\" is too long to add to the array\n", word);
            break;
        } else {
            // Get a random X and Y. Avoid a div by 0 on the Y.
            int randX = (HEIGHT - strlen(word) == 0) ? 0 : rand() % (HEIGHT - strlen(word)), randY = rand() % WIDTH;
            // Inserts the letters in the array
            for (i = 0; i < strlen(word); i++) {
                // If the spot isn't free break out of the for loop and try again
                if (tempArray[randX + i][randY] != '.') {
                    break;
                }
                tempArray[randX + i][randY] = word[i];
            }
        }
        // Check if the loop is completed
        if (i == strlen(word)) {
            copyArray(array, tempArray);
            break;
        }
    }
}

void insertDiagonally(char * word, char ** array, bool ascending) {
    /* Inserts word into an array
     * diagonally randomly,
     * making sure there is no
     * overwriting of over words
     */
    int i;
    while (1) {
        // Creates a copy of the array
        char **tempArray = create2DArray();
        copyArray(tempArray, array);
        // If the passed word is too long throws error
        if (strlen(word) > HEIGHT || strlen(word) > WIDTH) {
            printf("ERROR: \"%s\" is too long to add to the array\n", word);
            break;
        } else {
            // Get a random X and Y. Avoid a div by 0 on the Y.
            int randX = (HEIGHT - strlen(word) == 0) ? 0 : rand() % (HEIGHT - strlen(word));
            int randY = (WIDTH - strlen(word) == 0) ? 0 : rand() % (WIDTH - strlen(word));
            // Inserts the letters in the array
            for (i = 0; i < strlen(word); i++) {
                // If the spot isnt free will break out of the for loop and try again
                if (tempArray[randX + i][randY + i] != '.') {
                    break;
                }
                // Ternary for applying ascending or descending
                tempArray[randX + i][randY + i] = word[(ascending) ? i : strlen(word) - 1 - i];
            }
            // Check if the loop is completed
            if (i == strlen(word)) {
                copyArray(array, tempArray);
                break;
            }
        }
    }
}

void generateGrid(char ** array, char ** words) {
    /* Inserts an array of words
     * into an array with a 40%
     * chance of it being horizontal
     * or vertically and 20% for
     * diagonally
     */
    int i, rand_val;
    for (i = 0; i < 6; i++) {
        rand_val = rand() % 100;
        if (rand_val < 40) {
            insertVertically(words[i], array);
        } else if (rand_val < 80) {
            insertHorizontally(words[i], array);
        } else {
            // Rolls a 50/50 chance for the word to be ascending
            bool ascending = rand() % 100 > 50;
            insertDiagonally(words[i], array, ascending);
        }
    }
}

bool contains(char *word, char **array) {
    /* checks if a word is contained
     * in an 2d array and removes it
     * if present
     * returns bool
     */
    int i;
    for (i = 0; i < 8; i++) {
        // Ensure that a NULL value isn't being used
        if (array[i] != NULL && *array[i] == *word) {
            // Removes the word
            array[i] = NULL;
            return true;
        }
    }
    return false;
}

char getRandomLetter() {
    /* generates random letter
     * returns pointer to letter
     */
    char * letters[26] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};
    // Pick random number between 0 and 25 to choose a random letter
    return *letters[rand() % 25];
}

void fillArrayWithLetters(char **array) {
    /* Fills an array with random
     * letters
     */
    int i, n;
    for (i = 0; i < 16; i++) {
        for (n = 0; n < 16; n++) {
            // Check if the space is blank
            if (array[i][n] == '.') {
                array[i][n] = getRandomLetter();
            }
        }
    }
}

int main(){
    // Sets seed
    srand(time(NULL));

    // Categories
    char * categories[3][8] = {{"mercury","venus","earth","mars","jupiter","saturn","uranus","neptune"},
                               {"zebu","walrus","opossum","octopus","yak","turtle","weasel","canary"},
                               {"red","orange","yellow","green","blue","indigo","violet","purple"}};

    // Welcomes the user
    printf("Welcome to the word search!\n"
           "Which category would you like the words to be from?\n"
           "[1]: Planets\n"
           "[2]: Animals\n"
           "[3]: Colours\n");

    // Take their choice of category
    int categoryIndex = 1;
    scanf("%d", &categoryIndex);

    // Create grid
    char ** array = create2DArray();
    generateArray(array);
    generateGrid(array, categories[categoryIndex-1]);
    fillArrayWithLetters(array);


    // Initializes timer
    int startTime = clock()/CLOCKS_PER_SEC;
    int stopTime = startTime + 120;
    char guess[20] = "earth";
    int correct = 0;
    bool winner = false;

    // Start game loop
    while (clock() / CLOCKS_PER_SEC < stopTime) {
        printf("\n\n");
        printArray(array);
        printf("================\n");
        printf("Time remaining: %ds\n", (int) stopTime - (clock() / CLOCKS_PER_SEC));
        printf("Score: %d\n", correct);
        printf("Enter any words you can see:\n");
        // Check for winning the game
        if (correct >= 8) {
            winner = true;
            break;
        }
        // Take guess
        scanf("%s", guess);
        // Check for time being up
        if (clock() / CLOCKS_PER_SEC > stopTime) {
            break;
        }
        // Check if the guess is in the word search
        if (contains(guess, categories[categoryIndex - 1])) {
            correct ++;
            printf("\n\n================\n"
                   "Correct! You found %s!"
                   "\n================", guess);
        }
    }
    printf("\n\n================\n");
    // Win / Lose message
    printf((winner) ? "You Win! You found all the words!" : "Game Over! You ran out of time!");
    printf("\n================");
    return 0;
}