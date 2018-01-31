#include "TagMining.h"
#include <math.h>
#include <time.h>


TagMining::TagMining(string working_dir_name) {
  working_dir = working_dir_name;
}


void TagMining::PrepStopwords(set<string>* stopwords, string dic_filename) {
	ifstream dictionary(working_dir + "\\" + dic_filename, ios_base::in);

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
  BlockRemoval(contents, "<pre>", "</pre>");
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
		  (*contents)[i] += 'a' - 'A';
		if ((*contents)[i] == '\"' || (*contents)[i] == '\'' || (*contents)[i] == '\?' || (*contents)[i] == '!'
		    || (*contents)[i] == '(' || (*contents)[i] == ')' || (*contents)[i] == ',' || (*contents)[i] == ';')
      (*contents)[i] = ' ';
    if ((*contents)[i] == ':') {
      if (i != contents->size() - 1) {
        if ((*contents)[i + 1] == '/')
          (*contents)[i] = '~';
        else
          (*contents)[i] = ' ';
        }
      else
        (*contents)[i] = ' ';
    }
  }

	stringstream contents_stream(*contents);
	string raw, output;
	bool meaningless;
	while (contents_stream >> raw) {
    if (raw.find("http~//") != string::npos || raw.find("https~//") != string::npos
        || raw.find("0x") != string::npos || raw.find("..") != string::npos
        || raw.find("fff") != string::npos || raw.find("ggg") != string::npos
        || raw.find("hhh") != string::npos || raw.find("jjj") != string::npos
        || raw.find("kkk") != string::npos || raw.find("mmm") != string::npos
        || raw.find("nnn") != string::npos || raw.find("qqq") != string::npos
        || raw.find("rrr") != string::npos || raw.find("uuu") != string::npos
        || raw.find("vvv") != string::npos || raw.find("xxx") != string::npos
        || raw.find("yyy") != string::npos || raw.find("zzz") != string::npos
        || !(raw[0] == '.' || (raw[0] >= '0' && raw[0] <= '9') || (raw[0] >= 'a' && raw[0] <= 'z')))
      continue;

    int num_count = 0, len = raw.size();

		if (raw.back() == '.') {
			raw.erase(len - 1, 1);
			--len;
    }

    meaningless = false;

    for (int pos = 0; pos < len; ++pos) {
      if (!(raw[pos] == '.' || raw[pos] == '#' || raw[pos] == '+' || raw[pos] == '-' || raw[pos] == ' '
          || (raw[pos] >= '0' && raw[pos] <= '9') || (raw[pos] >='a' && raw[pos] <= 'z'))) {
        meaningless = true;
        break;
      }

      if (!((raw[pos] >= 'a' && raw[pos] <='z') || raw[pos] == '.' || raw[pos] == ' '))
        ++num_count;
    }

		if (num_count == len || stopwords->find(raw) != stopwords->end())
			meaningless = true;

		if (meaningless == false)
			output += raw + " ";
	}

	return output;
}


void TagMining::InsertToDfMap(string* contents) {
	stringstream temp_sstream;
	set<string> words_wo_duplicity;
	string word;

  temp_sstream << *contents;
  while (temp_sstream >> word)
    words_wo_duplicity.insert(word);
  for (set<string>::iterator it = words_wo_duplicity.begin(); it != words_wo_duplicity.end(); ++it) {
    if (df_map.count(*it))
      ++(df_map[*it]);
    else
      df_map.insert(make_pair(*it, 1));
  }
}


