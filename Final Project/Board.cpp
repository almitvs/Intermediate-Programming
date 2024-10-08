#include <iostream>
#include <utility>
#include <map>
#ifndef _WIN32
#include "Terminal.h"
#endif // !_WIN32
#include "Board.h"
#include "CreatePiece.h"
#include "Exceptions.h"

namespace Chess
{
  /////////////////////////////////////
  // DO NOT MODIFY THIS FUNCTION!!!! //
  /////////////////////////////////////
  Board::Board(){}

   /*
   * Function to return the piece at a given position if there is a piece
   */
  const Piece* Board::operator()(const Position& position) const {
    // Check if there is a piece at this position
    try {
      return occ.at(position);
    } catch (const std::out_of_range& e) {
      // Otherwise return a nullptr
      return nullptr;
    }
  }

  /*
   * Adds a piece at a given location unless the piece is invalid, the position is invalid, or the position is occupied
   */
  void Board::add_piece(const Position& position, const char& piece_designator) {
    // Create the piece
    Piece* new_piece = create_piece(piece_designator);
    // Throw an error if the piece is invalid
    if (new_piece == nullptr) {
      throw Exception("invalid designator");
    }
    // Throw an error if the position is invalid
    if ((position.first < 'A') || (position.first > 'H') || (position.second < '1') || (position.second > '8')) {
      throw Exception("invalid position");
    }
    // Throw an error if the position is already occupied
    if (this->operator()(position) != nullptr) {
      throw Exception("position is occupied");
    }
    // Otherwise add the piece to the board
    this->occ[position] = new_piece;
    return;
  }

  /*
   * Removes the piece at a given location
   */
  void Board::remove_piece(const Position& position) {
    if (this->operator()(position) != nullptr) {
      occ.erase(position);
    }
  }

  /*
   * Function to print the board and its pieces to stdout using a custom colorful UI
   */
  void Board::display() const {
    // Print the column labels
    for (char k = '@'; k <= 'I'; k++) {
      Terminal::set_default();
      Terminal::color_fg(true, Terminal::YELLOW);
      // Do not print anything for the first, empty square
      if (k != '@' && k != 'I') {
	std::cout << k;
      } else {
	std::cout << " ";
      }
    }
    std::cout << std::endl;
    // Boolean to switch the color with each square to create the checkerboard pattern
    bool is_blue = false;
    // Print the board
    for (char i = '8'; i >= '1'; i--) {
      for (char j = '@'; j <= 'I'; j++) {
	// Print the row label in the same fashion as the column labels
	if (j == '@' || j == 'I') {
	  Terminal::set_default();
	  Terminal::color_fg(true, Terminal::YELLOW);
	  std::cout << i;
	  if (j == 'I') {
           // Print a newline at the end of the row                                                                                                              
           Terminal::set_default();
           std::cout << std::endl;
	   is_blue = !is_blue;
	  }
	  // Print a square on the board
	} else {
	  // Determine the square color based on the patter
	  if (is_blue) {
	    Terminal::color_bg(Terminal::RED);
	  } else {
	    Terminal::color_bg(Terminal::MAGENTA);
	  }
	  // Check if there is a piece at this square
	  const Piece* cur = this->operator()(Position(j, i));
	  if (cur != nullptr) {
	    // Print the color of the piece based on its actual color
	    if (cur->is_white()) {
	      Terminal::color_fg(false, Terminal::WHITE);
	    } else {
	      Terminal::color_fg(true, Terminal::BLACK);
	    }
	    std::cout << cur->to_ascii();
	  } else {
	    // Print a space for no piece
	    std::cout << " ";
	  }
	  // Swap the square color                                                                                                                         
	  is_blue = !is_blue;
	}
      }
    }
    // Print the column labels                                                                                                                                   
    for (char k = '@'; k <= 'I'; k++) {
      Terminal::set_default();
      Terminal::color_fg(true, Terminal::YELLOW);
      // Do not print anything for the first, empty square                                                                                                       
      if (k != '@' && k != 'I') {
        std::cout << k;
      } else {
        std::cout << " ";
      }
    }
    std::cout << std::endl;
    // Set the colors to the default
    Terminal::set_default();
  }
  
  bool Board::has_valid_kings() const {
    int white_king_count = 0;
    int black_king_count = 0;
    for (std::map<std::pair<char, char>, Piece*>::const_iterator it = occ.begin();
	 it != occ.end();
	 it++) {
      if (it->second) {
	switch (it->second->to_ascii()) {
	case 'K':
	  white_king_count++;
	  break;
	case 'k':
	  black_king_count++;
	  break;
	}
      }
    }
    return (white_king_count == 1) && (black_king_count == 1);
  }

  void Board::clear() {
    for (Board::const_iterator it = this->cbegin(); it != this->cend(); ++it) {
      const Piece* cur = this->operator()(*it);
      if (cur != nullptr) {
	this->remove_piece(*it);
	delete cur;
      }
    }
    return;
  }
  
  /////////////////////////////////////
  // DO NOT MODIFY THIS FUNCTION!!!! //
  /////////////////////////////////////
  std::ostream& operator<<(std::ostream& os, const Board& board) {
    for(char r = '8'; r >= '1'; r--) {
      for(char c = 'A'; c <= 'H'; c++) {
	const Piece* piece = board(Position(c, r));
	if (piece) {
	  os << piece->to_ascii();
	} else {
	  os << '-';
	}
      }
      os << std::endl;
    }
    return os;
  }


  Board::~Board(){

    for (Board::const_iterator itr = this->cbegin();itr != this->cend(); ++itr ) {
      if ((*this)(*itr)) {
        delete (*this)(*itr);
        //std::cout << i << " piece is deleted" << std::endl;

      }
      //if there is a piece at the position, delete the piece
    }
  }
}
