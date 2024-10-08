#include "Bishop.h"

namespace Chess
{
  bool Bishop::legal_move_shape(const Position& start, const Position& end) const {
    //check the x and y difference is the same
    int vert_diff = abs(start.second - end.second);
    int horiz_diff = abs(start.first - end.first);

    if (vert_diff != horiz_diff) {
      return false;
    }

    //checking the pieces between happens in make_move

    return true;
  }
}
