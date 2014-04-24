#include <QVBoxLayout>
#include <string>
#include <QHBoxLayout>
#include "WebPage.h"
#include "fileWin.h"
#include <QListWidget>
//#include "fileListItem.h"


using namespace std;

fileWin::fileWin(QWidget* parent):QWidget (parent) { 

  inLabel = new QLabel("Incoming Links");
  outLabel = new QLabel("Outgoing Links");

  nameLabel = new QLabel;

  fileContents = new QTextEdit;
  incomingList = new QListWidget;
  outgoingList = new QListWidget;

  //set contents of file to read only
  fileContents->setReadOnly(true);


  QVBoxLayout *linksLayout = new QVBoxLayout;
  linksLayout->addWidget(inLabel);
  linksLayout->addWidget(incomingList);
  linksLayout->addWidget(outLabel);
  linksLayout->addWidget(outgoingList);

  QVBoxLayout *fileDisp = new QVBoxLayout;
  fileDisp->addWidget(nameLabel);
  fileDisp->addWidget(fileContents);

  QHBoxLayout *mainLayout = new QHBoxLayout;
  mainLayout->addLayout(fileDisp);
  mainLayout->addLayout(linksLayout);

  //  connect(incomingList, SIGNAL(itemDoubledClicked(QListWidgetItem*)), this, SLOT(fileClicked(QListWidgetItem*)));
  connect(outgoingList, SIGNAL(itemDoubleClicked(QListWidgetItem*)),this , SLOT(fileClicked(QListWidgetItem*)));
  connect(incomingList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(fileClicked(QListWidgetItem*)));
  setLayout(mainLayout);
}

fileWin::~fileWin() {
  delete inLabel;
  delete outLabel;
  delete nameLabel;
  delete fileContents;
  delete incomingList;
  delete outgoingList;
}

void fileWin::displayWebPage(WebPage* webpage) {
  ItemstoWebpages.clear();


  fileContents->setText(QString::fromStdString(webpage->getFileContents()));
  incomingList->clear();
  outgoingList->clear();  


 Set<WebPage*> outgoing = webpage->allOutgoingLinks();
  for (Set<WebPage*>::iterator it = outgoing.begin(); it!= outgoing.end(); ++it) {
    QListWidgetItem *newItem = new QListWidgetItem;
    newItem->setText(QString::fromStdString((**it).filename()));
    outgoingList->insertItem(0,newItem);

    //add to map
    pair<QListWidgetItem*, WebPage*> pair(newItem, (*it));
    ItemstoWebpages.insert(pair);
  }


  Set<WebPage*> incoming = webpage->allIncomingLinks();
  for (Set<WebPage*>::iterator it = incoming.begin(); it!= incoming.end(); ++it){
    QListWidgetItem *newItem = new QListWidgetItem;
      newItem->setText(QString::fromStdString((**it).filename()));
      incomingList->insertItem(0,newItem);
      //add to map
      pair<QListWidgetItem*, WebPage*> pair(newItem, (*it));
      ItemstoWebpages.insert(pair);
  }
  
  nameLabel->setText(QString::fromStdString(webpage->filename()));

}
	 
 	
void fileWin::fileClicked(QListWidgetItem* item) {

  // WebPage* temp;
  map < QListWidgetItem*, WebPage*>::iterator it = ItemstoWebpages.find(item);
  if (it!= ItemstoWebpages.end()) {
    //temp = it->second;
    displayWebPage(it->second);
  }

}
