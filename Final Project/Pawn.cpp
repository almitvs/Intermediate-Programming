#include "Pawn.h"

namespace Chess
{
  /*
   * Function to check that an attempted move of the pawn piece fits its valid move shape
   */
  bool Pawn::legal_move_shape(const Position& start, const Position& end) const {
    // Assumes that start and end are not the same position 
    // Check the color of the piece because pawns which have not been moved can move 2 spaces on their first move, and each color has different starts
    if (this->is_white()) {
      // Check if the move is a first move going two spaces
      if ((start.second == '2') && (end.second == '4') && (start.first == end.first)) {
	return true;
      }
      // Otherwise check for a normal move
      if ((end.second == start.second + 1) && (start.first == end.first)) {
	return true;
      }
    // Black pawns move in the opposite direction
    } else {
      // Check if the move is a first move going two spaces                                                                                                                                                    
      if ((start.second == '7') && (end.second == '5') && (start.first == end.first)) {
	return true;
      }
      // Otherwise check for a normal move                                                                                                                                                                     
      if ((end.second == start.second - 1) && (start.first == end.first)) {
	return true;
      }
    }
    // Otherwise return false
    return false;
  }

  /*
   * Function to check that an attempted move to capture of the pawn piece fits its valid move shape
   */
  bool Pawn::legal_capture_shape(const Position& start, const Position& end) const {
    // Check the color because the two colored pawns move in opposite directions
    if (this->is_white()) {
      // Check that the end position is on diagonal forward left or right
      if ((end.second == start.second + 1) && ((end.first == start.first - 1) || (end.first == start.first + 1))) {
	return true;
      }
    // Check moves for the black pieces
    } else {
      // Check that the end position is on diagonal forward left or right     
      if ((end.second == start.second - 1) && ((end.first == start.first - 1) || (end.first == start.first + 1))) {
	return true;
      }
    }
    // Otherwise return false
    return false;
  }
}
