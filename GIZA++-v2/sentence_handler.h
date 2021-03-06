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

/* --------------------------------------------------------------------------*
 *                                                                           *
 * Module : getSentence                                                      *
 *                                                                           *
 * Prototypes File: sentence_handler.h                                            *
 *                                                                           *
 * Objective: Defines clases and methods for handling I/O for the parallel   *
 *            corpus.                                                        *
 *****************************************************************************/

#ifndef GIZAPP_SENTENCE_HANDLER_H_
#define GIZAPP_SENTENCE_HANDLER_H_

#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include "util/vector.h"
#include "defs.h"
#include "vocab.h"
#include "globals.h"

/*----------------------- Class Prototype Definition ------------------------*
  Class Name: sentenceHandleer
  Objective: This class is defined to handle training sentece pairs from the
  parallel corpus. Each pair has: a target sentece, called here French; a
  source sentece, called here English sentece; and an integer number denoting
  the number of times this pair occured in trining corpus. Both source and
  target senteces are represented as integer vector (variable size arrays),
  each entry is a numeric value which is the token id for the particular token
  in the sentece.

  *---------------------------------------------------------------------------*/

class SentencePair {
 public:
  int sentenceNo;
  float noOcc;
  float realCount;
  Vector<WordIndex> eSent;
  Vector<WordIndex> fSent;

 public:
  SentencePair() { }
  ~SentencePair() { }

  void clear() {
    eSent.clear();
    fSent.clear();
    noOcc = 0;
    realCount = 0;
    sentenceNo = 0;
  }

  const Vector<WordIndex>&get_eSent() const { return eSent; }
  const Vector<WordIndex>&get_fSent() const { return fSent; }
  int getSentenceNo() const { return sentenceNo; }
  double getCount() const { return realCount; }
};

inline ostream&operator<<(ostream&of,const SentencePair&s)
{
  of << "Sent No: " << s.sentenceNo << " , No. Occurrences: " << s.noOcc << '\n';
  if (std::abs(s.noOcc - s.realCount) < kEPS)
    of << " Used No. Occurrences: " << s.realCount << '\n';
  unsigned int i;
  for (i=0; i < s.eSent.size(); i++)
    of << s.eSent[i] << ' ';
  of <<  '\n';
  for (i=1; i < s.fSent.size(); i++)
    of << s.fSent[i] << ' ';
  of << '\n';
  return of;
}

class SentenceHandler {
 public:
  // TODO: Should be private.
  const char * inputFilename; // parallel corpus file name, similar for all
  ifstream *inputFile;                 // parallel corpus file handler
  Vector<SentencePair> Buffer;          // sentence pair objects
  int noSentInBuffer;
  int currentSentence;
  int totalPairs1;
  double totalPairs2;
  bool readflag; // true if you reach the end of file
  bool allInMemory;
  int pair_no;
  Vector<double> *realCount;
  Vector<SentencePair> oldPairs;
  Vector<double> oldProbs;

  SentenceHandler(const char* filename, VocabList* elist = 0, VocabList* flist = 0);
  ~SentenceHandler();

  void rewind();
  bool getNextSentence(SentencePair&, VocabList* = 0, VocabList* = 0);  // will be defined in the definition file, this
  int getTotalNoPairs1() const { return totalPairs1; }
  double getTotalNoPairs2() const { return totalPairs2; }
  // method will read the next pair of sentence from memory buffer
  bool readNextSentence(SentencePair&);  // will be defined in the definition file, this
  void setProbOfSentence(const SentencePair&s,double d);
};

#endif  // GIZAPP_SENTENCE_HANDLER_H_
