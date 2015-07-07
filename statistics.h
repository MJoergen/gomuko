/*****************************************************************************
 *
 * Author:	Michael Joergensen
 *
 * Abstract:
 *
 * Global Contents:
 *
 * Local Contents:
 *
 * ---------------------------------------------------------------------------
 * Revision History:
 * $Log: statistics.h,v $
 * Revision 1.1  2015/07/05 09:05:56  mike
 * First version.
 *
 * Revision 1.1  2011/03/23 00:14:56  mij
 * Ported to gcc 4.5
 *
 * Revision 1.1  2011/03/21 21:36:11  mij
 * Checkin time.
 *
 ****************************************************************************/

/* statistics.h */

#ifndef _STATISTICS_H_
#define _STATISTICS_H_

/***************************************************************/

#include <ostream>
#include <stdint.h>

class CStatistics
{
  public:
    CStatistics() {Clear();}
    friend std::ostream& operator <<(std::ostream &os, const CStatistics &rhs);

    void Clear(void);

    uint32_t m_NodeCount;
    uint32_t m_EvalCount;
    uint32_t m_EndCount;
    uint32_t m_DrawCount;
    uint32_t m_TTLookup;
    uint32_t m_TTLookupFailed;
    uint32_t m_TTLookupSuccess;
    uint32_t m_TTBoundChange;
    uint32_t m_TTTooShallow;
    uint32_t m_TTWrite;
    uint32_t m_TTUpdate;
    uint32_t m_TTCollision;
    uint32_t m_TTReordering;
    uint32_t m_NegaScoutReSearch;
}; // end of CStatistics

#endif // _STATISTICS_H_

