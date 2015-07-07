/*****************************************************************************
 *
 * Author:	Michael Joergensen, idk
 *
 * Abstract:
 *
 * Global Contents:
 *
 * Local Contents:
 *
 * ---------------------------------------------------------------------------
 * Revision History:
 * $Log: stack.h,v $
 * Revision 1.1  2015/07/05 09:05:56  mike
 * First version.
 *
 * Revision 1.1  2011/03/23 00:14:56  mij
 * Ported to gcc 4.5
 *
 * Revision 1.5  2011/03/22 13:30:11  mij
 * Rewrote using std::vector
 *
 * Revision 1.4  2011/03/22 12:53:30  mij
 * Compile fix.
 *
 * Revision 1.3  2011/03/21 22:58:21  mij
 * Checkin time.
 *
 * Revision 1.2  2011/03/16 06:29:50  mij
 * Removed  at end of each line
 *
 * Revision 1.1  2011/03/16 06:27:27  mij
 * First version that compiles. Completely untested!
 *
 ****************************************************************************/

/* stack.h */

#ifndef _STACK_H_
#define _STACK_H_

#include <ostream>
#include <vector>
#include <stdint.h>
#include "assert.h"

template <class T>
class CStack
{
  public:
    CStack(uint32_t maxSize = 100) : m_MaxSize(maxSize), m_NumElements(0)
    {
      m_Elements.resize(maxSize);
    } /* end of CStack */

    void Push(const T& elem)
    {
      if (m_NumElements < m_MaxSize)
      {
        m_Elements[m_NumElements] = elem;
        m_NumElements++;
        return;
      }
      assert(0);
    } /* end of Push */

    const T& Pop(void)
    {
      assert(m_NumElements > 0);

      m_NumElements--;
      return m_Elements[m_NumElements];
    } /* end of Pop */

    uint32_t Size(void) const
    {
      return m_NumElements;
    } /* end of Size */

    void Reset(void)
    {
      m_NumElements = 0;
    } /* end of Reset */

    friend std::ostream& operator <<(std::ostream &os, const CStack<T> &rhs)
    {
      for (unsigned int i=0; i<rhs.m_NumElements; i++)
      {
        os << rhs.m_Elements[i] << " ";
      }
      return os;
    }

    const CStack<T>& operator+=(const CStack<T>& rhs)
    {
      for (unsigned int i=0; i<rhs.Size(); i++)
      {
        Push(rhs.m_Elements[i]);
      }
      return *this;
    }

    const CStack<T>& operator=(const CStack<T>& rhs)
    {
      Reset();
      *this += rhs;
      return *this;
    }

    const CStack<T>& operator=(const T& rhs)
    {
      Reset();
      Push(rhs);
      return *this;
    }

    const T& operator[](uint32_t idx) const
    {
      assert (idx < m_NumElements);

      return m_Elements[idx];
    }


  private:
    std::vector<T> m_Elements;
    uint32_t m_MaxSize;
    uint32_t m_NumElements;
}; /* end of CStack */

#endif /* _STACK_H_*/

