#include <iostream>
#include <ctype.h>

using namespace std;

//Declaring all Global Variables
int gameInput=0, fireHere = 0, j=0, turn_Count = 0, gridSize = 0, fireInput= 0, moveInput = 0;	
char mainInput;

bool shipMove=true;

enum ShipType {  // sets 5 different ship types
	CARRIER,
	BATTLESHIP,
	CRUISER,
	SUBMARINE,
	DESTROYER
};

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

class Ship 
{
protected:
	int health;
	ShipType type;
	int _lastFired;
	
public:
	// Get & Set for Health 
	int get_health() { return health; }
	void set_health(int h) { health = h; }
	
	ShipType get_type() { return type; }
	
	bool destroyed() { return health == 0; } // checks to see if the ship is destroyed
	
	//Get & Set for last turn fired Fired
	int lastFired() { return _lastFired; }
	void set_lastFired(int f) { _lastFired = f; }
	
	
	
	virtual int total_health() = 0;
	virtual bool can_attack(int turn) = 0;
};


// Battleship Class
class Battleship : public Ship {
public:
	Battleship() { type = BATTLESHIP; set_health(total_health()); set_lastFired(-1); }
	int total_health() { return 4; }
	bool can_attack(int turn) { return !destroyed(); }
	
} *_Battleship = NULL;

//Cruiser Class
class Cruiser : public Ship {
public:
	Cruiser() { type = CRUISER; set_health(total_health()); set_lastFired(-1); }
	int total_health() { return 3; }
	bool can_attack(int turn) { return !destroyed(); }
} *_Cruiser = NULL;


//Carrier Class
class Carrier : public Ship {
public:
	Carrier() { type = CARRIER; set_health(total_health()); set_lastFired(-1); }
	int total_health() { return 5; }
	bool can_attack(int turn) { return !destroyed(); }
} *_Carrier = NULL;


//Destroyer Class
class Destroyer : public Ship {
public:
	Destroyer() { type = DESTROYER; set_health(total_health()); set_lastFired(-1); }
	int total_health() { return 2; }
	bool can_attack(int turn) { return !destroyed(); }
} *_Destroyer = NULL;


//Submarine Class
class Submarine : public Ship {
public:
	Submarine() { type = SUBMARINE; set_health(total_health()); set_lastFired(-1); }
	int total_health() { return 2; }
	bool can_attack(int turn) { return !destroyed(); }
} *_Submarine = NULL;


// Forward Declarations //
void menu();
void main_menu(); 
void branching(char option);
void new_game();
void view_ranking();
void programStart();
void startMenu();
void battle_menu();
void fire_menu(void);
void battle_branch(char option);
void battleSwitch();
void skip();
void gameOver(void);
void victory(void);
void turnReset();
void turnCount();


/********************
 ////////////////////
 // grid class /////
 //////////////////
 ****************/

typedef enum { EMPTY, OCCUPIED, WRECKAGE } SQUARETYPE;
class playerGrid
{
private:
	int _Index;
	SQUARETYPE _Content;
	playerGrid* next;
	
public:
	//Constructor
	playerGrid(){}
	playerGrid(int Index, SQUARETYPE Content) {_Index = Index; _Content = Content; }
	
	void setContent(SQUARETYPE Content) { _Content = Content; }
	
	SQUARETYPE getContent() {return _Content;}
	
	playerGrid* getNext() {return next;}
	
	void setNext(playerGrid* nextThing) { next = nextThing; }
	
	int getIndex() {return _Index;}
	
	
} *pGridHead = NULL;




///////////////////
// main method ///
/////////////////

int main() {
	
	programStart(); // display title screen
	startMenu();	// display main menu
	return 0;
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

void main_menu() //displays the menu of options for the user
{
	cout << endl << endl << endl;
	cout << "\n\nMenu Options\n";
	cout << "------------------------------------------------------\n";
	cout << "n: New Game\n";
	cout << "v: View Ranking\n";
	cout << "q: Quit\n";
	cout << "\n\nPlease enter a choice (n, v, or q) ---> "; 
	
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
			cout << "Error: Invalid Input.  Please try again..." << endl; 
			startMenu(); // go back to Main Menu
			break;
	}
}

