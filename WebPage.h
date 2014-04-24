#ifndef WEBPAGE_H
#define WEBPAGE_H
#include "./lib/Set.h"
//#include "./lib/List.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;


class WebPage {
  public:
    WebPage ();    // constructor that creates an empty page
    WebPage (string filename);
      /* constructor that initializes the page from the given file.
         Should throw an exception if the file does not exist
         or has other problems. */
    ~WebPage ();   // destructor

    Set<string> allWords () const;
      /* Returns a set containing all individual words on the web page. */   

    friend ostream & operator<< (ostream & os, const WebPage & page);
      /* Declares an operator we can use to print the web page. */

    string getFileContents() { return file; }
 
    Set<WebPage*> allOutgoingLinks () const;
      /* Returns "pointers" to all webpages that this page has links to. 
         As discussed above, this could be as a set or via an iterator,
         and it could be as actual pointers, or as strings, 
         or possibly other. */   

    Set<WebPage*> allIncomingLinks () const;
      /* Returns "pointers" to all webpages that link to this page. 
         Same consideration as previous function. */   

    string filename () const;
      /* Returns the filename from which this page was read. */

    /* Based on which overall architecture you choose, you may 
       also need/want some or all of the following functions. */

    void parse ();
      /* actually parses the content of the filename that was passed 
         to the constructor into the object. */

     void addIncomingLink (WebPage* start);
      /* Adds a link from start to the current page 
         in the current page's storage. */

     void addOutgoingLink (WebPage* target); 
      /* Adds a link from the current page to the target
         in the current page's storage. */

     double getOldRank() const;
     double getNewRank() const;
     void setOldRank(double rank);
     void updateOldRank();
     void setNewRank(double rank);
     void addNewRank(double update);

     vector<string> getOutLinkNames() const;

  private:
    // you get to decide what goes here.
    Set<string> PageWords;
    string file;
    Set<WebPage*> outLinks;
    Set<WebPage*> inLinks;
    string fileName;
    vector<string> outLinkNames;
    double oldRank;
    double newRank;
};


#endif