void TagMining::ParseTrainingDataAndBuildDfMap() {
	ifstream raw_data(working_dir + "\\train.csv", ios_base::in);

  string ln_buffer, item_container;
	size_t marker_pos[3];

	getline(raw_data, ln_buffer);

  set<string> t_stopwords;
  PrepStopwords(&t_stopwords, "dictionary_t.txt");
  set<string> b_stopwords;
  PrepStopwords(&b_stopwords, "dictionary_b.txt");

  ofstream train_parsed_file(working_dir + "\\train_parsed.txt", ios_base::out);

	int prog = 0;
	while (getline(raw_data, ln_buffer)) {
		++prog;
		train_parsed_file << prog << endl;
		if (prog % 1000000 == 0)
			cout << prog << "-th item processed.\n";

		item_container.clear();
		item_container += ln_buffer;

		marker_pos[0] = item_container.find("\",\"") + 3;
		marker_pos[1] = item_container.find("\",\"", marker_pos[0]) + 3;

		while (1) {
			if (item_container.back() == '\"') {
				char c1 = raw_data.get();
				char c2 = raw_data.peek();
				raw_data.unget();
				if (c1 == EOF || (c1 == '\"' && c2 >= '1' && c2 <= '9'))
					break;
			}
			getline(raw_data, ln_buffer);
			item_container += " " + ln_buffer;
		}

		marker_pos[2] = item_container.rfind("\",\"") + 3;

		string contents;

		contents = item_container.substr(marker_pos[2], item_container.size() -1 - marker_pos[2]);
		train_parsed_file << contents << endl;
    stringstream tag_sstream(contents);
		string indv_tags;
		while (tag_sstream >> indv_tags) {
			if (inverted_tag_map.count(indv_tags))
			  inverted_tag_map[indv_tags].insert(prog);
      else {
        set<int> temp_item_no_set {prog};
        inverted_tag_map.insert(make_pair(indv_tags, temp_item_no_set));
      }
    }

		contents = item_container.substr(marker_pos[0], marker_pos[1] - marker_pos[0] - 3);
		contents = SiftOff(&contents, &t_stopwords);
		InsertToDfMap(&contents);
		train_parsed_file << contents << endl;

		contents = item_container.substr(marker_pos[1], marker_pos[2] - marker_pos[1] - 3);
		contents = SiftOff(&contents, &b_stopwords);
		InsertToDfMap(&contents);
		train_parsed_file << contents << endl;
	}

	raw_data.close();
	train_parsed_file.close();

  ofstream inverted_tag_map_file(working_dir + "\\inverted_tag_map.txt", ios_base::out);
  for (map< string, set<int> >::iterator map_it = inverted_tag_map.begin();
       map_it != inverted_tag_map.end(); ++map_it) {
    inverted_tag_map_file << map_it->first << " ";
    for (set<int>::iterator set_it = (map_it->second).begin(); set_it != (map_it->second).end(); ++set_it)
      inverted_tag_map_file << " " << *set_it;
    inverted_tag_map_file << endl;
  }
  inverted_tag_map_file.close();

	for (map<string, int>::iterator df_map_it = df_map.begin(); df_map_it != df_map.end(); ) {
	  if (df_map_it->second <= DFCUT)
	    df_map_it = df_map.erase(df_map_it);
    else
      ++df_map_it;
  }

  cout << "df_map size with DFCUT = " << DFCUT << " : " << df_map.size()
       << ",  no. of tags: " << inverted_tag_map.size() << endl;

	ofstream df_map_file(working_dir + "\\df_map.txt", ios_base::out);
	for (map<string, int>::iterator it = df_map.begin(); it != df_map.end(); ++it)
	  df_map_file << it->first << "  " << it->second << endl;
  df_map_file.close();
}

