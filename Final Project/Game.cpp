#include <cassert>
#include "Game.h"
#include <algorithm>

namespace Chess
{
	/////////////////////////////////////
	// DO NOT MODIFY THIS FUNCTION!!!! //
	/////////////////////////////////////
	Game::Game() : is_white_turn(true) {
		// Add the pawns
		for (int i = 0; i < 8; i++) {
			board.add_piece(Position('A' + i, '1' + 1), 'P');
			board.add_piece(Position('A' + i, '1' + 6), 'p');
		}

		// Add the rooks
		board.add_piece(Position( 'A'+0 , '1'+0 ) , 'R' );
		board.add_piece(Position( 'A'+7 , '1'+0 ) , 'R' );
		board.add_piece(Position( 'A'+0 , '1'+7 ) , 'r' );
		board.add_piece(Position( 'A'+7 , '1'+7 ) , 'r' );

		// Add the knights
		board.add_piece(Position( 'A'+1 , '1'+0 ) , 'N' );
		board.add_piece(Position( 'A'+6 , '1'+0 ) , 'N' );
		board.add_piece(Position( 'A'+1 , '1'+7 ) , 'n' );
		board.add_piece(Position( 'A'+6 , '1'+7 ) , 'n' );

		// Add the bishops
		board.add_piece(Position( 'A'+2 , '1'+0 ) , 'B' );
		board.add_piece(Position( 'A'+5 , '1'+0 ) , 'B' );
		board.add_piece(Position( 'A'+2 , '1'+7 ) , 'b' );
		board.add_piece(Position( 'A'+5 , '1'+7 ) , 'b' );

		// Add the kings and queens
		board.add_piece(Position( 'A'+3 , '1'+0 ) , 'Q' );
		board.add_piece(Position( 'A'+4 , '1'+0 ) , 'K' );
		board.add_piece(Position( 'A'+3 , '1'+7 ) , 'q' );
		board.add_piece(Position( 'A'+4 , '1'+7 ) , 'k' );
	} 

	void Game::make_move(const Position& start, const Position& end) {

		//check start position on board exception-start position is not on board
		if (start.first < 'A' || start.first > 'H' || start.second < '1' || start.second > '8') {
			throw Exception("start position is not on board");
			return;
		}

		//check end position on board exception-end position is not on board
		if (end.first < 'A' || end.first > 'H' || end.second < '1' || end.second > '8') {
			throw Exception("end position is not on board");
			return;
		}

		//check there is a piece at the start position exception-no piece at start position
		if (!board(start)) {
			throw Exception("no piece at start position");
			return;

		//check the selected piece has the color that matches current person's turn exception-piece color and turn do not match
		} else if (!(board(start)->is_white() == is_white_turn)){
			throw Exception("piece color and turn do not match");
			return;
		}

		//check that the start and end position are not the same
		if (start.first == end.first && start.second == end.second) {
			throw Exception("cannot capture own piece");
			return;
		}

		//check if there is a piece at the end 
		bool validCaptureShape = false;


		if (board(end)) {
			//check piece at the end is not your own, is it a valid capture -exception illegal capture shape (run legal capture)
			if (!(board(end)->is_white() == board(start)->is_white())) {
				if (!board(start)->legal_capture_shape(start, end)) {
					throw Exception("illegal capture shape");
					return;
				} else {
					//valid capture shape, check if the piece at the end is not a king
					if ((board(end)->to_ascii() == 'K' ) || (board(end)->to_ascii() == 'k' )) {
						throw Exception("illegal move shape");
						return;
					}
					validCaptureShape = true;
				}
			} else {
			//since piece exists, and not the opponent's, piece where you are moving is your own - exception-cannot capture own piece
				throw Exception("cannot capture own piece");
				return;
			}
			//making the capture will happen after passing all the other checks below
		} else if (!board(start)->legal_move_shape(start, end)) { //check for the piece type that it is a valid direction/shape exception-illegal move shape (run legal move)
			throw Exception("illegal move shape");
			return;
		}

    	
		//checks if there are pieces in between the start and end
		if (!checkPathClear(start, end)) {
			return;
		}

        //check if the move would place yourself in check - exception-move exposes check
		char pieceType = board(start)->to_ascii();
		Game replica(*this);
		delete replica.board(end);
		replica.board.remove_piece(end);
		replica.board.add_piece(end, pieceType);	//add piece's new location
		delete replica.board(start);
		replica.board.remove_piece(start);	//get rid of piece's old location

		if (replica.in_check(is_white_turn)) {
			throw Exception("move exposes check");
			return;
		}
		
		//if pass all above, check again if it is a capture (stored in boolean), if so then update board with changes of both pieces
		if (validCaptureShape) {
			delete board(end);
			board.remove_piece(end);	//get rid of piece at the ending spot

		}
		//update the map in board with new location of moving piece
		board.add_piece(end, pieceType); //add piece to the new spot, piecetype defined in char piecetype for replica

		delete board(start);
		board.remove_piece(start); //get rid of piece at the start spot

		//if the piece moved a pawn and at row 1 or 8, then delete the piece and add a queen piece
		if ((pieceType == 'p' && end.second == '1') || (pieceType == 'P' && end.second == '8')) {
		  if (pieceType == 'p') {
		    pieceType = 'q';
		  } else {
		    pieceType = 'Q';
		  }
		  delete board(end);
		  board.remove_piece(end);
		  board.add_piece(end, pieceType);
		}
		// Switch turns to the next player
		this->is_white_turn = !this->is_white_turn;
		return;
    }

