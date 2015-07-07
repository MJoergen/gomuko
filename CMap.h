/*****************************************************************************
 *
 * Author:      Michael Joergensen
 *
 * Abstract:
 *   Drop-in replacement for std::map. Uses a vector and does linear search.
 *
 * Global Contents:
 *
 * Local Contents:
 *
 * ---------------------------------------------------------------------------
 * Revision History:
 * $Log: CMap.h,v $
 * Revision 1.1  2015/07/07 16:00:44  mike
 * Cleanup.
 *
 * Revision 1.1  2015/07/07 06:57:58  mike
 * Cleanup.
 *
 ****************************************************************************/

/* CMap.h */
 
#ifndef _CMAP_H_
#define _CMAP_H_

#include <ostream>
#include <vector>

template <class KeyType, class ValType>
class CMap
{
    public:
        // Types
        typedef std::pair<KeyType, ValType> pair_t;

        // Accessors
        typename std::vector<pair_t>::const_iterator begin() const {return m_rep.begin();} 
        typename std::vector<pair_t>::const_iterator end()   const {return m_rep.end();} 

        // Modifiers
        size_t   erase(const KeyType& key);
        void     clear(void);
        ValType& operator[](const KeyType& key);

    private:
        std::vector<pair_t> m_rep;
}; /* end of CMap */


/***************************************************************
 ***************************************************************/
template <class KeyType, class ValType>
size_t CMap<KeyType, ValType>::erase(const KeyType& key)
{
    for (auto it=m_rep.begin(); it != m_rep.end(); ++it)
    {
        if (it->first == key)
        {
            *it = m_rep.back();
            m_rep.pop_back();
            return 1;
        }
    }
    return 0;
} // end of erase


/***************************************************************
 ***************************************************************/
template <class KeyType, class ValType>
void CMap<KeyType, ValType>::clear(void)
{
    m_rep.clear();
} // end of clear


/***************************************************************
 ***************************************************************/
template <class KeyType, class ValType>
ValType& CMap<KeyType, ValType>::operator[](const KeyType& key)
{
    for (auto it=m_rep.begin(); it != m_rep.end(); ++it)
    {
        if (it->first == key)
        {
            return it->second;
        }
    }

    m_rep.push_back(pair_t(key, ValType()));
    return m_rep.back().second;
} // end of operator []

#endif /* _CMAP_H_ */