/*  void TagMining::ParseTrainingDataAndBuildDfMap() {
	ifstream raw_data(working_dir + "\\train.csv", ios_base::in);

  string ln_buffer, item_container;
	size_t marker_pos[4];

	getline(raw_data, ln_buffer);

  set<string> t_stopwords;
  PrepStopwords(&t_stopwords, "dictionary_t.txt");
  set<string> b_stopwords;
  PrepStopwords(&b_stopwords, "dictionary_b.txt");

  ofstream train_parsed_file(working_dir + "\\train_parsed.txt", ios_base::out);

	int prog = 0;
	while (getline(raw_data, ln_buffer)) {
		++prog;
		train_parsed_file << prog << endl;
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
			item_container += " " + ln_buffer;
		}

		marker_pos[2] = item_container.rfind("\",\"") + 3;

		string contents;

		contents = item_container.substr(marker_pos[2], marker_pos[3] - marker_pos[2] + 1);
		train_parsed_file << contents << endl;
    stringstream tag_sstream(contents);
		string indv_tags;
		while (tag_sstream >> indv_tags) {
			if (inverted_tag_map.count(indv_tags))
			  inverted_tag_map[indv_tags].insert(prog);
      else {
        set<int> temp_item_no_set {prog};
        inverted_tag_map.insert(make_pair(indv_tags, temp_item_no_set));
      }
    }

		contents = item_container.substr(marker_pos[0], marker_pos[1] - marker_pos[0] - 3);
		contents = SiftOff(&contents, &t_stopwords);
		InsertToDfMap(&contents);
		train_parsed_file << contents << endl;

		contents = item_container.substr(marker_pos[1], marker_pos[2] - marker_pos[1] - 3);
		contents = SiftOff(&contents, &b_stopwords);
		InsertToDfMap(&contents);
		train_parsed_file << contents << endl;
	}

	raw_data.close();
	train_parsed_file.close();

  ofstream inverted_tag_map_file(working_dir + "\\inverted_tag_map.txt", ios_base::out);
  for (map< string, set<int> >::iterator map_it = inverted_tag_map.begin();
       map_it != inverted_tag_map.end(); ++map_it) {
    inverted_tag_map_file << map_it->first << " ";
    for (set<int>::iterator set_it = (map_it->second).begin(); set_it != (map_it->second).end(); ++set_it)
      inverted_tag_map_file << " " << *set_it;
    inverted_tag_map_file << endl;
  }
  inverted_tag_map_file.close();

	for (map<string, int>::iterator df_map_it = df_map.begin(); df_map_it != df_map.end(); ) {
	  if (df_map_it->second <= DFCUT)
	    df_map_it = df_map.erase(df_map_it);
    else
      ++df_map_it;
  }

  cout << "df_map size with DFCUT = " << DFCUT << " : " << df_map.size()
       << ",  no. of tags: " << inverted_tag_map.size() << endl;

	ofstream df_map_file(working_dir + "\\df_map.txt", ios_base::out);
	for (map<string, int>::iterator it = df_map.begin(); it != df_map.end(); ++it)
	  df_map_file << it->first << "  " << it->second << endl;
  df_map_file.close();
}
*/

double TagMining::PosWeightFtn(int pos, int length) {
  return pow((pos - 0.7 * length) / (0.7 * length), 2);
}


void TagMining::BuildLocalTagSet(const string& tags, set<string>* local_tags) {
  stringstream ln_sstream(tags);
  string word_buffer;

  while (ln_sstream >> word_buffer)
    local_tags->insert(word_buffer);
}


void TagMining::AddTitleWordsToDocVec(const string& title, map<string, double>* doc_vec) {
  stringstream ln_sstream(title);
  string word_buffer;

  while (ln_sstream >> word_buffer) {
    if (doc_vec->count(word_buffer))
      doc_vec->find(word_buffer)->second += 1;
    else
      doc_vec->insert(make_pair(word_buffer, 1));
  }
}


void TagMining::AddBodyWordsToDocVec(const string& body, map<string, double>* doc_vec) {
  int no_of_words = 0;
  for (unsigned i = 0; i < body.size(); ++i)
    if (body[i] == ' ')
      ++no_of_words;
  ++no_of_words;

  stringstream ln_sstream(body);
  string word_buffer;

  int pos = 0;
  while (ln_sstream >> word_buffer) {
    if (doc_vec->count(word_buffer))
      doc_vec->find(word_buffer)->second += PosWeightFtn(pos, no_of_words);
    else
      doc_vec->insert(make_pair(word_buffer, PosWeightFtn(pos, no_of_words)));
    ++pos;
  }
}


void TagMining::AddEntry(map<string, double>* tag_cor_strength_map, string tag, double val) {
  if(tag_cor_strength_map->count(tag))
    tag_cor_strength_map->find(tag)->second += val;
  else
    tag_cor_strength_map->insert(make_pair(tag, val));
}


