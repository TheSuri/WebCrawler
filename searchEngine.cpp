#include <map>
#include "./lib/Set.h"
#include "WebPage.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <QApplication>
#include <QVBoxLayout>
#include "searchWin.h"
#include "searchEngine.h"
#include "msort.h"
#include <vector>
#include "comp.h"
#include "Ad.h"

searchEngine::searchEngine() {

   window = new searchWin;
  connect(window, SIGNAL(searchReady()), this, SLOT(search()));
  //connect(this, SIGNAL(search()), this, SLOT(printResults()));
  valid_search = true;
  results_found = true;

  mainLayout = new QVBoxLayout;
  mainLayout->addWidget(window);

  setLayout(mainLayout);

}

searchEngine::searchEngine(string filename, string adfile, string billfile) {
   window = new searchWin;
  openWebPages(filename);
  openAds(adfile);
  billFile = billfile;
 connect(window, SIGNAL(searchReady()), this, SLOT(search()));
 connect(window, SIGNAL(exit(map<string,float>)), this, SLOT(searchExit(map<string,float>)));
  valid_search = true;
  results_found = true;

  mainLayout = new QVBoxLayout;
  mainLayout->addWidget(window);

  setLayout(mainLayout);
}

searchEngine::~searchEngine() { //destructor
  //delete set_allWebPages
  for (Set<WebPage*>::iterator it = set_allWebPages.begin(); it!= set_allWebPages.end(); ++it) {
    delete *it;
  }

  delete mainLayout;
  //delete window;

}

void searchEngine::openWebPages(string filename) {

  ifstream ifile(filename.c_str());
  if (ifile.fail()){
    cout << "Can't open file..exiting" << endl;
    return;
  }

  //OPEN EACH WEBPAGE FILE CONTAINED IN MAIN DATA FILE
  string webPageFileName;

  while(ifile >> webPageFileName){
    WebPage* webpoint;
    try {
      webpoint = new WebPage(webPageFileName);
    }
    catch(invalid_argument& a) { //webPageFileName is not a valid filename
      cout << "Cannot open a webpage file provided in the main file" << endl;
      cout << "Exiting" << endl;
      return;
    }

    //add webpage to the set of webpages
    set_allWebPages.insert(webpoint);
   

    //parse the webpage to create the set of words it contains
    webpoint->parse();

    //ADD THE WORDS FOUND IN THIS WEBPAGE TO THE MAP
    Set<string> dummy = webpoint->allWords();
    Set<string>::iterator it = dummy.begin();
   
    while (it != dummy.end()) { //WebPage's PageWords is not empty
      Set<WebPage*> pagesWithWord;
      WebPage* temp = webpoint; //need a DEEP COPY
      pagesWithWord.insert(temp);
     
      pair< map<string,Set<WebPage*> >::iterator, bool> ret;
      //try inserting the word into the map
      //won't throw an exception, just won't add if word already exists in map
      ret = map_WordstoPages.insert(pair< string, Set<WebPage*> >(*it,pagesWithWord));
  if (ret.second == false) { //if word was already added to the Map
  //add the webpage to the word's associated Set<WebPage*>
  map_WordstoPages[*it].insert(temp);
      }
      ++it;
    }// end of while

}


  ifile.close(); 

  //opened all webpages, now update their incoming/outgoing connections
 //go through set of all webpages
  for (Set<WebPage*>::iterator it= set_allWebPages.begin();it!= set_allWebPages.end(); ++it) {
    vector<string> outlinks= (*it)->getOutLinkNames();
    for (int i=0; i< static_cast<int>(outlinks.size()); i++) {
      for (Set<WebPage*>::iterator itr= set_allWebPages.begin(); itr!= set_allWebPages.end(); ++itr) {
  if ((**itr).filename() == outlinks[i]) {
    (**it).addOutgoingLink(*itr);
    (**itr).addIncomingLink(*it);
  }
      }
    }

  }
}  

void searchEngine::openAds(string filename) {

 ifstream ifile(filename.c_str());
  if (ifile.fail()){
    cout << "Can't open file..exiting" << endl;
    return;
  }

  string input;
  string keyword;
  float bid;


  //map< string, Set<Ad*> > keywordsToAds;
  int numAds;
  ifile >> numAds;

  while(getline(ifile, input)) {
    string company = "";
    stringstream ss(input);
    ss >> keyword;
    caseLower(keyword);
    ss >> bid;
    //give rest of the stringstream to "company"
    while (ss.tellg() != -1) {
      string concat;
      ss >> concat;
      company += concat;
    }

    Ad* newad = new Ad(company, keyword, bid);
    Set<Ad*> adsForWord;
    adsForWord.insert(newad);
    
    pair< map<string,Set<Ad*> >::iterator, bool> ret;
    //try inserting the keyword into the map
    //won't throw an exception, just won't add if word already exists in map
    ret = keywordsToAds.insert(pair< string, Set<Ad*> >(keyword,adsForWord));
    if (ret.second == false) { //if keyword was already added to the Map
      //add the company to the keyword's associated Set<string>
      keywordsToAds[keyword].insert(newad);
    }
   
    
  }// end of while loop

}

