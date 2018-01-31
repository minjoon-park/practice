#include "TagMining.h"
#include <math.h>
#include <algorithm>


Item::Item() {
}



TagMining::TagMining(string working_dir_name) {
  working_dir = working_dir_name;
}


void TagMining::PrepStopwords(set<string>* stopwords) {
	ifstream dictionary(working_dir + "\\dictionary.txt", ios_base::in);

	if (dictionary.is_open()) {
	  string stopword_buffer;
  	while (dictionary >> stopword_buffer)
	  	stopwords->insert(stopword_buffer);
  }
  else
    cout << "No stopwords file!\n";

  dictionary.close();
}


void TagMining::BlockRemoval(string* contents, string modifier1, string modifier2) {
	size_t start = 0, fin;
	while ((start = contents->find(modifier1, start)) != string::npos) {
		fin = contents->find(modifier2, start + modifier1.size());
		if(fin != string::npos)
  		contents->erase(start, fin - start + modifier2.size());
    else
      contents->erase(start, modifier1.size());
	}
}


void TagMining::ModifierRemoval(string* contents, string modifier) {
	size_t start = 0;
	while ((start = contents->find(modifier, start)) != string::npos)
		contents->erase(start, modifier.size());
}


string TagMining::SiftOff(string* contents, set<string>* stopwords) {
  BlockRemoval(contents, "<pre", ">");
	BlockRemoval(contents, "<a ", ">");
	BlockRemoval(contents, "<img", ">");

  string modifiers[46] =
    {"</a>", "<b>", "</b>", "<blockquote>", "</blockquote>", "<br>", "</br>", "<br/>", "<code>", "</code>",
     "<cr>", "<del>", "</del>", "<em>", "</em>", "<esc>", "<h1>", "</h1>", "<h2>", "</h2>",
     "<h3>", "</h3>", "<hr>", "<i>", "</i>", "<kbd>", "</kbd>", "<li>", "</li>", "<ol>",
     "</ol>", "<p>", "</p>", "</pre>", "<span>", "</span>", "<strike>", "</strike>", "<strong>", "</strong>",
     "<sub>", "</sub>", "<sup>", "</sup>", "<ul>", "</ul>"};
  for (int i = 0; i < 46; ++i)
    ModifierRemoval(contents, modifiers[i]);

	for (unsigned i = 0; i < contents->size(); ++i) {
  	if ((*contents)[i] >= 'A' && (*contents)[i] <= 'Z')
		  (*contents)[i] += 32;
		if ((*contents)[i] == '\"' || (*contents)[i] == '\'' || (*contents)[i] == '\?' || (*contents)[i] == '!'
		    || (*contents)[i] == '(' || (*contents)[i] == ')' || (*contents)[i] == ',' || (*contents)[i] == ';'
		    || (*contents)[i] == '=')
      (*contents)[i] = 32;
  }

	stringstream contents_stream(*contents);
	string raw, output;
	bool meaningless;
	while (contents_stream >> raw) {
    if (raw.find("http://") != string::npos || raw.find("https://") != string::npos
        || raw.find("0x") != string::npos || raw.find("..") != string::npos
        || raw.find("fff") != string::npos || raw.find("ggg") != string::npos
        || raw.find("hhh") != string::npos || raw.find("jjj") != string::npos
        || raw.find("kkk") != string::npos || raw.find("mmm") != string::npos
        || raw.find("nnn") != string::npos || raw.find("qqq") != string::npos
        || raw.find("rrr") != string::npos || raw.find("uuu") != string::npos
        || raw.find("vvv") != string::npos || raw.find("xxx") != string::npos
        || raw.find("yyy") != string::npos || raw.find("zzz") != string::npos)
      continue;

    unsigned numeric_count = 0, len = raw.size();
    if (len >= 5)
      for (unsigned i = 0; i < len; ++i)
        if (raw[i] >= '0' && raw[i] <= '9')
          ++numeric_count;
    if (numeric_count == len)
      continue;

    if (!(raw[0] == '.' || (raw[0] >= '0' && raw[0] <= '9') || (raw[0] >= 'a' && raw[0] <= 'z')))
      continue;

		if (raw.back() == '.') {
			raw.erase(len - 1, 1);
			--len;
    }

    meaningless = false;

    for (unsigned i = 1; i < len; ++i)
      if (!(raw[i] == '.' || raw[i] == '#' || raw[i] == '+' || raw[i] == '-'
          || (raw[i] >= '0' && raw[i] <= '9') || (raw[i] >='a' && raw[i] <= 'z'))) {
        meaningless = true;
        break;
      }

		if (stopwords->find(raw) != stopwords->end())
			meaningless = true;

		if (meaningless == false)
			output += raw + " ";
	}

	return output;
}


void TagMining::InsertToDfMap(Item* item) {
	stringstream temp_sstream;
	set<string> words_wo_duplicity;
	string word;

  temp_sstream << item->title << " " << item->body;
  while (temp_sstream >> word)
    words_wo_duplicity.insert(word);
  for (set<string>::iterator it = words_wo_duplicity.begin(); it != words_wo_duplicity.end(); ++it) {
    if (df_map.count(*it))
      ++(df_map[*it]);
    else
      df_map.insert(make_pair(*it, 1));
  }
}


