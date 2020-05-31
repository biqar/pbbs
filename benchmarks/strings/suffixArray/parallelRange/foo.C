// This code is part of the Problem Based Benchmark Suite (PBBS)
// Copyright (c) 2011-2017 Guy Blelloch and the PBBS team
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights (to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <iostream>
#include <algorithm>
#include "gettime.h"
#include "sequence.h"
#include "blockRadixSort.h"
#include "quickSort.h"
#include "sampleSort.h"
#include "parallel.h"
#include "SA.h"
using namespace std;

//#define printInfo

#ifdef printInfo
#define nextTimeM(_str) nextTime(_str)
#else
#define nextTimeM(_str) 
#endif

typedef unsigned int uint;
typedef unsigned char uchar;
typedef pair<uintT,uintT> intpair;
typedef pair<uintT,uintT> longpair;


struct longPairLess {
  bool operator() (longpair a, longpair b) { return a.first < b.first;  }
};

void sortLongPair(longpair *A, long n) {
    sampleSort(A, n, longPairLess());
}


struct seg {
  uintT start;
  uintT length;
  seg(uintT s, uintT l) : start(s), length(l) {}
};

struct isSeg {bool operator() (seg s) {return s.length > 1;}};

inline uintT grabChars(uchar *s, uint m, uintT nChars) {
  uintT r = s[0];
  for (uintT i=1; i < nChars; i++) r = r*m + s[i];
  return r; 
}

inline uintT grabCharsEnd(uchar *s, uint m, uintT nChars, uintT end) {
  uintT r = s[0];
  for (uintT i=1; i < nChars; i++) 
    r = r*m + ((i < end) ? s[i] : 0);
  return r; 
}

struct pairCompF {
  bool operator() (intpair A, intpair B) { return A.first < B.first;}};

template <typename kvpair>
void splitSegment(seg *segOut, uintT start, uintT l, uintT* ranks, kvpair *Cs) {
  if (l < 5000) { // sequential version

    uintT name = 0;
    ranks[Cs[0].second] = name + start + 1;
    for (uintT i=1; i < l; i++) {
      if (Cs[i-1].first != Cs[i].first) name = i;
      ranks[Cs[i].second] = name + start + 1;
    }

    name = 0;
    for (uintT i=1; i < l; i++) {
      if (Cs[i-1].first != Cs[i].first) {
	segOut[i-1] = seg(name+start,i-name);
	name = i;
      } else segOut[i-1] = seg(0,0);
    }
    segOut[l-1] = seg(name+start,l-name);

  } else { // parallel version
    uintT *names = newA(uintT,l);

    // mark start of each segment with equal keys
    parallel_for (uintT i = 1;  i < l;  i++) 
      names[i] = (Cs[i].first != Cs[i-1].first) ? i : 0;
    names[0] = 0;

    // scan start i across each segment
    sequence::scanI(names,names,l,utils::maxF<uintT>(),(uintT)0);

    // write new rank into original location
    parallel_for (uintT i = 0;  i < l;  i++) 
      ranks[Cs[i].second] = names[i]+start+1;

    // get starts and lengths of new segments
    parallel_for (uintT i = 1;  i < l;  i++)
      if (names[i] == i) 
    	segOut[i-1] = seg(start+names[i-1],i-names[i-1]);
      else segOut[i-1] = seg(0,0);
    segOut[l-1] = seg(start+names[l-1],l-names[l-1]);

    free(names);
  }
}  

uintT* suffixArrayInternal(uchar* ss, long n) { 
  uintT *ranks = newA(uintT,n);
  uchar *s = newA(uchar,n);
  uintT flags[256];
  for (uintT i=0; i < 256; i++) flags[i] = 0;
  parallel_for (uintT i=0; i < n; i++) 
    if (!flags[ss[i]]) flags[ss[i]] = 1;

  // renumber characters densely
  // start numbering at 1 leaving 0 to indicate end-of-string
  uintT m = sequence::scan(flags,flags,256,utils::addF<uintT>(),(uintT)1);
  parallel_for (uintT i=0; i < n; i++) 
    s[i] = flags[ss[i]];
#ifdef printInfo
  cout << "m = " << m << endl;
#endif

  double logm = log2(m);
  uintT nchars = floor(32.0/logm); 
  size_t max_val = pow(m,nchars);

  longpair *Cl = newA(longpair,n);
  // pack characters into 32-bit word (C.first) and tag with i (C.second)
  startTime();
  if(n+1 > nchars) {
    parallel_for (uintT i=0; i < n-nchars+1; i++) {
      Cl[i].first = grabChars(s+i,m,nchars); 
      Cl[i].second = i;
    }

    for (uintT i=n-nchars+1; i < n; i++) {
      Cl[i].first = grabCharsEnd(s+i,m,nchars,n-i); 
      Cl[i].second = i;
    }
  } else {
    for (uintT i=0; i < n; i++) {
      Cl[i].first = grabCharsEnd(s+i,m,nchars,n-i); 
      Cl[i].second = i;
    }
  }
  free(s);
  nextTimeM("copy");

  intSort::iSort(Cl, n, max_val, utils::firstF<uintT,uintT>());

  //sortLongPair(Cl,n); //, utils::firstF<uintT,uintT>());
  nextTimeM("sort");

  seg *segOuts = newA(seg,n);
  seg *segments= newA(seg,n);
  uintT *offsets = newA(uintT,n);
  splitSegment(segOuts, 0, n, ranks, Cl);
  nextTimeM("split");
  free(Cl);
  intpair *C = newA(intpair,n);
  uintT offset = nchars;
  uint round =0;
  uintT nKeys = n;
  while (1) {
    utils::myAssert(round++ < 40, "Suffix Array:  Too many rounds");
    uintT nSegs = sequence::filter(segOuts,segments,nKeys,isSeg());
    if (nSegs == 0) break;

    parallel_for (uintT i=0; i < nSegs; i++)
      offsets[i] = segments[i].length;

    nKeys = sequence::scan(offsets,offsets,nSegs,utils::addF<uintT>(),(uintT)0);
    #ifdef printInfo
    cout << "nSegs = " << nSegs << " nKeys = " << nKeys 
	 << " common length = " << offset << endl;
    #endif
    nextTimeM("filter and scan");    

    parallel_for (uintT i=0; i < nSegs; i++) {
      uintT start = segments[i].start;
      intpair *Ci = C + start;
      uintT l = segments[i].length;
      parallel_for_256 (uintT j=0; j < l; j++) {
	uintT o = Ci[j].second+offset;
	Ci[j].first = (o >= n) ? n-o : ranks[o];
      }
      if (l >= 256)
	intSort::iSort(Ci, l, n, utils::firstF<uintT,uintT>());
      else
	quickSort(Ci, l, pairCompF());
    }
    nextTimeM("sort");

    parallel_for (uintT i=0; i < nSegs; i++) {
      uintT start = segments[i].start;
      splitSegment(segOuts + offsets[i], start, segments[i].length, 
		   ranks, C + start);
    }
    nextTimeM("split");

    offset = 2 * offset;
  }
  parallel_for (uintT i=0; i < n; i++) ranks[i] = C[i].second;
  free(C); free(segOuts); free(segments); free(offsets); 
  return ranks;
}

intT* suffixArray(unsigned char* ss, long n) { 
  return (intT*)suffixArrayInternal(ss, n);
}
