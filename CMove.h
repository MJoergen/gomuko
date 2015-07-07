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
 * $Log: CMove.h,v $
 * Revision 1.1  2015/07/07 06:57:58  mike
 * Cleanup.
 *
 ****************************************************************************/

/* CMove.h */
 
#ifndef _CMOVE_H_
#define _CMOVE_H_

#include <ostream>

/***************************************************************
 * declaration of CMove
 ***************************************************************/

class CMove
{
    friend std::ostream& operator <<(std::ostream &os, const CMove &rhs);

    friend CMove  operator+(CMove lhs, const CMove& rhs) 
    {
        return lhs += rhs;
    }

    public:
    // Constructors
        CMove() : m_sq(0x7fff) {}
        CMove(const std::string& str) {FromString(str);}
        CMove(int16_t sq) : m_sq(sq) {}
        CMove(int8_t row, int8_t col);

    // Accessors
        int16_t     Sq() const {return m_sq;}

        bool        operator==(const CMove& rhs) const
        {
            return m_sq == rhs.m_sq;
        }

        bool        operator<(const CMove& rhs) const
        {
            return m_sq < rhs.m_sq;
        }

    // Modifiers
        CMove&      operator+=(const CMove& rhs) 
        {
            m_sq += rhs.m_sq;
            return *this;
        }

        CMove&      operator-=(const CMove& rhs) 
        {
            m_sq -= rhs.m_sq;
            return *this;
        }

    private:
        void    FromString(const std::string& str);

        int16_t m_sq;
}; /* end of CMove */

#endif /* _CMOVE_H_ */