void TagMining::BuildDfMapAndTagMaps() {       // without inverted_tag_map
	ifstream raw_data(working_dir + "\\train.csv", ios_base::in);
	string ln_buffer, item_container;
	size_t marker_pos[4];

	getline(raw_data, ln_buffer);

  set<string> stopwords;
  PrepStopwords(&stopwords);

  ofstream train_parsed_file(working_dir + "\\train_parsed.txt", ios_base::out);

	int prog = 0;
	while (getline(raw_data, ln_buffer)) {
		++prog;
		if (prog % 1000000 == 0)
			cout << prog << "-th item processed.\n";

		item_container.clear();
		item_container += ln_buffer;

		marker_pos[0] = item_container.find("\",\"") + 3;
		marker_pos[1] = item_container.find("\",\"", marker_pos[0]) + 3;

		while (1) {
			size_t x = item_container.rfind('\"');
			if (x == item_container.size() - 1) {
				char c1 = raw_data.get();
				char c2 = raw_data.peek();
				if (c1 == EOF || (c1 == '\"' && c2 >= '1' && c2 <= '9')) {
					raw_data.unget();
					marker_pos[3] = x - 1;
					break;
				}
			}
			getline(raw_data, ln_buffer);
			item_container += ln_buffer;
		}

		marker_pos[2] = item_container.rfind("\",\"") + 3;

		Item temp_item;

		temp_item.item_no = prog;

		temp_item.title = item_container.substr(marker_pos[0], marker_pos[1] - marker_pos[0] - 3);
		temp_item.title = SiftOff(&temp_item.title, &stopwords);

		temp_item.body = item_container.substr(marker_pos[1], marker_pos[2] - marker_pos[1] - 3);
		temp_item.body = SiftOff(&temp_item.body, &stopwords);

		stringstream tag_sstream(item_container.substr(marker_pos[2], marker_pos[3] - marker_pos[2] + 1));
		string indv_tags;
		while (tag_sstream >> indv_tags)
			temp_item.tags.insert(indv_tags);
    tag_map.insert(make_pair(temp_item.item_no, temp_item.tags));

		train_parsed_file << temp_item.item_no << endl;
		for (set<string>::iterator it = temp_item.tags.begin(); it != temp_item.tags.end(); ++it)
			train_parsed_file << *it << " ";
		train_parsed_file << endl;
		train_parsed_file << temp_item.title << endl;
		train_parsed_file << temp_item.body << endl;

		InsertToDfMap(&temp_item);
	}

  cout << "df_map size: " << df_map.size() << endl;

	raw_data.close();
	train_parsed_file.close();

	ofstream tag_map_file(working_dir + "\\item_to_tags.txt", ios_base::out);
	for (map< int, set<string> >::iterator map_it = tag_map.begin(); map_it != tag_map.end(); ++map_it) {
		tag_map_file << map_it->first << " ";
		for (set<string>::iterator set_it = (map_it->second).begin(); set_it != (map_it->second).end(); ++set_it)
		  tag_map_file << " " << *set_it;
    tag_map_file << endl;
  }
	tag_map_file.close();
	tag_map.clear();
}


