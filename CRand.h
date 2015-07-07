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
 * $Log: CRand.h,v $
 * Revision 1.1  2015/07/05 10:09:01  mike
 * Checkin time.
 *
 ****************************************************************************/

/* CRand.h */

#ifndef _CRAND_H_
#define _CRAND_H_

/***************************************************************/

#include <random>
#include <chrono>

class CRandom
{
    public:

        void seed(uint32_t seed)
        {
            m_Generator.seed(seed);
        }

        uint32_t generate()
        {
            return m_Generator();
        }

    private:
        std::mt19937 m_Generator;
}; /* end of CRandom */

extern CRandom ran;

#endif /* _CRAND_H_ */

