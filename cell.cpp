#include <iostream>
#include <vector>
#include <list>
#include "cell.h"
using namespace std;

Cell::Cell(int val,int r,int c,int sub){
    this->value = val;
    this->row = r + 1;
    this->col = c + 1;
    this->subGroup = sub;
    if(val != 0){
        this->domain.push_back(val);
    }
    else{
        for(int i = 1; i <= 9; i++)
            this->domain.push_back(i);
    }
    
}

void Cell::setConstraintedTo(vector<vector<Cell*> > board){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if(board[i][j] != this){
                if(board[i][j]->row == this->row || board[i][j]->col == this->col || board[i][j]->subGroup == this->subGroup)
                    this->constrainedTo.push_back(board[i][j]);

            }
        }
    }
}