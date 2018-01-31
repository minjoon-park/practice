#include "inverted_index.h"
#include <type_traits>

WordAttributes::WordAttributes() {
}

void WordAttributes::AddEntry(const string& file_name, int position) {
  if(file_positions_map.count(file_name))
    (file_positions_map.find(file_name)->second).push_back(position);
  else {
    vector<int> position_temp {position};
    file_positions_map.insert(make_pair(file_name, position_temp));
  }
}

int WordAttributes::Get_tf(const string& file_name) {
  return (file_positions_map.find(file_name)->second).size();
}

int WordAttributes::Get_idf() {
  return file_positions_map.size();
}

map< string, vector<int> > WordAttributes::GetMemberMap() {
  return file_positions_map;
}

InvertedIndex::InvertedIndex(string root_dir_of_files) {
  dirname = root_dir_of_files;
}

void InvertedIndex::BuildFileList(const string& dir_name) {
  DIR* dir;
  struct dirent* ent;

  if((dir = opendir(dir_name.c_str())) != NULL) {
    while(((ent = readdir(dir)) != NULL))  {
      if(!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, ".."))
        continue;

      string subdir_str = dir_name + "\\" + ent->d_name;

      if((opendir(subdir_str.c_str()) != NULL))
        BuildFileList(subdir_str.c_str());
      else {
        fstream current_file(subdir_str.c_str(), ios_base::in);
        int current_file_word_count = 0;
        string word_temp;

        while(current_file >> word_temp)
          ++current_file_word_count;

        file_list.insert(make_pair(subdir_str.erase(0, dirname.size()), current_file_word_count));

        current_file.close();
      }
    }
    closedir(dir);
  }
  else
    cout << "No such directory to open." << endl;
}

void InvertedIndex::BuildInvertedIndex() {
  BuildFileList(dirname);
  int progress = 0;

  for(map<string, int>::iterator it = file_list.begin(); it != file_list.end(); ++it) {
    ++progress;

    fstream working_file((dirname + it->first).c_str(), ios_base::in);

    string word_temp;
    int loc = 0;

    while(working_file >> word_temp) {
      ++loc;

      string word_case_insensitive;
      for(char& c : word_temp) {
        if(!((c >= 48 && c <=57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122)))
          continue;
        if(c >= 65 && c <= 91)
          c += 32;
        word_case_insensitive += c;
      }

      if(word_map.count(word_case_insensitive))
        (word_map.find(word_case_insensitive)->second).AddEntry(it->first, loc);
      else {
        WordAttributes word_attributes_temp;
        word_attributes_temp.AddEntry(it->first, loc);
        word_map.insert(make_pair(word_case_insensitive, word_attributes_temp));
      }
    }
    working_file.close();

    if(progress % 1000 == 0)
      cout << progress << "-th file has been just processed." << endl;
  }
}

void InvertedIndex::SearchAndPrint() {
  string keyword;
  cout << "Enter your keyword (case-insensitive):" << endl;

  while(cin >> keyword) {
    string keyword_case_insensitive;
    for(char& c : keyword) {
      if(c >= 65 && c <= 91)
        c += 32;
      keyword_case_insensitive += c;
    }

    int word_count = 0;
    map< string, vector<int> > word_attributes_for_keyword = (word_map.find(keyword_case_insensitive)->second).GetMemberMap();

    for(map< string, vector<int> >::iterator it = word_attributes_for_keyword.begin(); it != word_attributes_for_keyword.end(); ++it) {
      word_count += (it->second).size();
      fstream file_found((dirname + it->first).c_str(), ios_base::in);
      int loc_in_file = 0;
      const int buffer = 2;
      string word_temp;

      cout << dirname + it->first << " :" << endl;
      while(file_found >> word_temp) {
        ++loc_in_file;
        for(int j = 0; j < (it->second).size(); ++j) {
          if((it->second)[j] - buffer <= loc_in_file && loc_in_file <=  (it->second)[j] + buffer)
            cout << word_temp << " ";
          if(loc_in_file == (it->second)[j] + buffer)
              cout << endl;
        }
      }
      cout << endl;
      file_found.close();
    }
    cout << "There is(are) " << word_count << " appearance(s) of \"" << keyword << "\" in all the files under inspection." << endl;
  }
}

bool IteratorCompare(map<string, double>::iterator it1, map<string, double>::iterator it2) {
  return (it1->second > it2->second);
}

