#include "King.h"


namespace Chess {

    bool King::legal_move_shape(const Position& start, const Position& end) const {
        //check that the x and y diff is at most one

        int vert_diff = abs(start.second - end.second);
        int horiz_diff = abs(start.first - end.first);

        //already know that the piece will not be in the original spot
        if (vert_diff > 1 || horiz_diff > 1) {
            return false;
        }

        return true;

    }

}