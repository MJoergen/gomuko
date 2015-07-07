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
 * $Log: statistics.cc,v $
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

/* statistics.cc */

#include "statistics.h"

std::ostream& operator <<(std::ostream &os, const CStatistics &rhs)
{
  os << "NodeCount           = " << rhs.m_NodeCount << std::endl;
  os << "EvalCount           = " << rhs.m_EvalCount << std::endl;
  os << "EndCount            = " << rhs.m_EndCount << std::endl;
  os << "DrawCount           = " << rhs.m_DrawCount << std::endl;
  os << "m_TTLookup          = " << rhs.m_TTLookup << std::endl;
  os << "m_TTLookupFailed    = " << rhs.m_TTLookupFailed << std::endl;
  os << "m_TTLookupSuccess   = " << rhs.m_TTLookupSuccess << std::endl;
  os << "m_TTBoundChange     = " << rhs.m_TTBoundChange << std::endl;
  os << "m_TTTooShallow      = " << rhs.m_TTTooShallow << std::endl;
  os << "m_TTWrite           = " << rhs.m_TTWrite << std::endl;
  os << "m_TTUpdate          = " << rhs.m_TTUpdate << std::endl;
  os << "m_TTCollision       = " << rhs.m_TTCollision << std::endl;
  os << "m_TTReordering      = " << rhs.m_TTReordering << std::endl;
  os << "m_NegaScoutReSearch = " << rhs.m_NegaScoutReSearch << std::endl;
  return os;
}

void CStatistics::Clear(void)
{
  m_NodeCount = 0;
  m_EvalCount = 0;
  m_EndCount = 0;
  m_DrawCount = 0;
  m_TTLookup = 0;
  m_TTLookupFailed = 0;
  m_TTLookupSuccess = 0;
  m_TTBoundChange = 0;
  m_TTTooShallow = 0;
  m_TTWrite = 0;
  m_TTUpdate = 0;
  m_TTCollision = 0;
  m_TTReordering = 0;
  m_NegaScoutReSearch = 0;
} // end of Clear

