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
 * $Log: computer.h,v $
 * Revision 1.5  2015/07/05 14:57:44  mike
 * Checkin time.
 *
 * Revision 1.4  2015/07/05 11:53:57  mike
 * Checkin time.
 *
 * Revision 1.3  2015/07/05 11:40:41  mike
 * Cleanup.
 *
 * Revision 1.2  2015/07/05 10:09:01  mike
 * Checkin time.
 *
 * Revision 1.1  2015/07/05 09:05:55  mike
 * First version.
 *
 * Revision 1.8  2011/04/11 21:38:17  mij
 * Checkin time.
 *
 * Revision 1.7  2011/04/11 21:19:07  mij
 * CHeckin time.
 *
 * Revision 1.6  2011/03/29 08:10:35  mij
 * Added clearhash
 *
 * Revision 1.5  2011/03/26 20:33:30  mij
 * Minor bug fixes.
 *
 * Revision 1.4  2011/03/24 14:09:46  mij
 * Removed ValidateMove again ;-)
 *
 * Revision 1.3  2011/03/24 14:00:09  mij
 * Added maxDepth
 *
 * Revision 1.2  2011/03/24 13:23:06  mij
 * Added ValidateMove
 *
 * Revision 1.1  2011/03/23 00:14:56  mij
 * Ported to gcc 4.5
 *
 ****************************************************************************/

/* computer.h */

#ifndef _COMPUTER_H_
#define _COMPUTER_H_

/***************************************************************/

#include <stdio.h>
#include <string.h>

#include "boardgame.h"
#include "stack.h"
#include "statistics.h"
#include "hash.h"

#define MAX_LEVEL                       16
#define LARGE_EVAL                      32765
#define DEFAULT_HASH_TABLE_SIZE         (1<<11)
#define DEFAULT_SECS_PER_MOVE           2
#define DEFAULT_OUTPUT_THRESHOLD_LEVEL  0 
#define SLEEP_COUNT                     0

typedef enum
{
  lineBetter,
  lineWorse,
  lineGood,
  lineBad,
  lineCurrent
} t_lineType;

/*
 * declaration of CComputerPlayer
 */

template <class traits>
class CComputerPlayer
{
  public:
    typedef     typename traits::move_t t_move;
    typedef     typename traits::moveList_t t_moveList;
    typedef     CStack<t_move> t_moveStack;
    typedef     CHashEntry<traits> t_hashEntry;

    CComputerPlayer(traits& board,
        uint32_t hashTableSize = DEFAULT_HASH_TABLE_SIZE) :
      m_board(board), m_Level(1), m_DrawScore(0),
      m_hashTable(hashTableSize),
      m_SecsPerMove(DEFAULT_SECS_PER_MOVE),
      m_MaxDepth(MAX_LEVEL),
      m_bPrintLine(true)
    {
      AllocateMoveArray(board.MaxLegalMoves());
      m_moveStack = new t_moveStack[MAX_LEVEL+1]; // (MAX_LEVEL); TBD use std::vector
    }
    ~CComputerPlayer()
    {
      delete [] m_moveStack;
      FreeMoveArray();
    }

    template <class T>
    friend std::ostream& operator <<(std::ostream &os, const CComputerPlayer<T> &rhs);

    void        SetSecsPerMove(uint32_t level) {m_SecsPerMove = level;}
    uint32_t    GetSecsPerMove(void) const {return m_SecsPerMove;}

    void        SetMaxDepth(uint32_t maxDepth) {m_MaxDepth = maxDepth;}
    uint32_t    GetMaxDepth(void) const {return m_MaxDepth;}

    void        SetDrawScore(int32_t drawScore) {m_DrawScore = drawScore;}
    int32_t     GetDrawScore(void) const {return m_DrawScore;}

#ifdef ENABLE_HASH
    void        SetHashSize(uint32_t hashSize) {m_HashSize = hashSize;}
    uint32_t    GetHashSize(void) const {return m_HashSize;}
#endif // ENABLE_HASH

    void        SetLinePrinting(bool bPrintLine) {m_bPrintLine = bPrintLine;}
    bool        GetLinePrinting(void) const {return m_bPrintLine;}

    void        ClearStatistics(void) {m_statistics.Clear();}
    void        ClearHash(void)       {m_hashTable.Clear();}

    uint32_t    NumHashEntries(void) const {return m_hashTable.NumEntries();}

    bool        bIsMoveLegal(t_move& possibleMove) const;
    t_move      FindBestMove(void);

  private:

    /* These variables are known to the outside world. */
    traits&     m_board;
    uint32_t    m_Level;
    int32_t     m_DrawScore;
    CStatistics m_statistics;
    CHashTable<traits>  m_hashTable;

    /* These variables are purely internal. */
    static const uint32_t m_OutputThresholdLevel = DEFAULT_OUTPUT_THRESHOLD_LEVEL;
#ifdef ENABLE_HASH
    uint32_t      m_HashSize;
#endif // ENABLE_HASH
    uint32_t      m_SecsPerMove;
    uint32_t      m_MaxDepth;
    time_t        m_StartTime;
    time_t        m_EndTime;
    time_t        m_LastTime;
    uint32_t      m_LastCount;
    bool          m_bPrintLine;
    bool          m_bPrincipal;
    uint32_t      m_CurMoveNumber;
    int32_t       m_currentValue;
    uint32_t      m_NumMoves;
    t_move        m_Move;
    t_move        **m_moveArray;
    t_moveStack   *m_moveStack;
    t_moveStack   m_bestLine;
#ifdef ENABLE_TRACE
    t_moveStack   m_moveList;
#endif

    /* These functions are purely internal. */
    void        AllocateMoveArray(uint32_t);
    void        FreeMoveArray(void);
    bool        UpdateLine(void) const;
    void        PrintLine(t_lineType) const;
    void        CopyPVFromHash(t_moveStack& bestLine, const t_move& move) const;
    int32_t     AlphaBeta(t_moveStack& bestLine, int32_t alpha, int32_t beta, uint32_t level, bool bPrincipal);
}; /* end of CComputerPlayer */

/* Include template definitions. */
#include "computer.cc"

#endif /* _COMPUTER_H_ */

