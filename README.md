##CSCI 104 Team Project Repository

###Student Information
**Developer 1**:
  + Name: Gyan Neil Suri
  + USC Email: gsuri@usc.edu
  + USC ID: 7377104586

**Developer 2**:
  + Name: Melanie Shaul
  + USC Email: mshaul@usc.edu
  + USC ID: 2054463628

----------------------------------------------------------------------------------------------------------------------------------------------

WebCrawler:

compile: NOTE: you must have curl on your VM in order to run the webcrawler 
(If you dont have curl run this on terminal: sudo aptitude install libcurl4-openssl-dev)
run make -f Makefile.webCrawler

run instructions: ./webCrawler
the terminal will prompt you for the seed file (which should be in the current directory) - we have provided a "seed.txt" file for your convenience that includes webpages that work well with our crawler.

You can use a different seed file, type in different sites and save the file as whatever you want. Input the name of this text file when prompted.

the webcrawler will output textfiles with the first line of the textfile being the name of the URL of the webpage it loaded the text from. And an "allfiles.txt" with the names of all webpages text files in the "database" folder

Do NOT include the following websites in the seed.txt: http://google.com (google is heavily scripted and therefore doesn't work as well - the webcrawler will still function if you include a google website, but there will be a lot of junk in the output contents of webpage).

*in the seed file, when adding webpages make sure you use the complete http:// website name

*Important: Occassionally the parsing and downloading of the webpages will stop due to curl (no fault of ours), and you may have to quit the process and run it again (usually won't happen, but just in case). Even if you run it with the same seed file, it should work the next time.

----------------------------------------------------------------------------------------------------------------------------------------------
Search Engine:
compile instructions: do "qmake" and "make" (do NOT run qmake -project)

run instructions: ./team_project_128 (webpage input file) (advertisement input file) (advertisement billing output file)
NOTE: the webpage input file we provided is database/allfiles.txt

so run:
./team_project_128 database/allfiles.txt (advertisement file) (advertisement billing output file)

----------------------------------------------------------------------------------------------------------------------------------------------

Advertisements:

You must close the pop-up window advertisment before continuing with the Qt GUI.
In the output file, only companies with a non-zero bill will be shown.

Advertisements will only appear if the search results yielded results. 
We did not provide an adveristement input file.

**NOTE: in order to get the correct output results for the company billing info, you must quit the search engine via the QUIT BUTTON, not the X button in the top left corner.






NOTE: The repository does not contain folders for organization because we felt there was no logical way to organize it besides the way we have it.