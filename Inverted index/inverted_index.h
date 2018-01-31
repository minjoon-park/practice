#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <map>
#include <sstream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <dirent.h>
#include <math.h>
#include <algorithm>


using namespace std;

class WordAttributes {
  public:
    WordAttributes();
    void AddEntry(const string& file_name, int position);
    int Get_tf(const string& file_name);
    int Get_idf();
    map< string, vector<int> > GetMemberMap();

  private:
    map< string, vector<int> > file_positions_map;
};

class InvertedIndex {
  public:
    InvertedIndex(string root_dir_of_files);
    void BuildInvertedIndex(); // Build the inverted-index
    void SearchAndPrint(); // Search for a keyword and print the result
    int VectorSpaceSearch();

  private:
    string dirname;
    map<string, int> file_list;
    map<string, WordAttributes> word_map;

    void BuildFileList(const string& dir_name); // Build the list of files
    //bool IteratorCompare(map<string, double>::iterator it1, map<string, double>::iterator it2);
    void PrintSnippets(map<string, double>::iterator sorted_sim_values_it,
      const int num_of_keywords, const vector<string>& separated_keyword);
};

#endif // INVERTED_INDEX_H