void new_game()
{
	// needs to have player set the grid sizes
	cout << "How big should the grid be?  Enter a number between 5 and 15: ";
	cin >> gridSize;
	
	for (int i = 0; i < gridSize; i++)
	{
		if (i == 0)
		{
			pGridHead = new playerGrid(i,EMPTY);
		}
		else
		{
			//pGridHead->setNext();
		}
	}
	
	_Battleship = new Battleship();
	_Cruiser = new Cruiser();
	_Carrier = new Carrier();
	_Destroyer = new Destroyer();
	_Submarine = new Submarine();
	
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
void shipMove_menu(void)
{	
	cout << "\nMove which Ship?\n";
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

void battleSwitch(){
	// case1: player wins the game
	//if (computer->lost()) {
	//	victory();
		//save player name
		// check to see if player got high score
	//}
	// case2: computer wins the game
	if (_Battleship->destroyed() && _Cruiser->destroyed() && _Carrier->destroyed() && _Destroyer->destroyed() && _Submarine->destroyed()) {
		gameOver();
		// print high scores!!
	}
	battle_menu();			//Print out the battle menu
	cin >> gameInput;		//obtain user's input
	switch (gameInput) {
		case 1:          // fire at enemy
			shipMove=true;  // sets the shipMove to true for the next turn
			fire_menu();			// fire menu options called
			cin >> fireInput;
			
			switch (fireInput) {
				case 1:
					//battleship
					if (_Battleship->destroyed()) { // checks to see if battleship is active
						cout << endl << endl << endl;
						cout << "Error: Battleship is Destroyed...";
						cout << endl << endl << endl;
						battleSwitch(); // back to battle menu
					}
					
					else {
						//fire logic
					}
					
					break;
				case 2:
					//cruiser
					if (_Cruiser->destroyed()) {  // checks to see if cruiser is active
						cout << endl << endl << endl;
						cout << "Error: Cruiser is Destroyed...";
						cout << endl << endl << endl;
						battleSwitch();	// back to battle menu
					}
					
					else {
						//fire logic
					}
					break;
					
				case 3:
					//carrier
					if (_Carrier->destroyed()) { // checks to see if carrier is active
						cout << endl << endl << endl;
						cout << "Error: Carrier is Destroyed...";
						cout << endl << endl << endl;
						battleSwitch(); // back to battle menu
					}
					
					else {
						//fire logic
					}
					break;
				case 4:
					//destroyer
					if (_Destroyer->destroyed()) { // checks to see if destroyer is active
						cout << endl << endl << endl;
						cout << "Error: Destroyer is Destroyed...";
						cout << endl << endl << endl;
						battleSwitch(); // back to battle menu
					}
					
					else {
						//fire logic
						
					}
					break;
				case 5:
					//submarine
					if (_Submarine->destroyed()) { // checks to see if submarine is active
						cout << endl << endl << endl;
						cout << "Error: Submarine is Destroyed...";
						cout << endl << endl << endl;
						battleSwitch(); // back to battle menu
					}
					
					else {
						//fire logic
					}
					break;
					
				default:
					cout <<	endl << endl << endl;
					cout << "Error! Must be a number between 1-5" << endl;
					cout <<	endl << endl << endl;
					fire_menu();	//prints the fire menu again
					break;
			}
			
			break;
			
		case 2:          // move ships
			if(shipMove){ // checks to see if ships have been moved this turn or not
				shipMove_menu();
				fire_menu();			// fire menu options called
				cin >> moveInput;
				switch (moveInput) {
					case 1:
						if (_Battleship->destroyed()) { // checks to see if battleship is active
							cout << endl << endl << endl;
							cout << "Error: Battleship is Destroyed...";
							cout << endl << endl << endl;
							battleSwitch(); // back to battle menu
						}
						
						else {
							
							//if battleship can move
							// move & print out
							shipMove = false; // ships can no longer move for this turn
							//else {
							//	cout << endl << endl << endl;
							//	cout << "Error! Cannot move to that location";
							//	cout << endl << endl << endl;
							//}
						}
						
						break;
					case 2:
						if (_Cruiser->destroyed()) {   // checks to see if cruiser is active
							cout << endl << endl << endl;
							cout << "Error: Cruiser is Destroyed...";
							cout << endl << endl << endl;
							battleSwitch(); // back to battle menu
						}
						
						else {
							
							//if cruiser can move
							// move & print out
							shipMove = false; // ships can no longer move for this turn
							//else {
							//	cout << endl << endl << endl;
							//	cout << "Error! Cannot move to that location";
							//	cout << endl << endl << endl;
							//}
						}
						
						break;
						
					case 3:
						if (_Carrier->destroyed()) { // tests to see if the carrier is active
							cout << endl << endl << endl;
							cout << "Error: Carrier is Destroyed...";
							cout << endl << endl << endl;
							battleSwitch();
						}
						
						else {
							
							//if carrier can move
							// move & print out
							shipMove = false; // ships can no longer move for this turn
							//else {
							//	cout << endl << endl << endl;
							//	cout << "Error! Cannot move to that location";
							//	cout << endl << endl << endl;
							//}
							
						}
						
						break;
					case 4:
						if (_Destroyer->destroyed()) {  // checks to see if destroyer is active
							cout << endl << endl << endl;
							cout << "Error: Destroyer is Destroyed...";
							cout << endl << endl << endl;
							battleSwitch(); // back to battle menu
						}
						
						else {
							
							//if destroyer can move
							// move & print out
							shipMove = false; // ships can no longer move for this turn
							//else {
							//	cout << endl << endl << endl;
							//	cout << "Error! Cannot move to that location";
							//	cout << endl << endl << endl;
							//}
							
						}
						
						break;
					case 5:
						if (_Submarine->destroyed()) {  // checks to see if submarine is active
							cout << endl << endl << endl;
							cout << "Error: Submarine is Destroyed...";
							cout << endl << endl << endl;
							battleSwitch(); // back to Battle Menu
						}
						
						else {
							//if submarine can move
							// move & print out
							shipMove = false; // ships can no longer move for this turn
							//else {
							//	cout << endl << endl << endl;
							//	cout << "Error! Cannot move to that location";
							//	cout << endl << endl << endl;
							//}
							
						}
						break;
					default:
						cout <<	endl << endl << endl;
						cout << "Error! Must be a number between 1-5" << endl;
						cout <<	endl << endl << endl;
						shipMove_menu();	//prints the fire menu again
						break;
				}
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
			delete _Battleship;
			_Battleship = NULL;
			delete _Cruiser;
			_Cruiser = NULL;
			delete _Carrier;
			_Carrier = NULL;
			delete _Destroyer;
			_Destroyer = NULL;
			delete _Submarine;
			_Submarine = NULL;
			
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

void turnCount(){	//Increment the turn by 1.
	turn_Count++;
}

void turnReset(){	//Increment the turn by 1.
	turn_Count = 1;
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


///////////////////////////
// View Ranking methods //
/////////////////////////

void load_file()
{ 	// load scores from file
	FILE *filename;
	char name[80];
	// int ranking or score
	
	
	filename = fopen("Ranking.txt", "rb");
	if(filename != NULL)
	{
		while(fread(name, sizeof(name), 1, filename) == 1)
		{
			//fread(&ranking/score, sizeof(int), 1, filename);
			
		//	insert(name, ranking);
		}
		fclose(filename);
	}
	else
	{
		cout << "ERROR: Unable to load file" << endl;
	}
}

void save_file()
{
	FILE *filename;
	// new Player_Ranking
	
	
}



class Computer {

	void display_stats(){
	cout << endl << endl << endl;
		cout << "Ship\t\t     Health" << endl;
		cout << "---------------------------" << endl;
		cout << "Battleship\t\t" << battleship->get_health() << "/" << battleship->get_max_health() << endl;
		cout << "Carrier\t\t\t" << carrier->get_health() << "/" << carrier->get_max_health() << endl;
		cout << "Cruiser\t\t\t" << cruiser->get_health() << "/" << cruiser->get_max_health() << endl;
		cout << "Destroyer\t\t" << destroyer->get_health() << "/" << destroyer->get_max_health() << endl;
		cout << "Submarine\t\t" << submarine->get_health() << "/" << submarine->get_max_health() << endl;
	}
	private{
	
	Battleship* cbattleship;
	Carrier* ccarrier;
	Cruiser* ccruiser;
	Destroyer* cdestroyer;
	Submarine* csubmarine;
}
	
}