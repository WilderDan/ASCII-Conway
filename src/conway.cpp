/* Author: Dan Wilder
   Version: 1.5
   Date Last Modified: 16 Dec 2013
   Email: the.wldr@gmail.com

-------------------------------------------------------------------------------
    Description:
      This is program is an example of Conway's Game of Life. 
      http://en.wikipedia.org/wiki/Conway%27s_Game_of_Life

      It was designed with the intention of being run from
      a command line interface. 

    *** Check out/modify the accompanying config file to alter behavior.
      
    To Do List:
      - Clean up code
      - Manual Initialization Function
      - Error Checking!!!
      - Break up into smaller source files
      - Allow for Unicode characters such as \u266B (a music note character)
      - Alternative to infinite loop
      - Different 'species'
      - Log File
      - Fix any bugs that may and most probabably will arise
      - Find more stuff to add
 
-------------------------------------------------------------------------------

    Have fun. Do whatever you want with my code.
      ~ Dan Wilder
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>    // Needed for usleep
#include <fstream>
#include <cstdlib>
using namespace std;


 /*** Function Prototypes ***/ 

void ClearScreen();
void PrepScreen();
void DrawX_Chars(int, char);
void DrawScreen(char *, int);
void Initialize(char *);
void RandomInitialize(char *, int);
void InitializeByFile(char *, string);
void Draw_GridRow(char *, int);
void Evolution(char *);
bool IsAlive(bool, int);
void SetConfig();
void InitializeBlankState(char *);
int  CountNeighbors(char *, int, int);
void CustomInitialize(char *);


  /*** Global Variables ==> Values come from config file ***/

int CONFIG_SETTINGS[8];  

bool showPrepScreen;
bool isAutoEvolve;
int  microSeconds;
int  numRows;
int  numCols;
bool wrapped;
char liveCell;
bool prepend;

/*************************************************************************
 *                                                                       *
 *                                  main                                 *
 *                                                                       *
 *************************************************************************/ 

int main() {

  int genNum = 0;
  
  SetConfig(); // Read config file and store in global variables

  int size = numRows * numCols;

  char universe[size];
 
  if (showPrepScreen == true) {
    PrepScreen();
  }

  Initialize(universe); 
  DrawScreen(universe, genNum);

  cout << "This is the initial generation. Enter any character to begin evolution. ";
  
  cin.get();
  cin.get();            // Look into clearing input buffer... 
  
  while (true) {   // Infinite loop... Fix 
    genNum++; 
    Evolution(universe);
    DrawScreen(universe, genNum);
    
    if (isAutoEvolve == true) {
      usleep(microSeconds);
    }
    else {
      cin.get();
    }
  } 
  
  return 0;
}

/*************************************************************************
 *                                                                       *
 *                            SetConfig                                  *
 *                                                                       *
 *************************************************************************/

void SetConfig(){

  ifstream config;
 
  string description;
 
  config.open("config");

  if (config){   // File opened!

    int i = 0;
    while (config >> description){
      config >> CONFIG_SETTINGS[i];
      i++;
    }
    
    config.close();
  }

  else{
    ClearScreen();

    cout << "Error opening configuration file.\n\n";
    cout << "Check that you have a file called 'config' located\n";
    cout << "in the same directory as this program."; 

    exit(1);
  }

  showPrepScreen = CONFIG_SETTINGS[0];
  isAutoEvolve   = CONFIG_SETTINGS[1];
  microSeconds   = CONFIG_SETTINGS[2];
  numRows        = CONFIG_SETTINGS[3];
  numCols        = CONFIG_SETTINGS[4];
  wrapped        = CONFIG_SETTINGS[5];
  liveCell       = CONFIG_SETTINGS[6];
  prepend        = CONFIG_SETTINGS[7];
}

/*************************************************************************
 *                                                                       *
 *                                PrepScreen                             *
 *                                                                       *
 *************************************************************************/

