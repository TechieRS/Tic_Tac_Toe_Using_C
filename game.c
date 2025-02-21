#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// For cross-platform sleep and clearing the screen:
#ifdef _WIN32
#include <windows.h>
#define sleep_sec(sec) Sleep((sec)*1000)
#else
#include <unistd.h>
#define sleep_sec(sec) sleep(sec)
#endif

// ANSI color codes for styling
#define RED     "\033[1;31m"
#define BLUE    "\033[1;34m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define CYAN    "\033[1;36m"
#define MAGENTA "\033[1;35m"
#define RESET   "\033[0m"

char board[3][3];

// Function prototypes
void clearScreen();
void displayBanner();
void displayMainMenu();
void printInstructions();
void initBoard();
void printBoard();
char checkWinner();
int isDraw();
void getPlayerNames(char *playerX, char *playerO);
void playGame(const char *playerX, const char *playerO);
void pauseForKey();

int main() {
    int choice;
    char playerX[50] = "Player X";
    char playerO[50] = "Player O";

    do {
        clearScreen();
        displayBanner();
        displayMainMenu();
        printf(YELLOW "\nEnter your choice (1-3): " RESET);
        if (scanf("%d", &choice) != 1) {
            printf(RED "Invalid input! Exiting...\n" RESET);
            exit(1);
        }
        while(getchar() != '\n'); // clear input buffer

        switch(choice) {
            case 1:
                clearScreen();
                displayBanner();
                getPlayerNames(playerX, playerO);
                playGame(playerX, playerO);
                break;
            case 2:
                clearScreen();
                printInstructions();
                pauseForKey();
                break;
            case 3:
                clearScreen();
                printf(GREEN "\nThank you for playing! Goodbye!\n" RESET);
                break;
            default:
                printf(RED "\nInvalid option. Please try again.\n" RESET);
                sleep_sec(2);
                break;
        }
    } while(choice != 3);

    return 0;
}

// Clears the terminal screen (works on Windows and Unix-like systems)
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Displays an attractive ASCII banner
void displayBanner() {
    printf(CYAN "============================================\n" RESET);
    printf(MAGENTA "          WELCOME TO TIC-TAC-TOE            \n" RESET);
    printf(CYAN "============================================\n" RESET);
}

// Displays the main menu options
void displayMainMenu() {
    printf(YELLOW "\nMain Menu:\n" RESET);
    printf("  1. Start New Game\n");
    printf("  2. Instructions\n");
    printf("  3. Exit\n");
}

// Displays game instructions
void printInstructions() {
    printf(CYAN "============================================\n" RESET);
    printf(MAGENTA "               GAME INSTRUCTIONS            \n" RESET);
    printf(CYAN "============================================\n" RESET);
    printf("\nThis is a two-player Tic-Tac-Toe game.\n");
    printf("Players take turns placing their marks on a 3x3 board.\n");
    printf("Player X uses " RED "X" RESET " and Player O uses " BLUE "O" RESET ".\n");
    printf("Enter the row and column numbers (1-3) to make a move.\n");
    printf("The first player to get three in a row (vertically, horizontally, or diagonally) wins.\n");
    printf("If all spots are filled without a winner, the game ends in a draw.\n");
    printf("\nEnjoy the game!\n");
}

// Initializes the board with empty spaces
void initBoard() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            board[i][j] = ' ';
}

// Prints the current game board with color-coded symbols
void printBoard() {
    printf("\n");
    for (int i = 0; i < 3; i++) {
        printf(" ");
        for (int j = 0; j < 3; j++) {
            char cell = board[i][j];
            if(cell == 'X')
                printf(RED " %c " RESET, cell);
            else if(cell == 'O')
                printf(BLUE " %c " RESET, cell);
            else
                printf(" %c ", cell);
            if(j < 2)
                printf("|");
        }
        printf("\n");
        if(i < 2)
            printf("-----------\n");
    }
    printf("\n");
}

// Checks for a winning condition and returns the winning symbol or ' ' if none
char checkWinner() {
    for (int i = 0; i < 3; i++) {
        // Check rows
        if (board[i][0] != ' ' &&
            board[i][0] == board[i][1] &&
            board[i][1] == board[i][2])
            return board[i][0];
        // Check columns
        if (board[0][i] != ' ' &&
            board[0][i] == board[1][i] &&
            board[1][i] == board[2][i])
            return board[0][i];
    }
    // Check diagonals
    if (board[0][0] != ' ' &&
        board[0][0] == board[1][1] &&
        board[1][1] == board[2][2])
        return board[0][0];
    if (board[0][2] != ' ' &&
        board[0][2] == board[1][1] &&
        board[1][1] == board[2][0])
        return board[0][2];

    return ' ';
}

// Returns 1 if the board is full (draw condition), otherwise returns 0
int isDraw() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == ' ')
                return 0;
    return 1;
}

// Prompts the players to enter their names
void getPlayerNames(char *playerX, char *playerO) {
    printf(YELLOW "\nEnter name for " RESET RED "Player X" RESET ": ");
    fgets(playerX, 50, stdin);
    playerX[strcspn(playerX, "\n")] = '\0';  // remove newline

    printf(YELLOW "Enter name for " RESET BLUE "Player O" RESET ": ");
    fgets(playerO, 50, stdin);
    playerO[strcspn(playerO, "\n")] = '\0';
}

// The main game loop
void playGame(const char *playerX, const char *playerO) {
    int row, col;
    char currentPlayerSymbol = 'X';
    char winner = ' ';
    const char *currentPlayerName = playerX;

    initBoard();

    while(1) {
        clearScreen();
        displayBanner();
        printBoard();
        printf("%s's turn (", currentPlayerName);
        if (currentPlayerSymbol == 'X')
            printf(RED " %c " RESET, currentPlayerSymbol);
        else
            printf(BLUE " %c " RESET, currentPlayerSymbol);
        printf(")\n");

        printf(YELLOW "Enter row (1-3) and column (1-3): " RESET);
        if(scanf("%d %d", &row, &col) != 2) {
            printf(RED "Invalid input. Exiting.\n" RESET);
            exit(1);
        }
        while(getchar() != '\n');  // clear the input buffer

        // Adjust for 0-based indexing
        row--; 
        col--;

        // Validate move
        if(row < 0 || row > 2 || col < 0 || col > 2 || board[row][col] != ' ') {
            printf(YELLOW "\nInvalid move. Please try again.\n" RESET);
            pauseForKey();
            continue;
        }

        board[row][col] = currentPlayerSymbol;

        // Check for a win
        winner = checkWinner();
        if(winner != ' ') {
            clearScreen();
            displayBanner();
            printBoard();
            if(winner == 'X')
                printf(GREEN "\n%s wins!\n" RESET, playerX);
            else
                printf(GREEN "\n%s wins!\n" RESET, playerO);
            break;
        }

        // Check for draw
        if(isDraw()) {
            clearScreen();
            displayBanner();
            printBoard();
            printf(YELLOW "\nIt's a draw!\n" RESET);
            break;
        }

        // Switch players
        if(currentPlayerSymbol == 'X') {
            currentPlayerSymbol = 'O';
            currentPlayerName = playerO;
        } else {
            currentPlayerSymbol = 'X';
            currentPlayerName = playerX;
        }
    }
    pauseForKey();
}

// Pauses the game until the user presses Enter
void pauseForKey() {
    printf(YELLOW "\nPress Enter to continue..." RESET);
    while(getchar() != '\n');
}

