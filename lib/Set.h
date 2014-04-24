#ifndef SET_H
#define SET_H
//#include "List.h"
#include <stdexcept>
#include <cstdlib>
#include <iostream>
#include <set>
using namespace std;

template <class T>
class Set : public set<T> {
  public: 

    Set<T> intersect (const Set<T> & other) const;
      /* Returns the intersection of the current set with other.
         That is, returns the set of all items that are both in this
         and in other. */

    Set<T> Union (const Set<T> & other) const;
      /* Returns the union of the current set with other.
         That is, returns the set of all items that are in this set
         or in other (or both).
         The resulting set should not contain duplicates. */
};


template<class T>
Set<T> Set<T>::intersect (const Set<T> & other) const{
  Set intersection;
  for (typename set<T>::iterator it = this->begin(); it!= this->end(); ++it) {
    if (other.find(*it) != other.end() ) {
      intersection.insert(*it);
    }
  }
  return intersection;

}

template <class T>
Set<T> Set<T>::Union (const Set<T> & other) const {
  Set<T> uni(other);
  for (typename set<T>::iterator it = this->begin(); it!= this->end(); ++it){
    uni.insert(*it);
  }
  return uni;
}


#endif