void PrepScreen() {  
  ClearScreen();
  
  DrawX_Chars(10, ' ');
  cout << "  Welcome to Conway's Game of Life!\n\n";
  DrawX_Chars(5, ' ');
  cout << "Please check out the README and config file for\n";
  DrawX_Chars(5, ' ');
  cout << "information on how to customize your experience with this\n";
  DrawX_Chars(5, ' ');
  cout << "program.\n\n";

  DrawX_Chars(5, ' ');
  cout << "Window size is important for the display element of this\n";
  DrawX_Chars(5, ' ');
  cout << "program. For best results, you may want to enlarge your\n";
  DrawX_Chars(5, ' ');
  cout << "screen and/or edit the config file to make less rows/columns.\n\n";

  DrawX_Chars(5, ' ');
  cout << "Have fun and happy coding!\n";
  DrawX_Chars(10, ' ');
  cout << "~Dan Wilder\n\n\n";
  
  DrawX_Chars(5, ' ');
  cout << "Hit <ENTER> to continue.";
  DrawX_Chars(19, '\b');
  cin.get();
}

/*************************************************************************
 *                                                                       *
 *                            Initialize                                 *
 *                                                                       *
 *************************************************************************/

void Initialize(char *universe){
  // This functions selects which initialization function to use. 
  // The purpose of the initialization functons is to create the 
  // initial generation of the game.

  int choice,
      numLiveCells,
      size = numRows * numCols;
   
  string fileName; 

  InitializeBlankState(universe);  // set all cells to dead/blank

  ClearScreen();
 
  DrawX_Chars(5, ' '); // For formatting purposes... This prints 5 spaces.
  DrawX_Chars(35, '=');

  cout << "\n";
  DrawX_Chars(5, ' ');
  cout << "Select method of initialization:\n";

  DrawX_Chars(5, ' ');
  DrawX_Chars(35, '=');
  cout << "\n";

  DrawX_Chars(7, ' ');
  cout << "[1] Random Initialization\n";
  DrawX_Chars(7, ' ');
  cout << "[2] Read from File\n";
  DrawX_Chars(7, ' ');
  cout << "[3] Interactive Initialization\n\n";

  DrawX_Chars(5, ' ');
  cout << "Enter 1, 2, or 3\n";
  DrawX_Chars(7, ' ');
  cout << "==> ";
  cin >> choice;
  
  while (choice < 1 || choice > 3) {  // Improve error checking!!!
    DrawX_Chars(5, ' ');
    cout << "Invalid.\n  ==> ";
    cin >> choice;
  }

  switch (choice){

    case 1:
      cout << "\n*** You have chosen option [1] ***\n";
      DrawX_Chars(5, ' ');
      cout << "\n  Please enter number of live cells to start with\n";
      DrawX_Chars(10, ' ');
      cout << "    [Max Amount: " << size << "]\n";
      DrawX_Chars(7, ' ');
      cout << "==> ";
      cin >> numLiveCells;
      RandomInitialize(universe, numLiveCells); 
      break;

    case 2:
      cout << "\n*** You have chosen option [2] ***\n";
      DrawX_Chars(5, ' ');
      cout << "\n  Please enter pattern name\n  ==> ";
      cin >> fileName;

      InitializeByFile(universe, fileName);
      break;

    case 3:
      CustomInitialize(universe);
      cin.get();
      cin.get();

    default:
      ClearScreen();
      cout << "!!! Fatal error in Initialize function !!!";
  }
  
  
}

/*************************************************************************
 *                                                                       *
 *                        RandomInitialize                               *
 *                                                                       *
 *************************************************************************/

void RandomInitialize(char *cellGrid, int numLiveCells){

  // A number (numLiveCells) of random coordinates will be set to alive.

  int size = numRows * numCols;
  
  int coordinate[size];

  int randomElement,
      coordinateValue;
  int searchRange = size - 1; // Set to last element of array

  srand(time(0));

// Create an array with coordinate values {0, 1, 2, ..., 1199} 

  for (int i = 0; i < size; i++) { 
    coordinate[i] = i;              
  }

  for (int i = 0; i < numLiveCells; i++, searchRange--) {
    
    randomElement = rand() % (searchRange + 1); 
      // A random element is selected which will have a value that is 
      // between 0 and the search range to search from the coordinate
      // array. The + 1 is simply an offset value. 
 
    coordinateValue = coordinate[randomElement];
      // coordinateValue is set to a value that came from the 
      // coordinate array at that random element 

    coordinate[randomElement] = coordinate[searchRange];
      //In the coordinate array, the value at the random index becomes
      //the value at the searchRange index 

    cellGrid[coordinateValue] = liveCell;
      // Set the value in the cell grid at that coordinate to the character
      // representing alive 

      // The decrementing of the search range and repositioning of the
      // random coordinate in the coordinate array, ensures that the 
      // same coordinate will NOT be chosen more than once. 
  } 
}

