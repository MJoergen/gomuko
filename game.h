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
 * $Log: game.h,v $
 * Revision 1.5  2015/07/05 14:21:42  mike
 * Checkin time.
 *
 * Revision 1.4  2015/07/05 11:53:57  mike
 * Checkin time.
 *
 * Revision 1.3  2015/07/05 11:40:41  mike
 * Cleanup.
 *
 * Revision 1.2  2015/07/05 10:29:59  mike
 * Cleanup.
 *
 * Revision 1.1  2015/07/05 09:05:55  mike
 * First version.
 *
 *
 ****************************************************************************/

/* game.h */

#ifndef _GAME_H_
#define _GAME_H_

#include <iostream>
#include <sstream>
#include "boardgame.h"
#include "computer.h"
#include "trace.h"

template <class T>
class Game
{
    public:
        void Play()
        {
            T board;
            CComputerPlayer< T > ai(board);

            ai.SetSecsPerMove(3);
            while (board.GameResult() == t_gameResult::eStillPlaying)
            {
                TRACE(std::endl);
                TRACE(board);
                typename T::move_t bestMove = ai.FindBestMove();
                TRACE(ai);
                board.MakeMove(bestMove);
            }
            TRACE(board);
            std::cout << std::endl << ai << board;
        }

        t_gameResult GameResult(T& boardGame)
        {
            return boardGame.GameResult();
        }

        void ShowLegalMoves(const T& boardGame)
        {
            typename T::moveList_t moveList = boardGame.GetLegalMoves();

            std::cout << "Legal moves: ";
            for (typename T::move_t move : moveList)
            {
                std::cout << move << " ";
            }
            std::cout << std::endl;
        }

        void Loop()
        {
            T board;
            CComputerPlayer< T > ai(board, 1024*1024);
            CStack<typename T::move_t> moveList;

            bool bHumanEnabled = true;
            bool bComputerEnabled = true;
            bool bHumanToPlay = true;

            while (1) {
                std::cout << board;

                std::string str;
                str = "go";
                if (bHumanToPlay)
                    getline(std::cin, str);

                if (str == "help") {
                    board.Help();
                    continue;
                }
                if (str == "?") {
                    std::cout << "?         : Show this guide" << std::endl;
                    std::cout << "help      : Show game information" << std::endl;
                    std::cout << "go        : Force computer to make a move (thus changing sides)" << std::endl;
                    std::cout << "legal     : Show legal moves" << std::endl;
                    std::cout << "manual    : Toggle manual (whether computer plays or not)" << std::endl;
                    std::cout << "match     : Let computer play against itself for the rest of the game" << std::endl;
                    std::cout << "undo      : Take back last move (thus changing sides)" << std::endl;
                    std::cout << "reset     : Reset game to starting position" << std::endl;
                    std::cout << "secs      : Set number of seconds per move" << std::endl;
                    std::cout << "depth     : Set maximum search depth" << std::endl;
                    std::cout << "drawscore : Set willingness to draw games" << std::endl;
                    std::cout << "stats     : Show statistics" << std::endl;
                    std::cout << "clear     : Clear statistics" << std::endl;
                    std::cout << "quit      : Quit the game" << std::endl;
                    continue;
                }
                if (str == "go") {
                    typename T::move_t move = ai.FindBestMove();
                    std::cout << "Playing " << move << std::endl;
                    moveList.Push(move);
                    board.MakeMove(move);

                    if (!(board.GameResult() == t_gameResult::eStillPlaying)) {
                        std::cout << "Game over" << std::endl;
                        bHumanEnabled = true;
                    }
                    if (bHumanEnabled)
                        bHumanToPlay = true;
                    continue;
                }
                if (str == "legal") {
                    ShowLegalMoves(board);
                    continue;
                }
                if (str == "manual") {
                    bComputerEnabled = not bComputerEnabled;
                    if (bComputerEnabled) {std::cout << "Now playing against cmoputer" << std::endl; }
                    else {std::cout << "Computer is now disabled" << std::endl; }
                    continue;
                }
                if (str == "match") {
                    bHumanEnabled = false;
                    bHumanToPlay = false;
                    continue;
                }
                if (str == "undo") {
                    typename T::move_t move = moveList.Pop();
                    board.UndoMove(move);
                    continue;
                }
                if (!str.compare(0, 4, "secs")) {
                    uint32_t secs = ai.GetSecsPerMove();
                    std::string arg = str;
                    arg.erase(0, 4);
                    std::stringstream myStream(arg);
                    if (myStream >> secs)
                    {
                        ai.SetSecsPerMove(secs);
                        continue;
                    }
                }
                if (!str.compare(0, 5, "depth")) {
                    uint32_t maxDepth = ai.GetMaxDepth();
                    std::string arg = str;
                    arg.erase(0, 5);
                    std::stringstream myStream(arg);
                    if (myStream >> maxDepth)
                    {
                        ai.SetMaxDepth(maxDepth);
                        continue;
                    }
                }
                if (!str.compare(0, 9, "drawscore")) {
                    uint32_t drawScore = ai.GetDrawScore();
                    std::string arg = str;
                    arg.erase(0, 9);
                    std::stringstream myStream(arg);
                    if (myStream >> drawScore)
                    {
                        ai.SetDrawScore(drawScore);
                        continue;
                    }
                }
                if (str == "stats") {
                    std::cout << ai;
                    continue;
                }
                if (str == "clearstats") {
                    ai.ClearStatistics();
                    continue;
                }
                if (str == "clearhash") {
                    ai.ClearHash();
                    continue;
                }
                if (str == "reset") {
                    while (moveList.Size())
                    {
                        typename T::move_t move = moveList.Pop();
                        board.UndoMove(move);
                    }
                    continue;
                }
                if (str == "quit") {
                    break;
                }

                // Input move from player
                typename T::move_t move(str);
                if (ai.bIsMoveLegal(move)) {
                    std::cout << "Playing " << move << std::endl;
                    moveList.Push(move);
                    board.MakeMove(move);

                    if (!(GameResult(board) == t_gameResult::eStillPlaying)) {
                        std::cout << "Game over" << std::endl;
                        break;
                    }

                    if (bComputerEnabled)
                        bHumanToPlay = false;
                    continue;
                }

                std::cout << "Did not understand: " << str << std::endl;
                std::cout << "Type ? for list of commands" << std::endl;
            } // end of while

            return;
        } // end of Loop

}; // end of Game

#endif // _GAME_H_

