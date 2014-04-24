
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
#include <vector>

class searchEngine: public QWidget {
Q_OBJECT

public:
searchEngine();
 searchEngine(string filename, string adfile);
~searchEngine();

private:
 Set<WebPage*> pageRank(Set<WebPage*> searchResults);
Set<WebPage*> findUnion (const vector<string> & words, int size);
Set<WebPage*> findIntersect (const vector<string> & words, int size);
void caseLower(string& str);
void openWebPages(string filename);
 void openAds(string filename);
 void printResults(vector<WebPage*> results);

signals:


private slots:
void search();
void searchExit();


private:
searchWin* window;
map<string, Set<WebPage*> > map_WordstoPages;
Set<WebPage*> set_allWebPages;
 bool valid_search;
 bool results_found;
 QVBoxLayout* mainLayout;


};
