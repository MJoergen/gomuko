/*****************************************************************************
 *
 * Author:      Michael Joergensen
 *
 * Abstract:
 *
 * Global Contents:
 *
 * Local Contents:
 *
 * ---------------------------------------------------------------------------
 * Revision History:
 * $Log: CBoard.cc,v $
 * Revision 1.15  2015/07/07 06:57:58  mike
 * Cleanup.
 *
 * Revision 1.14  2015/07/07 06:36:23  mike
 * Cleanup.
 *
 * Revision 1.13  2015/07/07 06:31:54  mike
 * Cleanup.
 *
 * Revision 1.12  2015/07/06 22:45:17  mike
 * Avoid static variables inside functions.
 *
 * Revision 1.11  2015/07/06 22:04:46  mike
 * Optimization
 *
 * Revision 1.10  2015/07/06 20:36:12  mike
 * Optimization.
 *
 * Revision 1.9  2015/07/05 14:57:44  mike
 * Checkin time.
 *
 * Revision 1.8  2015/07/05 14:21:42  mike
 * Checkin time.
 *
 * Revision 1.7  2015/07/05 13:59:50  mike
 * Cleanup.
 *
 * Revision 1.6  2015/07/05 13:46:56  mike
 * Checkin time.
 *
 * Revision 1.5  2015/07/05 11:53:57  mike
 * Checkin time.
 *
 * Revision 1.4  2015/07/05 11:39:00  mike
 * Cleanup.
 *
 * Revision 1.3  2015/07/05 10:29:59  mike
 * Cleanup.
 *
 * Revision 1.2  2015/07/05 10:09:00  mike
 * Checkin time.
 *
 * Revision 1.1  2015/07/05 09:05:54  mike
 * First version.
 *
 ****************************************************************************/

/* CBoard.cc */

#include <iostream>
#include <assert.h>
#include "CBoard.h"

/***************************************************************
 * Display board to output stream
 ***************************************************************/
std::ostream& operator <<(std::ostream &os, const CBoard &rhs)
{
    for (int row=SIZE-1; row>=0; row--)
    {
        char c = '1' + row;
        os << c << "  ";
        for (int col=0; col<SIZE; col++)
        {
            CMove sq(row,col);
            os << rhs.m_board[sq.Sq()];
            os << " ";
        }
        os << std::endl;
    }
    os << "   ";
    for (unsigned int col=0; col<SIZE; col++)
    {
        char c = 'a' + col;
        os << c << " ";
    }

    os << std::endl << std::endl;
    os << "It is " << (rhs.m_bWhite ? 'X' : 'O') << "'s turn." << std::endl;

    os << "Lines: ";
    for (const auto& kv : rhs.m_lines)
    {
        const CLine&     line = kv.first;
        CBoard::lineVal_t val = kv.second;
        os << line << ":" << val << "  ";
    }
    os << std::endl;

    os << "Value: " << rhs.Evaluate() << std::endl;

    return os;
} // end of operator <<


/***************************************************************
 * Display line to output stream
 ***************************************************************/
std::ostream& operator <<(std::ostream &os, const CLine &rhs)
{
    CMove start = rhs.m_cline.first;
    CMove dir   = rhs.m_cline.second;
    CMove dir2  = dir+dir;
    CMove dir4  = dir2+dir2;
    CMove end   = start+dir4;
    return os << start << "-" << end;
} // end of operator <<


/***************************************************************
 * Reset board to initial position, ready for a new game
 ***************************************************************/
void CBoard::Reset() 
{
    for (int sq=0; sq<(SIZE+8)*(SIZE+8); sq++)
    {
        m_board[sq] = CPiece::t_piece::eInvalid;
    }
    for (int row=0; row<SIZE; row++)
    {
        for (int col=0; col<SIZE; col++)
        {
            CMove sq(row, col);
            m_board[sq.Sq()] = CPiece::t_piece::eEmpty;
        }
    }
    m_bWhite = true;

    m_legalMoves.clear();
    CMove firstMove(SIZE/2, SIZE/2);
    m_legalMoves.insert(firstMove);
    m_stack = {}; // clear

    m_lines.clear();
} // end of Reset


/***************************************************************
 * Display help and rules
 ***************************************************************/
void CBoard::Help(void) const
{
  std::cout << "\nWelcome to the game of Gomuko." << std::endl;
  std::cout << "The object of the game is to be the first player with" << std::endl;
  std::cout << "five pieces in a row. A row may be horizontal, vertical," << std::endl;
  std::cout << "and diagonal. A move consist of choosing a square." << std::endl;
} /* end of Help */


/***************************************************************
 * Perform a move
 ***************************************************************/
