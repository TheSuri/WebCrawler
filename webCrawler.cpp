#include <iostream>
#include <fstream>
#include <cwctype>
#include <curl/curl.h>
#include <sstream> 
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <cctype>
#include <deque>
#include <set>
using namespace std;
int numfile =0 ,nummaxfiles;
string data;
set <string> linksforcrawl;
void setlinks(string data4, vector <string> tags, string site);
void webcrawler(string seedFile);
void crawl(string site, int numfile);
void parse(string data, string site);
void crawldeeper();
void formatlinks();
size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up)
{ //callback must have this declaration
    //buf is a pointer to the data that curl has for us
    //size*nmemb is the size of the buffer

    for (int c = 0; c<size*nmemb; c++)
    {
        data.push_back(buf[c]);
    }
    return size*nmemb; //tell curl how many bytes we handled
}

void webcrawler(string seedFile)
{
  ofstream allsitesout("AllSites.txt",  ios::out );
  ifstream ifile(seedFile.c_str());
  if(ifile.fail()) cout << "Cannot read seed file" << endl;
  else{
    string site;
    while(1){
      ifile >> site;
      if(ifile.fail()) break;
        ++::numfile;
        cout<<endl<<"Downloading and parsing webpage "<<numfile<<endl;
        cout<< site;
        string fileNum;
        crawl(site, ::numfile);
        string database= "database/";
        stringstream ss;
        ss<<::numfile;
        ss>>fileNum;
        ss.clear();
        string opfile= database+fileNum+".txt";
        allsitesout<<site<<' '<<opfile<<endl;
        ifstream textfile(opfile.c_str());
      }
   }
   allsitesout.close();
}     

void crawldeeper()
{
  cout<<endl<<"Now crawling deeper into each of the outgoing links of seed file"<<endl;
  cout<<linksforcrawl.size();
  
  while(linksforcrawl.size()!=0 && numfile < nummaxfiles)
  {
    ifstream allsitesin("AllSites.txt",  ios::in );
    string allsitescrawled="";
    string buffer;
    allsitesin.clear();
    allsitesin.seekg(allsitesin.beg);
    while (getline(allsitesin, buffer))
    {
      allsitescrawled += (buffer + '\n');
    }
    allsitesin.close();
    set<string>::iterator outlinksit = linksforcrawl.begin();
    advance(outlinksit, rand()%linksforcrawl.size());
    if(allsitescrawled.find(*outlinksit)!=-1)
      {
        linksforcrawl.erase(*outlinksit);
        continue;
      }
    else
    {
      ofstream allsitesout("AllSites.txt",  ios::out |ios::app );
      ++::numfile;
      cout<<endl<<"Downloading and parsing webpage "<<numfile<<endl;
      crawl(*outlinksit, numfile);
      string database= "database/";
      string fileNum;
      stringstream ss;
      ss<<::numfile;
      ss>>fileNum;
      ss.clear();
      string opfile= database+fileNum+".txt";
      cout<<*outlinksit;
      allsitesout<<*outlinksit<<' '<<opfile<<endl;
      linksforcrawl.erase(*outlinksit);
      allsitesout.close();
    }
  }
}

void crawl(string site, int numfile)
{
  CURL* curl; //our curl object
    curl_global_init(CURL_GLOBAL_ALL); //pretty obvious
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, site.c_str());
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,1);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    parse(data, site);
    data="";
}

void parse(string data, string site)
{
  string data2="", data3="";
    for (int i = 0; data[i] != '\0'; i++)
  {
    if(data[i]=='>')
    {
      data2+=">";
      //data2+='\n';
      continue;
    }
    else if(data[i]=='<')
    {
      //data2+='\n';
      data2+="<";
      continue;
    }
    data2+=data[i];
  }
    int numberofTags=0;
    for (int i = 0; data[i] != '\0'; i++)
  {
    if(data2[i]=='<')
    {
      numberofTags++;
    }
  }
    vector <string> tags;
    string tag;
    int j=0;
    for (int i = 0; data[i] != '\0'; i++)
    {
    tag="";
    if(data2[i]=='<')
    {
      ++i;
      while(data2[i]!='>')
      {
        tag+=data2[i];
        i++;
        if(tag=="script"|| tag==" script"|| tag=="  script"||tag=="style"|| tag==" style" ||tag=="style ")
        {
          //cout<<"OOOOOOOOOOOO KILL EM "<<tag<<endl<<endl<<endl<<endl<<endl<<endl<<endl;
          int counter=i;
          while(data2[counter]!='>')
          {
            counter++;
          }
        i=counter+1;
        }
      }
      tags.push_back(tag);
      j++;
    }
      else
      {
      data3+=data2[i];
      }    
    }
    string data4="";
     j=0;
    for ( string::iterator it=data3.begin(); it!=data3.end(); ++it)
    {
      if((data4[j]=='\n' && *it=='\n') || (*it=='\t') || (data4[j]==' ' && *it==' '))
      {
        continue;
      }
      else
      {
        data4+=*it;
        j++;
      }
    }
    setlinks(data4, tags, site);
}

