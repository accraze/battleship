#include <iostream>
#include <ctype.h>

using namespace std;

//Declaring all Global Variables
int turnCount=1, gameInput=0, fireHere = 0, j=0;	
char mainInput, fireInput;
Battleship* pShip1 = NULL;
Destroyer* pShip2 = NULL;
Cruiser* pShip3 = NULL;
Carrier* pShip4 = NULL;
Submarine* pShip5 = NULL;
bool shipMove=true;

enum ShipType {  // sets 5 different ship types
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
void fire_menu(void);
void battle_branch(char option);
void skip();
void gameOver(void);
void victory(void);
void turnReset(void);
void turnCount(void);



void main() {
	
	programStart();
	startMenu();	

}

void main_menu() //menu displays the menu of options for the user
{
	cout << endl << endl << endl;
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
			
		case 'q':			// quit entire game
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
	pShip1 = new Battleship();
	pShip2 = new Cruiser();
	pShip3 = new Carrier();
	pShip4 = new Destroyer();
	pShip5 = new Submarine();
	
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
	cout << "\n\nEnter a choice (1, 2, 3, 4, 5 or 6) ---> "; 
	
}

void fire_menu(void)
{	
	cout << "\nWhich Ship to Fire with?\n";
	cout << "------------------------------------------------------\n";
	cout << "1: Battleship\n";
	cout << "2: Cruiser\n";
	cout << "3: Carrier\n";
	cout << "4: Destroyer\n";
	cout << "5: Submarine\n";
	cout << "\n\nEnter a choice (1, 2, 3, 4, or 5) ---> ";
	
}

//////////////////////////////////////
/////////////////////////////////////
// THIS IS THE MAIN BATTLE LOOP/////
///////////////////////////////////
//////////////////////////////////

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
	battle_menu();			//Print out the battle menu
	cin >> gameInput;		//obtain user's input
	switch (gameInput) {
		case 1:          // fire at enemy
			shipMove=true;
			// fill in firing logic here
			fire();
			cin >> fireInput;
				
				switch (fireInput) {
					case 1:
						//battleship
						break;
					case 2:
						//cruiser
						break;
						
					case 3:
						//carrier
						break;
					case 4:
						//destroyer
						break;
					case 5:
						//submarine
						break;
						
					default:
						cout << "Error! Must be a number between 1-5" << endl;
						fireMenu();	//prints the fire menu again
						break;
				}
			
			break;
			
		case 2:          // move ships
			if(shipMove){ // checks to see if ship has been moved this turn or not
			// fill in moving logic
			}
			
			else {
				cout << endl << endl << endl;
				cout << "Error! Ship cannot be moved this turn";
				cout << endl << endl << endl;
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
			delete pShip1;
			pShip1 = NULL;
			delete pShip2;
			pShip2 = NULL;
			delete pShip3;
			pShip3 = NULL;
			delete pShip4;
			pShip4 = NULL;
			delete pShip5;
			pShip5 = NULL;
			
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

void victory(void){  // method called when the player wins the game
	cout << endl << endl << endl;
	cout << "Hurray! You sunk all of the enemy's Battleships";
	cout << endl << endl << endl;
	// add player name to high score///
	//print high scores
	startMenu();
	
}

void gameOver(void){  // method called when the computer wins the game
	cout << endl << endl << endl;
	cout << "Wah Wah! You suck!";
	cout << endl << endl << endl;
	startMenu();
	
}


//======================================================
//
///___  _   _  ____  ____  ___ 
/// __)( )_( )(_  _)(  _ \/ __)
//\__ \ ) _ (  _)(_  )___/\__ \
//(___/(_) (_)(____)(__)  (___/
//
// contains: ship class, battleship class, cruiser class,
//           destroyer, submarine and carrier classes.
//======================================================

class Ship {
public:
	// Get & Set for Health 
	int get_health() { return health; }
	void set_health(int h) { health = h; }
	
	ShipType get_type() { return type; }
	
	bool destroyed() { return health == 0; } // checks to see if the ship is destroyed
	
	//Get & Set for last turn fired Fired
	int lastFired() { return lastFired; }
	void set_lastFired(int f) { lastFired = f; }
	
	
	
	virtual int total_health() = 0;
	virtual bool can_attack(int turn) = 0;
	
	
protected:
	
	
	int health;
	ShipType type;
	int lastFired;
}


// Battleship Class
class Battleship : public Ship {
public:
	Battleship() { type = BATTLESHIP; set_health(total_health()); set_lastFired(-1); }
	int total_health() { return 4; }
	bool can_attack(int turn) { return !destroyed(); }
	
}

//Cruiser Class
class Cruiser : public Ship {
public:
	Cruiser() { type = CRUISER; set_health(total_health()); set_lastFired(-1); }
	int total_health() { return 3; }
	bool can_attack(int turn) { return !destroyed(); }
}


//Carrier Class
class Carrier : public Ship {
public:
	Carrier() { type = CARRIER; set_health(total_health()); set_lastFired(-1); }
	int total_health() { return 5; }
	bool can_attack(int turn) { return !destroyed(); }
}


//Destroyer Class
class Destroyer : public Ship {
public:
	Destroyer() { type = DESTROYER; set_health(total_health()); set_lastFired(-1); }
	int total_health() { return 2; }
	bool can_attack(int turn) { return !destroyed(); }
}


//Submarine Class
class Submarine : public Ship {
public:
	Submarine() { type = SUBMARINE; set_health(total_health()); set_lastFired(-1); }
	int total_health() { return 2; }
	bool can_attack(int turn) { return !destroyed(); }
}
