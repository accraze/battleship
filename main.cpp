#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <sstream>
#include <assert.h>

using namespace std;

//Declaring all Global Variables
int gameInput=0, fireHere = 0, j=0, turn_Count = 0, gridSize = 0, fireInput= 0, moveInput = 0, moveLocation =0;	
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
	int turn_Count;
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
	
	// debugging function to print out ship info
	void print_ship() {
		string ship;
		stringstream concat;
		switch(get_type()) {
			case CARRIER: ship += "Carrier "; break;
			case BATTLESHIP: ship += "Battleship "; break;
			case CRUISER: ship += "Cruiser "; break;
			case SUBMARINE: ship += "Submarine "; break;
			case DESTROYER: ship += "Destroyer "; break;
			default: ship += "Unknown Ship "; break;
		}
		
		concat.str(""); concat << get_health();
		ship += concat.str();
		ship += "/";
		concat.str(""); concat << total_health();
		ship += concat.str();
		
		ship += " ";
		cout << ship;
	}
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
	bool can_attack(int turn) { return !destroyed() && (turn_Count - lastFired() >= 2); } // checks to make sure that sub can only fire every other turn
} *_Submarine = NULL;



/********************
 /////////////////////
 /// grid class /////
 ///////////////////
 //****************/

typedef enum { EMPTY, OCCUPIED, WRECKAGE } SQUARETYPE;
struct square {  // struct for each individual square in the grid
	int _Index;
	SQUARETYPE _Content;
	Ship* ship;
	square* prev;
	square* next;
};

class playerGrid
{
	
	
private:
	int total_size;
	square* _first; // points to the front of the squares
	square* _last;	// points to the last of the squares
	
public:
	int get_total_size() { return total_size; }
	square* get_first() { return _first; }
	square* get_last() { return _last; }
	
	//*************
	//Constructor
	//*************
	playerGrid(int total_size) {
		this->total_size = total_size;	// sets the total_size of the grid
		this->_first = NULL;			// initializes firt square pointer 'head'
		this->_last = NULL;				// initializes last square pointer 'tail'
	}
	
	//************
	//destructor
	//************
	~playerGrid(){  // deletes each individual square in the playerGrid
		square* temp1 = this->_first;  // start at the beginning
		while(temp1 != NULL) {
			square* temp2 = temp1->next; // get the next square
			delete temp1;   // delete current square
			temp1 = temp2;  // swap the next square to current square
		}
	}
	
	//******************************************************
	// this method finds the index before the desired square
	//******************************************************
	square* index_before_square(int index) {
		square* temp = this->_first;				// creates 
		while(temp != NULL && temp->next != NULL) { // iterates through linked list
			if(temp->next->_Index >= index)
				return (temp->_Index < index ? temp : NULL);
			
			temp = temp->next;
		}
		
		if(temp == NULL) return NULL;  // returns NULL if head == NULL or no grid is created
		
		return (temp->_Index < index ? temp : NULL);  // returns either 
	}
	
	//*****************************************
	//removes a square and returns the location
	//*****************************************
	square* remove_square(int index) {
		
		square* prev = index_before_square(index);
		square* old_location = (prev == NULL) ? this->_first : prev->next;
		
		if(old_location == NULL || old_location->_Index != index)
			return NULL;
		
		assert(old_location->_Index == index);
		
		// Actually delete this location.
		
		if(old_location->next != NULL) old_location->next->prev = prev;
		if(old_location->prev != NULL) old_location->prev->next = old_location->next;
		
		if(this->_first == old_location) _first = old_location->next;
		if(this->_last == old_location) _last = old_location->prev;
		
		return old_location;
	}
	
	//**********************************
	//removes a ship from the grid
	//**********************************
	Ship* remove_ship(int location) {
		square* temp = remove_square(location);
		
		if(temp == NULL) return NULL;
		
		Ship* old_ship = temp->ship;
		temp->ship = NULL;
		delete temp;
		
		return old_ship;
	}
	
	//********************************************
	//returns a ship object at requested location
	//********************************************
	Ship* get_square_ship(int index){
		
		square* iter = this->_first; // start at the top of the linked list
		
		//traverse linked list until the correct square is found
		while(iter != NULL) {
			if (iter->_Index == index) {   // iterates through the grid list until it find the correct index
				return iter->ship;  // return the ship
			}
			else {
				iter = iter->next;
			}
		}	
		return NULL;
	}
	
	//*************************************************************************
	// checks to see if a specific square is occupied
	//*************************************************************************
	bool squareOccupied(int index){  // checks to see if a square is occupied
		square* previousSquare = index_before_square(index);
		
		//makes sure there is a previous square
		if(previousSquare != NULL && previousSquare->next != NULL && previousSquare->next->_Index == index) 
			return true;
		
		// checks if there is only one square
		if(previousSquare == NULL && _first != NULL && _first->_Index == index)
			return true;
		
		return false;	
	}
	
