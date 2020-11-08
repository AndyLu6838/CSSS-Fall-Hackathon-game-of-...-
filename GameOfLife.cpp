// Author: Mario Talevski
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <fstream>
#include <string>
#include <sstream>

//Move OS defines up here to be used in different places
#if defined(_WIN32) || defined(WIN32) || defined(__MINGW32__) || defined(__BORLANDC__)
 #define OS_WIN
 //WINDOWS COLORS 
  #define COLOR_RED SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED)
  #define COLOR_WARNING SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN) 

  #define COLOR_BLUE SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE)

  #define COLOR_RESET SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15)

#elif defined(linux) || defined(__CYGWIN__)
  #define OS_LINUX

  #define COLOR_RED "\033[31m"
  #define COLOR_GREEN "\033[32m" 
  #define COLOR_BLUE "\033[34m"
  #define COLOR_RESET "\033[0m"

#elif (defined(__APPLE__) || defined(__OSX__) || defined(__MACOS__)) && defined(__MACH__)//To ensure that we are running on a mondern version of macOS
  #define OS_MAC

  #define COLOR_RED "\033[31m"
  #define COLOR_GREEN "\033[32m" 
  #define COLOR_BLUE "\033[34m"
  #define COLOR_RESET "\033[0m"

#endif

#if defined(OS_WIN)
  #include <windows.h> // Use for windows
#endif

using namespace std;

const int gridSize = 25;
void printGrid(bool gridOne[gridSize][gridSize]);
void determineState(bool gridOne[gridSize][gridSize], int starvationLimit, int overpopulationLimit, int neighbourRadius);
void clearScreen(void);


int main(){

    // system( "color A" );//LGT green
    cout << COLOR_RED;
    clearScreen();
    bool gridOne[gridSize][gridSize] = {};
    int x,y,n;
    string nc;
    string start;
    string filename;
    int starvationLimit, overpopulationLimit, neighbourRadius;
    string isCustomRules;
    string starvationLimitStr;
    string overpopulationLimitStr;
    string neighbourRadiusStr;

    cout << "                        CSSS FALL HACKATHON - THE GAME OF Os ... - Implementation in C++" << endl;
	 // TODO:  write introduction and get rid of this one
    cout << endl;
    cout << endl;
    cout << endl;
    cout << "This is the Game of Os!" << endl;
    cout << "Based on cellular automaton created by the British mathematician John Horton Conway in 1970." << endl;
    cout << endl<<endl;
    cout << "This game contains an infinite two-dimensional orthogonal grid of square cells, where each cell is either alive or dead. Much like us, humans." << endl;
    cout << "Every cell communicates with its eight neighbours, which are the cells that are horizontally, vertically, or diagonally adjacent. You could think of those neighbours as friends." <<endl<< endl;
    cout << "After each generation, the following transitions occur:" << endl;
    cout << "I. Any living cell which has less than two live neighbours dies. They were too lonely, they wanted more friends." << endl;
    cout << "II. Any living cell which has two or three live neighbours lives on to the next generation. Yay, now they can make even more friends!" << endl;
    cout << "III. Any living cell which has more than three live neighbours dies. Too many friends could be bad for their health too." << endl;
    cout << "IV. Any dead cell with exactly three live neighbours becomes a live cell. Ooh they are resurrected from their death!" << endl;
    cout << endl;
    cout << "Now this is just the default setting for this game. But if you want to be one of the cool kids and set your own number of alive and dead 'friends/neighbours' a cell needs to die or regenerate, you are very much welcome to do so!"<<endl;
    cout << "Life and death are represented in this game by the symbols: << endl;
    cout << "O - living cell" << endl;
    cout << ". - dead cell" << endl;
    cout << endl;
    cout << endl;
    cout << "Enter the number of cells, or 'r' to read cells from file: ";
    cin >> nc;
    cout << endl;

    if ( nc == "r" )
      {
	while (true)
	  {
	    
	    cout << "Enter name of file to read from: "<<endl;
	    cin  >> filename;
	    
	    ifstream readfile(filename);
	    if ( readfile.is_open() )
	      {
		string fileline;
    		int yCoord = 0;

		while (getline(readfile,fileline))
		  {
		   	stringstream ss(fileline);	    
        		int xCoord = 0;
        		char cell;

        		while (ss.get(cell)) {

			  if( cell == 'o' ) {
			    cout << "adding live cell to coord " << xCoord << ", " << yCoord << "!" << endl;
			    gridOne[xCoord][yCoord] = true;
			  }
			  if( cell == ' ') {
			    // don't count spaces
			    continue;
			  }
			  xCoord++;
			}
			yCoord++;
		  }
		break;
	      } else {
	      cout << "No such file, try again." << endl;
	    }
	  }
      }
    else
      {
        for(int i=0;i<stoi(nc);i++)
          {
            cout <<stoi(nc)<< "Enter the coordinates of cell " << i+1 << " : ";
            cin >> x >> y;
            gridOne[x][y] = true;
            printGrid(gridOne);
          }
      }
  
  
    cout << "Grid setup is done. Play with the default Game of Life rules? (y/n)" << endl;

    cin >> isCustomRules;
    if( isCustomRules == "y" || isCustomRules == "Y" )
      {
        starvationLimit = 2;
        overpopulationLimit = 3;
        neighbourRadius = 1;
      }
    else {
      cout << "Choose starvation limit (integer)" << endl;
      cin >> starvationLimitStr;
      starvationLimit = stoi(starvationLimitStr);

      cout << "Choose overpopulation limit (integer)" << endl;
      cin >> overpopulationLimitStr;
      overpopulationLimit = stoi(overpopulationLimitStr);

      cout << "Choose neighbour radius (integer)" << endl;
      cin >> neighbourRadiusStr;
      neighbourRadius = stoi(neighbourRadiusStr);
    }
	
	
    cout << "Rule configuration is done. Start the game ? (y/n)" << endl;

    printGrid(gridOne);
    cin >> start;
    if( start == "y" || start == "Y" )
      {
        while (true)
	  {
            printGrid(gridOne);
            determineState(gridOne, starvationLimit, overpopulationLimit, neighbourRadius);
            usleep(200000);
            clearScreen();
	  }
      }
    else
      {
        cout << COLOR_RESET;
        clearScreen();
        return 0;
      }   
}

