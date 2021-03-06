/*
  Copyright (C) 1999,2000,2001  Franz Josef Och (RWTH Aachen - Lehrstuhl fuer Informatik VI)

  This file is part of GIZA++ ( extension of GIZA).

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

#ifndef GIZAPP_FORWARD_BACKWARD_H_
#define GIZAPP_FORWARD_BACKWARD_H_

#ifndef NO_TRAINING

#include "util/array.h"
#include "util/array2.h"

class HMMNetwork {
 public:
  int as,bs;
  Array2<double> n;
  Array<Array2<double> > e;
  Array<double> alphainit;
  Array<double> betainit;
  int ab;
  double finalMultiply;

  HMMNetwork(int I,int J)
      : as(I),bs(J),n(as,bs), e(0),alphainit(as,1.0/as),betainit(as,1.0),ab(as*bs),finalMultiply(1.0)
  { }

  double getAlphainit(int i) const { return alphainit[i]; }
  double getBetainit(int i) const { return betainit[i]; }

  inline int size1() const { return as; }
  inline int size2() const { return bs; }

  inline const double& nodeProb(int i, int j) const { return n(i,j); }

  inline const double& outProb(int j, int i1, int i2) const {
    return e[min(int(e.size())-1,j)](i1,i2);
  }

  friend ostream&operator<<(ostream&out,const HMMNetwork&x) {
    return out <<"N: \n" << x.n << endl
               << "E: \n" << x.e << "A:\n"
               << x.alphainit << "B:\n" << x.betainit << endl;
  }
};

double ForwardBackwardTraining(const HMMNetwork&mc, Array<double>&gamma, Array<Array2<double> >&epsilon);

void HMMViterbi(const HMMNetwork&mc, Array<int>&vit);

double HMMRealViterbi(const HMMNetwork&net, Array<int>&vit, int pegi=-1, int pegj=-1, bool verbose=0);

double MaximumTraining(const HMMNetwork&net, Array<double>&g, Array<Array2<double> >&e);

void HMMViterbi(const HMMNetwork&net, Array<double>&g, Array<int>&vit);

#endif  // NO_TRAINING
#endif  // GIZAPP_FORWARD_BACKWARD_H_