/* void TagMining::BuildDfMapAndTagMaps() {     // with inverted_tag_map
	ifstream raw_data(working_dir + "\\train.csv", ios_base::in);
	string ln_buffer, item_container;
	size_t marker_pos[4];

	getline(raw_data, ln_buffer);

  set<string> stopwords;
  PrepStopwords(&stopwords);

  ofstream train_parsed_file(working_dir + "\\train_parsed.txt", ios_base::out);

	int prog = 0;
	while (getline(raw_data, ln_buffer)) {
		++prog;
		if (prog % 1000000 == 0)
			cout << prog << "-th item processed.\n";

		item_container.clear();
		item_container += ln_buffer;

		marker_pos[0] = item_container.find("\",\"") + 3;
		marker_pos[1] = item_container.find("\",\"", marker_pos[0]) + 3;

		while (1) {
			size_t x = item_container.rfind('\"');
			if (x == item_container.size() - 1) {
				char c1 = raw_data.get();
				char c2 = raw_data.peek();
				if (c1 == EOF || (c1 == '\"' && c2 >= '1' && c2 <= '9')) {
					raw_data.unget();
					marker_pos[3] = x - 1;
					break;
				}
			}
			getline(raw_data, ln_buffer);
			item_container += ln_buffer;
		}

		marker_pos[2] = item_container.rfind("\",\"") + 3;

		Item temp_item;

		temp_item.item_no = prog;

		temp_item.title = item_container.substr(marker_pos[0], marker_pos[1] - marker_pos[0] - 3);
		temp_item.title = SiftOff(&temp_item.title, &stopwords);

		temp_item.body = item_container.substr(marker_pos[1], marker_pos[2] - marker_pos[1] - 3);
		temp_item.body = SiftOff(&temp_item.body, &stopwords);

		stringstream tag_sstream(item_container.substr(marker_pos[2], marker_pos[3] - marker_pos[2] + 1));
		string indv_tags;
		while (tag_sstream >> indv_tags) {
			temp_item.tags.insert(indv_tags);
			if (inverted_tag_map.count(indv_tags))
			  inverted_tag_map[indv_tags].insert(temp_item.item_no);
      else {
        set<int> temp_item_no_set {temp_item.item_no};
        inverted_tag_map.insert(make_pair(indv_tags, temp_item_no_set));
      }
    }
    tag_map.insert(make_pair(temp_item.item_no, temp_item.tags));

		train_parsed_file << temp_item.item_no << endl;
		for (set<string>::iterator it = temp_item.tags.begin(); it != temp_item.tags.end(); ++it)
			train_parsed_file << *it << " ";
		train_parsed_file << endl;
		train_parsed_file << temp_item.title << endl;
		train_parsed_file << temp_item.body << endl;

		InsertToDfMap(&temp_item);
	}

  cout << "no. of tags: " << inverted_tag_map.size() << ",  df_map size: " << df_map.size() << endl;

	raw_data.close();
	train_parsed_file.close();

	ofstream tag_map_file(working_dir + "\\item_to_tags.txt", ios_base::out);
	for (map< int, set<string> >::iterator map_it = tag_map.begin(); map_it != tag_map.end(); ++map_it) {
		tag_map_file << map_it->first << " ";
		for (set<string>::iterator set_it = (map_it->second).begin(); set_it != (map_it->second).end(); ++set_it)
		  tag_map_file << " " << *set_it;
    tag_map_file << endl;
  }
	tag_map_file.close();
	tag_map.clear();

  ofstream inverted_tag_map_file(working_dir + "\\inverted_tag_map.txt", ios_base::out);
  for (map< string, set<int> >::iterator map_it = inverted_tag_map.begin();
       map_it != inverted_tag_map.end(); ++map_it) {
    inverted_tag_map_file << map_it->first << " ";
    for (set<int>::iterator set_it = (map_it->second).begin(); set_it != (map_it->second).end(); ++set_it)
      inverted_tag_map_file << " " << *set_it;
    inverted_tag_map_file << endl;
  }
  inverted_tag_map_file.close();
	inverted_tag_map.clear();
}
*/


double TagMining::PosWeightFtn(int pos, int length) {
  return (1 - .1) * pow((pos - 3 * length / 4.) / (3 * length / 4.), 2) + .1;
}


void TagMining::AddEntry(map<int, double>* loc_modified_tf, const int& item_no, double val) {
  if(loc_modified_tf->count(item_no))
    loc_modified_tf->find(item_no)->second += val;
  else
    loc_modified_tf->insert(make_pair(item_no, val));
}


void TagMining::AddTitleWordsToInvertedIndex(int item_no, string* title) {
	stringstream temp_sstream;
	string word;

  temp_sstream << *title;
  while (temp_sstream >> word)
    if (df_map[word] > DFCUT) {
      if (word_map.count(word))
        AddEntry(&(word_map[word]), item_no, 1);
      else {
        map<int, double> temp_mod_tf_map;
        temp_mod_tf_map.insert(make_pair(item_no, 1));
        word_map.insert(make_pair(word, temp_mod_tf_map));
      }
    }
}


void TagMining::AddBodyWordsToInvertedIndex(int item_no, string* body) {
	stringstream temp_sstream;
	string word;

	int no_of_words = 0;
	for (unsigned i = 0; i < (*body).size(); ++i)
	  if ((*body)[i] == 32)
	    ++no_of_words;
  ++no_of_words;

  temp_sstream << *body;
  int pos = 0;
  while (temp_sstream >> word) {
    if (df_map[word] > DFCUT) {
      if (word_map.count(word))
        AddEntry(&(word_map[word]), item_no, PosWeightFtn(pos, no_of_words));
      else {
        map<int, double> temp_mod_tf_map;
        temp_mod_tf_map.insert(make_pair(item_no, PosWeightFtn(pos, no_of_words)));
        word_map.insert(make_pair(word, temp_mod_tf_map));
      }
    }
    ++pos;
	}
}


void TagMining::BuildInvertedIndex() {
	ifstream parsed_train_data(working_dir + "\\train_parsed.txt", ios_base::in);
	string ln_buffer;

	int item_no;
	while (getline(parsed_train_data, ln_buffer)) {
		item_no = stoi(ln_buffer);
		if (item_no % 1000000 == 0)
			cout << item_no << "-th item processed.\n";

		getline(parsed_train_data, ln_buffer);

		getline(parsed_train_data, ln_buffer);
		AddTitleWordsToInvertedIndex(item_no, &ln_buffer);

		getline(parsed_train_data, ln_buffer);
		AddBodyWordsToInvertedIndex(item_no, &ln_buffer);
	}

  cout << "word_map size: " << word_map.size() << endl;

	parsed_train_data.close();
	df_map.clear();

  ofstream word_map_file(working_dir + "\\word_map.txt", ios_base::out);
  for (map< string, map<int, double> >::iterator map_it = word_map.begin();
       map_it != word_map.end(); ++map_it) {
    word_map_file << map_it->first << " ";
    for (map<int, double>::iterator tf_map_it = (map_it->second).begin();
         tf_map_it != (map_it->second).end(); ++tf_map_it)
      word_map_file << " " << tf_map_it->first << " " << tf_map_it->second;
    word_map_file << endl;
  }
  word_map_file.close();
  word_map.clear();
}