	//*********************************
	//sets a square content to wreckage
	//*********************************
	bool set_wreckage (int index){
		square* iter = this->_first; // start at the top of the linked list
		
		//traverse linked list until the correct square is found
		while(iter != NULL) {
			if (iter->_Index == index) {   // iterates through the grid list until it find the correct index
				iter->_Content = WRECKAGE; // sets the square to type wreckage.
				return true;
			}
			else {
				iter = iter->next;
			}
		}
		return false;
	}
	
	
	//*************************************
	//checks to see if a square is wreckage
	//*************************************
	bool is_wreckage (int index){
		square* iter = this->_first; // start at the top of the linked list
		
		//traverse linked list until the correct square is found
		while(iter != NULL) {
			if (iter->_Index == index) {   // iterates through the grid list until it find the correct index
				if(iter->_Content == WRECKAGE) // checks to see if the square is wreckage...
				{return true;}
			}
			else {
				iter = iter->next;
			}
		}
		return false;
	}
	
	//*******************************
	// lets player set ship onto grid
	//*******************************
	bool set_ship(int index, Ship* ship) {
		
		// checks to see if entered location is out of bounds
		if(index >= this->total_size){
			cout << endl << endl;
			cout << "Error! Value is out of Grid Boundary";
		    cout << endl << endl;
			return false; 
		}
		
		//checks to see if the entered location is occupied
		if(squareOccupied(index)){
			cout << endl << endl;
			cout << "Error! Grid Square is already Occupied";
		    cout << endl << endl;
			return false;
		}
		square* previousSquare = index_before_square(index);
		
		// creates a new 'square' and fills in all of it's properties
		square* newSquare = new square();
		newSquare->next = NULL;			// sets next to NULL
		newSquare->prev = NULL;			// sets prev to NULL
		newSquare->_Index = index;		// gives the correct grid index to the new square
		newSquare->ship = ship;         // places ship on square
		newSquare->_Content = OCCUPIED; // sets square to OCCUPIED
		
		newSquare->prev = previousSquare;  // insert square before previous 
		newSquare->next = (previousSquare != NULL ? previousSquare->next : this->_first);
		
		//re-aligns the prev and next pointers within the new square if they arent NULL
		if(newSquare->prev != NULL) newSquare->prev->next = newSquare;
		if(newSquare->next != NULL) newSquare->next->prev = newSquare;
		
		// if the newSquare is the first occupied square then set _first to point to the new square
		if( _first == NULL) 
			_first = newSquare;
		// if the newSquare is the first occupied square then set _last to point to the new square
		if(_last == NULL) 
			_last = newSquare;
		
		// rewinds 'head' to the beginning of the linked list
		while(_first->prev != NULL) _first = _first->prev;
		
		//forwards 'tail' to the end of the linked list
		while(_last->next  != NULL) _last = _last->next;
		return true;
	}
	
	//**********************************************************************
	//MOVE_SHIP: deletes ship from current square and sets it on a new square
	//**********************************************************************
	bool move_ship(int index, Ship* ship){
		
		// find the current location of ship
		square* iter = this->_first; // start at the top of the linked list
		int current_square = -1;
		
		//traverse linked list until the correct square is found
		while(iter != NULL) {
			if(iter->ship == ship)	// if the ships match......
				current_square = iter->_Index; // gets the current index of the ship
			
			iter = iter->next;	
		}
		if (current_square == -1) {
			return false;
		}
		square* unit = remove_square(current_square);
		if(unit == NULL) return false;
		bool return_value = true;
		if(!set_ship(index, unit->ship)) {
			bool set_old = set_ship(current_square, unit->ship);
			assert(set_old); // we just removed this successfully, if we can't add it again, something is wrong
			return_value = false;
		}
		
		delete unit;
		
		return return_value;
	}
	
	//*****************************************
	//Debugging function for print out the grid
	//*****************************************
	void print_grid() {
		square* last = this->_first;
		while(last != NULL) {
			cout << last->_Index << ": ";
			if(last->ship == NULL)
				cout << "no ship";
			else
				last->ship->print_ship();
			cout << endl;
			last = last->next;
		}
	}
	
} *_grid1 = NULL;
playerGrid *_grid2 = NULL;




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
void load_file();
void save_file();
int insert(char *player_name, int turn_Count);
void skip();
void gameOver(void);
void victory(void);
void turnReset();
void turnCount();
void playerGridSetup();
void fire();
void subfire();
void csubfire();
void cfire();
void computer_turn();
string whichShip(int ship);
void ship_destruct(playerGrid* ship_grid, int location, Ship* destroyed_ship);


//////////////////////////
//Computer (AI) class////
////////////////////////

class Computer {
	
	
	
	Battleship* cbattleship;
	Carrier* ccarrier;
	Cruiser* ccruiser;
	Destroyer* cdestroyer;
	Submarine* csubmarine;
	bool can_move;
	
	
public:
	Computer(){
		
		this->cbattleship = new Battleship();
		this->ccarrier = new Carrier();
		this->ccruiser = new Cruiser();
		this->cdestroyer = new Destroyer();
		this->csubmarine = new Submarine();
		this->can_move = true;  // lets computer know it can move
		
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
		cout << "Battleship\t\t\t" << cbattleship->get_health() << "/" << cbattleship->total_health() << endl;
		cout << "Carrier\t\t\t" << ccarrier->get_health() << "/" << ccarrier->total_health() << endl;
		cout << "Cruiser\t\t\t" << ccruiser->get_health() << "/" << ccruiser->total_health() << endl;
		cout << "Destroyer\t\t\t" << cdestroyer->get_health() << "/" << cdestroyer->total_health() << endl;
		cout << "Submarine\t\t\t" << csubmarine->get_health() << "/" << csubmarine->total_health() << endl;
	}
	