void clearScreen(void) {
  // Tested and working on Ubuntu and Cygwin
  #if defined(OS_WIN)
    HANDLE hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    DWORD cellCount;
    COORD homeCoords = { 0, 0 };

    if (hStdOut == INVALID_HANDLE_VALUE) return;

    /* Get the number of cells in the current buffer */
    GetConsoleScreenBufferInfo( hStdOut, &csbi );
    cellCount = csbi.dwSize.X *csbi.dwSize.Y;

    /* Fill the entire buffer with spaces */
    FillConsoleOutputCharacter(hStdOut, (TCHAR) ' ', cellCount, homeCoords, &count);

    /* Fill the entire buffer with the current colors and attributes */
    FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, cellCount, homeCoords, &count);

    SetConsoleCursorPosition( hStdOut, homeCoords );

  #elif defined(OS_LINUX) || defined(OS_MAC)
    cout << "\033[2J;" << "\033[1;1H"; // Clears screen and moves cursor to home pos on POSIX systems
  #endif

}


void printGrid(bool gridOne[gridSize][gridSize])
{
  for (int i=0; i < gridSize; i++)
  {
    for (int x=0; x < gridSize; x++)
    {
      if (gridOne[i][x]==true)
      {
        cout << "o ";
      }
      else
      {
        cout << ". ";
      }
    }
    cout << endl;
  }
}

void copyGrid (bool gridOne[gridSize][gridSize], bool gridTwo[gridSize][gridSize]){
	for(int i=0; i<gridSize; i++) //row
	{
		for(int j=0; j<gridSize; j++)//column
		{
			gridTwo[i][j] = gridOne[i][j];
		}
	}
}

/* 
param:  two integers, a and b
return: a mod b, should work with negatives
*/
int mod2(int a, int b){
    return (b + (a%b)) % b;
} 

/*
param:  gridOne, the boolean grid with the alive (true) and dead (false) cells  
        i, the integer index of the row of gridOne
        j, the integer index of the column of gridOne
return: the number of adjacent cells (including diagonal) to gridOne[i][j]
        that are alive (true).
*/
int liveNeighbours (bool gridOne[gridSize][gridSize], int i, int j){
    // count all trues from grid[i-1][j-1] to grid[i+1][j+1]
    int count = 0;
    
    // can change these bounds depending on 
    // how many neighbours you want to consider
    int lower_bound_cols = i-1;
    int upper_bound_cols = i+1;
    int lower_bound_rows = j-1;
    int upper_bound_rows = j+1;

    for (int a = lower_bound_cols; a <= upper_bound_cols; a++){
        for (int b = lower_bound_rows; b <= upper_bound_rows; b++){
            // basically calculates the mod of the index
            // if an index is out of bounds, the mod2 will bring it into bounds,
            // but put it on the opposite side, eg. mod2(-1, 25) = 24
            // lets the cells wrap around the array ("toroidal array")
            if (gridOne[mod2(a, gridSize)][mod2(b, gridSize)] == true && 
                    (mod2(a, gridSize) != i || mod2(b, gridSize) != j)){
                count++;
            }
        }
    }
    return count;
}

/*
param:  gridOne, the boolean grid with the alive (true) and dead (false) cells 
return: void, but modifies gridOne with trues and falses to simulate an iteration
*/
void determineState(bool gridOne[gridSize][gridSize], int starvationLimit, int overpopulationLimit, int neighbourRadius) {
    bool gridCopy[gridSize][gridSize];

    // make an unchanged copy of gridOne so changes made to gridOne
    // won't affect liveNeighbours
    copyGrid(gridOne, gridCopy);

    for (int i = 0; i < gridSize; i++){
        for (int j = 0; j < gridSize; j++){
            int alive = liveNeighbours(gridCopy, i, j);

            // TODO: Make it so that these rules can be changed
            /* Rules:
            Any cell with less than 2 live neighbours dies.
            Any cell with more than 3 live neighbours dies.
            Any cell with 2-3 live neighbours stays unchanged.
            Any cell with 3 live neighbours is alive.
            */

            if (alive < starvationLimit){
                gridOne[i][j] = false;
            }
            else if (alive > overpopulationLimit){
                gridOne[i][j] = false;
            }
            else if (alive == overpopulationLimit){
                gridOne[i][j] = true;
            }
        }
    }
}
