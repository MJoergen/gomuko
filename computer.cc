/*****************************************************************************
 *
 * Author:      Michael Joergensen
 *
 * Abstract:
 *      This file contains the implementation of the generic
 *      computerized board playing agent.
 *
 *      It uses the alpha-beta search algorithm.
 *      With time, it will include transposition tables.
 *
 * Global Contents:
 *
 * Local Contents:
 *
 * ---------------------------------------------------------------------------
 * Revision History:
 * $Log: computer.cc,v $
 * Revision 1.6  2015/07/05 14:57:44  mike
 * Checkin time.
 *
 * Revision 1.5  2015/07/05 11:53:57  mike
 * Checkin time.
 *
 * Revision 1.4  2015/07/05 11:40:41  mike
 * Cleanup.
 *
 * Revision 1.3  2015/07/05 10:29:59  mike
 * Cleanup.
 *
 * Revision 1.2  2015/07/05 10:09:01  mike
 * Checkin time.
 *
 * Revision 1.1  2015/07/05 09:05:55  mike
 * First version.
 *
 ****************************************************************************/

/* computer.cc */

#include <iostream>
#include <time.h>
#include "trace.h"
 
template <class traits>
const char *CHashEntry<traits>::TypeString(void) const
{
  switch (m_type)
  {
    case exact : return "exact";
    case lower : return "lower";
    case upper : return "upper";
  }
  return "???";
} /* end of TypeString */

template <class traits>
uint32_t CHashTable<traits>::NumEntries(void) const
{
  uint32_t count=0;

  for (uint32_t i=0; i<m_size; i++)
    if (m_pHashEntry[i].Valid())
      count++;
  return count;
} /* end of HashEntries */

template <class traits>
void CHashTable<traits>::Clear(void)
{
  for (uint32_t i=0; i<m_size; i++)
  {
    m_pHashEntry[i].Clear();
  }
} /* end of ClearHashTable */

/***************************************************************
 * Implementation of CComputerPlayer
 ***************************************************************/

template <class traits>
void CComputerPlayer<traits>::AllocateMoveArray(uint32_t maxLegalMoves) 
{
  m_moveArray = new t_move *[MAX_LEVEL+2];
  for (int level=0; level<MAX_LEVEL+2; level++)
  {
    m_moveArray[level] = new t_move[maxLegalMoves];
  }
}

template <class traits>
void CComputerPlayer<traits>::FreeMoveArray()
{
  for (int level=0; level<MAX_LEVEL+2; level++)
  {
    delete [] m_moveArray[level];
  }
  delete [] m_moveArray;
}

template <class traits>
void CComputerPlayer<traits>::CopyPVFromHash(t_moveStack& bestLine,
    const t_move& move) const
{
  bestLine.Push(move);

  m_board.MakeMove(move);

  const CHashValue& hashValue = m_board.GetHashValue();
  bool found;
  const CHashEntry<traits>& hashEntry = m_hashTable.GetHashEntry(found, hashValue);
  if (found)
  {
    CopyPVFromHash(bestLine, hashEntry.Move());
  }

  m_board.UndoMove(move);
}

template <class traits>
bool CComputerPlayer<traits>::bIsMoveLegal(t_move& possibleMove) const
{
    t_moveList moveList = m_board.GetLegalMoves();

    for (t_move move : moveList)
    {
        if (move == possibleMove)
        {
            possibleMove = move;
            return true;
        }
    }
    return false;
}