void TagMining::AddToCorrelationMap(set<string>* local_tags, map<string, double>* doc_vec) {
	string word;
	double tf_idf;

  for (map<string, double>::iterator doc_vec_it = doc_vec->begin();
       doc_vec_it != doc_vec->end(); ++doc_vec_it) {
    word = doc_vec_it->first;

    if (df_map.count(word)) {
      tf_idf = (doc_vec_it->second) * log(6034195.0 / df_map[word]);
      // tf = doc_vec_it->second;

      if (correlation_map.count(word))
        for (set<string>::iterator it = local_tags->begin(); it != local_tags->end(); ++it)
          AddEntry(&(correlation_map[word]), *it, tf_idf);
      else {
        map<string, double> temp_tag_cs_map;
        for (set<string>::iterator it = local_tags->begin(); it != local_tags->end(); ++it)
          temp_tag_cs_map.insert(make_pair(*it, tf_idf));
        correlation_map.insert(make_pair(word, temp_tag_cs_map));
      }
    }
  }
}


void TagMining::BuildCorrelationMap() {
	ifstream train_parsed_file(working_dir + "\\train_parsed.txt", ios_base::in);
	string ln_buffer;

	int item_no = 0;
	while (getline(train_parsed_file, ln_buffer)) {
		++item_no;
		if (item_no % 1000000 == 0)
			cout << item_no << "-th item processed.\n";

    set<string> local_tags;
		getline(train_parsed_file, ln_buffer);
		BuildLocalTagSet(ln_buffer, &local_tags);

    map<string, double> doc_vec;
    getline(train_parsed_file, ln_buffer);
    AddTitleWordsToDocVec(ln_buffer, &doc_vec);
    getline(train_parsed_file, ln_buffer);
    AddBodyWordsToDocVec(ln_buffer, &doc_vec);

    AddToCorrelationMap(&local_tags, &doc_vec);
	}

	train_parsed_file.close();

/*  for (map< string, map<string, double> >::iterator cor_map_it = correlation_map.begin();
       cor_map_it != correlation_map.end(); ++cor_map_it) {
    for (map<string, double>::iterator tag_cs_map_it = (cor_map_it->second).begin();
         tag_cs_map_it != (cor_map_it->second).end(); ++tag_cs_map_it) {
      tag_cs_map_it->second /= inverted_tag_map[tag_cs_map_it->first].size();

      tag_cs_map_it->second *= log(6034195.0 / inverted_tag_map[tag_cs_map_it->first].size())
                               / inverted_tag_map[tag_cs_map_it->first].size();
    }
  }  */
  for (map< string, map<string, double> >::iterator cor_map_it = correlation_map.begin();
       cor_map_it != correlation_map.end(); ) {
    for (map<string, double>::iterator tag_cs_map_it = (cor_map_it->second).begin();
         tag_cs_map_it != (cor_map_it->second).end(); ) {
      tag_cs_map_it->second /= inverted_tag_map[tag_cs_map_it->first].size();
      if (tag_cs_map_it->second < 0.001)
        tag_cs_map_it = (cor_map_it->second).erase(tag_cs_map_it);
      else
        ++tag_cs_map_it;
    }

    if ((cor_map_it->second).size() == 0)
      cor_map_it = correlation_map.erase(cor_map_it);
    else
      ++cor_map_it;
  }
  inverted_tag_map.clear();

//  cout << "correlation_map size for DFCUT = " << DFCUT << " and lower bound = 0.01: "
//       << correlation_map.size() << endl;

  ofstream cor_map_file(working_dir + "\\correlation_map.txt", ios_base::out);
  for (map< string, map<string, double> >::iterator cor_map_it = correlation_map.begin();
       cor_map_it != correlation_map.end(); ++cor_map_it) {
    cor_map_file << cor_map_it->first << " ";
    for (map<string, double>::iterator tag_cs_map_it = (cor_map_it->second).begin();
         tag_cs_map_it != (cor_map_it->second).end(); ++tag_cs_map_it)
      cor_map_file << " " << tag_cs_map_it->first << " " << tag_cs_map_it->second;
    cor_map_file << endl;
  }
  cor_map_file.close();
}


