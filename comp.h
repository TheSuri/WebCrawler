#ifndef COMP_H
#define COMP_H
#include "WebPage.h"
#include "Ad.h"
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

class AdComp {
 public:
  bool operator()(const Ad* lhs, const Ad* rhs) {
    if (lhs->getBid() ==  rhs->getBid() ) { //if same bid
      //sort by alphabet
      return lhs->getCompany() < rhs->getCompany();
    }
    return lhs->getBid() < rhs->getBid();
  }
  
};


#endif