   bool Game::checkPathClear(Chess::Position const &start, Chess::Position const &end) const {

	if ((start.first == end.first || start.second == end.second) || (abs(start.first - end.first) == abs(start.second - end.second))){

		// Check for certain pieces that there are no pieces in-between exception-path is not clear
		char startHoriz = start.first;
		char startVert = start.second;

		//check diagonal where start is bottom left of end position
		if ((startHoriz + 1) < end.first && (startVert + 1) < end.second) {
			startHoriz++;
			startVert++;

			while (startHoriz < end.first) {
				Position pos(startHoriz, startVert);
				if (board(pos)) {
					throw Exception("path is not clear");
					return false;
				}
				startHoriz++;
				startVert++;
			}
		}

		//check diagonal where start is upper left of end position
		if ((startHoriz + 1) < end.first && (startVert - 1) > end.second) {
			startHoriz++;
			startVert--;

			while (startHoriz < end.first) {
				Position pos(startHoriz, startVert);
				if (board(pos)) {
					throw Exception("path is not clear");
					return false;
				}
				startHoriz++;
				startVert--;
			}
		}

		//check diagonal where start is lower right of end position
		if ((startHoriz - 1) > end.first && (startVert + 1) < end.second) {
			startHoriz--;
			startVert++;

			while (startHoriz > end.first) {
				Position pos(startHoriz, startVert);
				if (board(pos)) {
					throw Exception("path is not clear");
					return false;
				}
				startHoriz--;
				startVert++;
			}
		}

		//check diagonal where start is upper right of end position
		if ((startHoriz - 1) > end.first && (startVert - 1) > end.second) {
			startHoriz--;
			startVert--;

			while (startHoriz > end.first) {
				Position pos(startHoriz, startVert);
				if (board(pos)) {
					throw Exception("path is not clear");
					return false;
				}
				startHoriz--;
				startVert--;
			}
		}



		//see if the start is to the left of the end
		if ((startHoriz + 1) < end.first) {
			startHoriz++;
			while (startHoriz < end.first) {
				Position pos(startHoriz, startVert);
				if (board(pos)) {
					throw Exception("path is not clear");
					return false;
				}
				startHoriz++;
			}
		} else if ((startHoriz - 1) > end.first) { //see if the start is to the right of end
			startHoriz--;
			while (startHoriz > end.first) {
				Position pos(startHoriz, startVert);
				if (board(pos)) {
					throw Exception("path is not clear");
					return false;
				}
				startHoriz--;
			}
		}

		//check start is above end
		if ((startVert - 1) > end.second) {
			startVert--;
			while (startVert > end.second) {
				Position pos(startHoriz, startVert);
				if (board(pos)) {
					throw Exception("path is not clear");
					return false;
				}
				startVert--;
			}
		} else if ((startVert + 1) < end.second) { 	//check start is below end
			startVert++;
			while (startVert < end.second) {
				Position pos(startHoriz, startVert);
				if (board(pos)) {
					throw Exception("path is not clear");
					return false;
				}
				startVert++;
			}
		}
	}



    return true;
}


  Game::Game(const Game &rhs) {
		this->board = rhs.board;
		this->is_white_turn = rhs.is_white_turn;
	}


