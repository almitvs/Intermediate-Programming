#include "Queen.h"
#include <cmath>
#include <iostream>
#include <cstdlib>

namespace Chess
{
    //function checking if the movement of Queen is legal
    bool Queen::legal_move_shape(const Position& start, const Position& end) const {

      //if the start position and end position is the same, return false
      if (start.first == end.first && start.second == end.second) {
	return false;
	//if the Queen is moving through the rows
      } else if (start.first == end.first) {
	return true;
	//if the Queen is moving through columns
      } else if (start.second == end.second) {
	return true;
	//handling the case diagonal
      } else if (std::abs(end.first - start.first) == std::abs(end.second - start.second)) {
	return true;
      } else {
	return false;
      }
    }
}
