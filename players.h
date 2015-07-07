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
 * $Log: players.h,v $
 * Revision 1.2  2015/07/05 11:40:42  mike
 * Cleanup.
 *
 * Revision 1.1  2015/07/05 09:05:56  mike
 * First version.
 *
 ****************************************************************************/

/* players.h */

#ifndef _PLAYERS_H_
#define _PLAYERS_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include "stack.h"
#include "boardgame.h"
#include "computer.h"


template <class T>
class Player
{
    public:
        virtual typename T::move_t GetMove(void) = 0;
        virtual typename T::move_t MakeMove(void)
        {
            typename T::move_t move = GetMove(); // Find our move
            m_board.MakeMove(move);
            m_moveList.Push(move);
            return move;
        }
        virtual typename T::move_t MakeMove(const typename T::move_t& move)
        {
            m_board.MakeMove(move); // Update board with opponents move
            m_moveList.Push(move);
            return MakeMove();
        }
        virtual void NewGame(void)
        {
            m_board.Reset();
            m_moveList.Reset();
        }
        virtual t_gameResult GameResult() const
        {
            return m_board.GameResult();
        }
        virtual void SetName(std::string str)
        {
            m_name = str;
        }
        virtual std::string GetName(void) const {return m_name; }

        friend std::ostream& operator <<(std::ostream &os, const Player<T> &rhs)
        {
            return os << rhs.m_board;
        }

    protected:
        T m_board;
        std::string m_name;
        CStack<typename T::move_t> m_moveList;
}; // end of class Player

template <class T>
class ComputerPlayerOptions
{
    public:
        ComputerPlayerOptions(
                std::string name,
                uint32_t secsPerMove,
                uint32_t maxDepth)
            :
                m_name(name),
                m_secsPerMove(secsPerMove),
                m_maxDepth(maxDepth)
    {}

        friend std::ostream& operator <<(std::ostream &os, const ComputerPlayerOptions<T> &rhs)
        {
            os << "Name = " << rhs.m_name << std::endl;
            os << "Seconds Per Move = " << rhs.m_secsPerMove << std::endl;
            os << "Maximum Search Depth = " << rhs.m_maxDepth << std::endl;
            return os;
        }

        std::string m_name;
        uint32_t m_secsPerMove;
        uint32_t m_maxDepth;

}; // end of ComputerPlayerOptions

template <class T>
class ComputerPlayer : public Player<T>
{
    public:
        ComputerPlayer<T> (const std::string& name="")
            : m_ai(Player<T>::m_board, 1024*1024)
            {
                Player<T>::SetName(name);
            }

        typename T::move_t GetMove(void)
        {
            LOG(Player<T>::m_board);

            TRACE(Player<T>::m_board);
            TRACE(Player<T>::m_board.Evaluate() << std::endl);

            return m_ai.FindBestMove();
        } // end of GetMove

        void NewGame(void)
        {
            Player<T>::NewGame();
            m_ai.ClearHash();
            m_ai.ClearStatistics();
        } // end of NewGame

        ComputerPlayerOptions<T> GetOptions() const
        {
            ComputerPlayerOptions<T> options(
                    Player<T>::GetName(),
                    m_ai.GetSecsPerMove(),
                    m_ai.GetMaxDepth()
                    );
            return options;
        } // end of WriteOptions

        void ReadOptions(std::string fileName)
        {
            std::ifstream myfile(fileName.c_str());
            if (myfile.is_open())
            {
                while (myfile.good())
                {
                    std::string str;
                    getline(myfile, str);

                    if (!str.compare(0, 4, "name")) {
                        std::string name = Player<T>::GetName();
                        std::string arg = str;
                        arg.erase(0, 4);
                        std::istringstream myStream(arg);
                        if (myStream >> name)
                        {
                            Player<T>::SetName(name);
                            continue;
                        }
                    }
                    if (!str.compare(0, 4, "secs")) {
                        uint32_t secs = m_ai.GetSecsPerMove();
                        std::string arg = str;
                        arg.erase(0, 4);
                        std::stringstream myStream(arg);
                        if (myStream >> secs)
                        {
                            m_ai.SetSecsPerMove(secs);
                            continue;
                        }
                    }
#ifdef ENABLE_HASH
                    if (!str.compare(0, 4, "hash")) {
                        uint32_t secs = m_ai.GetHashSize();
                        std::string arg = str;
                        arg.erase(0, 4);
                        std::stringstream myStream(arg);
                        if (myStream >> secs)
                        {
                            m_ai.SetHashSize(secs);
                            continue;
                        }
                    }
#endif
                    if (!str.compare(0, 5, "depth")) {
                        uint32_t maxDepth = m_ai.GetMaxDepth();
                        std::string arg = str;
                        arg.erase(0, 5);
                        std::stringstream myStream(arg);
                        if (myStream >> maxDepth)
                        {
                            m_ai.SetMaxDepth(maxDepth);
                            continue;
                        }
                    }
                } // end of while
            } // end of if
        } // end of ReadOptions

    private:
        CComputerPlayer<T> m_ai;
}; // end of class ComputerPlayer


template <class T>
class HumanPlayer : public Player<T>
{
  public:
    typename T::move_t GetMove(void)
    {
      std::string str;
      getline(std::cin, str);
      return str.c_str();
    } // end of FindMove

  private:
}; // end of class HumanPlayer

#endif // _PLAYERS_H_

