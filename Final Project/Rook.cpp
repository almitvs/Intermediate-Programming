#include "Rook.h"

namespace Chess
{
  /*
   * Function to check that an attempted move of the rook piece fits its valid move shape
   */
  bool Rook::legal_move_shape(const Position& start , const Position& end) const {
    // Assumes that start and end are not the same position
    // Check if the piece is moving through a columnn
    if (start.first == end.first) {
      return true;
    } else if (start.second == end.second) {
      // Check if the piece is moving through a row
      return true;
    } else {
      // Else return false
      return false;
    }
  }
}