void TagMining::ExamineCorMap() {
  double max = 0, min = 1000000;
  string word_max, tag_max;

  for (map< string, map<string, double> >::iterator cor_map_it = correlation_map.begin();
       cor_map_it != correlation_map.end(); ++cor_map_it)
    for (map<string, double>::iterator tag_cor_strength_map_it = (cor_map_it->second).begin();
         tag_cor_strength_map_it != (cor_map_it->second).end(); ++tag_cor_strength_map_it) {
      if (tag_cor_strength_map_it->second > max) {
        max = tag_cor_strength_map_it->second;
        word_max = cor_map_it->first;
        tag_max = tag_cor_strength_map_it->first;
      }
      if (tag_cor_strength_map_it->second < min)
        min = tag_cor_strength_map_it->second;
    }

  cout << "max correlation strength = " << max
       << " for " << word_max << " and " << tag_max << " pair." << endl
       << "min correlation strength = " << min << endl;

  int count_1 = 0, count_2 = 0, count_3 = 0, count_4 = 0;

  for (map< string, map<string, double> >::iterator cor_map_it = correlation_map.begin();
       cor_map_it != correlation_map.end(); ++cor_map_it)
    for (map<string, double>::iterator tag_cor_strength_map_it = (cor_map_it->second).begin();
         tag_cor_strength_map_it != (cor_map_it->second).end(); ++tag_cor_strength_map_it) {
      if (tag_cor_strength_map_it->second > max / 2)
        ++count_1;
      if (tag_cor_strength_map_it->second > max / 5)
        ++count_2;
      if (tag_cor_strength_map_it->second > max / 10)
        ++count_3;
      if (tag_cor_strength_map_it->second > max / 100)
        ++count_4;
    }

  cout << "no. of tag-word connection with correlation strength > (max / 2) = " << count_1 << endl
       << "> (max / 5) = " << count_2 << endl
       << "> (max / 10) = " << count_3 << endl
       << "> (max / 100) = " << count_4 << endl;
}


void TagMining::LoadMaps() {
  string ln_buffer, word_buffer, word, tag;

  ifstream df_map_file(working_dir + "\\df_map.txt", ios_base::in);
  int df;
  while (df_map_file >> word_buffer) {
    word = word_buffer;

    df_map_file >> word_buffer;
    df = stoi(word_buffer);

    df_map.insert(make_pair(word, df));
  }
  df_map_file.close();

  ifstream inverted_tag_map_file(working_dir + "\\inverted_tag_map.txt", ios_base::in);
  while (getline(inverted_tag_map_file, ln_buffer)) {
    stringstream ln_sstream(ln_buffer);

    ln_sstream >> word_buffer;
    tag = word_buffer;

    set<int> item_set;
    while (ln_sstream >> word_buffer)
      item_set.insert(stoi(word_buffer));

    inverted_tag_map.insert(make_pair(tag, item_set));
  }
  inverted_tag_map_file.close();
}


