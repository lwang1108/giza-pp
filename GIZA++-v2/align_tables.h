/*
  EGYPT Toolkit for Statistical Machine Translation

  Written by Yaser Al-Onaizan, Jan Curin, Michael Jahr, Kevin Knight, John Lafferty, Dan Melamed, David Purdy, Franz Och, Noah Smith, and David Yarowsky.

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
  USA.
*/

#ifndef GIZAPP_ALIGN_TABLES_H_
#define GIZAPP_ALIGN_TABLES_H_

#include <cmath>
#include <functional>
#include <utility>

#include "defs.h"
#include "util/vector.h"

// Alignments neighborhhoods (collection of alignments) are stored in
// a hash table (for easy lookup). Each alignment vector is mapped
// into a hash key using the operator defined above.
class AlignmentModel {
 public:
  class AlignmentHashFunc;
  class AlignmentComparator;

  typedef Vector<WordIndex> Key;
  typedef hash_map<Key, LogProb, AlignmentHashFunc,
                   AlignmentComparator> AlignmentHashMap;

  // This class is used to define a hash mapping function to map an
  // alignment (defined as a vector of integers) into a hash key.
  class AlignmentHashFunc : public std::unary_function<Key, size_t> {
   public:
    // to define the mapping function. it takes an alignment (a vector of
    // integers) and it returns an integer value (hash key).
    size_t operator()(const Key& key) const {
      size_t key_sum = 0;
      for (WordIndex j = 1; j < key.size(); ++j) {
        key_sum += static_cast<size_t>(std::pow(static_cast<double>(key[j]),
                                                static_cast<double>((j % 6) + 1)));
      }
      return key_sum % 1000000;
    }
  };

  // Returns true if two alignments are the same (two vectors have same enties)
  class AlignmentComparator {
   public:
    bool operator()(const Key& t1,
                    const Key& t2) const {
      if (t1.size() != t2.size()) {
        return false;
      }
      for (WordIndex j = 1; j < t1.size(); ++j) {
        if (t1[j] != t2[j]) {
          return false;
        }
      }
      return true;
    }
  };

  AlignmentModel() : m_alignment() {}
  ~AlignmentModel() {}

  AlignmentHashMap::iterator begin() { return m_alignment.begin(); }
  AlignmentHashMap::const_iterator begin() const {
    return m_alignment.begin();
  }

  AlignmentHashMap::iterator end() { return m_alignment.end(); }
  AlignmentHashMap::const_iterator end() const {
    return m_alignment.end();
  }

  const AlignmentHashMap& getHash() const { return m_alignment; }

  // Add a alignmnet.
  bool insert(const Key&aj, LogProb val=0.0);

  // Retrieve probability of alignment.
  LogProb getValue(const Key& align) const;

  // Clear hash table.
  void clear() { m_alignment.clear(); }

 private:
  AlignmentHashMap m_alignment;
};

inline bool AlignmentModel::insert(const Key& aj, LogProb val) {
  AlignmentHashMap::iterator it = m_alignment.find(aj);
  if (it != m_alignment.end() || val <= 0) {
    return false;
  }
  m_alignment.insert(pair<const Key, LogProb>(aj, val));
  return true;
}

inline LogProb AlignmentModel::getValue(const Key& align) const {
  const LogProb zero = 0.0;
  AlignmentHashMap::const_iterator it = m_alignment.find(align);
  if (it == m_alignment.end()) {
    return zero;
  } else {
    return it->second;
  }
}

#endif  // GIZAPP_ALIGN_TABLES_H_
