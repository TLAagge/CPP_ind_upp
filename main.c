// Includes and definitions 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "SafeInput.h"

#ifdef _WIN32 //Windows system specific includes and definitions
    #include <windows.h>
    #define SLEEP(ms) Sleep(ms)
    #define CLEAR_SCREEN "cls"
#else
    #include <unistd.h>//Unix system specific includes and definitions
    #define SLEEP(ms) usleep((ms) * 1000)
    #define CLEAR_SCREEN "clear"
#endif

//Data sturctures

typedef struct {
    char shortcode[10];     //Card shortcode (e.g., "1212")
    int hasAccess;          //1 = has access, 0 = denied
    char dateAdded[20];     //Date when card was added (YYYY-MM-DD HH:MM:SS)
} Card;

typedef struct {
    Card* cards;            //Dynamic array of cards
    int cardCount;          //Number of cards in system
    int capacity;           //Current capacity of cards array
} SystemState;

//Function declarations

//System initialization and cleanup
void initSystem(SystemState* state);
void cleanupSystem(SystemState* state);

//Card management
int findCardIndex(SystemState* state, const char* shortcode);//Returns index of card or -1 if not found
void addOrUpdateCard(SystemState* state, const char* shortcode, int hasAccess);//Adds new card or updates existing card
void getCurrentDateTime(char* buffer, size_t size);//Gets current date and time as string

//Menu and user interface
void showMainMenu(); 
void remoteOpenDoor();
void listAllCards(SystemState* state);
void addRemoveAccess(SystemState* state);
void scanCard(SystemState* state);
void showLamp(const char* color, int duration);

//Main function

int main() { //Program entry point 
    SystemState state;
    initSystem(&state); //Initialize system state
    
    int running = 1; //Main loop flag
    
    printf("==============================================\n");
    printf("    DOOR ACCESS CARD SYSTEM\n");
    printf("==============================================\n\n");
    
    while (running) { //Switch menu loop while running
        showMainMenu();
        int choice = safeInputInt("Enter your choice: ");
        
        switch (choice) {
            case 1:
                remoteOpenDoor();
                break;
            case 2:
                listAllCards(&state);
                break;
            case 3:
                addRemoveAccess(&state);
                break;
            case 4:
                scanCard(&state);
                break;
            case 5: //Exit
                printf("\nExiting system...\n");
                running = 0;
                break;
            default: //Invalid choice
                printf("\nInvalid choice. Please try again.\n");
                SLEEP(1500);
        }
    }
    
    cleanupSystem(&state); //Cleanup system state
    return 0; //Exit program
}

//System initialization and cleanup functions

void initSystem(SystemState* state) { //Initialize system state
    state->cardCount = 0; //Number of cards currently in system
    state->capacity = 10;  //Start with capacity for 10 cards
    state->cards = (Card*)malloc(state->capacity * sizeof(Card)); //Allocate memory for cards array
    
    if (state->cards == NULL) { //Check for allocation failure
        fprintf(stderr, "ERROR: Failed to allocate memory for card system\n"); 
        exit(1); //Exit program on failure
    }
}

void cleanupSystem(SystemState* state) { //Cleanup system state
    if (state->cards != NULL) { //Check if cards array is allocated
        free(state->cards); //Free allocated memory
        state->cards = NULL; //Set pointer to NULL after freeing
    }
    state->cardCount = 0; //Reset card count
    state->capacity = 0;  //Reset capacity
}

+//Card management functions
+
int findCardIndex(SystemState* state, const char* shortcode) { //Finds index of card by shortcode
    for (int i = 0; i < state->cardCount; i++) { //Loop through cards
        if (strcmp(state->cards[i].shortcode, shortcode) == 0) { //Check for match
            return i; //Return index if found
        }
    }
    return -1;  //Card not found
}

void addOrUpdateCard(SystemState* state, const char* shortcode, int hasAccess) { //Adds new card or updates existing card
    int index = findCardIndex(state, shortcode); //Check if card already exists
    
    if (index >= 0) { //Card exists, update access
        state->cards[index].hasAccess = hasAccess;
        getCurrentDateTime(state->cards[index].dateAdded, sizeof(state->cards[index].dateAdded)); //Update date added
    } else { //Card doesn't exist, add new card
        //Check if we need to expand capacity
        if (state->cardCount >= state->capacity) {
            state->capacity *= 2;  //Double the capacity
            Card* newCards = (Card*)realloc(state->cards, state->capacity * sizeof(Card)); //Reallocate memory
            
            if (newCards == NULL) { //Check for allocation failure
                fprintf(stderr, "ERROR: Failed to expand card system memory\n");
                return;
            }
            
            state->cards = newCards; //Update cards pointer
        }
        
        //Add new card
        strncpy(state->cards[state->cardCount].shortcode, shortcode, sizeof(state->cards[state->cardCount].shortcode) - 1); //Copy shortcode
        state->cards[state->cardCount].shortcode[sizeof(state->cards[state->cardCount].shortcode) - 1] = '\0';
        state->cards[state->cardCount].hasAccess = hasAccess; //Set access
        getCurrentDateTime(state->cards[state->cardCount].dateAdded, sizeof(state->cards[state->cardCount].dateAdded)); //Set date added
        state->cardCount++; //Increment card count
    } 
}