void TagMining::LoadTagMaps() {     // without inverted_tag_map
  string ln_buffer, word_buffer;

  ifstream tag_map_file(working_dir + "\\item_to_tags.txt", ios_base::in);
  int item_no;
  while (getline(tag_map_file, ln_buffer)) {
    stringstream ln_sstream(ln_buffer);
    set<string> local_tags;

    ln_sstream >> word_buffer;
    item_no = stoi(word_buffer);

    while (ln_sstream >> word_buffer)
      local_tags.insert(word_buffer);

    tag_map.insert(make_pair(item_no, local_tags));
  }
  tag_map_file.close();
}


/* void TagMining::LoadTagMaps() {    // with inverted_tag_map
  string ln_buffer, word_buffer;

  ifstream tag_map_file(working_dir + "\\item_to_tags.txt", ios_base::in);
  int item_no;
  while (getline(tag_map_file, ln_buffer)) {
    stringstream ln_sstream(ln_buffer);
    set<string> local_tags;

    ln_sstream >> word_buffer;
    item_no = stoi(word_buffer);

    while (ln_sstream >> word_buffer)
      local_tags.insert(word_buffer);

    tag_map.insert(make_pair(item_no, local_tags));
  }
  tag_map_file.close();

  ifstream inverted_tag_map_file(working_dir + "\\inverted_tag_map.txt", ios_base::in);
  string tag;
  while (getline(inverted_tag_map_file, ln_buffer)) {
    stringstream ln_sstream(ln_buffer);
    set<int> item_list;

    ln_sstream >> word_buffer;
    tag = word_buffer;

    while (ln_sstream >> word_buffer)
      item_list.insert(stoi(word_buffer));

    inverted_tag_map.insert(make_pair(tag, item_list));
  }
  inverted_tag_map_file.close();
}


*/


void TagMining::BuildCorrelationMap() {     // df is ignored.
  ifstream word_map_file(working_dir + "\\word_map.txt", ios_base::in);
  string ln_buffer, word_buffer;

  string word;
  int prog = 0;
  while (getline(word_map_file, ln_buffer)) {
    ++prog;
    if (prog % 100000 == 0)
      cout << prog << "-th word is processed.\n";

    stringstream ln_sstream(ln_buffer);

    ln_sstream >> word_buffer;
    word = word_buffer;

    map<string, double> tag_cor_map;

    int item_no;
    double tf;
    while (ln_sstream >> word_buffer) {
      item_no = stoi(word_buffer);
      ln_sstream >> word_buffer;
      tf = stod(word_buffer);
      for (set<string>::iterator set_it = tag_map[item_no].begin();
           set_it != tag_map[item_no].end(); ++set_it) {
        if (tag_cor_map.count(*set_it))
          tag_cor_map[*set_it] += tf;
        else
          tag_cor_map.insert(make_pair(*set_it, tf));
      }
    }

    correlation_map.insert(make_pair(word, tag_cor_map));
  }

  word_map_file.close();
  tag_map.clear();

  ofstream cor_map_file(working_dir + "\\correlation_map.txt", ios_base::out);
  for (map< string, map<string, double> >::iterator cor_map_it = correlation_map.begin();
       cor_map_it != correlation_map.end(); ++cor_map_it) {
    cor_map_file << cor_map_it->first << " ";
    for (map<string, double>::iterator tag_cor_map_it = (cor_map_it->second).begin();
         tag_cor_map_it != (cor_map_it->second).end(); ++tag_cor_map_it)
      cor_map_file << " " << tag_cor_map_it->first << " " << tag_cor_map_it->second;
    cor_map_file << endl;
  }
  cor_map_file.close();
}