	void grid_Setup(){
		int gridSize1 = gridSize - 1;
		int battleship, cruiser, carrier, destroyer, submarine;
		// sets battleship location to a random number between 0-GRIDSIZE
		battleship = rand() % gridSize1;
		
		do {
			// sets cruiser location to a random number between 0-GRIDSIZE
			cruiser = rand() % gridSize1;
		} while (cruiser == battleship);
		// checks to make sure the random location value is not the same as any of the other locations
		
		do {
			// sets carrier location to a random number between 0-GRIDSIZE
			carrier = rand() % gridSize1;
		} while (carrier == battleship || carrier == cruiser);
		// checks to make sure the random location value is not the same as any of the other locations
		
		do {
			// sets destroyer location to a random number between 0-GRIDSIZE
			destroyer = rand() % gridSize1;
		} while (destroyer == battleship || destroyer == cruiser || destroyer == carrier);
		// checks to make sure the random location value is not the same as any of the other locations
		
		do {
			// sets submarine location to a random number between 0-GRIDSIZE
			submarine = rand() % gridSize1;
		} while (submarine == battleship || submarine == cruiser || submarine == carrier || submarine == destroyer);
		// checks to make sure the random location value is not the same as any of the other locations
		
		_grid2->set_ship(battleship, cbattleship);  // sets battleship on computer grid
		_grid2->set_ship(cruiser, ccruiser);		// sets cruiser on computer grid
		_grid2->set_ship(carrier, ccarrier);		// sets carrier on computer grid
		_grid2->set_ship(destroyer, cdestroyer);	// sets destroyer on computer grid
		_grid2->set_ship(submarine, csubmarine);			// sets submarine on computer grid
		
		cout << endl << endl;
		cout << "The Computer's grid is set...";
		cout << endl << endl;
		
	}
	
	//*******************
	//COMPUTER's AI LOGIC
	//*******************
	void computer_turn(){
		cout << endl << "The Computer is taking its turn now" << endl;
		// randomly chooses between fire, move ship and skip turn (1-3)
		int choice = rand() % 3 + 1;
		
		switch(choice)
		{
			case 1:  // fire!
				if (!csubmarine->destroyed() && csubmarine->can_attack(turn_Count)) { // checks to make sure the sub has not fired in the last turn
					// fire away
					csubfire();
					csubmarine->set_lastFired(turn_Count);
					can_move = true;
				}	
				else if (!cdestroyer->destroyed()){
					// fire away
					cfire();
					can_move = true;
				}
				else if(!ccarrier->destroyed()) {
					// fire away
					cfire();
					can_move = true;
				}
				else if (!ccruiser->destroyed()){
					// fire away
					cfire();
					can_move = true;
				}
				else{
					// fire battleship
					cfire();
					can_move = true;
				}
				break;
				
				
			case 2: // move a ship
				if (can_move) {  // checks to see if the computer has already moved this turn
					int gridSize1 = gridSize - 1;
					moveLocation = rand() % gridSize1 ;  // get a random move location  (0 - gridSize)
					
					if (!csubmarine->destroyed()) {
						_grid2->move_ship(moveLocation,csubmarine); // move submarine
						can_move = false;
					}	
					else if (!cdestroyer->destroyed()){
						_grid2->move_ship(moveLocation,cdestroyer); // move destroyer
						can_move = false;
					}
					else if(!ccarrier->destroyed()) {
						_grid2->move_ship(moveLocation,ccarrier); // move carrier 
						can_move = false;
					}
					else if (!ccruiser->destroyed()){
						_grid2->move_ship(moveLocation,ccruiser); // move cruiser
						can_move = false;
					}
					else{
						_grid2->move_ship(moveLocation,cbattleship); // move battleship
						can_move = false;
					}
					computer_turn();	// back to computer turn
					
				}
				break;
				
			case 3:			// skip the computer's turn
				cout << endl;
				cout << "The Computer is skipping it's turn: Your Move!";
				cout << endl;
				break;
				
			default:
				cout << endl << "Computer is still thinking...." << endl; 
				computer_turn(); // make the computer decide what to do again.
				break;
		}
		
		
	}
	
	
	
}*_computer = NULL;





////////////////////
// main method ////
//////////////////

int main() {
	load_file();
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
			// make sure all objects are deleted!!
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
			delete _grid1;
			_grid1 = NULL;
			delete _grid2;
			_grid2 = NULL;
			delete _computer;
			_computer = NULL;
			exit (1);	// exits the game
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
	cout << "How big should the grid be?  Enter a number between 10 and 50: ";
	cin >> gridSize;
	while(gridSize < 10 || gridSize > 50){		//Checks if the input is in bounds
		
		cout << "ERROR: Invalid Input (Value out of bounds)" << endl << endl << endl;
		cout << "How big should the grid be?  Enter a number between 10 and 50: " << endl;
		cin  >> gridSize;
	}
	
	//Creates two new grid objetcs
	_grid1 = new playerGrid(gridSize);	// grid 1 is player grid
	_grid2 = new playerGrid(gridSize);	// grid 2 is computer grid
	
	//declare all player's ships
	_Battleship = new Battleship();
	_Cruiser = new Cruiser();
	_Carrier = new Carrier();
	_Destroyer = new Destroyer();
	_Submarine = new Submarine();
	
	//create enemy computer
	_computer = new Computer();
	
	//set the player's ships on grid
	playerGridSetup();
	
	//set the computer's ships on grid
	_computer->grid_Setup();
	
	turnReset();			//Set the turn counter back to 1.
	battleSwitch();			//Go to the battle menu
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
	cout << endl << "Ranking Menu Options" << endl;
	cout << "------------------------------------------------------" << endl;
	cout << "p: Review your list" << endl; 
	cout << "q: Save and quit" << endl;
	cout << "r: return to main menu" << endl;
	cout << endl << endl << "Please enter a choice (p, q, or r) ---> "; 
}