//search function
void searchEngine::search() { 
 
  Set<WebPage*> searchResults;
  Set<Ad*> srelevantAds;
  vector<Ad*> relevantAds;
  valid_search = true;
  results_found = true;
  string search_line = window->getSearchQuery();
  stringstream in(search_line);
  string word;
  stringstream ss(search_line); // start stringstream at beginning again
 
  //CASE 1:
  if (window->word_Search()) { //user only entered one word query
    ss >> word;
    caseLower(word);
    int i=0; char c; bool error = false;
    while (i < static_cast<int>(word.size())) {
      c = word.at(i);
      if (!isalpha(c) && !isdigit(c)){
  //cout << "Invalid search query..." << endl;
  valid_search = false;
  error = true;
  break;
      }
      i++;
    }
    if (!error) {
      map<string, Set<WebPage*> >::iterator it; 
      it  = map_WordstoPages.find(word);
      if (it != map_WordstoPages.end() ) { 
  searchResults = it->second;
      }
      else { //word is not in any webpages
  results_found = false;
  //searchResults should be an empty set
      }
    }

    //now find all relevant ads

    //find the set of companies that have ads for the keyword;
    // Set<string> companies = keywordsToCompanies[word];
    Set<Ad*> ads = keywordsToAds[word];

    //add each pair of company and keyword to the vector of relevantAds
    for(Set<Ad*>::iterator it = ads.begin(); it!=ads.end(); ++it){
      srelevantAds.insert(*it);
    }
    
  }
  
  //CASE 2:
  else if (window->and_Search() ){ //user wants an AND search
    vector<string> queryWords;
    bool error = false;
    char ch;
    
    int word_count = 0;
    int commas = 0;
    while (ss.tellg() != -1) { //not end of input
      ch = ss.peek();
      word = ""; 
     
      if (ch != ' ' && !isalpha(ch) && !isdigit(ch) && ch!=','){
  error = true;
  break;
      }
      
      if (ch == ',' && word_count == commas +1){
  commas++;
  ch = ss.get();
  ch = ss.peek();
      }
      else if (ch == ',') {
  error = true;
  break;
      }
      
      while (isalpha(ch) || isdigit(ch) ){
  ch = ss.get();
  if(isupper(ch)) ch=tolower(ch);
  word += ch;
  ch = ss.peek();
      }
      
      
      //store the word
      if (!word.empty()){
  if (commas != word_count) {
    error = true;
    break;
  }
  queryWords.push_back(word);
  word_count++;
      }
      
      if (ch == ' ') ss.ignore();
    }
    
    //have vector of all query words
    
    
    if (!error && word_count == commas+1 && !queryWords.empty()) {
      //find intersection of them all
      Set<WebPage*> andSet = findIntersect(queryWords, queryWords.size());
      if (andSet.size() >0) searchResults = andSet;
      else results_found = false;
    }
    else {
      valid_search = false;
    }

    //now find all relevant ads

    //for each word in the queryWords vector:
    for (int i=0; i<(int)queryWords.size(); i++) {
      //find the set of Ads that have ads for the keyword;
      Set<Ad*> ads = keywordsToAds[queryWords[i]];
      
      //add each ad to the set of srelevantAds (for no duplicates)
      for(Set<Ad*>::iterator it = ads.begin(); it!=ads.end(); ++it){
	srelevantAds.insert(*it);
      }
    }

  }
  
  //CASE 3:
  else if (window->or_Search() ){
    vector<string> queryWords;
    bool error = false;
    char ch;
    
    int word_count = 0;
    int commas = 0;
    while (ss.tellg() != -1) { //not end of input
      ch = ss.peek();
      word = ""; 
    
      if (ch != ' ' && !isalpha(ch) && !isdigit(ch) && ch!=','){
  error = true;
  break;
      }
      
      if (ch == ',' && word_count == commas +1){
  commas++;
  ch = ss.get();
  ch = ss.peek();
      }
      else if (ch == ',') {
  error = true;
  break;
      }
      
      while (isalpha(ch) || isdigit(ch) ){
  ch = ss.get();
  if(isupper(ch)) ch=tolower(ch);
  word += ch;
  ch = ss.peek();
      }
      
      
      //store the word
      if (!word.empty()){
  if (commas != word_count) {
    error = true;
    break;
  }
  queryWords.push_back(word);
  word_count++;
      }
      
      if (ch == ' ') ss.ignore();
    }
    
    //have vector of all query words
    
    
    if (!error && word_count == commas+1 && !queryWords.empty()) {
      //find union of them all
      Set<WebPage*> orSet;
      orSet = findUnion(queryWords, queryWords.size());
      //cout << orSet.size() << " results found." << endl;
      //print out results
      if (orSet.size() >0){
  searchResults = orSet;
      }
      else {results_found = false;}
    }
    else {
      valid_search = false;
    }
    //now find all relevant ads

    //for each word in the queryWords vector:
    for (int i=0; i<(int)queryWords.size(); i++) {
      //find the set of Ads that have ads for the keyword;
      Set<Ad*> ads = keywordsToAds[queryWords[i]];
      
      //add each ad to the vector of relevantAds
      for(Set<Ad*>::iterator it = ads.begin(); it!=ads.end(); ++it){
	srelevantAds.insert(*it);
      }
    }
    
  }
  
  else { //query did not match a case
    valid_search = false;
  }

  searchResults = pageRank(searchResults);
  vector<WebPage*> search;
  //convert into vector
  int i=0;
  for(Set<WebPage*>::iterator it = searchResults.begin(); it!=searchResults.end(); ++it){
    search.push_back(*it);
    i++;
  }

  //convert set of Ad* to vector for sorting
for(Set<Ad*>::iterator it = srelevantAds.begin(); it!=srelevantAds.end(); ++it){
    relevantAds.push_back(*it);
  }

  if (window->sortByRank()) { //user wants to sort by rank
  //merge sort
  MSort<WebPage*, RankComp> ms;
  //put in type of comparator
  RankComp compR;
  ms.mergeSort(search, compR);
  //result
  // for(unsigned int i=0; i<search.size(); i++){
  //   cout << search[i]->filename() << " " << search[i]->getOldRank() << endl;
  // }
  }
  else { //user wants to sort by name (default)
    MSort<WebPage*, AlphaComp> ms;
    //put in type of comparator
    AlphaComp compR;
    ms.mergeSort(search, compR);

  }

  //sort the relevantAds by highest bid
  //mergesort
  MSort<Ad*, AdComp> ms;
  AdComp comp;
  ms.mergeSort(relevantAds, comp);

  printResults(search, relevantAds);

}