void setlinks(string data4, vector <string> tags, string site)
{
  string str2="href=";
    string outgoinglink="";
    set <string> outgoinglinks;
    for(int j=0; j<tags.size(); j++)
    {
      string tag= tags[j];
      if(tag[0]=='a')
      {
      int found = tag.find(str2);
      if(found!=-1)
      {
      bool flag=false;
      for(int i=found+6; tag[i]!='"';i++)
      {
        outgoinglink+=tag[i];
      }
      string mainsite;
      int i= 0, count =3;
      for(i; count!=0; i++)
      {
      if(site[i]=='/')
      {
         --count;
        if(count==0) break;
        mainsite+=site[i];
      }  
      else
        mainsite+=site[i];
      }
      
      if(outgoinglink[0]=='/' && outgoinglink[1]!='/')
      {
        outgoinglink=mainsite+outgoinglink;
      }
      for(string::iterator it=outgoinglink.begin(); it!=outgoinglink.end(); ++it)
        {
          if( (int) (outgoinglink[i]) < 32 || (int) (outgoinglink[i]) > 126 || outgoinglink[i]=='\b' || outgoinglink[i]=='\0')
          {
          flag=true;
          break;
          }
        }
      if (flag==false)
      {
        string space=" ", png=".png", pdf= ".pdf", quote="'", semicolon=";", question="?", doublequote="\"", jpg="jpg", ppt="ppt", jpeg="jpeg" ;
        if(outgoinglink.find(space)==-1 && outgoinglink.find(pdf)==-1 && outgoinglink.find(png)==-1 && outgoinglink.find(quote)==-1 && outgoinglink.find(semicolon)==-1 && outgoinglink.find(question)==-1 && outgoinglink.find(doublequote)==-1 && outgoinglink.find(jpg)==-1 && outgoinglink.find(ppt)==-1 && outgoinglink.find(jpeg)==-1)
        {
          string bar= "////", bar3="http:///" ;
          if(outgoinglink.find(bar)!=-1)
          {
          int k =outgoinglink.find(bar);
          outgoinglink.replace(k, k+3, "//");
          } 
          if(outgoinglink.find(bar3)!=-1)
          {
            cout<<endl<<"replacing "<<outgoinglink<<endl<<"with mainsite: "<<mainsite<<'+';
            int k =outgoinglink.find(bar3);
            outgoinglink.replace(k, k+7, "");
            cout<<outgoinglink<<'=';
            outgoinglink=mainsite+outgoinglink;
            cout<<outgoinglink<<endl;
          }
          outgoinglinks.insert(outgoinglink);
          outgoinglink="";
        }
        } 
      }
    }
  }
     
/* ---------------------------------------------------------
------------------------------------------------------------
        Writng new webpage into txt file      */
string database="database/", str="http://";
string fileNum;
stringstream ss;
ss<<::numfile;
ss>>fileNum;
ss.clear();
string opfile= database+fileNum+".txt";
ofstream ofile(opfile.c_str());
ofile<<site<<endl<<data4<<endl<<"AllOutgoingLinks:"<<endl;
int i=1;
for (set<string>::iterator outlinksit = outgoinglinks.begin(); outlinksit != outgoinglinks.end(); outlinksit++) 
  {
    if(i>8)
    {
      break;
    }
    if((*outlinksit).find(str)!=-1)
      {
        ofile<<*outlinksit<<endl;
        //if(allsites.find(outgoinglinks[i])
        linksforcrawl.insert(*outlinksit);
        i++;
      }
   }   
ofile.close();
}

void formatlinks()
{
  string currfile;
  long pos;
  ifstream allsitesout("AllSites.txt",  ios::out );
  string buffer;
  string allsites;
  while(allsitesout)
  {
    allsitesout>>buffer;
    allsites+=buffer+' ';
  }
  allsitesout.close();
  
  for(int i=1; i<=numfile; i++)
  {
    string str="", buffer="";
    stringstream ss;
    ss<<i;
    ss>>currfile;
    ss.clear();
    currfile="database/"+currfile+".txt";
    ifstream ifile(currfile.c_str(), ios::in | ios::out);
    if(ifile.fail())
      {
        cout<<"failed to open"<<currfile;
        continue;
      }

    while(ifile)
    {
      getline(ifile, buffer);
      str=str+buffer+'\n';
      if(buffer=="AllOutgoingLinks:")
      {
        break;
      }
    }
   //cout<<"in file "<<currfile<<endl;
   while(ifile)
   {
    ifile>>buffer;
    int k= allsites.find(buffer);
    if(k!=-1)
    {
      str+='['+buffer+']'+'(';
      while(allsites[k]!=' ')
      {
        ++k;
      }  
      k=k+1;
      while(!(iswspace(allsites[k])))
      {
        str+=allsites[k];
        k++;
      }
      str+=")\n";
    }
    else
    {
      str+=buffer+'\n';
    }
   }
  
  ifile.close();
  ofstream ocurrfile(currfile.c_str(), ios::out);
  ocurrfile<<str;
  ocurrfile.close();
  str="";
 }
}

int main(int argc, char const *argv[])
{
    cout<<"Please enter name of seed file. (seed file should be in same directory as this program)"<<endl;  
    string seed;
    cin>>seed;
    cout<<endl<<"Please enter the total number of links to parse. (70 is an optimal number as tested)"<<endl;
    cin>>nummaxfiles;
    cout<<"Please wait while the program loads webpages from the internet...(30 seconds to 3 minutes)"<<endl;
    webcrawler(seed);
    crawldeeper(); //Goes deeper into each link it just crawled. and crawls those outgoing links
    cout<<endl<<"Now formatting all links..."<<endl;
    formatlinks();
    cout<<"done."<<endl;
    ofstream ofile("database/allfiles.txt", ios::out);
    for(int i=1; i<=numfile; i++)
    {
      ofile<<"database/"<<i<<".txt"<<endl;
    }
  return 0;
}