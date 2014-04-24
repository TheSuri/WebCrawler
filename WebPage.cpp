#include "WebPage.h"
#include <fstream>
#include <iostream>
#include "./lib/Set.h"
#include <string>
#include <sstream>

using namespace std;

WebPage::WebPage() { //constructor
  file = "";
  newRank = -1;
  oldRank = -1;
}

WebPage::WebPage (string filename) {
  fileName = filename; //set private data member "fileName"
  ifstream ifile(filename.c_str());
  if (ifile.fail())
    throw invalid_argument("File cannot be opened");

  //create string file to store file contents (to print out later)
  file = "";
  string line;
  while(!ifile.eof()){
    getline(ifile,line);
    string line_anchorText = "";
    stringstream ss(line);
    char ch;
    ch = ss.get();
   
    while (ss.tellg() != -1) { //not at end of line
      //check for possible link
      line_anchorText = "";
      int pos = -1;
      if (ch == '[') {
	//save position to return here if not a valid link 
	pos = ss.tellg();

	bool link = true;
	ch = ss.get();
	while (ss.tellg() != -1 && ch != ']') {
	  line_anchorText += ch;
	  ch = ss.get();
	}

	if (ch != ']') link = false;
	if (link) { //check for (
	  if (ss.tellg() != -1) ch = ss.get();
	  if (ch != '(') link = false;
	}
      
	string filename = "";
	if (link) { //get the name of the linked file
	  if (ss.tellg() != -1) ch = ss.get();
	  else link = false;
	  while (ss.tellg() != -1 && link && ch != ')') {
	    filename += ch;
	    ch = ss.get();
	  }
	  if (ch != ')') link = false;
	}
      
	if (link) { //a valid link
	  file += line_anchorText;
	  outLinkNames.push_back(filename);
	}
	else { //not a valid link
	  //return ch to one after the [
	  file += '[';
	  ss.seekg(pos);
	  if (ss.tellg() == -1) {
	    ss.clear();
	    ss.seekg(pos);
	  }

	}
	ch = ss.get();
      }
      else {
	//parse normally
	file += ch;
	ch = ss.get();
      }
    }
    
    if (ss.tellg() == -1) { //end of line
      file += "\n";
    }
  }// END

  ifile.close();
  
}

WebPage::~WebPage() { //destructor
}

Set<string> WebPage::allWords () const {
  return PageWords;
}

ostream& operator<<(ostream & os, const WebPage & page) {
  os << page.file;
  return os;
}

Set<WebPage*> WebPage::allOutgoingLinks () const {
  return outLinks;   
}

Set<WebPage*> WebPage::allIncomingLinks () const {
  return inLinks;
}

string WebPage::filename () const {
  return fileName;
}   

void WebPage::parse (){
  
  //reopen file to now only store words
  ifstream ifile(fileName.c_str());
  string word;
  string link_word;
  
  
  while(!ifile.eof()) {
    //check if a link
    string line;
    string line_anchorText="";
    getline(ifile,line);
    stringstream ss(line);
    char ch;
    bool link=true;
    ch = ss.get();
    while (ss.tellg() != -1 && (ch == ' ' || ch == '[')) {//could be a webpage
      if (ch == '[') break;
      ch = ss.get();
    }
    
    if (ch != '[') link = false; 
    else  { //ch = '['
      ch = ss.get();
      while (ss.tellg() != -1 && ch != ']') {
	line_anchorText += ch;
	ch = ss.get();
      }
    }
    if (ch != ']') link = false;
    if (link) {
      if (ss.tellg() != -1) ch = ss.get();
      if (ch != '(') link = false;
    }
    
    if (link) {
      if (ss.tellg() != -1) ch = ss.get();
      else link = false;
      while (ss.tellg() != -1 && link && ch != ')') {
	ch = ss.get();
      }
      if (ch != ')') link = false;
    }
    
    if (link) {
      //do something with link_anchorText
      stringstream in(line_anchorText);
      ch = in.get();
      while (in.tellg() != -1) {
	word = "";
	while (isalpha(ch) || isdigit(ch)){
	  if (isupper(ch)) ch=tolower(ch);
	  word += ch;
	  ch = in.get();
	}
      // found a character that needs to be ignored...so add this complete word
      if (!word.empty()){
	PageWords.insert(word);
      }
      ch = in.get();
      }

      //check if there are words after the valid link
      if (ss.tellg() != -1) {  
	ch = ss.get();   
	while (ss.tellg() != -1) {
	  string afterLinkWord = "";
	  while (isalpha(ch) || isdigit(ch)) {
	    afterLinkWord += ch;
	    ch = ss.get();
	  }
	  if (!afterLinkWord.empty()) {
	    PageWords.insert(afterLinkWord);
	  }
	  ch = ss.get();
	}
      }
    }
    else {   //carry on without considering webpage links
      stringstream ss(line);
      ch = ss.get();
      while (ss.tellg() != -1) {
	word = "";
	while (isalpha(ch) || isdigit(ch)){
	  if (isupper(ch)) ch=tolower(ch);
	  word += ch;
	  ch = ss.get();
	}
	// found a character that needs to be ignored...so add this complete word
	if (!word.empty()){
	  PageWords.insert(word);
	}
	ch = ss.get();
      }
    }
  }
  ifile.close();
  
}

void WebPage::addIncomingLink (WebPage* start) {

  pair< Set <WebPage*>::iterator, bool> ret;  

ret = inLinks.insert(start);
 if (ret.second == false) {
 }

}

void WebPage::addOutgoingLink(WebPage* target) {

  outLinks.insert(target);
}

vector<string> WebPage::getOutLinkNames() const {
  return outLinkNames;
}

double WebPage::getOldRank() const {

  return oldRank;
}

double WebPage::getNewRank() const {
  return newRank;
}

void WebPage::setNewRank(double rankVal) {
  newRank = rankVal;
}

void WebPage::setOldRank(double rank) {
  oldRank = rank;
}

void WebPage::updateOldRank() {
  oldRank = newRank;
}

void WebPage::addNewRank(double update) {
  newRank += update;
}