void TagMining::LoadCorMap() {
  string ln_buffer, word_buffer, word, tag;

  ifstream cor_map_file(working_dir + "\\correlation_map.txt", ios_base::in);
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

/*
void TagMining::BuildTrainingSet(string tag) {
  training_set.clear();
  set<int> item_list = inverted_tag_map[tag];
  int no_of_items = item_list.size();

	ifstream train_parsed_file(working_dir + "\\train_parsed.txt", ios_base::in);
	string ln_buffer;

	int item_no = 0, no_of_items_parsed = 0;
	no_of_tags_to_predict = 0;

  if (train_parsed_file.is_open()) {
  	while (getline(train_parsed_file, ln_buffer)) {
	  	++item_no;

  		if (!item_list.count(item_no)) {
        getline(train_parsed_file, ln_buffer);
        getline(train_parsed_file, ln_buffer);
        getline(train_parsed_file, ln_buffer);
        continue;
      }

      set<string> local_tags;
	  	getline(train_parsed_file, ln_buffer);
		  BuildLocalTagSet(ln_buffer, &local_tags);
      no_of_tags_to_predict += local_tags.size();

      map<string, double> doc_vec;
      getline(train_parsed_file, ln_buffer);
      AddTitleWordsToDocVec(ln_buffer, &doc_vec);
      getline(train_parsed_file, ln_buffer);
      AddBodyWordsToDocVec(ln_buffer, &doc_vec);

      for (map<string, double>::iterator it = doc_vec.begin(); it != doc_vec.end(); ) {
        if (!df_map.count(it->first))
          it = doc_vec.erase(it);
        else {
          it->second *= log(6034195 / df_map[it->first]);
          ++it;
        }
      }

      training_set.insert(make_pair(item_no, make_pair(local_tags, doc_vec)));

      ++no_of_items_parsed;
      if (no_of_items_parsed == no_of_items)
        break;
	  }

	  cout << "no. of tags to predict: " << no_of_tags_to_predict << endl << endl;
  }
  else
    cout << "where is the training source file?\n";

	train_parsed_file.close();
}


void TagMining::Train(double threshold_val, double portion_of_words_for_tag) {
  threshold = threshold_val;

  map<string, int> no_of_keywords_for_tags;

  for (map< string, map<string, double> >::iterator cor_map_it = correlation_map.begin();
       cor_map_it != correlation_map.end(); ++cor_map_it)
    for (map<string, double>::iterator tag_cs_map_it = (cor_map_it->second).begin();
         tag_cs_map_it != (cor_map_it->second).end(); ++tag_cs_map_it)
      if (tag_cs_map_it->second >= threshold) {
        if (no_of_keywords_for_tags.count(tag_cs_map_it->first))
          ++(no_of_keywords_for_tags[tag_cs_map_it->first]);
        else
          no_of_keywords_for_tags.insert(make_pair(tag_cs_map_it->first, 1));
      }

  int prediction = 0, correct = 0;

  for (map< int, pair< set<string>, map<string, double> > >::iterator training_set_it = training_set.begin();
       training_set_it != training_set.end(); ++training_set_it) {
    map<string, int> tag_candidates;

    for (map<string, double>::iterator doc_vec_it = (training_set_it->second).second.begin();
         doc_vec_it != (training_set_it->second).second.end(); ++doc_vec_it) {
      if (doc_vec_it->second > threshold)
        for (map<string, double>::iterator tag_cs_map_it = correlation_map[doc_vec_it->first].begin();
             tag_cs_map_it != correlation_map[doc_vec_it->first].end(); ++tag_cs_map_it)
          if (tag_cs_map_it->second > threshold) {
            if (tag_candidates.count(tag_cs_map_it->first))
              ++(tag_candidates[tag_cs_map_it->first]);
            else
              tag_candidates.insert(make_pair(tag_cs_map_it->first, 1));
          }
    }

    for (map<string, int>::iterator tag_cand_map_it = tag_candidates.begin();
         tag_cand_map_it != tag_candidates.end(); ) {
      if (tag_cand_map_it->second < no_of_keywords_for_tags[tag_cand_map_it->first] / min_no_of_common_words_val)
        tag_cand_map_it = tag_candidates.erase(tag_cand_map_it);
      else
        ++tag_cand_map_it;
    }

    prediction += tag_candidates.size();

    for (map<string, int>::iterator tag_cand_map_it = tag_candidates.begin();
         tag_cand_map_it != tag_candidates.end(); ++tag_cand_map_it)
      if ((training_set_it->second).first.count(tag_cand_map_it->first))
        ++correct;


  }

  double precision = (double)correct / prediction ;
  double recall = (double)correct / no_of_tags_to_predict;
  cout << "Precision = " << 100 * precision << "%,  Recall = " << 100 * recall << "%" << endl;
  cout << "prediction: " << prediction << ",  correct: " << correct << endl;
  cout << "F1-metric = " << 2.0 * precision * recall / (precision + recall) << endl << endl;

}
*/


void TagMining::ConvertParsedDataFileToDocVectorFile() {
	ifstream train_parsed_file(working_dir + "\\train_parsed.txt", ios_base::in);
	ofstream doc_vec_file(working_dir + "\\doc_vectors.txt", ios_base::out);
	string ln_buffer;

	int item_no = 0;

	if (train_parsed_file.is_open())
  	while (getline(train_parsed_file, ln_buffer)) {
	  	++item_no;
	  	if (item_no % 1000000 == 0)
	  	  cout << item_no << "-th item doc-vector processed.\n";
      doc_vec_file << item_no << endl;

	  	getline(train_parsed_file, ln_buffer);
		  doc_vec_file << ln_buffer << endl;

      map<string, double> doc_vec;
      getline(train_parsed_file, ln_buffer);
      AddTitleWordsToDocVec(ln_buffer, &doc_vec);
      getline(train_parsed_file, ln_buffer);
      AddBodyWordsToDocVec(ln_buffer, &doc_vec);

      for (map<string, double>::iterator it = doc_vec.begin(); it != doc_vec.end(); ) {
        if (!df_map.count(it->first))
          it = doc_vec.erase(it);
        else {
          it->second *= log(6034195 / df_map[it->first]);
          doc_vec_file << it->first << " " << it->second << " ";
          ++it;
        }
      }
      doc_vec_file << endl;
	  }
  else
    cout << "parsed data file open error.\n";

	train_parsed_file.close();
	doc_vec_file.close();
}


void TagMining::BuildTrainingSet(int size_of_training_set) {
	ifstream train_parsed_file(working_dir + "\\train_parsed.txt", ios_base::in);
	string ln_buffer;

  srand(time(NULL));
//  int start = (600000 * rand()) % (6034195 - size_of_training_set) + 1;
  int start = 120345;

	int item_no = 0;
	no_of_tags_to_predict = 0;

	if (start > 6034195 - size_of_training_set)
	  cout << "wrong start for training set.\n";
  else if (!train_parsed_file.is_open())
    cout << "where is the training source file?\n";
  else {
  	while (getline(train_parsed_file, ln_buffer)) {
	  	++item_no;

  		if (item_no < start) {
        getline(train_parsed_file, ln_buffer);
        getline(train_parsed_file, ln_buffer);
        getline(train_parsed_file, ln_buffer);
        continue;
      }

      if (item_no > start + size_of_training_set)
        break;

      set<string> local_tags;
	  	getline(train_parsed_file, ln_buffer);
		  BuildLocalTagSet(ln_buffer, &local_tags);
      no_of_tags_to_predict += local_tags.size();

      map<string, double> doc_vec;
      getline(train_parsed_file, ln_buffer);
      AddTitleWordsToDocVec(ln_buffer, &doc_vec);
      getline(train_parsed_file, ln_buffer);
      AddBodyWordsToDocVec(ln_buffer, &doc_vec);

      for (map<string, double>::iterator it = doc_vec.begin(); it != doc_vec.end(); ) {
        if (!df_map.count(it->first))
          it = doc_vec.erase(it);
        else {
          it->second *= log(6034195 / df_map[it->first]);
          ++it;
        }
      }

      training_set.insert(make_pair(item_no, make_pair(local_tags, doc_vec)));
	  }

	  cout << "no. of tags to predict: " << no_of_tags_to_predict << endl << endl;
  }

	train_parsed_file.close();
}


void TagMining::Train(double threshold_val, int min_no_of_common_words_val) {
  threshold = threshold_val;
  min_no_of_common_words = min_no_of_common_words_val;

  map<string, int> no_of_keywords_for_tags;

  for (map< string, map<string, double> >::iterator cor_map_it = correlation_map.begin();
       cor_map_it != correlation_map.end(); ++cor_map_it)
    for (map<string, double>::iterator tag_cs_map_it = (cor_map_it->second).begin();
         tag_cs_map_it != (cor_map_it->second).end(); ++tag_cs_map_it)
      if (tag_cs_map_it->second > threshold) {
        if (no_of_keywords_for_tags.count(tag_cs_map_it->first))
          ++(no_of_keywords_for_tags[tag_cs_map_it->first]);
        else
          no_of_keywords_for_tags.insert(make_pair(tag_cs_map_it->first, 1));
      }

  int prediction = 0, correct = 0;

  for (map< int, pair< set<string>, map<string, double> > >::iterator training_set_it = training_set.begin();
       training_set_it != training_set.end(); ++training_set_it) {
    map<string, int> tag_candidates;

    for (map<string, double>::iterator doc_vec_it = (training_set_it->second).second.begin();
         doc_vec_it != (training_set_it->second).second.end(); ++doc_vec_it) {
      if (doc_vec_it->second > threshold)
        for (map<string, double>::iterator tag_cs_map_it = correlation_map[doc_vec_it->first].begin();
             tag_cs_map_it != correlation_map[doc_vec_it->first].end(); ++tag_cs_map_it)
          if (tag_cs_map_it->second > threshold) {
            if (tag_candidates.count(tag_cs_map_it->first))
              ++(tag_candidates[tag_cs_map_it->first]);
            else
              tag_candidates.insert(make_pair(tag_cs_map_it->first, 1));
          }
    }

    for (map<string, int>::iterator tag_cand_map_it = tag_candidates.begin();
         tag_cand_map_it != tag_candidates.end(); ) {
      int cut = no_of_keywords_for_tags[tag_cand_map_it->first];

      if (cut < min_no_of_common_words) {
        if (tag_cand_map_it->second < cut)
          tag_cand_map_it = tag_candidates.erase(tag_cand_map_it);
        else
          ++tag_cand_map_it;
      }
      else {
        if (tag_cand_map_it->second < min_no_of_common_words)
          tag_cand_map_it = tag_candidates.erase(tag_cand_map_it);
        else
          ++tag_cand_map_it;
      }
    }

    prediction += tag_candidates.size();

    for (map<string, int>::iterator tag_cand_map_it = tag_candidates.begin();
         tag_cand_map_it != tag_candidates.end(); ++tag_cand_map_it)
      if ((training_set_it->second).first.count(tag_cand_map_it->first))
        ++correct;
  }

  double precision = (double)correct / prediction ;
  double recall = (double)correct / no_of_tags_to_predict;
  cout << "Precision = " << 100 * precision << "%,  Recall = " << 100 * recall << "%" << endl;
  cout << "prediction: " << prediction << ",  correct: " << correct << endl;
  cout << "F1-metric = " << 2.0 * precision * recall / (precision + recall) << endl << endl;
}



/*
void TagMining::Train(double threshold_val) {
  int prediction = 0, correct = 0;
  threshold = threshold_val;

//  ofstream train_result_file(working_dir + "\\train_result.txt", ios_base::app);

  for (map< int, pair< set<string>, map<string, double> > >::iterator training_set_it = training_set.begin();
       training_set_it != training_set.end(); ++training_set_it) {
    set<string> tag_candidates;

    for (map<string, double>::iterator doc_vec_it = (training_set_it->second).second.begin();
         doc_vec_it != (training_set_it->second).second.end(); ++doc_vec_it) {
      if (doc_vec_it->second > threshold)
        for (map<string, double>::iterator tag_cs_map_it = correlation_map[doc_vec_it->first].begin();
             tag_cs_map_it != correlation_map[doc_vec_it->first].end(); ++tag_cs_map_it)
          if (tag_cs_map_it->second > threshold)
            tag_candidates.insert(tag_cs_map_it->first);
    }

    prediction += tag_candidates.size();

    for (set<string>::iterator it = tag_candidates.begin(); it != tag_candidates.end(); ++it)
      if ((training_set_it->second).first.count(*it))
        ++correct;

    train_result_file << training_set_it->first << endl;
    for (set<string>::iterator it = (training_set_it->second).first.begin();
         it != (training_set_it->second).first.end(); ++it)
      train_result_file << *it << " ";
    train_result_file << endl;
    for (set<string>::iterator it = tag_candidates.begin(); it != tag_candidates.end(); ++it) {
      train_result_file << *it << " ";
      if ((training_set_it->second).first.count(*it))
        ++correct;
    }
    train_result_file << endl << endl;
  }

//  train_result_file.close();

cout << "prediction: " << prediction << ",  correct: " << correct
     << ",  no. of tags to predict: " << no_of_tags_to_predict << endl;

  double precision = (double)correct / prediction ;
  double recall = (double)correct / no_of_tags_to_predict;
  cout << "Precision = " << 100 * precision << "%,  Recall = " << 100 * recall << "%" << endl;
  cout << "F1-metric = " << 2.0 * precision * recall / (precision + recall) << endl << endl;

}
*/



