//recursive AND intersection

  Set<WebPage*> searchEngine::findIntersect (const vector<string> & words, int size) {
  
  map<string, Set<WebPage*> >::iterator it; 

  if (size == 1) {// base case
    Set<WebPage*> empty; //empty set
    
    it = map_WordstoPages.find(words[0]);

    if (it != map_WordstoPages.end()) { //if words[0] exists in map
      return map_WordstoPages[words[0]];
    }
    else { //word is not in any Webpages
      return empty;               //   return an empty set
    }
  } //end of base case

  Set<WebPage*> tempSet;
  it  = map_WordstoPages.find(words[size-1]);

  if (it != map_WordstoPages.end()) { //word in webpages
    tempSet = it->second; //tempSet should= it
  } 
  return tempSet.intersect(findIntersect(words,size-1));
  }


    //recursive OR union
  Set<WebPage*> searchEngine::findUnion (const vector<string> & words, int size) {
  map<string, Set<WebPage*> >::iterator it; 

  if (size == 1) {// base case
    Set<WebPage*> empty; //empty set
    it = map_WordstoPages.find(words[0]);
    if (it != map_WordstoPages.end() ){
      return map_WordstoPages[(words[0])];
    }
    else { //word is not in any Webpages
      return empty;               //   return an empty set
    }
  } //end of base case

  Set<WebPage*> tempSet;
  
    it = map_WordstoPages.find(words[size-1]);
 
    if (it != map_WordstoPages.end()) { //word in webpages
      tempSet = it->second; //tempSet should = it
  } 
    return tempSet.Union(findUnion(words,size-1));
  }

  void searchEngine::caseLower(string& str) {
    for (int i=0; i<static_cast<int>(str.size()); i++) {
      if (isupper(str.at(i))) str.at(i)=tolower(str.at(i));
    }
  }


void searchEngine::printResults(vector<WebPage*> results, vector<Ad*> ads) {

  if (!valid_search) {
    //indicate invalid search
    window->setResultStatus(-1);
  }
  else if (!results_found) {
    //indicate no results found
    window->setResultStatus(0);
  }
  else {
    window->setResultStatus(results.size());
    
    //iterate over results and add to file list
    while(!results.empty()){
      window->addToFileList(results.back());
      results.pop_back();
    }

    while(!ads.empty()) {
      window->addToAdList(ads.back());
      ads.pop_back();
    }

  }
  
}

