#include <iostream>
#include <ctype.h>

using namespace std;

void menu(); 
void branching(char option);
void new_game();
void view_ranking();
void programStart();
void battle_menu();
void battle_branch(char option);
void fire();
void change_location();
void display_ships();
void enemy_ships();
void skip();


int main() {
	char ch; 
	
	programStart();
	
	do {
		menu();
		ch = tolower(getchar()); 
		cin.ignore();
		branching(ch);
	} while (ch != 'q');
	
	return 0; 
};

void menu()		//menu displays the menu of options for the user
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

void branching(char option)		
{
	switch(option)
	{
		case 'n':
			new_game();
			break;
			
		case 'v':
			view_ranking();
			break;
			
		case 'q':
			break;
			
		default:
			printf("\nError: Invalid Input.  Please try again..."); 
			break;
	}
}

void new_game()
{
	// needs to have player set the grid sizes
	// needs to have player place battleships on the grid
	// then battle will commence
}

void view_ranking()
{
	// This needs to read from file and display 10 highest scores in the game 
}

void battle()
{
	char ch; 
	
	
	do {
		battle_menu();
		ch = tolower(getchar()); 
		cin.ignore();
		battle_branch(ch);
		// call computer turn NEED TO WRITE THIS CODE!!!!
	} while (ch != 'q');
	
}

void battle_menu()
{
	cout << "\nBattle Options\n";
	cout << "------------------------------------------------------\n";
	cout << "f: Fire!\n";
	cout << "c: Change Battleship location\n";
	cout << "d: Display remaining Battleships\n";
	cout << "e: Display Enemy's remaining Battleships\n";
	cout << "s: Skip Turn\n";
	cout << "q: Quit\n";
	cout << "\n\nPlease enter a choice (f, c, d, e, s or q) ---> "; 
	
}

void battle_branch(char option)
{
	switch(option)
	{
		case 'f':	// fire
			fire();
			break;
			
		case 'c':	// change location
			change_location();
			break;
			
		case 'd':	// your remaining battleships
			display_ships();
			break;
		case 'e':	// enemy remaining battleships
			enemy_ships();
			break;
			
		case 's':	// skip turn
			skip();
			break;
			
		case 'q':	// quit
			break;
			
		default:
			printf("\nError: Invalid Input.  Please try again..."); 
			break;
	}
	
	
}

void fire(){
// fires on a square in the grid

// turn counter++ for score
	
}

void change_location(){
// change one battleship location
}

void display_ships(){
// display your remaining ships
}

void enemy_ships(){
// display enemy remaining ships	
}

void skip(){
// skip turn	
}

