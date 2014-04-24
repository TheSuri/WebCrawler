#ifndef WEBCRAWLER_H
#define WEBCRAWLER_H
#include <iostream>
using namespace std;
#include <string>
#include <fstream>
#include "lib/Set.h"
#include <map>
#include <queue>
#include <vector>

class webCrawler{
	public:
		webCrawler();
		webCrawler(string f, string o);
		~webCrawler();
		queue<string> getOutgoingLinks(string f);
		void crawl(string f);
	private:
		vector<string> allLinks;
		Set<string> linksFound;

};

#endif
