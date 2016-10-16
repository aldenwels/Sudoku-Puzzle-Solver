//
//  cell.h
//  program2
//
//  Created by Philip Gringer on 10/3/16.
//  Copyright © 2016 Philip Gringer. All rights reserved.
//

#ifndef cell_h
#define cell_h
#include <vector>
using namespace std;

class Cell {
    public:
    
        int value, row, col, subGroup;
        vector<int> domain;
        vector<Cell*> constrainedTo;
    
        Cell(int val,int r,int c, int sub);
        void setConstraintedTo(vector<vector<Cell*> > board);
    
};

#endif /* cell_h */
