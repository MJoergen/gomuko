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
 * $Log: CMove.cc,v $
 * Revision 1.1  2015/07/07 06:57:58  mike
 * Cleanup.
 *
 ****************************************************************************/

/* CMove.cc */

#include "CMove.h"
#include "CBoard.h"

/***************************************************************
 * Constructor
 ***************************************************************/
CMove::CMove(int8_t row, int8_t col)
{
    m_sq = (row+4)*(SIZE+8) + (col+4);
}

/***************************************************************
 * Convert to string
 ***************************************************************/
std::ostream& operator <<(std::ostream &os, const CMove &rhs)
{
    char c = 'a' + (rhs.m_sq%(SIZE+8)) - 4;
    char r = '1' + (rhs.m_sq/(SIZE+8)) - 4;
    return os << c << r;
} // end of operator <<


/***************************************************************
 * Read from string
 ***************************************************************/
void CMove::FromString(const std::string& str)
{
    int8_t col = str[0] - 'a';
    int8_t row = str[1] - '1';
    m_sq = (row+4)*(SIZE+8) + (col+4);
} // end of FromString


