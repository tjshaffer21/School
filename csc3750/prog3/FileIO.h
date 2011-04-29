#if !defined (FILEIO_H)
#define FILEIO_H

#include "ListIterator.h"
#include "List.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

class FileIO
{
   private:
      int operation;
      char delimiter;
      ifstream* infile;
      ofstream* outfile;
      bool ef;
      int MAX;

   public:
      FileIO(char* file, int op);
      FileIO(char* file, char delim);
      ~FileIO();
      ListIterator<string>* getTokens();
      bool eof();
      string readLine();
      void writeLine(string line);
};

#endif
