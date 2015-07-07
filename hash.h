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
 * $Log: hash.h,v $
 * Revision 1.2  2015/07/05 11:40:41  mike
 * Cleanup.
 *
 * Revision 1.1  2015/07/05 09:05:55  mike
 * First version.
 *
 * Revision 1.3  2011/03/27 18:29:51  mij
 * Optimized hash table usage.
 *
 * Revision 1.2  2011/03/27 18:08:42  mij
 * Uses std::vector instead of pointer.
 *
 ****************************************************************************/

/* hash.h */

#include <vector>
#include <stdint.h>

typedef enum {exact, lower, upper} t_valueType;

template <class traits>
class CHashEntry
{
  public:
    typedef     typename traits::move_t t_move;
    static const uint32_t EntryStrLen = t_move::MoveStrLen + 40;

    CHashEntry() : m_hashValue(0, 0) {}
    CHashEntry(
        const CHashValue& hashValue,
        int32_t val,
        uint32_t level,
        t_valueType type,
        const t_move& move) :
      m_hashValue(hashValue),
      m_val(val),
      m_level(level),
      m_type(type),
      m_move(move)
      {}

    void        Clear(void) { m_hashValue.Clear(); }

    const char  *TypeString(void) const;
    bool        Valid(void)       const { return m_hashValue.Valid(); }

    const bool  operator==(const CHashEntry& rhs) const
    {
      return rhs.hashValue == m_hashValue;
    }
    friend std::ostream& operator <<(std::ostream &os, const CHashEntry<traits> &rhs)
    {
      os << "val=" << rhs.m_val << ", level=" << rhs.m_level << ", type=" << rhs.TypeString() << ", " << rhs.m_move;
      return os;
    }

    const CHashValue& GetHashValue(void) const {return m_hashValue;}
    int32_t           Val(void)          const {return m_val;}
    uint32_t          Level(void)        const {return m_level;}
    t_valueType       Type(void)         const {return m_type;}
    const t_move&     Move(void)         const {return m_move;}

  private:
    CHashValue  m_hashValue;
    int32_t     m_val;
    uint32_t    m_level;
    t_valueType m_type;
    t_move      m_move;
}; /* end of CHashEntry<traits> */

template <class traits>
class CHashTable
{
  public:
    CHashTable(uint32_t size) : m_size(size), m_mask(size-1)
    {
      m_pHashEntry.resize(size);
    }

    uint32_t    NumEntries(void) const;
    void        Clear(void);

    const CHashEntry<traits>& GetHashEntry(bool& found,
        const CHashValue& hashValue) const
    {
      uint32_t hashIndex = hashValue.GetIndex() & m_mask;
      const CHashEntry<traits>& entry1 = m_pHashEntry[hashIndex];

      found = (entry1.GetHashValue() == hashValue);

      if (found)
        return entry1;

      if (!entry1.Valid())
        return entry1;

      // Try next index
      hashIndex = (hashIndex+1) & m_mask;
      const CHashEntry<traits>& entry2 = m_pHashEntry[hashIndex];

      found = (entry2.GetHashValue() == hashValue);

      return entry2;
    }

    CHashEntry<traits>& GetHashEntry(bool& found,
        const CHashValue& hashValue)
    {
      uint32_t hashIndex = hashValue.GetIndex() & m_mask;
      CHashEntry<traits>& entry1 = m_pHashEntry[hashIndex];

      found = (entry1.GetHashValue() == hashValue);

      if (found)
        return entry1;

      if (!entry1.Valid())
        return entry1;

      // Try next index
      hashIndex = (hashIndex+1) & m_mask;
      CHashEntry<traits>& entry2 = m_pHashEntry[hashIndex];

      found = (entry2.GetHashValue() == hashValue);

      if (found)
        return entry2;

      if (!entry2.Valid())
        return entry2;

      return entry1;
    }

  private:
    uint32_t      m_size;
    uint32_t      m_mask;
    std::vector<CHashEntry<traits> > m_pHashEntry;
}; /* end of CHashTable */

