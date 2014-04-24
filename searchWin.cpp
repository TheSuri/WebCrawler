#include <string>
#include <map>
#include <iostream>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDialog>
#include "searchWin.h"
#include "WebPage.h"
#include "fileWin.h"
#include <QListWidget>
#include <QRadioButton>


using namespace std;

searchWin::searchWin(QWidget* parent): QWidget (parent) { //constructor

  //allocate buttons
  searchButton = new QPushButton("&Search");
  quitButton = new QPushButton("&Quit");
  statusLabel = new QLabel;
  andSearch = new QRadioButton("AND");
  orSearch = new QRadioButton("OR");
  wordSearch = new QRadioButton("single word");
  searchText = new QLineEdit;
  fileWindow = new fileWin;
  sort = new QLabel("Sort by:");
  alphaSort = new QRadioButton("Alphabetically");
  rankSort = new QRadioButton("Relevance");
  wordSearch->setChecked(true);

  searchType = new QButtonGroup;
  sortType = new QButtonGroup;
  searchType->addButton(andSearch);
  searchType->addButton(orSearch);
  searchType->addButton(wordSearch);
  sortType->addButton(alphaSort);
  sortType->addButton(rankSort);


  QFormLayout* searchLineLayout = new QFormLayout;
  searchLineLayout->addRow(searchText, searchButton);
  searchLineLayout->addRow(andSearch);
  searchLineLayout->addRow(orSearch);
  searchLineLayout->addRow(wordSearch);
  searchLineLayout->addRow(sort);
  searchLineLayout->addRow(alphaSort, rankSort);

  file_list = new QListWidget;
  

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addLayout(searchLineLayout);
  mainLayout->addWidget(statusLabel);
  mainLayout->addWidget(file_list);
  mainLayout->addWidget(quitButton);

  //connect button signals to slots
  connect(searchButton, SIGNAL(clicked()), this, SLOT(searchClicked()));
  connect (quitButton, SIGNAL(clicked()), this, SLOT(quitClicked()));
  connect(file_list, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(openFileWin(QListWidgetItem*)));
  connect(fileWindow, SIGNAL(fileWinClicked(QListWidgetItem*)), this, SLOT(openFileWin(QListWidgetItem*)));

  setLayout(mainLayout);
}

searchWin::~searchWin() {
  delete statusLabel;
  delete searchText;
  delete searchButton;
  delete quitButton;
  delete file_list;
  delete fileWindow;
  ItemstoWebpages.clear();
}

void searchWin::searchClicked() {
  //clear the map of list widgets to webpages
  ItemstoWebpages.clear();

  //clear the file_list if any previous searches are there
  //this deletes all the listwidget items also
 file_list->clear();

  //get the search query
   string search_query = searchText->text().toStdString();

  if (search_query != "") {
    search_text = search_query;
  }
  else {
    searchText->setText(QString("Please enter a search query"));
  }
  emit searchReady();

}

void searchWin::quitClicked() {
  fileWindow->close();
  emit exit();
  close();
}


void searchWin::addToFileList(WebPage* webpage) {
 
  QListWidgetItem *newItem = new QListWidgetItem;
  newItem->setText(QString::fromStdString(webpage->filename()));
  pair<QListWidgetItem*, WebPage*> pair(newItem, webpage);
  ItemstoWebpages.insert(pair);
  //need to somehow store the webpage*
  //may want to create derived class of QListWidgetItem that can store the webpage*
  file_list->insertItem(0,newItem);
}


void searchWin::setResultStatus(int status) {
  if (status == -1) { //invalid query
    statusLabel->setText("Invalid Search Query");
  }
  else if (status == 0) { //no results found
    statusLabel->setText("No Results Found");
  }
  else {
    statusLabel->setText(QString::number(status)+ " Results Found");
  }
}

void searchWin::openFileWin(QListWidgetItem *item) {
  //WebPage* temp = item->getWebPage();
  //fileWindow->displayWebPage(temp);

  //WebPage* temp;
  map < QListWidgetItem*, WebPage*>::iterator it = ItemstoWebpages.find(item);
  if (it!= ItemstoWebpages.end()) {
    //temp = it->second;
    fileWindow->displayWebPage(it->second);
  }

  fileWindow->show();
}
