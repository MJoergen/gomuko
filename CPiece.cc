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
 * $Log: CPiece.cc,v $
 * Revision 1.1  2015/07/07 06:57:58  mike
 * Cleanup.
 *
 ****************************************************************************/

/* CPiece.cc */

#include "CPiece.h"

/***************************************************************
 * Display CPiece to output stream
 ***************************************************************/
std::ostream& operator <<(std::ostream &os, const CPiece &rhs)
{
    switch (rhs.m_piece)
    {
        case CPiece::t_piece::eEmpty   : os << "."; break;
        case CPiece::t_piece::eWhite   : os << "X"; break;
        case CPiece::t_piece::eBlack   : os << "O"; break;
        case CPiece::t_piece::eInvalid : os << "?"; break;
    }
    return os;
} // end of operator <<



