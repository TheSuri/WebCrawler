#ifndef MSORT_H
#define MSORT_H
#include "WebPage.h"
#include "comp.h"
#include <string>
#include <vector>


template <class T, class Comparator>
class MSort{
public:
	void mergeSort(vector<T>& myArray, Comparator& comp);
	void msort(vector<T>& myArray, int start, int end, Comparator& comp);
	void merge(vector<T>& myArray, int start, int mid, int end, Comparator& comp);
};

template <class T, class Comparator>
void MSort<T,Comparator>::mergeSort(vector<T>& myArray, Comparator& comp){
	msort(myArray, 0, myArray.size(), comp);
}

template <class T, class Comparator>
void MSort<T, Comparator>::msort(vector<T>& myArray, int start, int end, Comparator& comp){ //recursive
	//base case
	if(start >= end-1){ return; }
	//recursive
	int mid = (start+end)/2;
	msort(myArray, start, mid, comp);
	msort(myArray, mid, end, comp);
	//merge
	merge(myArray, start, mid, end, comp);
}

template <class T, class Comparator>
void MSort<T,Comparator>::merge(vector<T>& myArray, int start, int mid, int end, Comparator& comp){
	vector<T> temp;
	int fromLeft = start;
	int fromMid = mid;
	for(int k=0; k<(end-start); k++){
		if(fromMid>=end || (fromLeft<mid && !comp(myArray[fromLeft],myArray[fromMid]))){
			temp.push_back(myArray[fromLeft]);
			fromLeft++;
		}
		else{
			temp.push_back(myArray[fromMid]);
			fromMid++;
		}
	}
	//copy back to original vector
	for(int k=start; k<end; k++){
		myArray[k] = temp[k-start];
	}
}



#endif