/*************************************************************************
 *                                                                       *
 *                            Evolution                                  *
 *                                                                       *
 *************************************************************************/


void Evolution(char *universe){

  // This function updates the array according to the rules of Conway's 
  // Game of Life. 
  
  bool alive;
  char currentGen[numRows][numCols]; 
  char nextGen[numRows][numCols];  

  int numNeighbors,     // Number of neighboring live cells
      i_coord,
      j_coord;


     // This converts the cellGrid which is a single array representation
     // of the state of each cell into an easier to process, multi 
     // dimensional array which also represents the state of each cell
     // in the same manner as its one dimensional counterpart.

  for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numCols; j++) {
      currentGen[i][j] = universe[(i*numCols) + j];
    }
  }
 
// Examine current generation to create new generation 

  for (int i = 0; i < numRows; i++) {      // For each row
    for (int j = 0; j < numCols; j++) {    // For each column...
                                      // Meaning for every cell

      if (currentGen[i][j] == liveCell) { 
        alive = true;
      }
      else {
        alive = false;
      }

      numNeighbors = CountNeighbors(universe, i, j); 

      alive = IsAlive(alive, numNeighbors); 
      
      if (alive) {
        nextGen[i][j] = liveCell;
      }
      else {
        nextGen[i][j] = ' ';
      }
    } 
   }

  
/***  convert newGen[][] back to 1 dimensional cellGrid.

      This is so that later when the DrawScreen function is 
      called, it knows where to find the latest values.
 ***/

  for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numCols; j++) {
      universe[(i*numCols) + j] = nextGen[i][j];
    }
  }

}

/*************************************************************************
 *                                                                       *
 *                            IsAlive                                    *
 *                                                                       *
 *************************************************************************/

bool IsAlive(bool livingNow, int numNeighbors) {

/* This function determines the living status of the
   next generation. 

   This function is the core of Conway's Game of Life
*/
  
  if (livingNow == true) {

    if ( (numNeighbors < 2) || (numNeighbors > 3) ) {
      livingNow = false;
    }
  }
  
  else if (livingNow == false) {
    if (numNeighbors == 3) {
      livingNow = true;
    }
  }

  return livingNow;
}

/*************************************************************************
 *                                                                       *
 *                            DrawScreen                                 *
 *                                                                       *
 *************************************************************************/

void DrawScreen(char *grid, int genNum){
  
  ClearScreen();
 
  DrawX_Chars(25, ' '); 
  cout << "Generation Number: " << genNum << endl; 
  cout << endl;

  DrawX_Chars(5, ' '); 
  DrawX_Chars(numCols, '_');   // Top Border
  cout << endl;

  for (int i = 0; i < numRows; i++) {
    DrawX_Chars(4, ' ');
    cout << "|"; 
    Draw_GridRow(grid, i);
    cout << "|" << endl;
  } 
   
  DrawX_Chars(4, ' '); 
  cout << "|";
  DrawX_Chars(numCols, '=');  // Bottom Border
  cout << "|" << endl;

  cout << endl;
  cout << endl;
  cout << endl;
}

/*************************************************************************
 *                                                                       *
 *                           Draw_GridRow                                *
 *                                                                       *
 *************************************************************************/

void Draw_GridRow(char *grid, int rowNumber) {
 
  int startElement = rowNumber * numCols; 

  int currentElement;
  
  for (int i = 0; i < numCols; i++) {  
    currentElement = startElement + i;
    cout << grid[currentElement]; // Prints each element of the row
  }
}

/*************************************************************************
 *                                                                       *
 *                           DrawX_Chars                                 *
 *                                                                       *
 *************************************************************************/

void DrawX_Chars(int numChars, char character){
  for (int i = 0; i < numChars; i++) { 
    cout << character;
  }
}

/*************************************************************************
 *                                                                       *
 *                           ClearScreen                                 *
 *                                                                       *
 *************************************************************************/

void ClearScreen(){
  DrawX_Chars(100, '\n');
}

/*************************************************************************
 *                                                                       *
 *                       InitializeByFile                                *
 *                                                                       *
 *************************************************************************/

void InitializeByFile(char *universe, string fileName){

  ifstream inputFile;

  int i,
      j,
      coordinate;

  if (prepend == true) {
    fileName = "init_gen/" + fileName;
  }

  inputFile.open(fileName.c_str());
  
  if (inputFile){    // If File opened: 

    while (inputFile >> i && inputFile >> j){
      coordinate = (i * numCols) + j; 
      universe[coordinate] = liveCell;
    }
  }
  
  else {
    cout << "Error. Could not open file.";
  }
}