void getCurrentDateTime(char* buffer, size_t size) { //Gets current date and time as string
    time_t now = time(NULL); //Get current time
    struct tm* t = localtime(&now); //Convert to local time
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", t); //Format date and time
}

//User interface functions

void showMainMenu() { //Displays main menu
    printf("\n==============================================\n");
    printf("MAIN MENU\n");
    printf("==============================================\n");
    printf("1. Remote open door\n");
    printf("2. List all cards in system\n");
    printf("3. Add/remove access\n");
    printf("4. Fake card test scanning\n");
    printf("5. Exit\n");
    printf("==============================================\n");
}

void remoteOpenDoor() { //Simulates remote door opening
    printf("\n==============================================\n");
    printf("Opening door remotely...\n");
    printf("==============================================\n");
    showLamp("GREEN", 3000);
    printf("Door closed.\n");
    printf("\nPress Enter to continue...");
    getchar();
}

void listAllCards(SystemState* state) { //Lists all cards in the system
    printf("\n==============================================\n");
    printf("ALL CARDS IN SYSTEM\n");
    printf("==============================================\n");
    
    if (state->cardCount == 0) {
        printf("No cards in system.\n");
    } else {
        printf("%-10s %-15s %-20s\n", "Shortcode", "Access", "Date Added");
        printf("----------------------------------------------\n");
        
        for (int i = 0; i < state->cardCount; i++) {
            printf("%-10s %-15s %-20s\n",
                   state->cards[i].shortcode,
                   state->cards[i].hasAccess ? "HAS ACCESS" : "DENIED",
                   state->cards[i].dateAdded);
        }
    }
    
    printf("\nPress Enter to continue...");
    getchar();
}

void addRemoveAccess(SystemState* state) { //Adds or removes access for a card
    char shortcode[20];
    
    printf("\n==============================================\n");
    printf("ADD/REMOVE ACCESS\n");
    printf("==============================================\n");
    printf("Enter card shortcode: ");
    safeInput(shortcode, sizeof(shortcode));
    
    if (strlen(shortcode) == 0) {
        printf("ERROR: Shortcode cannot be empty.\n");
        SLEEP(1500);
        return;
    }
    
    int index = findCardIndex(state, shortcode);
    
    if (index >= 0) {
        //Card exists
        printf("\nCard '%s' found.\n", shortcode);
        printf("Current access: %s\n", state->cards[index].hasAccess ? "HAS ACCESS" : "DENIED");
        printf("\n1. Grant access\n");
        printf("2. Deny access\n");
        printf("3. Cancel\n");
        
        int choice = safeInputInt("Enter your choice: ");
        
        if (choice == 1) {
            addOrUpdateCard(state, shortcode, 1);
            printf("\nAccess GRANTED for card '%s'\n", shortcode);
        } else if (choice == 2) {
            addOrUpdateCard(state, shortcode, 0);
            printf("\nAccess DENIED for card '%s'\n", shortcode);
        } else {
            printf("\nOperation cancelled.\n");
        }
    } else {
        //Card doesn't exist
        printf("\nCard '%s' not found in system.\n", shortcode);
        printf("Do you want to create this card?\n");
        printf("1. Yes, grant access\n");
        printf("2. Yes, but deny access\n");
        printf("3. No, cancel\n");
        
        int choice = safeInputInt("Enter your choice: ");
        
        if (choice == 1) {
            addOrUpdateCard(state, shortcode, 1);
            printf("\nCard '%s' created with access GRANTED\n", shortcode);
        } else if (choice == 2) {
            addOrUpdateCard(state, shortcode, 0);
            printf("\nCard '%s' created with access DENIED\n", shortcode);
        } else {
            printf("\nOperation cancelled.\n");
        }
    }
    
    SLEEP(1500);
}

void scanCard(SystemState* state) { //Simulates scanning a card
    char shortcode[20]; 
    
    printf("\n==============================================\n");
    printf("CARD SCANNING SIMULATION\n");
    printf("==============================================\n");
    printf("Enter card shortcode to scan: ");
    safeInput(shortcode, sizeof(shortcode));
    
    if (strlen(shortcode) == 0) {
        printf("ERROR: Invalid card.\n");
        showLamp("RED", 2000);
        printf("\nPress Enter to continue...");
        getchar();
        return;
    }
    
    int index = findCardIndex(state, shortcode);
    
    if (index >= 0 && state->cards[index].hasAccess) {
        //Card has access
        printf("\nCard '%s' scanned.\n", shortcode);
        printf("Access granted!\n");
        showLamp("GREEN", 3000);
    } else {
        //Card denied or not found
        printf("\nCard '%s' scanned.\n", shortcode);
        printf("Access DENIED!\n");
        showLamp("RED", 2000);
    }
    
    printf("\nPress Enter to continue...");
    getchar();
}

void showLamp(const char* color, int duration) { //Simulates lamp display
    printf("\n");
    printf("*******************************************\n");
    printf("*   CURRENTLY LAMP IS: %-20s*\n", color);
    printf("*******************************************\n");
    
    SLEEP(duration);
}
