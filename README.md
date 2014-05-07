##CSCI 104 Team Project Repository

###Student Information
**Developer 1**:
  + Name: Gyan Neil SUri
  + USC Email: gsuri@usc.edu
  + USC ID: 7377104586

**Developer 2**:
  + Name: Melanie Shaul
  + USC Email: mshaul@usc.edu
  + USC ID: 2054463628



WebCrawler:

compile: NOTE: you must have curl on your VM in order to run the webcrawler 
run make -f Makefile.webCrawler

run instructions: ./webCrawler
the terminal will prompt you for the seed file (which should be in the current directory) - we have provided a "seed.txt" file for your convenience
the webcrawler will output textfiles of the webpages and a "allfiles.txt" with the names of all webpages text files in the "database" folder


Search Engine:
compile instructions: do "qmake" and "make" (do NOT run qmake -project)

run instructions: ./team_project_128 <webpage input file> <advertisement input file> <advertisement billing output file>
NOTE: the webpage input file we provided is database/allfiles.txt

so run:
./team_project_128 database/allfiles.txt <advertisement file> <advertisement billing output file>


Advertisements:

You must close the pop-up window advertisment before continuing with the Qt GUI.
In the output file, only companies with a non-zero bill will be shown.

**NOTE: in order to get the correct output results for the company billing info, you must quit the search engine via the QUIT BUTTON, not the X button in the top left corner.