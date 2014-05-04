#include <QApplication>
#include <iostream>
#include "searchEngine.h"

using namespace std;

int main(int argc, char* argv[])
{

  //check for number of inputs correct
  if (argc != 4) {
    cout << "Enter correct number of input files" << endl;
    return 0;
  }

  QApplication myapp(argc, argv);
  searchEngine myengine(argv[1], argv[2], argv[3]);
  myengine.show();

  return myapp.exec();

  return 0;
}