template <class traits>
int32_t CComputerPlayer<traits>::AlphaBeta(t_moveStack& bestLine,
    int32_t alpha, int32_t beta, uint32_t level, bool bPrincipal)
{
  int32_t bestVal;

  /* Increment statistics */
  m_statistics.m_NodeCount++;

  if (m_statistics.m_NodeCount - m_LastCount > SLEEP_COUNT)
  {
    if (UpdateLine())
    {
      /* We must abort search now. */

      /* Return a lower bound. */
      return alpha;
    }
    m_LastCount = m_statistics.m_NodeCount;
  }

  /* Get all the legal moves in the current position */
  t_moveList legalMoves = m_board.GetLegalMoves();
  uint32_t numLegal = legalMoves.size();

  /* Is the game over? */
  switch (m_board.GameResult())
  {
      case t_gameResult::eWon  :
          m_statistics.m_EndCount++;

          bestLine.Reset();
          bestVal = GAME_OVER - (m_Level - level);

          TRACE( (bPrincipal ? "P  " : "-  ") << m_moveList << ": eval=" << bestVal << " (Game Over)" << std::endl);

          return bestVal;
      case t_gameResult::eLost :
          m_statistics.m_EndCount++;

          bestLine.Reset();
          bestVal = -GAME_OVER + (m_Level - level);

          TRACE( (bPrincipal ? "P  " : "-  ") << m_moveList << ": eval=" << bestVal << " (Game Over)" << std::endl);

          return bestVal;
      case t_gameResult::eDraw :
          m_statistics.m_DrawCount++;

          bestLine.Reset();
          bestVal = m_DrawScore;

          TRACE( (bPrincipal ? "P  " : "-  ") << m_moveList << ": eval=" << bestVal << " (Draw)" << std::endl);

          return bestVal;
      case t_gameResult::eStillPlaying :
          break;
  }

  assert(numLegal != 0);

  /* Are we at the leaf of the tree? */
  if (level==0)
  {
    m_statistics.m_EvalCount++;

    bestLine.Reset();
    bestVal = m_board.Evaluate();

    TRACE( (bPrincipal ? "P  " : "-  ") << m_moveList << ": eval=" << bestVal << " (Static)" << std::endl);

    return bestVal;
  }

#ifdef ENABLE_HASH
  m_statistics.m_TTLookup++;

  const CHashValue& hashValue = m_board.GetHashValue();

  bool found;
  t_hashEntry& hashEntry = m_hashTable.GetHashEntry(found, hashValue);
  if (!found)
  {
    m_statistics.m_TTLookupFailed++;
  }
  else
  {
    TRACE("Found hash entry: " << hashEntry << std::endl);

    bestVal = hashEntry.Val();
    const t_move& move = hashEntry.Move();

    assert (bIsMoveLegal((t_move &)move));

    /* If the level is good enough, */
    /* we can trust the value in the hash table. */
    if (hashEntry.Level() >= level)
    {
      m_statistics.m_TTLookupSuccess++;

      bestLine.Reset();
      CopyPVFromHash(bestLine, move);

      /* Try to narrow the search window */
      switch (hashEntry.Type())
      {
        case lower:
          if (bestVal > alpha)
            alpha = bestVal;
          if (alpha >= beta)
          {
            TRACE("Window closed, returning lower bound " << alpha << std::endl);
            return alpha;
          }
          break;
        case upper:
          if (bestVal < beta)
            beta = bestVal;
          if (alpha >= beta)
          {
            TRACE("Window closed, returning upper bound " << beta << std::endl);
            return beta;
          }
          break;
        case exact:
          {
            TRACE("Returning exact value " << bestVal << std::endl);
            return bestVal;
          }
      } /* end of switch */

      m_statistics.m_TTBoundChange++;
    }
    else
      m_statistics.m_TTTooShallow++;

    /* Search the move in the hash table first. */
    for (uint32_t i=1; i<numLegal; i++)
    {
      if (move == legalMoves[i])
      {
        m_statistics.m_TTReordering++;

        TRACE("Move reordering.");

        t_move tempMove = legalMoves[0];
        legalMoves[0] = legalMoves[i];
        legalMoves[i] = tempMove;
        break;
      }
    }

  }
#endif

  /* Get first legal move */
  t_move& move = legalMoves[0];

  /* Temporary storage of Principal Variation */
  t_moveStack& bestLineSoFar = m_moveStack[level];

  /* Do recursive search */
  m_board.MakeMove(move);
  DEBUG( ((t_moveStack&) m_moveList).Push(move); )
  bestVal = -AlphaBeta(bestLineSoFar, -beta, -alpha, level-1, bPrincipal);
  DEBUG( ((t_moveStack&) m_moveList).Pop(); )
  m_board.UndoMove(move);

  /* Update the current PV */
  bestLine = move;
  bestLine += bestLineSoFar;

  /* Save original value of alpha for later use. */
  int32_t alphaOrig = alpha;

  /* Iterate through all remaining moves */
  for (uint32_t i=1; i<numLegal && bestVal < beta; i++)
  {
    /* Narrow the search window */
    if (bestVal > alpha)
    {
      alpha = bestVal;
    }

    /* Get next legal move */
    move = legalMoves[i];

    /* Do recursive searh */
    m_board.MakeMove(move);
    DEBUG( ((t_moveStack&) m_moveList).Push(move); )

    /* First try a NULL window */
    int32_t val = -AlphaBeta(bestLineSoFar, -alpha-1, -alpha, level-1, 0);
    if (val > alpha && val < beta)
    {
      m_statistics.m_NegaScoutReSearch++;
      TRACE((bPrincipal ? "P  " : "-  ") << m_moveList << ": Researching. alpha=" << alphaOrig <<
            ", val=" << bestVal << ", beta=" << beta << std::endl);

      /* OK. This was a fail low. Try again with the full window. */
      val = -AlphaBeta(bestLineSoFar, -beta, -alpha, level-1, bPrincipal);
    }
    DEBUG( ((t_moveStack&) m_moveList).Pop(); )
    m_board.UndoMove(move);

    if (val > bestVal)
    {
      bestVal = val;

      /* Update the current PV */
      bestLine = move;
      bestLine += bestLineSoFar;
    }
  } /* end of for */

  /* Either we've searched all moves, or there was a fail high */
  if (bestVal >= beta)
  {
    TRACE((bPrincipal ? "P  " : "-  ") << m_moveList << ": alpha=" << alphaOrig << 
        ", eval=" << bestVal << ", beta=" << beta << " (Fail high)" << std::endl);
  }
  else if (bestVal <= alphaOrig)
  {
    TRACE((bPrincipal ? "P  " : "-  ") << m_moveList << ": alpha=" << alphaOrig << 
        ", eval=" << bestVal << ", beta=" << beta << " (Fail low)" << std::endl);
  }
  else
  {
    TRACE((bPrincipal ? "P  " : "-  ") << m_moveList << ": alpha=" << alphaOrig << 
        ", eval=" << bestVal << ", beta=" << beta << " (Exact)" << std::endl);
  }

#ifdef ENABLE_HASH
  {
    bool update = false;

    m_statistics.m_TTWrite++;
    if (hashEntry.Valid())
    {
      if (hashEntry.GetHashValue() == hashValue)
      {
        m_statistics.m_TTUpdate++;
        update = true;
      }
      else
        m_statistics.m_TTCollision++;
    }

    if (bestVal <= alphaOrig)
    {
      hashEntry = t_hashEntry(hashValue, bestVal+1, level, upper, bestLine[0]);
    }
    else if (bestVal >= beta)
    {
      hashEntry = t_hashEntry(hashValue, bestVal-1, level, lower, bestLine[0]);
    }
    else
    {
      hashEntry = t_hashEntry(hashValue, bestVal, level, exact, bestLine[0]);
    }

    TRACE("Hash entry " << (update ? "updated" : "written") << ": " << hashEntry << std::endl);
  }
#endif

  return bestVal;
} /* end of AlphaBeta */

