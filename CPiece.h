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
 * $Log: CPiece.h,v $
 * Revision 1.1  2015/07/07 06:57:58  mike
 * Cleanup.
 *
 ****************************************************************************/

/* CPiece.h */
 
#ifndef _CPIECE_H_
#define _CPIECE_H_

#include <ostream>

/***************************************************************
 * declaration of CPiece
 ***************************************************************/

class CPiece
{
    friend std::ostream& operator <<(std::ostream &os, const CPiece &rhs);
    friend class CBoard;

    public:
        // Types
        typedef enum class int8_t
        {
            eEmpty   =  0,
            eWhite   =  1,
            eBlack   = -1,
            eInvalid = -2
        } t_piece;

        // Constructors
        CPiece() : m_piece(t_piece::eInvalid) {}
        CPiece(t_piece piece) : m_piece(piece) {}

        // Accessors
        bool operator==(const CPiece& rhs) const
        {
            return m_piece == rhs.m_piece;
        }
        bool operator!=(const CPiece& rhs) const
        {
            return m_piece != rhs.m_piece;
        }

    private:
        t_piece m_piece;
}; // end of CPiece

#endif /* _CPIECE_H_ */

