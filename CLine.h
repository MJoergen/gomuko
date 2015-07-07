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
 * $Log: CLine.h,v $
 * Revision 1.2  2015/07/07 15:56:55  mike
 * Optimization
 *
 * Revision 1.1  2015/07/07 06:57:58  mike
 * Cleanup.
 *
 ****************************************************************************/

/* CLine.h */
 
#ifndef _CLINE_H_
#define _CLINE_H_

#include <ostream>

/***************************************************************
 * declaration of CLine
 ***************************************************************/

union CLine
{
    friend std::ostream& operator <<(std::ostream &os, const CLine &rhs);

    public:
        // Constructors
        CLine(CMove start, CMove dir) : m_cline(start, dir) {}

        // Accessors
        bool operator==(const CLine& rhs) const
        {
            return (m_word == rhs.m_word);
        }
        bool operator<(const CLine& rhs) const
        {
            return (m_word < rhs.m_word);
        }
        const CMove& getStartPos() const { return m_cline.first; }
        const CMove& getDir() const { return m_cline.second; }

        // Modifiers
        CLine& operator=(const CLine& rhs) {
            m_word = rhs.m_word;
            return *this;
        }

    private:
        uint32_t                m_word;  // Used for comparison
        std::pair<CMove, CMove> m_cline; // Start position, direction
}; /* end of CLine */

#endif /* _CLINE_H_ */

