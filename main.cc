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
 * $Log: main.cc,v $
 * Revision 1.2  2015/07/05 10:09:01  mike
 * Checkin time.
 *
 * Revision 1.1  2015/07/05 09:05:55  mike
 * First version.
 *
 ****************************************************************************/

/* main.cc */

#include <iostream>
#include <stdlib.h>

#include "computer.h"
#include "game.h"
#include "match.h"
#include "players.h"
#include "CBoard.h"
#include "trace.h"
#include "CRand.h"

#ifdef ENABLE_TRACE
std::ostream *gpTrace = 0;
#endif
std::ostream *gpLog = &std::cout;

typedef struct
{
  uint32_t matchMode;
} t_options;

const t_options DEFAULT_OPTIONS = {
  0,     // matchMode
};

static t_options globalOptions = DEFAULT_OPTIONS;

int main(int argc, char *argv[])
{
    // Initialize
    std::cout.setf(std::ios::unitbuf);
    srand(time(0));

    ComputerPlayer<CBoard> player1("Player 1");
    ComputerPlayer<CBoard> player2("Player 2");

    // Parse command line options
    while (1)
    {
        char opt = getopt(argc, argv, "m:t:f:s:l:r:");
        if (opt == -1)
        {
            break;
        }
        switch (opt)
        {
            case 'm' : globalOptions.matchMode = atoll(optarg); break;
#ifdef ENABLE_TRACE
            case 't' : gpTrace = new CTrace(optarg); break;
#else
            case 't' : std::cout << "Trace not supported" << std::endl; break;
#endif
            case 'f' : player1.ReadOptions(optarg); break;
            case 's' : player2.ReadOptions(optarg); break;
            case 'l' : gpLog = new CTrace(optarg); break;
            case 'r' : srand(atol(optarg)); break;
            default  : 
                       std::cout << "Usage: " << argv[0] << " <options>" << std::endl;
                       std::cout << "-m <number> : Select matchMode and play <number> games." << std::endl;
                       std::cout << "-t <file>   : Dump trace to file." << std::endl;
                       std::cout << "-f <file>   : Read options for first player." << std::endl;
                       std::cout << "-s <file>   : Read options for second player." << std::endl;
                       std::cout << "-l <file>   : Dump games to log file." << std::endl;
                       std::cout << "-r <number> : Initialize random seed." << std::endl;
                       exit(-1);
        } // end of switch
    } // end of while

    if (globalOptions.matchMode > 0)
    {
        Match<CBoard> match;
        LOG("Player 1:" << std::endl);
        LOG(player1.GetOptions() << std::endl);
        LOG("Player 2:" << std::endl);
        LOG(player2.GetOptions() << std::endl);
        match.PlayMatch(&player1, &player2, globalOptions.matchMode);
    }
    else
    {
        Game<CBoard> game;
        game.Loop();
    }

} // end of main