	bool Game::in_check(const bool& white) const {

	  //check to see if the functions are in check or not

	  //representing King in K or k based on their color
	  char king_designator = white ? 'K' : 'k';
	  Position king_position; //initialize king position
	  bool king_found = false; 

	  //iterate through board to find the position of the king first
	  for (char row = '1' ; row <= '8' && !king_found; ++row) {
	    for (char col = 'A'; col <= 'H' && !king_found; ++col) {
	      Position current_position(col, row);

	      //get the piece at the currnet_position
	      const Piece* piece = board.operator()(current_position);

	      // if the piece at the position is a king, mark it the king
	      if (piece && piece->to_ascii() == king_designator) {
		king_position = current_position;
		king_found = true;
	      }
	    }
	  }

	    //see if any piece to take king in the next move
	  for (char row = '1'; row <= '8'; ++row) {
	    for (char col = 'A' ; col <= 'H'; ++col) {

		//get the position of the board
		Position from_position(col, row);

		//return the piece at the position
		const Piece* threatening_piece = board.operator()(from_position);

		//check to make sure it's opposite color
		if (threatening_piece && threatening_piece->is_white() != white) {

		  try {
		  //if the king could be captured by the threatening piece
		  if (threatening_piece->legal_capture_shape(from_position, king_position) && checkPathClear(from_position, king_position)) {
		    return true;
		    }
		  }
		  catch(...) {
		  }
		    
		  }
		}
	      }
	    
	    return false;
	  }


  
	bool Game::in_mate(const bool& white) const {
	  //check for checkmate

	  //if not in check, return false 
	  if (!in_check(white)) return false;

	  //iterate through the board
	  for (char row = '1'; row <= '8'; ++row) {
	    for (char col = 'A'; col <= 'H'; ++col) {

	      //get the piece at the position
	      Position current_position(col, row);
	      const Piece* current_piece = board.operator()(current_position);

	      //if such piece exist and it is king's color
	      if (current_piece != nullptr && current_piece->is_white() == white) {

		//move through the board for potential position
		for (char endrow = '1'; endrow <= '8'; ++endrow) {
		  for (char endcol = 'A'; endcol <= 'H'; ++endcol) {

		    //get the position
		    Position end_position (endcol, endrow);

		    //copy the game
		    Game copygame(*this);

		    //make move in the copied game
		    try {
		      copygame.make_move(current_position, end_position);

		      //if make some move could prevent the check, return false
		      if (!copygame.in_check(white)) return false;

		      //for some reason make_move does not move, continue the loop
		    } catch(...) {
		    }
		  }
		}
	      }
	    }
	  }

	  //return true
	  return true;
	}

	  
        /*
	 * Function to check if a player is in stalemate
	 */
	bool Game::in_stalemate(const bool& white) const {
	  // A player cannot be in check or checkmate and stalemate at once
	  if (in_check(white)) {
	    return false;
	  }
	  //iterate through the board to check if the player's pieces have valid moves
	  for (char row = '1'; row <= '8'; ++row) {
	    for (char col = 'A'; col <= 'H'; ++col) {
	      Position current_position(col, row);
	      const Piece* current_piece = board(current_position);
	      //moving any pieces would make current color in check 
	      if (current_piece != nullptr && current_piece->is_white() == white) {
		for (char endrow = '1'; endrow <= '8'; ++endrow) {
		  for (char endcol = 'A'; endcol <= 'H'; ++endcol) {
		    Position end_position(endcol, endrow);
		    Game copygame(*this);
		    try {
		      // Check if the player has any valid move to the end_position
		      copygame.make_move(current_position, end_position);
		      // Return true if a remove does not throw an error, thus does not result in check
		      return false;
		    }
		    // Catch errors thrown by invalid moves
		    catch (...) {
		      // Keep searching for a valid move
		      continue;
		    }
		  }
		}
	      }
	    }
	  }
	  return true;
	}

    // Return the total material point value of the designated player
    int Game::point_value(const bool& white) {
      int total = 0;
      // Iterate through the board to find this player's pieces
      for (Board::iterator it = board.begin(); it != board.end(); ++it) {
	const Piece* cur = board(*it);
	// Do not count the points if there is no piece or the piece is the wrong color
	if (cur != nullptr && cur->is_white() == white) {
	  total += cur->point_value();
	}
      }
      // Return the total points
      return total;
    }


	/*
	* Function to read a board from a stream
	*/
    std::istream& operator>> (std::istream& is, Game& game) {
      Game new_game(true);
      // Variable to store the symbol at a given postion on the board
      char square;
      std::string line;
      // Iterate through the board
      for (char i = '8'; i >= '1'; i--) {
	// Throw an error if there is nothing left to read when the board has not been iterated through
	if (is.eof()) {
	  throw Chess::Exception("game failed to load");
	}
	getline(is, line);
	// Throw an error if the row is the wrong size
	if (line.length() != 8) {
	  throw Chess::Exception("game failed to load");
	}
	for (char j = 'A'; j <= 'H'; j++) {
	  square = line[j - 65];
	  // Attempt to add a piece if the space is not empty
	  if (square != '-') {
	    new_game.board.add_piece(Position(j, i), square);
	  }
	}
      }
      if (!new_game.board.has_valid_kings()) {
	throw Chess::Exception("game failed to load");
      }
      // Throw an error if there is no character left over for the turn state
      if (is.eof()) {
	throw Chess::Exception("game failed to load");
      }
      getline(is, line);
      // Throw an error if the last line contains too much
      if (line.length() != 1) {
        throw Chess::Exception("game failed to load");
      }
      square = line[0];
      // Store whose turn it is or throw an error
      if (square == 'w') {
	new_game.is_white_turn = true;
      } else if (square == 'b') {
	new_game.is_white_turn = false;
      } else {
	throw Chess::Exception("game failed to load");
      }
      // The board is valid if it had reached this point
      game = new_game;
      return is;
    }

    /////////////////////////////////////
    // DO NOT MODIFY THIS FUNCTION!!!! //
    /////////////////////////////////////
	std::ostream& operator<< (std::ostream& os, const Game& game) {
		// Write the board out and then either the character 'w' or the character 'b',
		// depending on whose turn it is
		return os << game.board << (game.turn_white() ? 'w' : 'b');
	}
}