template <class traits>
bool CComputerPlayer<traits>::UpdateLine(void) const
{
  const time_t CurTime = time(0);
  if (CurTime > m_LastTime)
  {
    ((CComputerPlayer *) this)->m_LastTime = CurTime;
    PrintLine(t_lineType::lineCurrent);
  }
  sleep(0);
  if (CurTime > m_EndTime && m_bPrincipal)
    return true;
  return false;
}

template <class traits>
void CComputerPlayer<traits>::PrintLine(t_lineType lineType) const
{
    if (!m_bPrintLine)
        return;

    char moveTimeStr[15];
    {
        uint32_t moveTimeSecs = time(0) - m_StartTime;
        uint32_t moveTimeMins = moveTimeSecs/60;
        moveTimeSecs -= moveTimeMins * 60;

        sprintf(moveTimeStr, "%01u:%02u", moveTimeMins, moveTimeSecs);
    }

    switch (lineType)
    {
        case t_lineType::lineCurrent:
            if (m_Level >= m_OutputThresholdLevel)
            {
                if (!gpLog)
                {
                    std::cout << m_Level << " " << moveTimeStr << " " << m_CurMoveNumber+1
                        << "/" << m_NumMoves << "  " << m_Move << "\r";
                }
            }
            break;
        case t_lineType::lineBetter:
            TRACE(m_Level << " " << m_CurMoveNumber+1 << "/" << m_NumMoves << ": " << m_currentValue << " ++ " << m_Move << std::endl);
            if (m_Level >= m_OutputThresholdLevel)
            {
                LOG(m_Level << " " << moveTimeStr << "  ++  " << m_Move << std::endl);
            }
            break;
        case t_lineType::lineWorse:
            TRACE(m_Level << " " << m_CurMoveNumber+1 << "/" << m_NumMoves << ": " << m_currentValue << " -- " << m_Move << std::endl);
            if (m_Level >= m_OutputThresholdLevel)
            {
                LOG(m_Level << " " << moveTimeStr << "  --  " << m_Move << std::endl);
            }
            break;
        case t_lineType::lineGood:
            TRACE(m_Level << " " << m_CurMoveNumber+1 << "/" << m_NumMoves << ": " << m_currentValue << "    " << m_Move << std::endl);

            if (m_Level >= m_OutputThresholdLevel)
            {
                LOG(m_Level << " " << moveTimeStr << " " << m_currentValue
                        << "  " << m_Move << " " << m_bestLine << std::endl);
            }
            break;
        case t_lineType::lineBad:
            TRACE(m_Level << " " << m_CurMoveNumber+1 << "/" << m_NumMoves << ": " << m_currentValue << "    " << m_Move << std::endl);
            break;
    } // end of switch

} // end of PrintLine