/* void TagMining::BuildCorrelationMap() {    // df is considered.
  ifstream word_map_file(working_dir + "\\word_map.txt", ios_base::in);
  string ln_buffer, word_buffer;

  string word;
  while (getline(word_map_file, ln_buffer)) {
    stringstream ln_sstream(ln_buffer);

    ln_sstream >> word_buffer;
    word = word_buffer;

    map<string, double> tag_cor_map;

    int item_no, df = 0;
    double tf;
    while (ln_sstream >> word_buffer) {
      item_no = stoi(word_buffer);
      ln_sstream >> word_buffer;
      tf = stod(word_buffer);
      ++df;
      for (set<string>::iterator set_it = tag_map[item_no].begin();
           set_it != tag_map[item_no].end(); ++set_it) {
        if (tag_cor_map.count(*set_it))
          tag_cor_map[*set_it] += tf;
        else
          tag_cor_map.insert(make_pair(*set_it, tf));
      }
    }

    double idf = log((double)SizeOfTrainingSet / df);
    for (map<string, double>::iterator map_it = tag_cor_map.begin(); map_it != tag_cor_map.end(); ++map_it)
      map_it->second *= idf;

    correlation_map.insert(make_pair(word, tag_cor_map));
  }

  word_map_file.close();
  tag_map.clear();

  for (map< string, map<string, double> >::iterator cor_map_it = correlation_map.begin();
       cor_map_it != correlation_map.end(); ++cor_map_it)
    for (map<string, double>::iterator tag_cor_map_it = (cor_map_it->second).begin();
         tag_cor_map_it != (cor_map_it->second).end(); ++ tag_cor_map_it)
      tag_cor_map_it->second /= inverted_tag_map[tag_cor_map_it->first].size();
  inverted_tag_map.clear();

  ofstream cor_map_file(working_dir + "\\correlation_map.txt", ios_base::out);
  for (map< string, map<string, double> >::iterator cor_map_it = correlation_map.begin();
       cor_map_it != correlation_map.end(); ++cor_map_it) {
    cor_map_file << cor_map_it->first << " ";
    for (map<string, double>::iterator tag_cor_map_it = (cor_map_it->second).begin();
         tag_cor_map_it != (cor_map_it->second).end(); ++tag_cor_map_it)
      cor_map_file << " " << tag_cor_map_it->first << " " << tag_cor_map_it->second;
    cor_map_file << endl;
  }
  cor_map_file.close();
}
*/


void TagMining::LoadCorMap() {
  string ln_buffer, word_buffer;

  ifstream cor_map_file(working_dir + "\\correlation_map.txt", ios_base::in);
  string word, tag;
  double cor_val;
  while (getline(cor_map_file, ln_buffer)) {
    stringstream ln_sstream(ln_buffer);

    ln_sstream >> word_buffer;
    word = word_buffer;

    map<string, double> tag_cor_map;
    while (ln_sstream >> word_buffer) {
      tag = word_buffer;
      ln_sstream >> word_buffer;
      cor_val = stod(word_buffer);
      tag_cor_map.insert(make_pair(tag, cor_val));
    }

    correlation_map.insert(make_pair(word, tag_cor_map));
  }
  cor_map_file.close();
}


void TagMining::DetermineThreshold() {
  ifstream train_parsed_file(working_dir + "\\train_parsed.txt", ios_base::in);
  string ln_buffer, word_buffer;
  set< pair<double, double> > range;

  ofstream val_file(working_dir + "\\validation.txt", ios_base::out);

  int item_no;

  while (getline(train_parsed_file, ln_buffer)) {
    item_no = stoi(ln_buffer);

    if (item_no <= SizeOfTrainingSet - SizeOfValidationSet) {
      getline(train_parsed_file, ln_buffer);
      getline(train_parsed_file, ln_buffer);
      getline(train_parsed_file, ln_buffer);
      continue;
    }

    getline(train_parsed_file, ln_buffer);
    stringstream ln_sstream(ln_buffer);
    set<string> tag_set;
    while (ln_sstream >> word_buffer)
      tag_set.insert(word_buffer);

    map<string, double> word_list;

    getline(train_parsed_file, ln_buffer);
    ln_sstream.str("");
    ln_sstream.clear();
    ln_sstream << ln_buffer;
    while (ln_sstream >> word_buffer) {
      if (word_list.count(word_buffer))
        word_list[word_buffer] += 1;
      else
        word_list.insert(make_pair(word_buffer, 1));
	  }

    getline(train_parsed_file, ln_buffer);
  	int no_of_words = 0;
	  for (unsigned i = 0; i < ln_buffer.size(); ++i)
	    if (ln_buffer[i] == 32)
	      ++no_of_words;
    ++no_of_words;

    ln_sstream.str("");
    ln_sstream.clear();
    ln_sstream << ln_buffer;
    int pos = 0;
    while (ln_sstream >> word_buffer) {
      if (word_list.count(word_buffer))
        word_list[word_buffer] += PosWeightFtn(pos, no_of_words);
      else
        word_list.insert(make_pair(word_buffer, PosWeightFtn(pos, no_of_words)));
      ++pos;
	  }

    map<string, double> tag_list;

    for (map<string, double>::iterator word_list_it = word_list.begin();
         word_list_it != word_list.end(); ++word_list_it)
      if (correlation_map.count(word_list_it->first))
        for (map<string, double>::iterator tag_cor_map_it = correlation_map[word_list_it->first].begin();
             tag_cor_map_it != correlation_map[word_list_it->first].end(); ++tag_cor_map_it) {
          if (tag_list.count(tag_cor_map_it->first))
            tag_list[tag_cor_map_it->first] += tag_cor_map_it->second * word_list_it->second;
          else
            tag_list.insert(make_pair(tag_cor_map_it->first, tag_cor_map_it->second * word_list_it->second));
        }

    int no_of_local_tags = tag_set.size() + 1;

    pair<string, double> tag_ranking[no_of_local_tags];
    map<string, double>::iterator tag_list_it = tag_list.begin();

    for (int i = 0; i < no_of_local_tags; ++i) {
      tag_ranking[i] = make_pair(tag_list_it->first, tag_list_it->second);
      ++tag_list_it;
    }
    while (tag_list_it != tag_list.end()) {
      for (int i = 0; i < no_of_local_tags; ++i)
        if (tag_list_it->second > tag_ranking[i].second) {
          tag_ranking[i] = make_pair(tag_list_it->first, tag_list_it->second);
          break;
        }
      ++tag_list_it;
    }

    val_file << item_no << " ";
    for (set<string>::iterator it = tag_set.begin(); it != tag_set.end(); ++it)
      val_file << " " << *it;
    val_file << endl;
    for (int i = 0; i < no_of_local_tags; ++i)
      val_file << " " << tag_ranking[i].first << " " << tag_ranking[i].second << " ";
    val_file << endl;

    vector<double> cor_val_ranking;
    for (int i = 0; i < no_of_local_tags; ++i)
      cor_val_ranking.push_back(tag_ranking[i].second);
    sort(cor_val_ranking.begin(), cor_val_ranking.end());

    range.insert(make_pair(cor_val_ranking[0], cor_val_ranking[1]));
  }

  train_parsed_file.close();
  val_file.close();

  double min = 10000000, max = 0;
  for (set < pair<double, double> >::iterator it = range.begin(); it != range.end(); ++it) {
    if (it->first > max)
      max = it->first;
    if (it->second < min)
      min = it->second;
  }

  cout << "lower bound = " << max << ",  upper bound = " << min << endl;
  threshold = (min - max) / 2;
  cout << "threshold = " << threshold << endl;
}






