void CBoard::MakeMove(const CMove& move)
{
    static const std::vector<CMove> dirs = {
        CMove(SIZE+9), // NE
        CMove(SIZE+8), // N
        CMove(SIZE+7), // NW
        CMove(1),      // E
        CMove(-1),     // W
        CMove(-(SIZE+7)), // SE
        CMove(-(SIZE+8)), // S
        CMove(-(SIZE+9))  // SW
    };

    int16_t sq = move.Sq();

    assert(m_board[sq] == CPiece::t_piece::eEmpty);

    m_board[sq] = m_bWhite ? CPiece::t_piece::eWhite : CPiece::t_piece::eBlack;
    m_bWhite ^= 1;

    // Update list of legal moves
    std::vector<CMove> newLegalMoves;
    m_legalMoves.erase(move);
    for (CMove dir : dirs)
    {
        CMove newMove = move + dir;
        if (m_board[newMove.Sq()] == CPiece::t_piece::eEmpty)
        {
            std::pair<std::set<CMove>::iterator, bool> ret;
            ret = m_legalMoves.insert(newMove);
            if (ret.second)
                newLegalMoves.push_back(newMove);
        }
    } // end of for
    m_stack.push(newLegalMoves);

    // Update list of active lines
    UpdateLines(move);

} /* end of MakeMove */


/***************************************************************
 * Undo the move
 ***************************************************************/
void CBoard::UndoMove(const CMove& move)
{
    int16_t sq = move.Sq();

    m_bWhite ^= 1;
    assert(m_board[sq] == (m_bWhite ? CPiece::t_piece::eWhite : CPiece::t_piece::eBlack));

    m_board[sq] = CPiece::t_piece::eEmpty;

    // Update list of legal moves
    std::vector<CMove> newLegalMoves = m_stack.top();
    m_stack.pop();
    for (CMove legalMove : newLegalMoves)
    {
        m_legalMoves.erase(legalMove);
    }
    m_legalMoves.insert(move);

    // Update list of active lines
    UpdateLines(move);
} /* end of UndoMove */


/***************************************************************
 * Count the number of pieces in a line of five consecutive squares
 * Returns:  -9 if the line reaches outside the board
 *           =0 if the line contains pieces of both colors
 *           >0 the number of white pieces (no black pieces)
 *           <0 the number of black pieces (no white pieces)
 ***************************************************************/
int CBoard::countPieces(const CLine& line)
{
    const CMove& move = line.getStartPos();
    const CMove& dir = line.getDir();
    CMove here = move;
    int myCount = 0;
    int oppCount = 0;
    for (int i=0; i<5; ++i)
    {
        CPiece piece = m_board[here.Sq()];
        if (piece == CPiece::t_piece::eInvalid)
            return -9;
        if (piece == CPiece::t_piece::eWhite)
            myCount++;
        if (piece == CPiece::t_piece::eBlack)
            oppCount++;
        here += dir;
    }
    if (myCount > 0 && oppCount == 0)
        return myCount;
    if (oppCount > 0 && myCount == 0)
        return -oppCount;
    return 0;
} // end of countPieces


static const CMove halfDirs[] = {
    CMove(SIZE+9), // NE
    CMove(SIZE+8), // N
    CMove(SIZE+7), // NW
    CMove(1)       // E
};

/***************************************************************
 * Used in MakeMove and UndoMove
 ***************************************************************/
void CBoard::UpdateLines(const CMove& move)
{
    for (CMove dir : halfDirs)
    {
        CMove startPos = move;
        for (int i=0; i<5; ++i)
        {
            CLine line(startPos, dir);
            int val = countPieces(line);
            if (val != -9)
            {
                if (val == 0)
                    m_lines.erase(line);
                else
                    m_lines[line] = val;
            }
            startPos -= dir;
        }
    } // end of for

} // end of UpdateLines


/***************************************************************
 * Returns a list of all the possible legal moves in the current position
 ***************************************************************/
CBoard::moveList_t CBoard::GetLegalMoves() const
{
    moveList_t moveList;
    for (CMove move : m_legalMoves)
        moveList.push_back(move);
    return moveList;
} /* end of GetLegalMoves */


/***************************************************************
 * Returns the result of the current game
 ***************************************************************/
t_gameResult CBoard::GameResult() const
{
    for (const auto& kv : m_lines)
    {
        CBoard::lineVal_t val = kv.second;
        if (val == 5)
        {
            if (m_bWhite)
                return t_gameResult::eWon;
            else
                return t_gameResult::eLost;
        }
        if (val == -5)
        {
            if (m_bWhite)
                return t_gameResult::eLost;
            else
                return t_gameResult::eWon;
        }
    }
    if (m_legalMoves.size() == 0)
        return t_gameResult::eDraw;
    return t_gameResult::eStillPlaying;
} // end of GameResult


/***************************************************************
 * Returns a static evaluation of the current position
 ***************************************************************/
int32_t CBoard::Evaluate() const
{
    int bestWhite = 0;
    int bestBlack = 0;
    int numWhite = 0;
    int numBlack = 0;
    for (const auto& kv : m_lines)
    {
        CBoard::lineVal_t val = kv.second;
        if (val > bestWhite)
        {
            bestWhite = val;
            numWhite = 0;
        }
        if (val == bestWhite)
        {
            numWhite++;
        }
        if (val < bestBlack)
        {
            bestBlack = val;
            numBlack = 0;
        }
        if (val == bestBlack)
        {
            numBlack--;
        }
    }
    if (m_bWhite)
    {
        if (bestWhite >= -bestBlack)
            return bestWhite*100 + numWhite;
        else
            return bestBlack*100 + numBlack;
    } else {
        if (-bestBlack >= bestWhite)
            return -(bestBlack*100 + numBlack);
        else
            return -(bestWhite*100 + numWhite);
    }
} // end of Evaluate

