/*
 * Mahyar Mike Vahabi
 * mvahabi
 * Winter 22 - CSE101 - pa8
 * Order.cpp - Client file for Dictionary
*/
#include "Dictionary.h"
#include<iostream>
#include<fstream>
#include<string>
using namespace std;

int main(int argc, char * argv[]){

   int i = 1;
   std::string line;
   Dictionary D;

   // check command line for correct number of arguments
   if( argc != 3 ){
      cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
      return(EXIT_FAILURE);
   }

   // open files for reading and writing 
   ifstream in(argv[1]);
   ofstream out(argv[2]);

   while(getline(in, line)){
       D.setValue(line, i);
       i++;
   }

   out << D.to_string() << endl;
   out << D.pre_string();

   in.close();
   out.close();
   D.clear();

   return 1;
}
