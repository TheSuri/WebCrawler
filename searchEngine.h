
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
#include "Ad.h"
#include <vector>

class searchEngine: public QWidget {
Q_OBJECT

public:
searchEngine();
 searchEngine(string filename, string adfile, string billfile);
~searchEngine();

private:
 Set<WebPage*> pageRank(Set<WebPage*> searchResults);
Set<WebPage*> findUnion (const vector<string> & words, int size);
Set<WebPage*> findIntersect (const vector<string> & words, int size);
void caseLower(string& str);
void openWebPages(string filename);
 void openAds(string filename);
 void printResults(vector<WebPage*> results, vector<Ad*> ads);

signals:


private slots:
void search();
 void searchExit(map<string,float> bills);


private:
searchWin* window;
map<string, Set<WebPage*> > map_WordstoPages;
Set<WebPage*> set_allWebPages;
 map< string, Set<Ad*> > keywordsToAds;
 bool valid_search;
 bool results_found;
 string billFile;
 QVBoxLayout* mainLayout;


};
