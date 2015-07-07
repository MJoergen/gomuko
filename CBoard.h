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
 * $Log: CBoard.h,v $
 * Revision 1.15  2015/07/07 16:00:44  mike
 * Cleanup.
 *
 * Revision 1.14  2015/07/07 11:59:56  mike
 * Cleanup.
 *
 * Revision 1.13  2015/07/07 06:57:58  mike
 * Cleanup.
 *
 * Revision 1.12  2015/07/07 06:36:23  mike
 * Cleanup.
 *
 * Revision 1.11  2015/07/07 06:31:54  mike
 * Cleanup.
 *
 * Revision 1.10  2015/07/06 22:04:46  mike
 * Optimization
 *
 * Revision 1.9  2015/07/06 20:36:12  mike
 * Optimization.
 *
 * Revision 1.8  2015/07/05 13:59:50  mike
 * Cleanup.
 *
 * Revision 1.7  2015/07/05 13:46:56  mike
 * Checkin time.
 *
 * Revision 1.6  2015/07/05 11:45:34  mike
 * Cleanup.
 *
 * Revision 1.5  2015/07/05 11:40:41  mike
 * Cleanup.
 *
 * Revision 1.4  2015/07/05 11:39:00  mike
 * Cleanup.
 *
 * Revision 1.3  2015/07/05 10:29:59  mike
 * Cleanup.
 *
 * Revision 1.2  2015/07/05 10:09:00  mike
 * Checkin time.
 *
 * Revision 1.1  2015/07/05 09:05:55  mike
 * First version.
 *
 ****************************************************************************/

/* CBoard.h */
 
#ifndef _CBOARD_H_
#define _CBOARD_H_

#include <stdint.h>
#include <ostream>
#include <set>
#include <vector>
#include <stack>
#include "boardgame.h"
#include "CPiece.h"
#include "CMove.h"
#include "CLine.h"
#include "CMap.h"
 
#define SIZE 9

/***************************************************************
 * declaration of CBoard
 ***************************************************************/

class CBoard
{
    friend std::ostream& operator <<(std::ostream &os, const CBoard &rhs);

    public:
        // Types
        typedef      CMove               move_t;
        typedef      std::vector<CMove>  moveList_t;

        // Constructors
        CBoard() { Reset(); }

        // Accessors
        void         Help(void)          const;
        moveList_t   GetLegalMoves(void) const;
        int32_t      Evaluate(void)      const;
        t_gameResult GameResult(void)    const;
        uint32_t     MaxLegalMoves(void) const {return SIZE*SIZE;}

        // Modifiers
        void         Reset(void);
        void         MakeMove(const CMove& move);
        void         UndoMove(const CMove& move);

    private:
        // Types
        typedef int                      lineVal_t;

        // Variables
        CPiece                      m_board[(SIZE+8)*(SIZE+8)];
        bool                        m_bWhite;
        std::set<CMove>             m_legalMoves;
        std::stack<moveList_t>      m_stack;
        CMap<CLine, lineVal_t>      m_lines;

        // Functions
        int countPieces(const CLine& line);
        void UpdateLines(const CMove& move);

}; /* end of CBoard */

#endif /* _CBOARD_H_ */