Set <WebPage*> searchEngine::pageRank(Set <WebPage*> searchResults) {
  Set<WebPage*> searchSetT = searchResults;
  //expand one level
  for(Set<WebPage*>::iterator it = searchResults.begin(); it!=searchResults.end(); ++it){
    Set<WebPage*> outgoing = (*it)->allOutgoingLinks();
    Set<WebPage*> incoming = (*it)->allIncomingLinks();
    for(Set<WebPage*>::iterator itOut = outgoing.begin(); itOut!=outgoing.end(); ++itOut){
      // if(searchResults.find(*itOut)!=searchResults.end()) searchSetT.insert(*itOut);
      searchSetT.insert(*itOut);
    }
    for(Set<WebPage*>::iterator itIn = incoming.begin(); itIn!=incoming.end(); ++itIn){
      //if(searchResults.find(*itIn)!= incoming.end()) searchSetT.insert(*itIn);
      searchSetT.insert(*itIn);
    }
  }

  //initialize values
  int sizeT = searchSetT.size();
  for (Set<WebPage*>::iterator it= searchSetT.begin(); it!=searchSetT.end(); ++it) {
    (*it)->setOldRank((double) 1/sizeT);
    (*it)->setNewRank(0);
  }

  //for loop to iterate 30 times
  for (int iter = 0; iter <30; iter++) {

    for(Set<WebPage*>::iterator it = searchSetT.begin(); it!=searchSetT.end(); ++it){
      Set<WebPage*> outgoing = ((*it)->allOutgoingLinks()).intersect(searchSetT);

      for(Set<WebPage*>::iterator itOut = outgoing.begin(); itOut!=outgoing.end(); ++itOut){
        (*itOut)->addNewRank(0.85*((*it)->getOldRank())/outgoing.size());
        //cout << "new rank 1: " << (*itOut)->filename() << " " << 0.85*((*it)->getOldRank())/outgoing.size() << " " << (*itOut)->getNewRank() << endl;
      }
      if(outgoing.size()==0){
        for(Set<WebPage*>::iterator itAll = searchSetT.begin(); itAll!=searchSetT.end(); ++itAll){
          (*itAll)->addNewRank(0.85*((*it)->getOldRank())/sizeT);
          //cout << "new rank 2: " << (*itAll)->filename() << " " << 0.15*((*it)->getOldRank())/sizeT << " " << (*itAll)->getNewRank() << endl;
        }
      }

      for(Set<WebPage*>::iterator itAll = searchSetT.begin(); itAll!=searchSetT.end(); ++itAll){
        (*itAll)->addNewRank(0.15*((*it)->getOldRank())/sizeT);
        //cout << "new rank 3: " << (*itAll)->filename() << " " << 0.15*((*it)->getOldRank())/sizeT << " " << (*itAll)->getNewRank() << endl;
      }
    }

    //not inside set T
    for(Set<WebPage*>::iterator it = set_allWebPages.begin(); it!=set_allWebPages.end(); ++it){
      if(searchSetT.find(*it)==searchSetT.end()){
        for(Set<WebPage*>::iterator itAll = set_allWebPages.begin(); itAll!=set_allWebPages.end(); ++itAll){
          (*itAll)->addNewRank(((*it)->getOldRank())/set_allWebPages.size());
        }
      }
    }  
    //new update = old update
    //cout << "RANK" << endl;
    double total = 0;

    for(Set<WebPage*>::iterator it = set_allWebPages.begin(); it!=set_allWebPages.end(); ++it){
      (*it)->updateOldRank();
      (*it)->setNewRank(0);
      //cout << (*it)->getOldRank() << endl;
      total = total + (*it)->getOldRank();
    }
    //cout << "total: " << total << endl;
  }
  
  
  // for (Set<WebPage*>::iterator it = searchSetT.begin(); it!= searchSetT.end(); ++it) {
  //   cout << "WebPage: " << (*it)->filename() << " Rank: " << (*it)->getOldRank() << endl;
  // }

  return searchSetT;

}


void searchEngine::searchExit(map<string,float> bills) {
  //iterate over bills and write to file

  ofstream ofile(billFile.c_str());

  for (map<string,float>::iterator it = bills.begin(); it!= bills.end(); ++it) {
    ofile << it->first << endl;
    ofile << it->second << endl;
    ofile << endl;
  }

  ofile.close();

  close();
}