void ranking_branch(char option)
{
	
	switch(option)
	{
		case 'p':
			printall();
			break;
			
		case 'q':
			// TODO: Add code to save data into a file
			save_file();
			break;
			
		case 'r':
			programStart(); // title screen
			startMenu();   // back to main menu
			break;
			
		default:
			cout << endl << "Error: Invalid Input.  Please try again..."; 
			break;
	}
}


int insert(char *player_name, int turn_Count)
{
	struct PlayerRanking *new_node, *iter_node;
	new_node = new PlayerRanking;
	
	if(new_node == 0){
		cout << "Out of memory" << endl;
		return -1;
	}
	
	strcpy(new_node->player_name, player_name);
	new_node->turn_Count = turn_Count;
	
	iter_node = HEAD;  // assign temp to head
	
	if ((HEAD == NULL)||(new_node->turn_Count < iter_node->turn_Count)){
		new_node->next = HEAD;
		HEAD = new_node;
	}
	else
	{
		while (iter_node->next != NULL){
			if(new_node->turn_Count < iter_node->next->turn_Count){
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
	//delete new_node;
	//new_node = NULL;
	//delete iter_node;
	//iter_node = NULL;
	return 0;
}

void printall()
{
	struct PlayerRanking *iter_node = HEAD;
	
	cout << endl << "Rankings: " << endl;
	
	while(iter_node != NULL)
	{
		
		cout << endl << "Player Name: " << iter_node->player_name << endl;
		cout << endl << "Score: " << iter_node->turn_Count << endl;
		cout << endl;
		iter_node = iter_node->next;
		
	}
	
	startMenu();
}

void save_file() 
{
	FILE *fileName;
	struct PlayerRanking *node = HEAD;
	
	fileName = fopen("ranking.dbm", "wb");
	
	if(fileName != NULL)
	{
		while(node != NULL)
		{
			fwrite(node->player_name, sizeof(node->player_name), 1, fileName);
			fwrite(&node->turn_Count, sizeof(int), 1, fileName);
			
			node = node->next;
		}
		fclose(fileName);
	}
	else
	{ 
		cout << "Error: Unable to save to the file \"ranking.dbm\"." << endl;
	}
}

void load_file(){
	
	FILE *fileName;
	char name[1024];
	int turn_Count;
	
	fileName = fopen("ranking.dbm", "rb");
	
	if(fileName != NULL)
	{
		while(fread(name, sizeof(name), 1, fileName) == 1)
		{
			fread(&turn_Count, sizeof(int), 1, fileName);
			
			insert(name, turn_Count);
		}
		fclose(fileName);
	}
	else
	{
		cout << "Error: Unable to load file" << endl;
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
	if (_computer->lost()) {
		victory();
		//check to see if player got high score
	}
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
						fire();
						turnCount();
						_computer->computer_turn(); // computer turn
						battleSwitch();
						
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
						fire();
						turnCount();
						_computer->computer_turn(); // computer turn
						battleSwitch();
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
						fire();
						turnCount();
						_computer->computer_turn(); // computer turn
						battleSwitch();
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
						fire();
						turnCount();
						_computer->computer_turn(); // computer turn
						battleSwitch(); // back to battle menu
						
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
						subfire();
						turnCount();
						_computer->computer_turn(); // computer turn
						battleSwitch(); // back to battle menu
						
					}
					break;
					
				default:
					cout <<	endl << endl << endl;
					cout << "Error! Must be a number between 1-5" << endl;
					cout <<	endl << endl << endl;
					battleSwitch();	//back to battle menu
					break;
			}
			
			break;
			
		case 2:          // move ships
			if(shipMove){ // checks to see if ships have been moved this turn or not
				shipMove_menu(); // asks which ship to move
				cin >> moveInput;
				switch (moveInput) {
					case 1:
						if (_Battleship->destroyed()) { // checks to see if battleship is active
							cout << endl << endl << endl;
							cout << "Error: Battleship is Destroyed...";
							cout << endl << endl << endl;
							
						}
						
						else {
							cout << endl << endl << endl;
							cout << "Move Battleship to Which Grid Square? (0-" << gridSize - 1 << ") ";
							cin >> moveLocation;
							//error handling
							while (moveLocation < 0 || moveLocation > gridSize - 1) {
								cout << endl << endl << endl;
								cout << "Error: That Square is not on the Grid!"<< endl;
								cout << endl;
								cout << "Move to Which Grid Square? (0-" << gridSize - 1 << ") ";
								cin >> moveLocation;
							}
							
							if (_grid1->move_ship(moveLocation, _Battleship)){
							    shipMove = false; // ships can no longer move for this turn
								cout << endl << endl << endl;
								cout << "Battleship was successfully moved";
								cout << endl << endl << endl;
								
							}else {
								cout << endl << endl << endl;
								cout << "Error! Ship could not move to that location";
								cout << endl << endl << endl;
								
							}
						}
						
						battleSwitch(); // back to battle menu
						break;
					case 2:
						if (_Cruiser->destroyed()) {   // checks to see if cruiser is active
							cout << endl << endl << endl;
							cout << "Error: Cruiser is Destroyed...";
							cout << endl << endl << endl;
							battleSwitch(); // back to battle menu
						}
						
						else {
							
							cout << endl << endl << endl;
							cout << "Move Cruiser to Which Grid Square? (0-" << gridSize - 1 << ") ";
							cin >> moveLocation;
							//error handling
							while (moveLocation < 0 || moveLocation > gridSize - 1) {
								cout << endl << endl << endl;
								cout << "Error: That Square is not on the Grid!"<< endl;
								cout << endl;
								cout << "Move to Which Grid Square? (0-" << gridSize - 1 << ") ";
								cin >> moveLocation;
							}
							
							if (_grid1->move_ship(moveLocation, _Cruiser)){
							    shipMove = false; // ships can no longer move for this turn
								cout << endl << endl << endl;
								cout << "Cruiser was successfully moved";
								cout << endl << endl << endl;
								
							}else {
								cout << endl << endl << endl;
								cout << "Error! Ship could not move to that location";
								cout << endl << endl << endl;
								
							}
						}
						
						battleSwitch(); // back to battle menu
						break;
						
					case 3:
						if (_Carrier->destroyed()) { // tests to see if the carrier is active
							cout << endl << endl << endl;
							cout << "Error: Carrier is Destroyed...";
							cout << endl << endl << endl;
							battleSwitch();
						}
						
						else {
							
							cout << endl << endl << endl;
							cout << "Move Carrier to Which Grid Square? (0-" << gridSize - 1 << ") ";
							cin >> moveLocation;
							//error handling
							while (moveLocation < 0 || moveLocation > gridSize - 1) {
								cout << endl << endl << endl;
								cout << "Error: That Square is not on the Grid!"<< endl;
								cout << endl;
								cout << "Move to Which Grid Square? (0-" << gridSize - 1 << ") ";
								cin >> moveLocation;
							}
							
							if (_grid1->move_ship(moveLocation, _Carrier)){
							    shipMove = false; // ships can no longer move for this turn
								cout << endl << endl << endl;
								cout << "Carrier was successfully moved";
								cout << endl << endl << endl;
								
							}else {
								cout << endl << endl << endl;
								cout << "Error! Ship could not move to that location";
								cout << endl << endl << endl;
								
							}
						}
						
						battleSwitch(); // back to battle menu
						
						break;
					case 4:
						if (_Destroyer->destroyed()) {  // checks to see if destroyer is active
							cout << endl << endl << endl;
							cout << "Error: Destroyer is Destroyed...";
							cout << endl << endl << endl;
							battleSwitch(); // back to battle menu
						}
						
						else {
							
							cout << endl << endl << endl;
							cout << "Move Destroyer to Which Grid Square? (0-" << gridSize - 1 << ") ";
							cin >> moveLocation;
							//error handling
							while (moveLocation < 0 || moveLocation > gridSize - 1) {
								cout << endl << endl << endl;
								cout << "Error: That Square is not on the Grid!"<< endl;
								cout << endl;
								cout << "Move to Which Grid Square? (0-" << gridSize - 1 << ") ";
								cin >> moveLocation;
							}
							
							if (_grid1->move_ship(moveLocation, _Destroyer)){
							    shipMove = false; // ships can no longer move for this turn
								cout << endl << endl << endl;
								cout << "Destroyer was successfully moved";
								cout << endl << endl << endl;
								
							}else {
								cout << endl << endl << endl;
								cout << "Error! Ship could not move to that location";
								cout << endl << endl << endl;
								
							}
						}
						
						battleSwitch(); // back to battle menu
						
						break;
					case 5:
						if (_Submarine->destroyed()) {  // checks to see if submarine is active
							cout << endl << endl << endl;
							cout << "Error: Submarine is Destroyed...";
							cout << endl << endl << endl;
							battleSwitch(); // back to Battle Menu
						}
						
						else {
							cout << endl << endl << endl;
							cout << "Move Submarine to Which Grid Square? (0-" << gridSize - 1 << ") ";
							cin >> moveLocation;
							//error handling
							while (moveLocation < 0 || moveLocation > gridSize - 1) {
								cout << endl << endl << endl;
								cout << "Error: That Square is not on the Grid!"<< endl;
								cout << endl;
								cout << "Move to Which Grid Square? (0-" << gridSize - 1 << ") ";
								cin >> moveLocation;
							}
							
							if (_grid1->move_ship(moveLocation, _Submarine)){
							    shipMove = false; // ships can no longer move for this turn
								cout << endl << endl << endl;
								cout << "Submarine was successfully moved";
								cout << endl << endl << endl;
								
							}else {
								cout << endl << endl << endl;
								cout << "Error! Ship could not move to that location";
								cout << endl << endl << endl;
								
							}
						}
						
						battleSwitch(); // back to battle menu
						break;
					default:
						cout <<	endl << endl << endl;
						cout << "Error! Must be a number between 1-5" << endl;
						cout <<	endl << endl << endl;
						shipMove_menu();	//prints the fire menu again
						break;
				}
			}
			
			else { // if shipMove=false.... ship cannot be moved this turn
				cout << endl << endl << endl;
				cout << "Error! Ship cannot be moved this turn";
				cout << endl << endl << endl;
				battleSwitch(); // back to battle menu
			}
			
			break;
			
		case 3:          // display the player's ships/health
			cout << endl << endl << endl;
			cout << "PLAYER'S SHIPS";
			cout << endl;
			cout << "Ship\t\t     Health" << endl;
			cout << "---------------------------" << endl;
			cout << "Battleship\t\t\t" << _Battleship->get_health() << "/" << _Battleship->total_health() << endl;
			cout << "Carrier\t\t\t" << _Carrier->get_health() << "/" << _Carrier->total_health() << endl;
			cout << "Cruiser\t\t\t" << _Cruiser->get_health() << "/" << _Cruiser->total_health() << endl;
			cout << "Destroyer\t\t\t" << _Destroyer->get_health() << "/" << _Destroyer->total_health() << endl;
			cout << "Submarine\t\t\t" << _Submarine->get_health() << "/" << _Submarine->total_health() << endl;
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
			delete _grid1;
			_grid1 = NULL;
			delete _grid2;
			_grid2 = NULL;
			delete _computer;
			_computer = NULL;
			programStart(); // display title screen
			startMenu();	// display main menu
			break;
			
		case 7:
			_grid2->print_grid();
			battleSwitch();// Go Back to Battle Menu...
		case 8:
			_grid1->print_grid();
			battleSwitch();// Go Back to Battle Menu...
			break;
			
			
		default:
			cout << endl << endl << endl;
			cout << "Error: Invalid Input";
			cout << endl << endl << endl;
			battleSwitch();    // if 1-6 not entered, Go Back To Battle Menu!
			break;
	}
}

//**************************************
// this method sets up the players board
//**************************************
void playerGridSetup(){
	
	// place battleship on the grid
	int location_index;
	cout << endl << endl << "Place your battleship on a grid square (0-" << gridSize-1 << ")" << endl ;
    cin  >> location_index;		//gets location index for battleship
	while(location_index < 0  || location_index > gridSize-1){		//Checks to see if the user's input would be somewhere on the grid.
		cout << "Error: Value is not on Grid" << endl << endl;
		cout << endl << "Place your battleship on a grid square (0-" << gridSize-1 << ")" << endl ;
		cin  >> location_index;		//gets location for battleship
		
	}
	_grid1->set_ship(location_index, _Battleship ); // sets the battleship at given location on the grid
	
	//place carrier on the grid
	cout << endl << endl << "Place your carrier on a grid square (0-" << gridSize-1 << ")" << endl ;
    cin  >> location_index;		//gets location index for carrier
	while(location_index < 0  || location_index > gridSize-1){		//Checks to see if location_index is on Grid
		cout << "Error: Value is not on Grid" << endl << endl;
		cout << endl << "Place your carrier on a grid square (0-" << gridSize-1 << ")" << endl ;
		cin  >> location_index;		//gets location for carrier
		
	}
	_grid1->set_ship(location_index, _Carrier ); // sets the carrier at given location on the grid
	
	//place cruiser on the grid
	cout << endl << endl << "Place your cruiser on a grid square (0-" << gridSize-1 << ")" << endl ;
    cin  >> location_index;		//gets location index for cruiser
	while(location_index < 0  || location_index > gridSize-1){		//Checks to see if location_index is on Grid
		cout << "Error: Value is not on Grid" << endl << endl;
		cout << endl << "Place your cruiser on a grid square (0-" << gridSize-1 << ")" << endl ;
		cin  >> location_index;		///gets location for cruiser
		
	}
	_grid1->set_ship(location_index, _Cruiser ); // sets the cruiser at given location on the grid	
	
	//place destroyer on the grid
	cout << endl << endl << "Place your destroyer on a grid square (0-" << gridSize-1 << ")" << endl ;
    cin  >> location_index;		//gets location index for battleship
	while(location_index < 0  || location_index > gridSize-1){		//Checks to see if location_index is on Grid
		cout << "Error: Value is not on Grid" << endl << endl;
		cout << endl << "Place your destroyer on a grid square (0-" << gridSize-1 << ")" << endl ;
		cin  >> location_index;		//Places the destroyer location into battle
		
	}
	_grid1->set_ship(location_index, _Destroyer ); // sets the destroyer at given location on the grid
	
	//place submarine on the grid
	cout << endl << endl << "Place your submarine on a grid square (0-" << gridSize-1 << ")" << endl ;
    cin  >> location_index;		//gets location index for submarine
	while(location_index < 0  || location_index > gridSize-1){		//Checks to see if location_index is on Grid
		cout << "Error: Value is not on Grid" << endl << endl;
		cout << endl << "Place your submarine on a grid square (0-" << gridSize-1 << ")" << endl ;
		cin  >> location_index;		//Places the submarine location into battle
		
	}
	_grid1->set_ship(location_index, _Submarine ); // sets the submarine at given location on the grid
	
	cout << endl << endl << endl;
	cout << "Your grid is now set...";
	cout << endl;
	
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
	insert(player_name, turn_Count);
	printall();
	save_file();
	programStart(); // title screen
	startMenu();// main menu
	
}

void gameOver(void){  // method called when the computer wins the game
	cout << endl << endl << endl;
	cout << "Wah Wah! You suck!";
	cout << endl << endl << endl;
	printall();
	save_file();
	programStart(); // title screen
	startMenu();// main menu
	
}

void fire(){		// Fire method for each ship
	Ship* temp;
	int fire_at, health;
	
	cout << "Which square do you want to hit? "; // pick square to fire at
	cin  >> fire_at;
	
	if(!_grid2->squareOccupied(fire_at)) //check if sqaure is empty
	{	cout << endl;
		cout << "MISS: Square is Empty" ;
		cout << endl;
		
	}
	//else if(_grid2->is_wreckage(fire_at)) // check if square has wreckage
	//{	
	//	cout << endl << "Square Contains wreckage." << endl;
	//}
	else   // if square is occupied
	{
		temp = _grid2->get_square_ship(fire_at); // get square you want to fire at
		health = temp->get_health();			// save health of ship in square
		
		
		cout << endl << "It's a HIT!" << endl;
		health--;					// Otherwise decrease health and
		temp->set_health(health);	// set it to the ship
		if(temp->get_health() == NULL)			//if ships health is null then ship is destroyed
		{ cout << "Enemy Ship Destroyed! --> " << whichShip(temp->get_type()); 
			ship_destruct(_grid2, fire_at, temp);
		} //returns 'destroyed' and the shiptype (enum)
	}	
}

void subfire(){		// Fire method for each ship
	Ship* temp;
	Ship* temp2;
	Ship* temp3;
	int fire_at, health, health2, health3;
	
	if (_Submarine->can_attack(turn_Count) || _Submarine->lastFired() == -1) {
		cout << "Which square do you want to hit? "; // pick square to fire at
		cin  >> fire_at;
		_Submarine->set_lastFired(turn_Count);
		if(!_grid2->squareOccupied(fire_at)) //check if square is empty
		{	
			cout << endl << "SUB MISS: Square is Empty" << endl;
			
		}
		//else if(_grid2->is_wreckage(fire_at)) // check if square has wreckage
		//{	cout<< endl;
		//	cout << "Square Contains wreckage." << endl ;
		//}
		else   // if square is occupied
		{
			temp = _grid2->get_square_ship(fire_at); // get square you want to fire at
			health = temp->get_health();			// save health of ship in square
			
			cout << endl << "It's a HIT!" << endl;
			health--;					// Otherwise decrease health and
			temp->set_health(health);	// set it to the ship
			if(temp->get_health() == NULL)			//if ships health is null then ship is destroyed
			{ 
				cout << "Enemy Ship Destroyed! --> " << whichShip(temp->get_type()); 
				ship_destruct(_grid2, fire_at, temp);
			} //returns 'destroyed' and the shiptype (enum)
			
			//checks to see if the 2nd square has a ship and then takes 1 point HP
			if (_grid2->squareOccupied(fire_at + 1)) {
				temp2 = _grid2->get_square_ship(fire_at + 1); // get square you want to fire at
				health2 = temp2->get_health();			// save health of ship in square
				
				cout << endl << "2nd Square is a HIT!" << endl;
				health2--;					// Otherwise decrease health and
				temp2->set_health(health2);	// set it to the ship
				if(temp2->get_health() == NULL)			//if ships health is null then ship is destroyed
				{ cout << "Enemy Ship Destroyed! --> " << whichShip(temp2->get_type()); 
					ship_destruct(_grid2, fire_at + 1, temp2);
				} //returns 'destroyed' and the shiptype (enum)
				//delete temp2;
				//temp2 = NULL;
			}
			//checks to see if the 3rd square has a ship and then takes 1 point HP
			if (_grid2->squareOccupied(fire_at + 2)) {
				temp3 = _grid2->get_square_ship(fire_at + 2); // get square you want to fire at
				health3 = temp3->get_health();			// save health of ship in square
				cout << endl << "3rd Square is a HIT!" << endl;
				health3--;					// decrease health and
				temp3->set_health(health3);	// set it to the ship
				if(temp3->get_health() == NULL)			//if ships health is null then ship is destroyed
				{ cout << "Enemy Ship Destroyed! --> " << whichShip(temp3->get_type()); 
					ship_destruct(_grid2, fire_at + 2, temp3);
				} //returns 'destroyed' and the shiptype (enum)
				//delete temp3;
				//temp3 = NULL;
			}
			
			//delete temp;
			//temp = NULL;
		}
	}
	// lets you know that the submarine cannot be used this turn
	else {
		cout << endl << "Error: Submarine is still reloading... please wait one more turn before using the sub" << endl;
		battleSwitch(); // back to battle menu
	}
	
	
	
}

void cfire(){		// Fire method for computer's ship
	Ship* temp2;
	int cfire_at, chealth;
	
	cfire_at = rand() % gridSize;
	
	if(!_grid1->squareOccupied(cfire_at)) //check if square is empty
	{	
		cout << endl << "The Computer has missed...!" << endl ;
		
	}
	//else if(_grid1->is_wreckage(cfire_at)) // check if square has wreckage
	//{	
	//	cout << endl << "Square Contains wreckage." << endl;
	//	
	//}
	else   // if square is occupied... it's a hit
	{
		temp2 = _grid1->get_square_ship(cfire_at); // get square you want to fire at
		chealth = temp2->get_health();			// save health of ship in square
		
		chealth--;					//  decrease health and
		temp2->set_health(chealth);	// set it to the ship
		if(temp2->get_health() == NULL)			//if ships health is null then ship is destroyed
		{ 
			cout << "Your Ship is Destroyed! --> " << whichShip(temp2->get_type());
			ship_destruct(_grid1, cfire_at, temp2);
		} //returns 'destroyed' and the shiptype (enum)
		
		//delete temp2;
		//temp2 = NULL;
	}
}

void csubfire(){		// Fire method for computer submarine
	Ship* temp;
	Ship* temp2;
	Ship* temp3;
	int cfire_at, chealth, chealth2, chealth3;
	
	cfire_at = rand() % gridSize;
	
	if(!_grid1->squareOccupied(cfire_at)) //check if sqaure is empty
	{	
		cout << endl << "The Computer's sub has missed..." << endl;
		
	}
	//else if(_grid1->is_wreckage(cfire_at)) // check if square has wreckage
	//{	
	//	cout << endl << "Square contains wreckage." << endl ;
	//}
	else   // if square is occupied
	{
		
		temp = _grid1->get_square_ship(cfire_at); // get square you want to fire at
		chealth = temp->get_health();			// save health of ship in square
		
		chealth--;					//  decrease health and
		temp->set_health(chealth);	// set it to the ship
		if(temp->get_health() == NULL)			//if ships health is null then ship is destroyed
		{ 
			cout << "Your Ship is Destroyed! --> " << whichShip(temp->get_type());
			ship_destruct(_grid1, cfire_at, temp);
		} //returns 'destroyed' and the shiptype (enum)
		
		// checks for the 2nd square
		if(_grid1->squareOccupied(cfire_at + 1)){
			temp2 = _grid1->get_square_ship(cfire_at + 1); // get square you want to fire at
			chealth2 = temp2->get_health();			// save health of ship in square
			
			chealth2--;					//  decrease health and
			temp2->set_health(chealth2);	// set it to the ship
			if(temp2->get_health() == NULL)			//if ships health is null then ship is destroyed
			{ 
				cout << "Your Ship is Destroyed! --> " << whichShip(temp2->get_type());
				ship_destruct(_grid1, cfire_at + 1, temp2);
			} //returns 'destroyed' and the shiptype (enum)
			//delete temp2;
			//temp2 = NULL;
		}
		
		if(_grid1->squareOccupied(cfire_at + 2)){
			// checks for the 3rd square
			temp3 = _grid1->get_square_ship(cfire_at + 2); // get square you want to fire at
			chealth3 = temp3->get_health();			// save health of ship in square
			
			chealth3--;					//  decrease health and
			temp3->set_health(chealth3);	// set it to the ship
			if(temp3->get_health() == NULL)			//if ships health is null then ship is destroyed
			{ 
				cout << "Your Ship is Destroyed! --> " << whichShip(temp3->get_type());
				ship_destruct(_grid1, cfire_at + 2, temp3);
			} //returns 'destroyed' and the shiptype (enum)
			//delete temp3;
			//temp3 = NULL;
		}
		//delete temp;
		//temp = NULL;
	}	
	
}



// this method 'decodes' the ship type from integer to string
string whichShip(int ship){
	if (ship == 0)
		return "Carrier";
	else if (ship == 1) 
		return "Battleship";
	else if (ship == 2)
		return "Cruiser";
	else if (ship == 3) 
		return "Submarine";
	else if (ship == 4)
		return "Destroyer";	
	else {
		return "Error: No Such Ship"; // returns error if there is no ship
	}
	
}

// method to call when ship health == 0 or NULL to send exploding wreckage to the next 2 neighboring cells
void ship_destruct(playerGrid* ship_grid, int location, Ship* destroyed_ship) {
	
	if(destroyed_ship->get_type() == 3){
		// delete the submarine since it's wreckage just floats to the bottom of the ocean
		Ship* sunk_sub = ship_grid->remove_ship(location);
		cout << endl << "Submarine's wreckage has sunk deep to it's watery grave";
	}
	// sets the square to wreckage
	if(ship_grid->set_wreckage(location) && destroyed_ship->get_type() != 3 )
		cout << endl << "Square: " << location << " is now wreckage" << endl;
	
	//  when battleship or carrier is destroyed, any ships downwind (right) by 1 or 2 cells are damaged by 1 hp
	// first cell
	if(destroyed_ship->get_type() == 0 || destroyed_ship->get_type() == 1){
		Ship* temp = ship_grid->get_square_ship(location + 1);
		if (temp != NULL) {
			int health1 = temp->get_health();
			health1--;
			cout << endl << "Exploding Wreckage has damaged another ship!";
		}
		// 2nd cell
		Ship* temp2 = ship_grid->get_square_ship(location + 2);
		if (temp2 != NULL){
			int health2 = temp2->get_health();
			health2--;
			cout << endl << "Exploding Wreckage has damaged a 2nd ship!!" << endl;
		}
		//delete temp;
		//temp = NULL;
		//delete temp2;
		//temp2 = NULL;
	}
	
	
	
}

