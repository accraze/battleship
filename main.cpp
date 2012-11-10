#include <iostream>
#include <ctype.h>

using namespace std;

//Declaring all Global Variables
int gameInput=0, fireHere = 0, j=0, turn_Count = 0, gridSize = 0, fireInput= 0, moveInput = 0;	
char mainInput, rank_input, player_name[1024];

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

struct PlayerRanking{
	char player_name[1024];
	int turn_count;
	PlayerRanking *next;
} *HEAD = NULL;

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
void ranking_menu();
void ranking_branch(char option);
void printall();
int insert(char *player_name, int turn_count);
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

//////////////////////////
//Computer (AI) class////
////////////////////////

class Computer {
	
	
	
	Battleship* cbattleship;
	Carrier* ccarrier;
	Cruiser* ccruiser;
	Destroyer* cdestroyer;
	Submarine* csubmarine;
	
public:
	Computer(){
		
		this->cbattleship = new Battleship();
		this->ccarrier = new Carrier();
		this->ccruiser = new Cruiser();
		this->cdestroyer = new Destroyer();
		this->csubmarine = new Submarine();
	}
	
	~Computer() {
		delete cbattleship;
		delete ccarrier;
		delete ccruiser;
		delete cdestroyer;
		delete csubmarine;
	}
	
	bool lost(){
		if(cbattleship->destroyed() && ccarrier->destroyed() && ccruiser->destroyed() && cdestroyer->destroyed() && csubmarine->destroyed())
		{return true;}
		else {return false;}
	}
	
	void display_stats(){
		cout << endl << endl << endl;
		cout << "ENEMY'S SHIPS";
		cout << endl;
		cout << "Ship\t\t     Health" << endl;
		cout << "---------------------------" << endl;
		cout << "Battleship\t\t" << cbattleship->get_health() << "/" << cbattleship->total_health() << endl;
		cout << "Carrier\t\t" << ccarrier->get_health() << "/" << ccarrier->total_health() << endl;
		cout << "Cruiser\t\t" << ccruiser->get_health() << "/" << ccruiser->total_health() << endl;
		cout << "Destroyer\t\t" << cdestroyer->get_health() << "/" << cdestroyer->total_health() << endl;
		cout << "Submarine\t\t" << csubmarine->get_health() << "/" << csubmarine->total_health() << endl;
	}
	
	
	
}*_computer = NULL;


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
	// get player name
	cout << "Please enter your name: " ;
	cin >> player_name;
	
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
	_computer = new Computer();
	
	turnReset();			//Set the turn counter back to 1.
	battleSwitch();			//Go to the game menu
}

void view_ranking()
{
	// This needs to read from file and display 10 highest scores in the game
	rank_input;
	ranking_menu();
	cin >> rank_input;
	ranking_branch(tolower(rank_input));
	
}

void ranking_menu()
{
	cout << endl << "Menu Options" << endl;
	cout << "------------------------------------------------------" << endl;
	cout << "p: Review your list" << endl; 
	cout << "q: Save and quit" << endl;
	cout << "r: return to main menu" << endl;
	cout << endl << endl << "Please enter a choice (p or q) ---> "; 
}

void ranking_branch(char option)
{
	char player_name[1024];
	int turn_count;
	
	switch(option)
	{
		case 'p':
			printall();
			break;
			
		case 'q':
			// TODO: Add code to save data into a file
			//save_file();
			break;
			
		case 'r':
			startMenu();
			break;
			
		default:
			cout << endl << "Error: Invalid Input.  Please try again..."; 
			break;
	}
}


int insert(char *player_name, int turn_count)
{
	struct PlayerRanking *new_node, *iter_node;
	new_node = new PlayerRanking;
	
	if(new_node == 0){
		cout << "Out of memory" << endl;
		return -1;
	}
	
	strcpy(new_node->player_name, player_name);
	new_node->turn_count = turn_count;
	
	iter_node = HEAD;  // assign temp to head
	
	if ((HEAD == NULL)||(new_node->turn_count < iter_node->turn_count)){
		new_node->next = HEAD;
		HEAD = new_node;
	}
	else
	{
		while (iter_node->next != NULL){
			if(new_node->turn_count < iter_node->turn_count){
				new_node->next = iter_node->next;
				iter_node->next = new_node;
				return 0;
			}
			else
				iter_node = iter_node->next;
		}
		new_node->next = NULL;
		iter_node->next = new_node;
	}
	return 0;
}

void printall()
{
	struct PlayerRanking *iter_node = HEAD;
	
	cout << endl << "Rankings: " << endl;
	
	while(iter_node != NULL)
	{
		cout << endl << "Player Name: " << iter_node->player_name << endl;
		cout << endl << "Score: " << iter_node->turn_count << endl;
		cout << endl;
		
		iter_node = iter_node->next;
	}
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
	cin >> player_name;
	cin >> turn_Count;
	insert(player_name, turn_Count);
	
	// check to see if player got high score
	//}
	// case2: computer wins the game
	if (_Battleship->destroyed() && _Cruiser->destroyed() && _Carrier->destroyed() && _Destroyer->destroyed() && _Submarine->destroyed()) {
		gameOver();
		// print high scores!!
		printall();
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
			cout << endl << endl << endl;
			cout << "PLAYER'S SHIPS";
			cout << endl;
			cout << "Ship\t\t     Health" << endl;
			cout << "---------------------------" << endl;
			cout << "Battleship\t\t" << _Battleship->get_health() << "/" << _Battleship->total_health() << endl;
			cout << "Carrier\t\t" << _Carrier->get_health() << "/" << _Carrier->total_health() << endl;
			cout << "Cruiser\t\t" << _Cruiser->get_health() << "/" << _Cruiser->total_health() << endl;
			cout << "Destroyer\t\t" << _Destroyer->get_health() << "/" << _Destroyer->total_health() << endl;
			cout << "Submarine\t\t" << _Submarine->get_health() << "/" << _Submarine->total_health() << endl;
			battleSwitch(); // Go Back to Battle Menu...
			break;
			
		case 4:          // display the computers ships/health
			
			_computer->display_stats();
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
			cout << endl << endl << endl;
			cout << "Error: Invalid Input";
			cout << endl << endl << endl;
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