template <class traits>
typename traits::move_t CComputerPlayer<traits>::FindBestMove(void)
{
  TRACE_FUNCTION("FindBestMove");

  TRACE(m_board);

  /* Temporary list of all legal moves in current position */
  t_moveList legalMoves = m_board.GetLegalMoves();
  m_NumMoves = legalMoves.size();

  int32_t     bestVal = m_board.Evaluate();
  m_LastCount = 0;

  TRACE("numMoves=" << m_NumMoves << std::endl);

  m_StartTime = time(0);
  m_EndTime = m_StartTime + m_SecsPerMove;

  m_Level = 1;
  while (m_Level<=m_MaxDepth)
  {
    int32_t  alpha = bestVal - 30;
    int32_t  beta = bestVal + 30;
    uint32_t numBestMoves = 0;

    bool     m_bPrincipal = true;
    for (m_CurMoveNumber=0;
        m_CurMoveNumber<m_NumMoves && time(0) < m_EndTime;
        m_CurMoveNumber++)
    {
      m_Move = legalMoves[m_CurMoveNumber];
      
      DEBUG( ((t_moveStack&) m_moveList).Push(m_Move); )

      PrintLine(t_lineType::lineCurrent);

      TRACE(m_Move << std::endl);

      m_board.MakeMove(m_Move);

      m_currentValue =
        -AlphaBeta(m_bestLine, -beta, -alpha, m_Level, m_bPrincipal);
      if (m_currentValue <= alpha)
      {
        if (m_CurMoveNumber==0 && time(0) < m_EndTime)
        {
          m_bPrincipal = true;
          PrintLine(t_lineType::lineWorse);

          m_currentValue =
            -AlphaBeta(m_bestLine, -alpha-1, LARGE_EVAL,
                m_Level, true);
        }
      }
      else if (m_currentValue >= beta)
      {
        if (time(0) < m_EndTime)
        {
          m_bPrincipal = true;
          PrintLine(t_lineType::lineBetter);
          m_currentValue =
            -AlphaBeta(m_bestLine, -LARGE_EVAL, -beta+1, m_Level, true);
        }
      }
      DEBUG( ((t_moveStack&) m_moveList).Pop(); )
      m_board.UndoMove(m_Move);

      if (m_currentValue > alpha && m_currentValue < beta)
      {
//        m_board.UpdateWeights(m_currentValue);
      }

      if (m_CurMoveNumber == 0)
        bestVal = m_currentValue;

      if (m_currentValue >= bestVal)
      {
        if (m_currentValue == bestVal)
          numBestMoves++;
        else
        {
          numBestMoves=1;
          bestVal = m_currentValue;
        }

        if (rand() % numBestMoves == 0)
        {
          if (!(m_CurMoveNumber==0 && time(0) >= m_EndTime))
          {
            PrintLine(t_lineType::lineGood);
          }

          const t_move tempMove = legalMoves[0];
          legalMoves[0] = legalMoves[m_CurMoveNumber];
          legalMoves[m_CurMoveNumber] = tempMove;
        }
      }
      else
      {
        PrintLine(t_lineType::lineBad);
      }

      alpha = bestVal-1;
      beta = bestVal+1;
      m_bPrincipal = false;
    }

    m_Level+=2;
  }
  LOG(std::endl);

//  m_board.FinalUpdateWeights();

  return legalMoves[0];
} /* end of FindBestMove */

template <class T>
std::ostream& operator <<(std::ostream &os, const CComputerPlayer <T> &rhs)
{
  os << rhs.m_statistics;
  return os << "Valid               = " << rhs.NumHashEntries() << std::endl;
}

