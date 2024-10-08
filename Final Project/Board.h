#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <map>
#include "Piece.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Mystery.h"


namespace Chess
{
  class Board {

		// Throughout, we will be accessing board positions using Position defined in Piece.h.
		// The assumption is that the first value is the column with values in
		// {'A','B','C','D','E','F','G','H'} (all caps)
		// and the second is the row, with values in {'1','2','3','4','5','6','7','8'}

	public:
		// Default constructor
		Board();
    

        Board& operator=( const Board& other) {

			clear();



			for (Board::const_iterator it = other.cbegin(); it != other.cend(); ++it) {
				if ((other)(*it)) {
				char type = (other)(*it)->to_ascii();
				add_piece(*it, type);
				}

			}
			// this->occ = other.occ;
			return *this;
        }

		Board ( Board& other){
			clear();

			for (Board::iterator it = other.begin(); it != other.end(); ++it) {
				if ((other)(*it)) {
				char type = (other)(*it)->to_ascii();
				add_piece(*it, type);
				}

			}
		}
    
		// Returns a const pointer to the piece at a prescribed location if it exists,
		// or nullptr if there is nothing there.
		const Piece* operator() (const Position& position) const;

		// Attempts to add a new piece with the specified designator, at the given position.
		// Throw exception for the following cases:
		// -- the designator is invalid, throw exception with error message "invalid designator"
		// -- the specified position is not on the board, throw exception with error message "invalid position"
		// -- if the specified position is occupied, throw exception with error message "position is occupied"
		void add_piece(const Position& position, const char& piece_designator);

                // Remove a piece form the board
                void remove_piece(const Position& position);
    
		// Displays the board by printing it to stdout
		void display() const;

		// Returns true if the board has the right number of kings on it
		bool has_valid_kings() const;

        class iterator{

		  Position pos;

		  public:

		  iterator(Position initial) : pos(initial) {}
		  
		  iterator& operator++() {
		    if (pos == Position('H', '1')) {
		      pos = Position('X', 'X');
		    } else if (pos.first == 'H') {
		      pos = Position('A', pos.second - 1);
		    } else {
		      pos = Position(pos.first + 1, pos.second);
		    }
		    return *this;
		  }

		  bool operator != (const iterator& o) const { return this->pos != o.pos; }
		  
		  Position& operator*() { return pos; }
		  
        };

		iterator begin() { return iterator(Position('A', '8')); }
		iterator end() { return iterator(Position('X', 'X')); }

		class const_iterator{
			Position pos;
		  public:
			const_iterator( Position const initial) : pos(initial) {}
		  
		  const_iterator& operator++() {
		    if (pos == Position('H', '1')) {
		      pos = Position('X', 'X');
		    } else if (pos.first == 'H') {
		      pos = Position('A', pos.second - 1);
		    } else {
		      pos = Position(pos.first + 1, pos.second);
		    }
		    return *this;
		  }

		  bool operator != (const const_iterator& o) const { return this->pos != o.pos; }
		  
		  Position& operator*() { return pos; }
		};

			const_iterator cbegin()const { return const_iterator(Position('A', '8')); }
			const_iterator cend()const { return const_iterator(Position('X', 'X')); }

        void clear();

		~Board();
    
	private:
		// The sparse map storing the pieces, keyed off locations
		std::map<Position, Piece*> occ;

        // Write the board state to an output stream
        friend std::ostream& operator<< (std::ostream& os, const Board& board);
	};
}
#endif // BOARD_H