/*

void TagMining::LoadTagSet() {
  string word, id;

  ifstream tag_set_file(working_dir + "\\tags.txt", ios_base::in);
  while (tag_set_file >> word) {
    tag_set_file >> id;
    tag_set.insert(make_pair(word, stoi(id)));
  }
  tag_set_file.close();
}

void TagMining::LoadDfMaps() {
  string word, id, df;

  ifstream title_df_map_file(working_dir + "\\title_df_map.txt", ios_base::in);
  while (title_df_map_file >> word) {
    title_df_map_file >> id;
    title_df_map_file >> df;
    title_df_map.insert(make_pair(word, make_pair(stoi(id), stoi(df))));
  }
  title_df_map_file.close();

  ifstream body_df_map_file(working_dir + "\\body_df_map.txt", ios_base::in);
  while (body_df_map_file >> word) {
    body_df_map_file >> id;
    body_df_map_file >> df;
    body_df_map.insert(make_pair(word, make_pair(stoi(id), stoi(df))));
  }
  body_df_map_file.close();
}

void TagMining::ProcessTags(int item_no, string* data_ln_buffer) {
  stringstream local_tags(*data_ln_buffer);
  string tag_buffer;
  int tag_id;

  while (local_tags >> tag_buffer) {
    tag_id = tag_set[tag_buffer];
    if(inverted_tag_map.count(tag_id))
      inverted_tag_map[tag_id].insert(item_no);
    else {
      set<int> temp;
      temp.insert(item_no);
      inverted_tag_map.insert(make_pair(tag_id, temp));
    }
  }
}

void TagMining::ProcessText(string* data_ln_buffer, char where, set<int>* p_doc_vector) {
  stringstream contents(*data_ln_buffer);
  string word;

  map< string, pair<int, int> >::iterator df_map_it;

  if (where == 't')
    while (contents >> word) {
      df_map_it = title_df_map.find(word);
      if (df_map_it != title_df_map.end())
        p_doc_vector->insert(title_df_map.find(word).first);
    }
  else
    for (map<string, int>::iterator it = tf_map.begin(); it != tf_map.end(); ++it) {
      df_map_it = body_df_map.find(it->first);
      if (df_map_it != title_df_map.end())
        p_doc_vector->insert(make_pair((df_map_it->second).first,
                                       it->second * log((double)SizeOfTotalSet / (df_map_it->second).second)));
    }
}

void TagMining::BuildInvertedTagMapAndFullTrainItemDocVectors() {
  ifstream parsed_data(working_dir + "\\train_parsed.txt", ios_base::in);
  string data_ln_buffer;

  int item_no = 0;
  while (getline(parsed_data, data_ln_buffer)) {
    ++item_no;
    if (item_no % 1000000 == 0)
      cout << item_no << "-th item processed.\n";

    getline(parsed_data, data_ln_buffer);
    ProcessTags(item_no, &data_ln_buffer);

    getline(parsed_data, data_ln_buffer);
    set< pair<int, double> > doc_vector;
    ProcessText(&data_ln_buffer, 't', &doc_vector);
    getline(parsed_data, data_ln_buffer);
    ProcessText(&data_ln_buffer, 'b', &doc_vector);

    train_item_doc_vector_map.insert(make_pair(item_no, doc_vector));
  }

  parsed_data.close();

  ofstream inverted_tag_map_file(working_dir + "\\inverted_tag_map.txt", ios_base::out);
  for (map< int, set<int> >::iterator it = inverted_tag_map.begin(); it != inverted_tag_map.end(); ++it) {
    inverted_tag_map_file << it->first << "    ";
    for (set<int>::iterator set_it = (it->second).begin(); set_it != (it->second).end(); ++set_it)
      inverted_tag_map_file << " " << *set_it;
    inverted_tag_map_file << endl;
  }
  inverted_tag_map_file.close();

  ofstream train_item_doc_vector_file(working_dir + "\\train_item_doc_vectors.txt", ios_base::out);
  for (map< int, set< pair<int, double> > >::iterator map_it = train_item_doc_vector_map.begin();
       map_it != train_item_doc_vector_map.end(); ++map_it) {
    train_item_doc_vector_file << map_it->first << " ";

    for (set< pair<int, double> >::iterator set_it = (map_it->second).begin();
         set_it != (map_it->second).end(); ++set_it)
      train_item_doc_vector_file << " " << set_it->first << " " << set_it->second;

    train_item_doc_vector_file << endl;
  }
  train_item_doc_vector_file.close();
}


void TagMining::LoadInvertedTagMap() {
  string ln_buffer, buffer;
  int id;

  ifstream inverted_tag_map_file(working_dir + "\\inverted_tag_map.txt", ios_base::in);
  while (getline(inverted_tag_map_file, ln_buffer)) {
    stringstream ln_sstream(ln_buffer);
    ln_sstream >> buffer;
    id = stoi(buffer);

    set<int> item_no_set;
    while (ln_sstream >> buffer)
      item_no_set.insert(stoi(buffer));

    inverted_tag_map.insert(make_pair(id, item_no_set));
  }
  inverted_tag_map_file.close();
}

void TagMining::LoadTrainItemDocVectors(char l_or_v) {
  string ln_buffer, buffer;
  int item_id, vec_comp_id;
  double tf_idf;

  ifstream train_item_doc_vector_file(working_dir + "\\train_item_doc_vectors.txt", ios_base::in);

  int item_count = 0;
  while (getline(train_item_doc_vector_file, ln_buffer)) {
    ++item_count;
    if (l_or_v == 'l') {
      if (item_count > size_of_learning_set)
        break;
    }
    else {
      if (item_count <= size_of_learning_set) {
        getline(train_item_doc_vector_file, ln_buffer);
        continue;
      }
    }

    stringstream ln_sstream(ln_buffer);

    ln_sstream >> buffer;
    item_id = stoi(buffer);

    set< pair<int, double> > doc_vector;
    while (ln_sstream >> buffer) {
      vec_comp_id = stoi(buffer);
      ln_sstream >> buffer;
      tf_idf = stod(buffer);
      doc_vector.insert(make_pair(vec_comp_id, tf_idf));
    }

    train_item_doc_vector_map.insert(make_pair(item_id, doc_vector));
  }

  train_item_doc_vector_file.close();
}

void TagMining::TrainForEachTag(int tag_id, double lambda, double learn_rate) {
  map<int, double> beta_cur;
  for (int i = 1; i <= no_of_words; ++i)
    beta_cur.insert(make_pair(i, 0));

  for (int iteration_count = 1; iteration_count <= 10000; ++iteration_count) {
    cout << iteration_count << '\t';

    double beta_L2_norm = 0;
    for (int i = 1; i <= no_of_words; ++i)
      beta_L2_norm += beta_cur[i] * beta_cur[i];

    map<int, double> grad_vector;
    for (int i = 1; i <= no_of_words; ++i)
      grad_vector.insert(make_pair(i, 0));

    double m_log_likelihood_cur = 0;

    for (int item_no = 1; item_no <= size_of_learning_set; ++item_no) {
      double probability = 0;

      for (set< pair<int, double> >::iterator it = train_item_doc_vector_map[item_no].begin();
           it != train_item_doc_vector_map[item_no].end(); ++it)
        probability -= beta_cur[it->first] * it->second;

      if (inverted_tag_map[tag_id].count(item_no)) {
        probability = 1 / (1 + exp(probability));
        for (set< pair<int, double> >::iterator it = train_item_doc_vector_map[item_no].begin();
             it != train_item_doc_vector_map[item_no].end(); ++it)
          grad_vector[it->first]
            += it->second / probability * (1 - probability) * probability;
      }
      else {
        probability = 1 - 1 / (1 + exp(probability));
        for (set< pair<int, double> >::iterator it = train_item_doc_vector_map[item_no].begin();
             it != train_item_doc_vector_map[item_no].end(); ++it)
          grad_vector[it->first]
            -= it->second / probability * (1 - probability) * probability;
      }

      m_log_likelihood_cur -= log(probability);
    }

    m_log_likelihood_cur += lambda / 2 * beta_L2_norm;
    cout << "-log(likelihood) = " << m_log_likelihood_cur << endl;
    if (m_log_likelihood - m_log_likelihood_cur < .0001 && m_log_likelihood - m_log_likelihood_cur > 0)
      break;
    if ((m_log_likelihood - m_log_likelihood_cur < .01 && m_log_likelihood - m_log_likelihood_cur > 0)
        || m_log_likelihood - m_log_likelihood_cur < 0)
      learn_rate /= 2;
    m_log_likelihood = m_log_likelihood_cur;

    for (int i = 1; i <= no_of_words; ++i)
      grad_vector[i] = grad_vector[i] - lambda * beta_cur[i];

    double grad_vector_L2_norm = 0;
    for (int i = 1; i <= no_of_words; ++i)
      grad_vector_L2_norm += grad_vector[i] * grad_vector[i];

    for (int i = 1; i <= no_of_words; ++i)
      beta_cur[i] += learn_rate * grad_vector[i] / sqrt(grad_vector_L2_norm);
  }

  beta.insert(make_pair(tag_id, beta_cur));
}

void TagMining::WriteModelFile() {
  ofstream model_file(working_dir + "\\model.txt", ios_base::out);

  for (int i = 1; i <= no_of_tags; ++i) {
    model_file << i << " ";
    for (map<int, double>::iterator it = beta[i].begin(); it != beta[i].end(); ++it)
      model_file << " " << it->first << " " << it->second;
    model_file << endl;
  }

  model_file.close();
}




void TagMining::ParseTestFile() {
	fstream raw_data(working_dir + "\\test.csv", ios_base::in);
	string ln_buffer, item_container;
	size_t marker_pos[3];

	ofstream out(working_dir + "\\test_parsed.txt", ios_base::out);

	getline(raw_data, ln_buffer);

	int prog = SizeOfTrainingSet;
	while (getline(raw_data, ln_buffer)) {
		++prog;
		if (prog % 1000000 == 0)
			cout << prog << "-th item processed.\n";

		item_container.clear();
		item_container += ln_buffer;

		marker_pos[0] = item_container.find("\",\"") + 3;
		marker_pos[1] = item_container.find("\",\"", marker_pos[0]) + 3;

		while (1) {
			size_t x = item_container.rfind('\"');
			if (x == item_container.size() - 1) {
				char c1 = raw_data.get();
				char c2 = raw_data.peek();
				if (c1 == EOF || (c1 == '\"' && c2 >= '1' && c2 <= '9')) {
					raw_data.unget();
					marker_pos[2] = x - 1;
					break;
				}
			}
			getline(raw_data, ln_buffer);
			item_container += ln_buffer;
		}

		Item temp_item;

		temp_item.item_no = prog;

		temp_item.title = item_container.substr(marker_pos[0], marker_pos[1] - marker_pos[0] - 3);
		CodeRemoval(&temp_item.title);
		temp_item.title = SiftOff(&temp_item.title);

		temp_item.body = item_container.substr(marker_pos[1], marker_pos[2] - marker_pos[1] + 1);
		CodeRemoval(&temp_item.body);
		temp_item.body = SiftOff(&temp_item.body);

		out << temp_item.item_no << endl;
		out << temp_item.title << endl;
		out << temp_item.body << endl;
	}

	raw_data.close();
	out.close();
}


void TagMining::BuildTestItemDocVectors() {
  ifstream parsed_data(working_dir + "\\test_parsed.txt", ios_base::in);
  string data_ln_buffer;

  int item_no = SizeOfTrainingSet;
  while (getline(parsed_data, data_ln_buffer)) {
    ++item_no;
    if (item_no % 500000 == 0)
      cout << item_no << "-th item processed.\n";

    getline(parsed_data, data_ln_buffer);
    set< pair<int, double> > doc_vector;
    ProcessText(&data_ln_buffer, 't', &doc_vector);
    getline(parsed_data, data_ln_buffer);
    ProcessText(&data_ln_buffer, 'b', &doc_vector);

    test_item_doc_vector_map.insert(make_pair(item_no, doc_vector));
  }

  parsed_data.close();

  ofstream test_item_doc_vector_file(working_dir + "\\test_item_doc_vectors.txt", ios_base::out);
  for (map< int, set< pair<int, double> > >::iterator map_it = test_item_doc_vector_map.begin();
       map_it != test_item_doc_vector_map.end(); ++map_it) {
    test_item_doc_vector_file << map_it->first << " ";

    for (set< pair<int, double> >::iterator set_it = (map_it->second).begin();
         set_it != (map_it->second).end(); ++set_it)
      test_item_doc_vector_file << " " << set_it->first << " " << set_it->second;

    test_item_doc_vector_file << endl;
  }
  test_item_doc_vector_file.close();
}




void TagMining::Comp() {
  cout << "compare title and body\n";
  for (map< string, pair<int, int> >::iterator it = title_df_map.begin(); it != title_df_map.end(); ++it)
    if (!body_df_map.count(it->first))
      cout << it->first << "  ";

  cout << endl;

  cout << "\n compare tags and body\n";
 // for (map<string, int>::iterator it = tag_set.begin(); it != tag_set.end(); ++it)
 //   if (!body_df_map.count(it->first))
 //     cout << it->first << "  ";
}
*/
















