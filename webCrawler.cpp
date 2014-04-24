#include "webCrawler.h"
#include <iostream>
using namespace std;
#include <fstream>


webCrawler::webCrawler(){
}

webCrawler::~webCrawler(){

}

webCrawler::webCrawler(string f, string output){
	ifstream ifile(f.c_str());
	if(ifile.fail()) cout << "Cannot read file" << endl;
	else{
		string file;
		while(1){
			ifile >> file;
			if(ifile.fail()) break;
			if(linksFound.find(file)==linksFound.end()){
				crawl(file);
			}
		}
		
    //output file
    //filename
    // int index=0;
    // for(int i=f.size()-1; i>0; i--){
    //   if(f[i]=='/'){ index = i; break; }
    // }
    //string output = f.substr(0,index) + "/allLinks.txt";
    for(unsigned int i=0; i<allLinks.size(); i++){
      cout << allLinks[i] << endl;
    }
    ofstream ofile(output.c_str());
    for(unsigned int i=0; i<allLinks.size(); i++){
      ofile << allLinks[i] << endl;
    }
    
	}
}


void webCrawler::crawl(string f){  //recursive DFS
	//base case
	if(linksFound.find(f)!=linksFound.end()){
    //cout << f << endl;
		return;
	}
	linksFound.insert(f);
	allLinks.push_back(f);
	queue<string> outgoing = getOutgoingLinks(f);
	while(!outgoing.empty()){
		string s = outgoing.front();
		outgoing.pop();
		crawl(s);
	}
	

	
}

queue<string> webCrawler::getOutgoingLinks(string f){
  queue<string> outgoing;
  ifstream ifile(f.c_str());
  if(ifile.fail()){ throw invalid_argument("Cannot read file"); }
  string str;
  string str2 = " "; //use for parsing links
  string n = ""; string l = "";
  bool link;
  while(1){
    link = false;
    ifile >> str;
    if(ifile.fail()){ break; }
    for(unsigned int i=0; i<str.size(); i++){
      str[i] = tolower(str[i]);
    }
    //int numSize = str.size();
    while(str[0]=='['){
      ifile >> str2;
      if(ifile.fail()){ break; }
      if(str.find(']')!=string::npos || str2.find(']')!=string::npos){ link = true; }
      if(!ifile.fail()){
        while(str2[str2.length()-1]!=')' || (str2[str2.length()-1]==')' && !link)){ //not the end of link
          str = str + " " + str2;
          ifile >> str2;
          if(ifile.fail()){ break; }
          if(str.find(']')!=string::npos || str2.find(']')!=string::npos){ link = true; }
        }
      }
      if(link){
      	string s = str+" "+str2;
		  //n = s.substr(1, s.find_first_of(']')-1); //anchor text
    	if(s.find_first_of('(')<s.find_first_of(']')) s = s.substr(s.find_first_of('(')+1);
    	l = s.substr(s.find_first_of('(')+1);
    	l = l.substr(0, l.find_first_of(')')); //hyperlink
    	//allLinks.push_back(l);
    	outgoing.push(l);
      }
      str = str2;
    }
    //read last string
    if(str.find_first_of('(')!=string::npos && str[str.length()-1]==')'){
      l = str.substr(str.find_first_of('(')+1);
      l = l.substr(0, l.find_first_of(')'));
      outgoing.push(l);
    }
  }
  return outgoing;
} 


 int main(int argc, char* argv[]){
	if(argc < 3){
		cout << "Please enter source file" << endl;
		return 1;
	}
	string f(argv[1]);
  string output(argv[2]);
	webCrawler* c = new webCrawler(f, output);
	delete c;
}
