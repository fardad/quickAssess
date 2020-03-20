#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <iostream>
#include "debug.h"
#include "Command.h"
#include "Vals.h"
#include "SubVals.h"
#include "Date.h"
#include "User.h"
#include "colors.h"

#define  QA_VERSION "0.8"   
#define QA_DATE "2020-03-20"

using namespace std;
const int configFile = 0;
const int questionFile = 1;
const int allFiles = 2;


void prn(string val);

bool getAssignmentValues(SubVals& SV, string fname);

void getFileName(char* fname, char* argv[], int fileType);

void getStudnetQuestionFileName(char* fname, char* argv[]);

bool fileExists(const char* fname);

/*
0: /home/fardad.soleimanloo/getfiles
1 : 244
2 : NBB
3 : exam
4 : 12
*/
int main(int argc, char* argv[]) {
#ifndef _MSC_VER
   char lc_all[] = "LC_ALL=en_CA.UTF-8";
   char lc_col[] = "LC_COLLATE=en_CA.UTF-8";
   putenv(lc_all);
   putenv(lc_col);
#endif
   Date now;
   cout << col_grey << "quickAssess (V" << QA_VERSION << ")" << endl;
   cout << "by Fardad S. (Last update: " << QA_DATE << ")" << endl
      << "===============================================================" << col_end << endl <<
      col_white << "System date and time: " << now << col_end << endl << endl;

#ifdef FS_DEBUG
   cout << "Debugging------------------------------------" << endl;
#endif // FS_DEBUG

   bool ok = true;
   if (argc != 5) {
      cout << "Bad getfiles arguments, reprot this to your professor." << endl;
      ok = false;
   }
   else {
      SubVals cfgVals;
      Date cfgDate;
      std::stringstream ssDate;
      char fname[1024];
      getFileName(fname, argv, configFile);
      if (ok && getAssignmentValues(cfgVals, fname)) {
         if (!cfgVals.exist("publish_date")) {
            ok = false;
            cout << "Publish date is missing, report this to your professor!" << endl;
         }
         prn("Publish date");
         if (ok) {
            ssDate << cfgVals["publish_date"][0];
            cfgDate.read(ssDate);
            if (cfgDate.bad()) {
               cout << "Bad publish date format in config file." << endl
                  << "Please report this to your professor!" << endl;
               ok = false;
            }
            if (ok && now < cfgDate) {
               cout << "The " << argv[3] << " is not open yet, the time is now " << now << endl
                  << "The " << argv[3] << " will open at " << cfgDate << endl;
               ok = false;
            }
         }
         if (ok) {
            Command cmd("mkdir ~/");
            cmd += "___";
            cmd += argv[1];
            cmd += argv[3];
            cmd += "___";
            cmd.run();
            cmd += "/";
            cmd += argv[4];
            cmd.run();
#ifndef  FS_DEBUG
            Command("clear").run();
#endif //  FS_DEBUG
            getStudnetQuestionFileName(fname, argv);
            if (fileExists(fname)) {
               cout << "The " << argv[3] << " has already been activated, continue working on q" << argv[4] << ".cpp" << endl;
            }
            else {
               getFileName(fname, argv, allFiles);
               prn(fname);
               Command cp("cp ");
               cp += fname;
               cp += " ";
               cp += cmd.substr(6);
               cp += "/.";
               if (cp.run()) {
                  cout << "Could not fetch the files for the " << argv[3] << "." << endl;
                  cout << "Fist make sure these values are not misspelled: " << endl;
                  cout << argv[1] << " " << argv[2] << " " << argv[1] << " " << argv[1] << endl;
                  cout << "If the above values are correct, reprot this to your professor." << endl;
                  ok = false;
               }
            }
         }
      }
      else {
         cout << "Could not read values from cofiguration file: " << fname << endl;
         cout << "Report this to your professor" << endl;
      }
   }
   return int(!ok);
}
bool getAssignmentValues(SubVals& SV, string fname) {
   bool ok = false;
   Vals V('|');
   ifstream file(fname.c_str());
   while (file) {
      file >> V;
      if (V.size() > 1) {
         ok = true;
         SV.add(V[0], Vals(V[1], ','));
      }
   }
   file.close();
   return ok;
}
/*
0: /home/fardad.soleimanloo/getfiles
1: 244
2: NBB
3: exam
4: 12
*/

void prn(string val) {
#ifdef FS_DEBUG
   cout << val << endl;
#endif // FS_DEBUG
}
bool getAssignmentValues(SubVals& SV, string fname);

void getFileName(char* fname, char* argv[], int fileType) {
#ifdef  FS_DEBUG
   strcpy(fname, "/home/fardad.soleimanloo/getfiles");
#else
   strcpy(fname, argv[0]);
#endif //  FS_DEBUG
   for (int i = strlen(fname); i > 0; i--) {
      if (fname[i] == '/') {
         fname[i + 1] = 0;
         i = 0;
      }
   }
   strcat(fname, "submitter_files/");
   strcat(fname, argv[1]);
   strcat(fname, "/");
   strcat(fname, argv[3]);
   strcat(fname, "/");
   strcat(fname, argv[2]);
   switch (fileType) {
   case configFile:
      strcat(fname, "/");
      strcat(fname, argv[4]);
      strcat(fname, ".cfg");
      break;
   case questionFile:
      strcat(fname, "/");
      strcat(fname, argv[4]);
      strcat(fname, "/q");
      strcat(fname, argv[4]);
      strcat(fname, ".cpp");
      break;
   case allFiles:
      strcat(fname, "/");
      strcat(fname, argv[4]);
      strcat(fname, "/q*.*");
      break;
   }
}
void getStudnetQuestionFileName(char* fname, char* argv[]) {
   strcpy(fname, theUser.homedir());
   strcat(fname, "___");
   strcat(fname, argv[1]);
   strcat(fname, argv[3]);
   strcat(fname, "___/");
   strcat(fname, argv[4]);
   strcat(fname, "/q");
   strcat(fname, argv[4]);
   strcat(fname, ".cpp");
}
bool fileExists(const char* fname) {
   return ifstream(fname).good();
}