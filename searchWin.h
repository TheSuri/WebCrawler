#ifndef SEARCHWIN_H
#define SEARCHWIN_H

#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QString>
#include <string>
#include <QListWidget>
#include "WebPage.h"
#include "fileWin.h"
#include <QRadioButton>
#include <QButtonGroup>

using namespace std;

class searchWin : public QWidget {
Q_OBJECT

  public:
  searchWin(QWidget* parent = NULL);
  ~searchWin();
  string getSearchQuery() { return search_text; }
  void setResultStatus(int status);
  void addToFileList(WebPage* webpage);
  bool sortByRank() { return rankSort->isChecked(); }
  bool and_Search() { return andSearch->isChecked(); }
  bool or_Search() { return orSearch->isChecked(); }
  bool word_Search() { return wordSearch->isChecked(); }

  //define initial size?
 signals:
  void searchReady();
  void exit();


  private slots:
  void searchClicked();
  void quitClicked();
  void openFileWin(QListWidgetItem *item);
  //scroll bars?

 private:
  QLabel* statusLabel;
  QLineEdit* searchText;
  QPushButton* searchButton;
  QPushButton* quitButton;
  string search_text;
  QListWidget* file_list;
  fileWin* fileWindow;
  QRadioButton* alphaSort;
  QRadioButton* rankSort;
  QRadioButton* andSearch;
  QRadioButton* orSearch;
  QRadioButton* wordSearch;
  QButtonGroup* searchType;
  QButtonGroup* sortType;
  QLabel* sort;
  map <QListWidgetItem* , WebPage*> ItemstoWebpages;
};

#endif
