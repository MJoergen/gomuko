
#ifndef _BOARDGAME_H_
#define _BOARDGAME_H_

#include <ostream>
#include <stdio.h>
#include "CRand.h"

/***************************************************************
 * declaration of CSquare 
 ***************************************************************/

class CSquare 
{
  public: 
    CSquare() {}
    ~CSquare() {}

    CSquare(uint32_t col, uint32_t row) :
      m_col(col), m_row(row) {}

    uint32_t  Col(void) const {return m_col;} 
    uint32_t  Row(void) const {return m_row;} 

    friend std::ostream& operator <<(std::ostream &os, const CSquare &rhs)
    {
      char c = rhs.m_col + 'a';
      char r = rhs.m_row + '1';
      return os << c << r;
    }

    void    FromString(const char *s)
    {
      m_col = s[0] - 'a';
      m_row = s[1] - '1';
    }
    bool    operator!= (const CSquare& square) const
    {
      if (square.m_col != m_col)
        return true;
      if (square.m_row != m_row)
        return true;
      return false;
    }
    bool    operator== (const CSquare& square) const
    {
      return (!(*this != square));
    }

  private:
    uint32_t  m_col;
    uint32_t  m_row;
}; /* end of class CSquare */

typedef enum class
{
  eWon = 0,
  eLost,
  eDraw,
  eStillPlaying
} t_gameResult;

class CHashValue
{
  public:
    CHashValue(void) :
      m_hi(ran.generate()),
      m_lo(ran.generate())
      {}
    CHashValue(uint32_t hi, uint32_t lo) :
      m_hi(hi),
      m_lo(lo)
      {}
    uint32_t    Lo(void)    const { return m_lo; }
    bool        Valid(void) const { return (m_hi || m_lo); }
    void        Clear(void)
    {
      m_hi = 0;
      m_lo = 0;
    }
    uint32_t    GetIndex() const 
    {
      return m_lo + m_hi;
    }
    void        Update(const CHashValue& rhs)
    {
      m_hi ^= rhs.m_hi;
      m_lo ^= rhs.m_lo;
    }
    friend std::ostream& operator <<(std::ostream &os, const CHashValue &rhs)
    {
      char s[20];
      sprintf(s, "%08x%08x", rhs.m_hi, rhs.m_lo);

      return os << s;
    }
    const bool operator==(const CHashValue& rhs) const
    {
      return (m_hi == rhs.m_hi && m_lo == rhs.m_lo);
    }
    const bool operator!=(const CHashValue& rhs) const
    {
      return !(*this == rhs);
    }

  private:
    uint32_t      m_hi;
    uint32_t      m_lo;
}; /* end of CHashValue */

template<uint32_t cols, uint32_t rows>
class CHashData
{
  public:
    CHashValue  m_WhitePiece[cols][rows];
    CHashValue  m_BlackPiece[cols][rows];
    CHashValue  m_WhiteToMove;
};

#define GAME_OVER                       10000

#endif /* _BOARDGAMES_BOARDGAME_H_ */