void InvertedIndex::PrintSnippets(map<string, double>::iterator sorted_sim_values_it,
    const int num_of_keywords, const vector<string>& separated_keyword) {
  fstream file_found((dirname + sorted_sim_values_it->first).c_str(), ios_base::in);
  int loc_in_file = 0;
  const int buffer = 4;
  string word_temp;

  cout << dirname + sorted_sim_values_it->first << " :" << "  " << sorted_sim_values_it->second << endl;
  while(file_found >> word_temp) {
    ++loc_in_file;

    for(int j = 0; j < num_of_keywords; ++j) {
      bool already_printed = false;

      if(!(word_map.find(separated_keyword[j])->second).GetMemberMap().count(sorted_sim_values_it->first))
        continue;

      vector<int> pos = (word_map.find(separated_keyword[j])->second).GetMemberMap().find(sorted_sim_values_it->first)->second;

      for(int k = 0; k < pos.size(); ++k) {
        if(loc_in_file == pos[k] - buffer && loc_in_file != 1)
          cout << "... ";
        if(pos[k] - buffer <= loc_in_file && loc_in_file <= pos[k] + buffer) {
          cout << word_temp << " ";
          already_printed = true;
        }
        //if(loc_in_file == pos[k] + buffer && loc_in_file != file_list.find(sorted_sim_values_it->first)->second)
          //cout << " ... " << endl;
        if(already_printed == true)
          break;
      }

      if(already_printed == true)
        break;
    }
  }
  cout << endl;
  file_found.close();
}

int InvertedIndex::VectorSpaceSearch() {
  cout << "Enter your keyword(s) (case-insensitive):" << endl;

  string keywords, keyword_temp;
  getline(cin, keywords);
  stringstream keywords_stream(keywords);
  vector<string> separated_keyword;

  cout << endl;

  while(keywords_stream >> keyword_temp) {
    string keyword_case_insensitive;
      for(char& c : keyword_temp) {
        if(!((c >= 48 && c <=57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122)))
          continue;
        if(c >= 65 && c <= 91)
          c += 32;
        keyword_case_insensitive += c;
      }
    if(!word_map.count(keyword_case_insensitive))
      continue;
    separated_keyword.push_back(keyword_case_insensitive);
  }

  int num_of_keywords = separated_keyword.size();

  if(num_of_keywords == 0) {
    cout << "No document matching your keywords found." << endl;
    return -1;
  }

  map<string, double> sim_value_for_doc;

  for(int i = 0; i < num_of_keywords; ++i) {
    string key = separated_keyword[i];

    map< string, vector<int> > files_with_key = (word_map.find(key)->second).GetMemberMap();
    double global_factor = log((double)file_list.size() / (double)files_with_key.size()) / sqrt((double)num_of_keywords);

    for(map< string, vector<int> >::iterator it = files_with_key.begin(); it != files_with_key.end(); ++it) {
      if(sim_value_for_doc.count(it->first))
        sim_value_for_doc.find(it->first)->second += (double)(it->second).size() / (double)(file_list.find(it->first)->second) * global_factor;
      else
        sim_value_for_doc.insert(make_pair(it->first, (double)(it->second).size() / (double)(file_list.find(it->first)->second) * global_factor));
    }
  }

/*/ Print out file name(s) of the largest sim-value
  vector<string> best_matches;
  double max = 0.0;

  for(map<string, double>::iterator it = sim_value_for_doc.begin(); it != sim_value_for_doc.end(); ++it) {
    if(it->second >= max) {
      max = it->second;
      if(it->second > max)
        best_matches.clear();
      best_matches.push_back(it->first);
    }
  }

  cout << "The best matching file(s) is(are)" << endl;
  for(int i = 0; i < best_matches.size(); ++i)
    cout << best_matches[i] << endl;

  cout << endl << "Its(Their) sim-value is " << max << "," << endl;
  cout << "and there is(are) " << best_matches.size() << " of them." << endl; */

  vector<map<string, double>::iterator> sorted_sim_values;
  for(map<string, double>::iterator it = sim_value_for_doc.begin(); it != sim_value_for_doc.end(); ++it)
    sorted_sim_values.push_back(it);

  sort(sorted_sim_values.begin(), sorted_sim_values.end(), IteratorCompare);

  int num_of_matching_files = sorted_sim_values.size();

  for(int i = 0; i < min(5, num_of_matching_files); ++i){
    PrintSnippets(sorted_sim_values[i], num_of_keywords, separated_keyword);
    cout << endl;
  }

  return 0;
}














