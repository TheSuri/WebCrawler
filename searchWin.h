#ifndef SEARCHWIN_H
#define SEARCHWIN_H

#include <QPushButton>
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QString>
#include <string>
#include <QListWidget>
#include "WebPage.h"
#include "fileWin.h"
#include "Ad.h"
#include <QRadioButton>
#include <QButtonGroup>
#include <QCloseEvent>
#include <QMessageBox>

using namespace std;

class searchWin : public QWidget {
Q_OBJECT

  public:
  searchWin(QWidget* parent = NULL);
  ~searchWin();
  string getSearchQuery() { return search_text; }
  void setResultStatus(int status);
  void addToFileList(WebPage* webpage);
  void addToAdList(Ad* ad);
  bool sortByRank() { return rankSort->isChecked(); }
  bool and_Search() { return andSearch->isChecked(); }
  bool or_Search() { return orSearch->isChecked(); }
  bool word_Search() { return wordSearch->isChecked(); }

 protected:
  virtual void closeEvent(QCloseEvent* event);

  //define initial size?
 signals:
  void searchReady();
  void exit(map<string, float> bills);


  private slots:
  void searchClicked();
  void quitClicked();
  void openAdWin(QListWidgetItem *item);
  void openFileWin(QListWidgetItem *item);
  void billCompany(QListWidgetItem *item);
 

 private:
  QLabel* statusLabel;
  QLineEdit* searchText;
  QPushButton* searchButton;
  QPushButton* quitButton;
  string search_text;
  QListWidget* file_list;
  QListWidget* ad_list;
  fileWin* fileWindow;
  QMessageBox* adMessage;
  QRadioButton* alphaSort;
  QRadioButton* rankSort;
  QRadioButton* andSearch;
  QRadioButton* orSearch;
  QRadioButton* wordSearch;
  QButtonGroup* searchType;
  QButtonGroup* sortType;
  QLabel* sort;
  map <QListWidgetItem* , WebPage*> ItemstoWebpages;
  map <QListWidgetItem*, Ad*> ItemstoAds;
  map <string, float> companyBills;
};

#endif
