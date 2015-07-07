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
 * $Log: match.h,v $
 * Revision 1.3  2015/07/05 11:53:58  mike
 * Checkin time.
 *
 * Revision 1.2  2015/07/05 11:40:42  mike
 * Cleanup.
 *
 * Revision 1.1  2015/07/05 09:05:56  mike
 * First version.
 *
 * Revision 1.13  2011/04/23 23:32:05  mij
 * Fixed logging errors.
 *
 * Revision 1.12  2011/04/23 23:25:20  mij
 * Fixed logging errors.
 *
 * Revision 1.11  2011/04/23 23:10:44  mij
 * Improved result logging.
 *
 * Revision 1.10  2011/04/11 21:38:17  mij
 * Checkin time.
 *
 * Revision 1.9  2011/04/11 21:19:07  mij
 * CHeckin time.
 *
 * Revision 1.8  2011/04/04 22:57:18  mij
 * Added estimated completion time.
 *
 * Revision 1.7  2011/04/02 21:08:08  mij
 * Added PlayMatch
 *
 * Revision 1.6  2011/04/01 19:33:10  mij
 * Bugfix.
 *
 * Revision 1.5  2011/04/01 18:53:54  mij
 * Now Play() returns game result.
 *
 * Revision 1.4  2011/03/30 20:52:50  mij
 * Bugfixes.
 *
 * Revision 1.3  2011/03/30 20:33:08  mij
 * Added possibility to set evaluation function parameters.
 *
 * Revision 1.2  2011/03/29 09:47:35  mij
 * Bugfixes.
 *
 * Revision 1.1  2011/03/29 09:13:29  mij
 * First version of match.
 *
 ****************************************************************************/

/* match.h */

#ifndef _MATCH_H_
#define _MATCH_H_

#include <iostream>
#include <string>
#include "boardgame.h"
#include "stack.h"
#include "computer.h"
#include "players.h"


template <class T>
class Match
{
  public:
    void SetPlayer1(Player<T> *pPlayer) {m_pPlayer1 = pPlayer;}
    void SetPlayer2(Player<T> *pPlayer) {m_pPlayer2 = pPlayer;}
    int Play();
    int PlayGame(Player<T> &player1, Player<T> &player2);
    float PlayMatch(Player<T> *pPlayer1, Player<T> *pPlayer2, uint32_t numGames);

  private:
    Player<T> *m_pPlayer1;
    Player<T> *m_pPlayer2;
}; // end of class Match


template <class T>
int Match<T>::PlayGame(Player<T> &player1, Player<T> &player2)
{
    player1.NewGame();
    player2.NewGame();

    typename T::move_t move;

    move = player1.MakeMove();

    while (1)
    {
        LOG("Player " << player1.GetName() << " plays move " << move << std::endl << std::endl);

        switch (player1.GameResult())
        {
            case t_gameResult::eWon:
                LOG(player1);
                LOG("Player " << player2.GetName() << " WON" << std::endl << std::endl);
                return -1;
            case t_gameResult::eLost:
                LOG(player1);
                LOG("Player " << player1.GetName() << " WON" << std::endl << std::endl);
                return 1;
            case t_gameResult::eDraw:
                LOG(player1);
                LOG("Game ended in a DRAW" << std::endl);
                return 0;
            case t_gameResult::eStillPlaying:
                break;
        } // end of switch

        move = player2.MakeMove(move);
        LOG("Player " << player2.GetName() << " plays move " << move << std::endl << std::endl);

        switch (player2.GameResult())
        {
            case t_gameResult::eWon:
                LOG(player2);
                LOG("Player " << player1.GetName() << " WON" << std::endl << std::endl);
                return 1;
            case t_gameResult::eLost:
                LOG(player2);
                LOG("Player " << player2.GetName() << " WON" << std::endl << std::endl);
                return -1;
            case t_gameResult::eDraw:
                LOG(player2);
                LOG("Game ended in a DRAW" << std::endl);
                return 0;
            case t_gameResult::eStillPlaying:
                break;
        } // end of switch

        move = player1.MakeMove(move);
    } // end of while
} // end of Match::PlayGame

template <class T>
float Match<T>::PlayMatch(Player<T> *pPlayer1, Player<T> *pPlayer2, uint32_t numGames)
{
  const time_t startTime = time(0);

  int results_a[3] = {0,0,0};
  int results_b[3] = {0,0,0};
  for (unsigned int i=0; i<numGames; ++i)
  {
    time_t curTime = time(0);
    if (i>0)
    {
      time_t endTime = ((curTime - startTime)*numGames)/i + startTime;
      if (!gpLog)
      {
        std::cout << "Estimated completion at " << ctime(&endTime) << std::endl;
      }
    }
    LOG("Starting game " << i+1 << "/" << numGames << std::endl);

    int result;
    if ((i%2) == 0)
    {
      result = PlayGame(*pPlayer1, *pPlayer2);
      results_a[result+1]++;
    }
    else
    {
      result = PlayGame(*pPlayer2, *pPlayer1);
      results_b[result+1]++;
    }

  }
  LOG("Total score:" << std::endl);
  LOG(pPlayer1->GetName() << " vs " << pPlayer2->GetName());
  LOG(": +" << results_a[2] << " =" << results_a[1] << " -" << results_a[0] << std::endl);
  LOG(pPlayer2->GetName() << " vs " << pPlayer1->GetName());
  LOG(": +" << results_b[2] << " =" << results_b[1] << " -" << results_b[0] << std::endl);
  float player1 = (results_a[2]+results_b[0] - results_a[0]-results_b[2])/(1.0*numGames);
  LOG("Advantage for player " << pPlayer1->GetName() << " = " << player1 << std::endl);
  float player_first = (results_a[2]+results_b[2] - results_a[0]-results_b[0])/(1.0*numGames);
  LOG("Advantage for first player = " << player_first << std::endl);
  return player1;
} // end of PlayMatch

#endif // _MATCH_H_

