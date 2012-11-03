#include <iostream>
#include <ctype.h>

using namespace std;

//Declaring all Global Variables
int turnCount=1, gameInput=0, fireHere = 0, j=0;	
char mainInput;
Battleship* pShip1 = NULL;
Destroyer* pShip2 = NULL;
Cruiser* pShip3 = NULL;
Carrier* pShip4 = NULL;
Submarine* pShip5 = NULL;
bool shipMove=true;

enum ShipType {
	CARRIER,
	BATTLESHIP,
	CRUISER,
	SUBMARINE,
	DESTROYER
};


// Forward Declarations //
void menu(); 
void branching(char option);
void new_game();
void view_ranking();
void programStart();
void battle_menu();
void battle_branch(char option);
void skip();



void main() {
	char ch; 
	
	programStart();
	startMenu();	

}

void main_menu()		//menu displays the menu of options for the user
{
	cout << "\n\nMenu Options\n";
	cout << "------------------------------------------------------\n";
	cout << "n: New Game\n";
	cout << "v: View Ranking\n";
	cout << "q: Quit\n";
	cout << "\n\nPlease enter a choice (n, v, or q) ---> "; 
}

void programStart()
{  cout << "***********************************************************************"<<endl;
    cout << "* ____       _______ _______ _      ______  _____ _    _ _____ _____  *"<<endl;
    cout << "*|  _ \\   /\\|__   __|__   __| |    |  ____|/ ____| |  | |_   _|  __ \\ *"<<endl;
    cout << "*| |_) | /  \\  | |     | |  | |    | |__  | (___ | |__| | | | | |__) |*"<<endl;
    cout << "*|  _ < / /\\ \\ | |     | |  | |    |  __|  \\___ \\|  __  | | | |  ___/ *"<<endl;
    cout << "*| |_) / ____ \\| |     | |  | |____| |____ ____) | |  | |_| |_| |     *"<<endl;
    cout << "*|____/_/    \\_\\_|     |_|  |______|______|_____/|_|  |_|_____|_|     *"<<endl;
    cout << "***********************************************************************"<<endl;
    cout << ""<< endl;
    cout << "     '            '            .    .     .                  __       " << endl;
    cout << "        \\  '  /          .                       .          /  \\      " << endl;
    cout << "   <          >   / .                               .       | 0|      " << endl;
    cout << "      \\   \\    /       >                          ___\\\\__\\\\_|  |__//_ " << endl;
    cout << "      '  \\   /     /                              \\_________________/ " << endl;
}

void startMenu(void) {

	mainInput = 0;
	main_menu();
	cin>>mainInput;
	branching(tolower(mainInput));

}

void branching(char option)		
{
	switch(option)
	{
		case 'n':
			new_game();	// start a new game
			break;
			
		case 'v':
			view_ranking();   // view high scores
			break;
			
		case 'q':			// quit game
			break;
			
		default:
			printf("\nError: Invalid Input.  Please try again..."); 
			startMenu(); // go back to Main Menu
			break;
	}
}

void new_game()
{
	// needs to have player set the grid sizes
	// needs to have player place battleships on the grid
	turnReset();			//Set the turn counter back to 1.
	battleSwitch();			//Go to the game menu
}

void view_ranking()
{
	// This needs to read from file and display 10 highest scores in the game 
}


void battle_menu()
{
	cout << "\nBattle Options\n";
	cout << "------------------------------------------------------\n";
	cout << "1: Fire!\n";
	cout << "2: Change Battleship location\n";
	cout << "3: Display remaining Battleships\n";
	cout << "4: Display Enemy's remaining Battleships\n";
	cout << "5: Skip Turn\n";
	cout << "6: Quit\n";
	cout << "\n\nPlease enter a choice (1, 2, 3, 4, 5 or 6) ---> "; 
	
}


void battleSwitch(void){
	// case1: player wins the game
	if (computer->lost()) {
		victory();
	  //save player name
	  // check to see if player got high score
	}
	// case2: computer wins the game
	if (ship1->destroyed() && ship2->destroyed() && ship3->destroyed() && ship4->destroyed() && ship5->destroyed()) {
		gameOver();
		// print high scores!!
	}
	
	if()
	
	battle_menu();			//Print out the battle menu
	cin >> gameInput;		//obtain user's input
	switch (gameInput) {
		case 1:          // fire at enemy
			shipMove=true;
			// fill in firing logic here
			break;
			
		case 2:          // move ships
			if(shipMovie){ // checks to see if ship has been moved this turn or not
			// fill in moving logic
			}
			break;
			
		case 3:          // display the player's ships/health
			// print out the players remaining ships
			// print out the players remaing health etc...
			battleSwitch(); // Go Back to Battle Menu...
			break;
			
		case 4:          // display the computers ships/health
			// print out the computers remaining ships
			battleSwitch();// Go Back to Battle Menu...
			break;	
			
		case 5:          // skip players turn
			skip();
			turnCount(); // turn counter increment
			// computers turn
			battleSwitch();// Go Back to Battle Menu...
			break;		
			
		case 6:          // quit the game
			// DELETE ALL OBJECTS!!!!!
			
			break;
			
		default:
			battleSwitch();    // if 1-6 not entered, Go Back To Battle Menu!
			break;
	}
}

void skip(){
// skip turn
	cout << endl << endl << endl;
	cout << "Skipping Player's Turn";
	cout << endl << endl << endl;
}

void turnCount(void){	//Increment the turn by 1.
	turnCount++;
}

void turnReset(void){	//Increment the turn by 1.
	turnCount = 1;
}

void victory(void){
	cout << "Hurray! You sunk all of the enemy's Battleships";
	startMenu();
	
}

void gameOver(void){
	cout << "Wah Wah! You suck!";
	startMenu();
	
}


