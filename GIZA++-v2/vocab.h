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

#ifndef GIZAPP_VOCAB_H_
#define GIZAPP_VOCAB_H_

#include "defs.h"
#include "util/vector.h"

#include <climits>
#include <fstream>
#include <sstream>
#include <map>

const unsigned int kMaxVocabSize = UINT_MAX;

class WordEntry {
 public:
  string word;
  double freq;
  WordEntry():word("\0"), freq(0) { }
  WordEntry(string w, int f) : word(w), freq(f) { }
};

class VocabList {
 private:
  Vector<WordEntry> list;
  map<string,int> s2i;
  double total;
  WordIndex noUniqueTokens;
  WordIndex noUniqueTokensInCorpus;
  const char* fname;

 public:
  explicit VocabList(const char* filename = 0)
      : list(), total(0), noUniqueTokens(0),
        noUniqueTokensInCorpus(0), fname(filename) { }

  VocabList(const VocabList& a)
  : list(a.list), total(a.total), noUniqueTokens(a.noUniqueTokens),
    noUniqueTokensInCorpus(0), fname(a.fname) { }

  ~VocabList() { }

  void setName(const char* filename) { fname = filename; }
  std::size_t size() const { return list.size(); }
  inline WordIndex uniqTokens() const { return noUniqueTokens; };
  inline WordIndex uniqTokensInCorpus() const { return noUniqueTokensInCorpus; }
  inline double totalVocab() const { return total; }
  inline Vector<WordEntry>& getVocabList() { return(list); }
  inline const Vector<WordEntry>& getVocabList() const { return list; }
  void readVocabList();

  void incFreq(WordIndex id , double f) {
    if (id < list.size()) {
      if (list[id].freq < kEPS)
        noUniqueTokensInCorpus++;
      list[id].freq += f;
      total += f;
    }
  }

  void clearAllFreq() {
    for (WordIndex id = 0; id < list.size(); id++)
      list[id].freq = 0;
    total = 0;
    noUniqueTokensInCorpus = 0;
  }

  int operator()(const string& x) const {
    map<string,int>::const_iterator i=s2i.find(x);
    if (i!=s2i.end())
      return i->second;
    else {
      cerr << "ERROR: no word index for '"<<x<<"'\n";
      return 0;
    }
  }

  const string operator()(WordIndex id) const { // Yaser - 2000-12-13
    if (id < list.size())
      return list[id].word;
    else return 0;
  }

  const string operator[](WordIndex id) const { // Yaser - 2000-12-13
    if (id < list.size())
      return list[id].word;
    else return 0;
  }

  void printVocabList(ostream& of) {
    for (WordIndex i = 1; i < list.size(); i++) {
      if (list[i].word != "" && list[i].freq > 0)
        of << i << ' ' << list[i].word << ' ' << list[i].freq << '\n';
    }
  }
};

#endif  // GIZAPP_VOCAB_H_