/*************************************************************************
 *                                                                       *
 *                    InitializeBlankState                               *
 *                                                                       *
 *************************************************************************/ 

void InitializeBlankState(char *universe){

  int size = numRows * numCols;

  for (int i = 0; i < size; i++){
    universe[i] = ' ';
  }
}

/*************************************************************************
 *                                                                       *
 *                        CountNeighbors                                 *
 *                                                                       *
 *************************************************************************/

int CountNeighbors(char *universe, int i, int j){
  
  char currentGen[numRows][numCols];

  int i_coord,
      j_coord,
      numNeighbors = 0;

  for (int k = 0; k < numRows; k++) {
    for (int s = 0; s < numCols; s++) {
      currentGen[k][s] = universe[(k*numCols) + s];
    }
  }

  if (wrapped == true){
    for (int k = 1; k <= 8; k++){  // For each of its 8 neighbors

     /*               
   
         For the below switch statement, each case corresponds to an 
         iteration of the for loop... That is to say, k = 1 corresponds to 
         case 1 while k = 5 corresponds with case 5. Each case is designed 
         to check the living/dead status of the neighbor cell that corresponds 
         with that iteration. 

             
         =====================  =====================  =====================          
         | Case 1 (i-1, j-1) |  | Case 2 (i-1, j)   |  | Case 3 (i-1, j+1) |       
         =====================  ====================== =====================     
         =====================  =====================  =====================          
         | Case 4 (i, j-1)   |  | Current Cell(i,j) |  | Case 5 (i, j+1)   |       
         =====================  =====================  =====================
         =====================  =====================  =====================
         | Case 6 (i+1, j-1) |  | Case 7 (i+1, j)   |  | Case 8 (i+1, j+1) |
         =====================  =====================  =====================
                             Cell Diagram of Neighbor cells
                       [where i represents row and j representing column]

      
       */
       
         switch(k){

/*** 
       The cell grid wraps around. 

       This means:
         ~ The bottommost row and uppermost row are neighbor rows 
         ~ The rightmost row and leftmost row are neighbor rows
***/

          case 1:  // Top Left Neighbor 

            if ( (i - 1) < 0) { 
              i_coord = numRows - 1;     
            } else {
              i_coord = i - 1;  
            }

            if ( (j - 1) < 0) { 
              j_coord = numCols - 1;     
            } else {
              j_coord = j - 1; 
            } 

            if (currentGen[i_coord][j_coord] == liveCell) {
              numNeighbors++; 
            }
            break;

          case 2: // Neighbor Directly Above
            if ( (i - 1) < 0) {
              i_coord = numRows - 1;
            } else {
              i_coord = i - 1;
            }

            if (currentGen[i_coord][j] == liveCell) {
              numNeighbors++; 
            } 
            break;

          case 3: // Top Right neighbor
            if ( (i - 1) < 0) {
              i_coord = numRows - 1;
            } else {
              i_coord = i - 1;
            }

            if ( (j + 1) > numCols - 1) {
              j_coord = 0;
            } else {
              j_coord = j + 1;
            } 

            if (currentGen[i_coord][j_coord] == liveCell) {
              numNeighbors++; 
            } 
            break;

          case 4: // Neighbor Directly to the Left

            if ( (j - 1) < 0) {
              j_coord = numCols - 1;
            } else {
              j_coord = j - 1;
            } 

            if (currentGen[i][j_coord] == liveCell) {
              numNeighbors++; 
            } 
            break;

          case 5: // Neighbor Directly to the Right

            if ( (j + 1) > numCols - 1) {
              j_coord = 0;
            } else {
              j_coord = j + 1;
            } 

            if (currentGen[i][j_coord] == liveCell) {
              numNeighbors++; 
            } 
            break;

          case 6: // Bottom Left Neighbor
            if ( (i + 1) > numRows - 1) {
              i_coord = 0;
            } else {
              i_coord = i + 1;
            }

            if ( (j - 1) < 0) {
              j_coord = numCols - 1;
            } else {
              j_coord = j - 1;
            } 

            if (currentGen[i_coord][j_coord] == liveCell) {
              numNeighbors++; 
            } 
            break;

          case 7: // Neighbor Directly Below
            if ( (i + 1) > numRows - 1) {
              i_coord = 0;
            } else {
              i_coord = i + 1;
            }

            if (currentGen[i_coord][j] == liveCell) {
              numNeighbors++; 
            }  
            break;

          case 8: // Bottom Right Neighbor
            if ( (i + 1) > numRows - 1) {
              i_coord = 0;
            } else {
              i_coord = i + 1;
            }

            if ( (j + 1) > numCols - 1) {
              j_coord = 0;
            } else {
              j_coord = j + 1;
            } 

            if (currentGen[i_coord][j_coord] == liveCell) {
              numNeighbors++; 
            }  
            break;

          default: // This should never execute
            cout << "Unknown error in counting neighbors!!!";  
        }
      } 
    } // End Grid Wrap Condition Block

    else {
      for (int k = 1; k <= 8; k++){  // For each of its 8 neighbors

     /*               
   
         Write description here

      
       */
       
         switch(k){

          case 1:  // Top Left Neighbor 

              i_coord = i - 1;  
            
              j_coord = j - 1; 
            
            if ( (i_coord >= 0 && j_coord >= 0) &&
                 (currentGen[i_coord][j_coord] == liveCell) 
               )
            {
              numNeighbors++; 
            }
            break;

          case 2: // Neighbor Directly Above
          
              i_coord = i - 1;
            
            if ( (i_coord >= 0) &&
                 (currentGen[i_coord][j] == liveCell) 
               ){
              numNeighbors++; 
            }
            break;

          case 3: // Top Right neighbor
          
              i_coord = i - 1;
            
              j_coord = j + 1;
            
            if ( (i_coord >= 0 && j_coord < numCols) &&
                 (currentGen[i_coord][j_coord] == liveCell) 
               ){
              numNeighbors++; 
            }
            break;

          case 4: // Neighbor Directly to the Left

              j_coord = j - 1;
            
            if ( (j_coord >= 0) &&
                 (currentGen[i][j_coord] == liveCell) 
               ){
              numNeighbors++; 
            }
            break;

          case 5: // Neighbor Directly to the Right
       
              j_coord = j + 1;

            if ( (j_coord < numCols) &&
                 (currentGen[i][j_coord] == liveCell) 
               ){
              numNeighbors++; 
            }
            break;

          case 6: // Bottom Left Neighbor
         
              i_coord = i + 1;
            
              j_coord = j - 1;
         
            if ( (i_coord < numRows && j_coord >= 0) &&
                 (currentGen[i_coord][j_coord] == liveCell) 
               ){
              numNeighbors++; 
            } 
            break;

          case 7: // Neighbor Directly Below
         
              i_coord = i + 1;
            
            if ( (i_coord < numRows ) &&
                 (currentGen[i_coord][j] == liveCell) 
               ){
              numNeighbors++; 
            }
            break;

          case 8: // Bottom Right Neighbor
          
              i_coord = i + 1;
            
              j_coord = j + 1;
            

            if ( (i_coord < numRows && j_coord < numCols) &&
                 (currentGen[i_coord][j_coord] == liveCell) 
               ){
              numNeighbors++; 
            }
            break;

          default: // This should never execute
            cout << "Unknown error in counting neighbors!!!";  
        }
      } 
    }
  
  return numNeighbors;
} 

/*************************************************************************
 *                                                                       *
 *                        CustomInitialize                               *
 *                                                                       *
 *************************************************************************/

void CustomInitialize(char *universe){

  string inputString,
         line;
  ifstream helpText;
  bool done = false;

  while (done == false) {
    //DrawScreen(universe, 0);
    DrawX_Chars(5, ' ');
    cout << "Enter 'help' for list of commands.\n";
    DrawX_Chars(7, ' ');
    cout << "Input Command ==> ";

     // Get Input String
    cin.ignore(256, '\n');
    cin.clear();
    getline(cin, inputString);
    cout << "'" << inputString << "'";
    if (inputString == "help"){

      //ClearScreen();
      helpText.open("help/help_");
      if (helpText){
        while (getline(helpText, line)) {
          cout << line;
          cout << "\n";
        }
        //cin.get();
      
        helpText.close();
      }
      else{
        cout << "Error opening file.";
      }
    }
    else if (inputString == "start"){
      done = true;
    }
    else {
     cout << "Did not recognize input";
    }
  } 
 
}



