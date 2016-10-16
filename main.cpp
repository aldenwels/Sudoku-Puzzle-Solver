#include <iostream>
#include <vector>
#include <fstream>
#include <list>
#include <queue>
#include "cell.h"
using namespace std;

vector<vector<Cell*> > setBoard(fstream& input);
void printBoard(vector<vector<Cell*> > board,fstream& outfile);
void constrain(vector<vector<Cell*> > board);
void AC3(vector<vector<Cell*> > board);
void backTrack(vector<vector<Cell*> > board);
void forwardCheck(Cell* current);
bool backtrack(vector<vector<Cell*> > board);
bool FindMinRemainingValue(vector<vector<Cell*> > board, int &row, int &col);
bool isSafe(vector<vector<Cell*> > board,Cell* current, int num);
void forwardCheck(Cell* current, int num);


vector<vector<Cell*> > setBoard(fstream& input){
    vector<vector<Cell*> > board(9,vector<Cell*>(9));
    char holdc;
    int hold;
    int subGroup = 1;
    //subgroups are determined by row and column
    for(int i = 0; i < 9; i++){
        if(i < 3)
            subGroup = 1;
        if(i >= 3 && i < 6)
            subGroup = 4;
        if(i >= 6)
            subGroup = 7;
        for(int j = 0; j < 9; j++){
            input >> holdc;
            hold = holdc - '0';
            board[i][j] = new Cell(hold,i,j,subGroup);
            if((j + 1) % 3 == 0 && j > 0)
                subGroup++;
        }
    }
    return board;
}

void printBoard(vector<vector<Cell*> > board,fstream& outfile){
    for(int i = 0; i < 9; i++){
        cout << endl;
        for(int j = 0; j < 9; j++){
            outfile << board[i][j]->value << " ";
        }
    }
    cout << endl;
}


void constrain(vector<vector<Cell*> > board){
    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 9; j++)
            board[i][j]->setConstraintedTo(board);
}

void AC3(vector<vector<Cell*> > board){
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    queue<Cell*> mainQ;
    
    //fill queue with all variables of board
    for(int i = 0; i < 9; i++)
        for(int j = 0; j < 9; j++){
            mainQ.push(board[i][j]);
        }
    
    
    while(!mainQ.empty()){
        
        //set current to front of queue
        Cell* current = mainQ.front();
        

        for(int i = 0; i < current->constrainedTo.size(); i++){
            if(current->constrainedTo[i]->value != 0){
                int contrainedVal = current->constrainedTo[i]->value;
                
                //iterate of domain vector and remove contrainsed values
                //ie if
                for(int j = 0; j < current->domain.size();j++){
                    if(current->domain[j] == contrainedVal){
                        //remove from domain
                        current->domain.erase(current->domain.begin()+j);
                        for(int k = 0; k < current->constrainedTo.size(); k++)
                            //add all of this variables peers back to queue if they are not already assigned
                            mainQ.push(current->constrainedTo[k]);
                    }
                }
            }
        }
        //if domain has been fully reduced assign the variable
        if(current->domain.size() == 1)
            current->value = current->domain[0];
        mainQ.pop();
    }
   

}




bool backtrack(vector<vector<Cell*> > board)
{
    
    int row, col;
    
    // find minimum remaining value
    if (!FindMinRemainingValue(board, row, col))
        return true; // success!
    
    // consider digits in variables domain
    for (int i = 0; i < board[row][col]->domain.size(); i++){

            int value = board[row][col]->domain[i];
            // make tentative assignment
            board[row][col]->value = value;
        
            forwardCheck(board[row][col],value);
            
            // return, if success
            if (backtrack(board))
                return true;
            
            // failure, unmake & try again
            board[row][col]->value = 0;
            //add back to domains that this value was removed from
            for(int j = 0; j < board[row][col]->constrainedTo.size(); j++){
                vector<int>::iterator it;
                //find out if value is in domain or not, if it is not in the domain, and the variable is unassigned then
                //add to the domain of that variable
                it = find(board[row][col]->constrainedTo[j]->domain.begin(), board[row][col]->constrainedTo[j]->domain.end(), value);
                if(it == board[row][col]->constrainedTo[j]->domain.end() && board[row][col]->constrainedTo[j]->value == 0)
                    board[row][col]->constrainedTo[j]->domain.push_back(value);
            }
    }
    return false;
}


bool FindMinRemainingValue(vector<vector<Cell*> > board, int &row, int &col)
{
    
    //finds variable with smallest domain and returns that row and col
    bool hasEmpty = false;
    int minDomain = 10;
    int minRow = 0, minCol = 0;
    for (row = 0; row < 9; row++){
        for (col = 0; col < 9; col++){
            if (board[row][col]->value == 0 && (board[row][col]->domain.size() < minDomain)){
                minDomain = board[row][col]->domain.size();
                minRow = row;
                minCol = col;
                hasEmpty = true;
            }
        }
    }
    row = minRow;
    col = minCol;
    return hasEmpty;
}


bool isSafe(vector<vector<Cell*> > board,Cell* current, int num)
{
    //checks to make sure this value can be assigned due to constraints
    for(int i = 0; i < current->constrainedTo.size(); i++){
        if(current->constrainedTo[i]->value == num){
            return false;
        }
    }
    return true;
}

void forwardCheck(Cell* current, int num){
    //erases tentatively assigned value from the domain of variables constrained to the
    //variable just assigned
    for(int i = 0; i < current->constrainedTo.size(); i++){
        for(int j = 0; j < current->constrainedTo[i]->domain.size(); j++){
            if(current->constrainedTo[i]->domain[j] == num && current->constrainedTo[i]->value == 0)
                current->constrainedTo[i]->domain.erase(current->constrainedTo[i]->domain.begin()+j);
        }
    }
}


int main() {
    int puzzCount = 0;
    fstream input, outfile;
    
    input.open("input.txt");
    outfile.open("sudokus_solved.txt");
    
    while(!input.eof()){
        //allocate
        vector<vector<Cell*> > board(9,vector<Cell*>(9));
        vector<vector<Cell*> > boardBackTrack(9,vector<Cell*>(9));
        //set boards
        board = setBoard(input);
        boardBackTrack = board;
        
        //print
        outfile << "initial: " << endl;
        printBoard(board,outfile);
        constrain(board);
        
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        AC3(board);
        //assign variables if domain is reduced to one variable through AC3
        for(int i = 0; i < 9; i++){
            for(int j = 0; j < 9; j++){
                if(board[i][j]->value == 0 && board[i][j]->domain.size() <= 1)
                    board[i][j]->value = board[i][j]->domain[0];
            }
        }
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        outfile << "AC-3 took "
        << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
        << " us. In running time\n";

        
        outfile << "With AC-3 \n";
        printBoard(board,outfile);
            
        //BACKTRACKING
        constrain(boardBackTrack);
        start = std::chrono::steady_clock::now();
        backtrack(boardBackTrack);
        end = std::chrono::steady_clock::now();
        outfile << "Backtracking took "
        << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
        << " us. In running time\n";
        outfile << "Solved Using backtracking: \n";
        printBoard(boardBackTrack,outfile);
    
    }
    return 0;
}
