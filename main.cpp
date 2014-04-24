#include <QApplication>
#include "searchEngine.h"

using namespace std;

int main(int argc, char* argv[])
{

  //check for number of inputs correct

  QApplication myapp(argc, argv);
  searchEngine myengine(argv[1], argv[2]);
  myengine.show();

  return myapp.exec();
  return 0;
}
