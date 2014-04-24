#ifndef FILEWIN_H
#define FILEWIN_H

#include <QListWidget>
#include <QPushButton>
#include <QTextEdit>
#include <string>
#include <QLabel>
#include <QString>
#include "WebPage.h"
//#include "fileListItem.h"

using namespace std;

class fileWin: public QWidget {
  Q_OBJECT

    public:
  fileWin(QWidget* parent = NULL);
  ~fileWin();
  void displayWebPage(WebPage* webpage);

  private slots:
  void fileClicked(QListWidgetItem* item);

 signals:
  void fileWinClicked(QListWidgetItem* item);

 private:
  QListWidget* incomingList;
  QListWidget* outgoingList;
  QLabel* inLabel;
  QLabel* outLabel;
  QLabel* nameLabel;
  QTextEdit* fileContents;
  map <QListWidgetItem*, WebPage*> ItemstoWebpages;

};

#endif
