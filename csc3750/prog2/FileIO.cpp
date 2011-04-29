#include "FileIO.h"

FileIO::FileIO(char* fle, int op)  //op = 1 for read and 2 for write
{
   operation = op;

   if (operation == 1)
   {
      infile = new ifstream(fle);
   }
   else
   {
      outfile = new ofstream(fle);
   }

   ef = false;
   MAX = 80;
}

FileIO::FileIO(char* fle, char delim)  //assumed to be a read
{
   infile = new ifstream(fle);
   delimiter = delim;
   operation = 1;
   ef = false;
   MAX = 80;
}

FileIO::~FileIO()
{
   if (operation == 1)
   {
      delete infile;
   }
   else
   {
      delete outfile;
   }
}

// read a whole line and return an iterator for its tokens

ListIterator<string>* FileIO::getTokens()
{
   if (operation != 1)  //opened for writing so read disallowed
   {
      return NULL;
   }

   List<string>* list = new List<string>();

   //Martha:
   string token = "";
   char* buffer = new char[MAX];

   if (!ef)
   {
      infile->getline(buffer, MAX);

      for (int x = 0; x < MAX; x++)
      {
         if (buffer[x] != delimiter && buffer[x] != '\0')
         {
            token.append(1, buffer[x]);
         }
         else
         {
            string* temp = new string(token);
            list->add(temp);
            token = "";

            if (buffer[x] == '\0')
            {
               break;
            }
         }
      }

      if (infile->eof())
      {
         ef = true;
      }
   }

   delete buffer;  
   ListIterator<string>* iter = list->iterator();
   delete list;
   return iter;
}

bool FileIO::eof()
{
   return ef;
}

string FileIO::readLine()
{
   string line = "";
   char* buffer = new char[MAX];

   if (operation != 1)  //opened for writing so read disallowed
   {
      return line;
   }

   if (!ef)
   {
      infile->getline(buffer, MAX);

      for (int x = 0; x < MAX; x++)
      {
         if (buffer[x] != '\0')
         {
            line.append(1, buffer[x]);
         }
         else
         {
            break;
         }
      }

      if (infile->eof())
      {
         ef = true;
      }
   }

   delete buffer;
   return line;
}

void FileIO::writeLine(string line)
{
   if (operation == 2)
   {
      *outfile << line << "\r\n";
   }
}
