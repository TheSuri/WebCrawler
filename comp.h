#ifndef COMP_H
#define COMP_H
#include "WebPage.h"
#include <string>

class Comparator{
public:
	virtual bool operator()(const WebPage* lhs, const WebPage* rhs) = 0;
};

class AlphaComp : public Comparator {
public:
    bool operator()(const WebPage* lhs, const WebPage* rhs) 
    { // Uses string's built in operator< 
      // to do lexicographic (alphabetical) comparison
      return lhs->filename() > rhs->filename(); 
    }
  };

class RankComp : public Comparator{
public:
	bool operator()(const WebPage* lhs, const WebPage* rhs){
		return lhs->getOldRank() < rhs->getOldRank();
	}
};


#endif
