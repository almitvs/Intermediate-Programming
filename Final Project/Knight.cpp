#include "Knight.h"
#include <iostream>
#include <cstdlib>
#include <cmath>

namespace Chess
{
    
    //check the legal movement of Knight
    bool Knight::legal_move_shape(const Position& start, const Position& end) const {

      //calculate row and col difference
      int row = std::abs(start.first - end.first);
      int col = std::abs(start.second - end.second);

      //check if the knight is moving in L shape
      return (row == 2 && col == 1) || (row == 1 && col == 2);
    }
}
