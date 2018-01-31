#include "TagMining.h"
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
	stringstream temp_sstream(*contents);
	set<string> words_wo_duplicity;
	string word;

  while (temp_sstream >> word)
    words_wo_duplicity.insert(word);

  for (set<string>::iterator it = words_wo_duplicity.begin(); it != words_wo_duplicity.end(); ++it) {
    if (df_map.count(*it))
      ++(df_map[*it].second);
    else
      df_map.insert(make_pair(*it, make_pair(0, 1)));
  }
}


void TagMining::ParseLearningDataAndBuildMaps() {
	ifstream raw_data(working_dir + "\\train.csv", ios_base::in);

  string ln_buffer, item_container;
	size_t marker_pos[3];

  set<string> t_stopwords;
  PrepStopwords(&t_stopwords, "dictionary_t.txt");
  set<string> b_stopwords;
  PrepStopwords(&b_stopwords, "dictionary_b.txt");

  ofstream train_parsed_file(working_dir + "\\train_parsed.txt", ios_base::out);

	int item_no = 0;
	map< string, set<int> > temp_inverted_tag_map;

	getline(raw_data, ln_buffer);

	while (getline(raw_data, ln_buffer)) {
		++item_no;
		train_parsed_file << item_no << endl;
		if (item_no % 1000000 == 0)
			cout << item_no << "-th item processed.\n";

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

		string contents, full_contents;

		contents = item_container.substr(marker_pos[2], item_container.size() - 1 - marker_pos[2]);
    stringstream tag_sstream(contents);
		string indv_tags;
		while (tag_sstream >> indv_tags) {
			if (temp_inverted_tag_map.count(indv_tags))
			  temp_inverted_tag_map[indv_tags].insert(item_no);
      else {
        set<int> temp_item_no_set = {item_no};
        temp_inverted_tag_map.insert(make_pair(indv_tags, temp_item_no_set));
      }
    }

		contents = item_container.substr(marker_pos[0], marker_pos[1] - marker_pos[0] - 3);
		contents = SiftOff(&contents, &t_stopwords);
		train_parsed_file << contents << endl;
		full_contents = contents;

		contents = item_container.substr(marker_pos[1], marker_pos[2] - marker_pos[1] - 3);
		contents = SiftOff(&contents, &b_stopwords);
		train_parsed_file << contents << endl;
		full_contents += " " + contents;

		InsertToDfMap(&full_contents);
	}

	raw_data.close();
	train_parsed_file.close();

  int id = 0;
  ofstream tag_set_file(working_dir + "\\tag_set.txt", ios_base::out);
  ofstream inverted_tag_map_file(working_dir + "\\inverted_tag_map.txt", ios_base::out);

  for (map< string, set<int> >::iterator it = temp_inverted_tag_map.begin();
       it != temp_inverted_tag_map.end(); ++it) {
    ++id;

    tag_set.insert(make_pair(it->first, id));
    tag_set_file << it->first << " " << id << endl;

    inverted_tag_map.insert(make_pair(id, it->second));
    inverted_tag_map_file << id << " ";
    for (set<int>::iterator set_it = (it->second).begin(); set_it != (it->second).end(); ++set_it)
      inverted_tag_map_file << " " << *set_it;
    inverted_tag_map_file << endl;
  }

  tag_set_file.close();
  inverted_tag_map_file.close();

  id = 0;
	for (map< string, pair<int, int> >::iterator it = df_map.begin(); it != df_map.end(); ) {
	  if ((it->second).second <= DFCUT)
	    it = df_map.erase(it);
    else {
      ++id;
      (it->second).first = id;
      ++it;
    }
  }

	ofstream df_map_file(working_dir + "\\df_map.txt", ios_base::out);

	for (map< string, pair<int, int> >::iterator it = df_map.begin(); it != df_map.end(); ++it)
	  df_map_file << it->first << " " << (it->second).first << " " << (it->second).second << endl;

  df_map_file.close();

  cout << "df_map size with DFCUT = " << DFCUT << " : " << df_map.size()
       << ", no. of tags : " << tag_set.size() << endl;
}


void TagMining::LoadMaps() {
  string word, id, df;

  ifstream tag_set_file(working_dir + "\\tag_set.txt", ios_base::in);
  while (tag_set_file >> word) {
    tag_set_file >> id;
    tag_set.insert(make_pair(word, stoi(id)));
  }
  tag_set_file.close();

  ifstream df_map_file(working_dir + "\\df_map.txt", ios_base::in);
  while (df_map_file >> word) {
    df_map_file >> id;
    df_map_file >> df;
    df_map.insert(make_pair(word, make_pair(stoi(id), stoi(df))));
  }
  df_map_file.close();

  string ln_buffer, buffer;
  int tag_id;

  ifstream inverted_tag_map_file(working_dir + "\\inverted_tag_map.txt", ios_base::in);
  while (getline(inverted_tag_map_file, ln_buffer)) {
    stringstream ln_sstream(ln_buffer);
    ln_sstream >> buffer;
    tag_id = stoi(buffer);

    set<int> item_no_set;
    while (ln_sstream >> buffer)
      item_no_set.insert(stoi(buffer));

    inverted_tag_map.insert(make_pair(tag_id, item_no_set));
  }
  inverted_tag_map_file.close();
}


void TagMining::CheckTagsDist() {
  ofstream tag_df_file(working_dir + "\\tag_df.txt", ios_base::out);

  for (map< int, set<int> >::iterator map_it = inverted_tag_map.begin();
       map_it != inverted_tag_map.end(); ++map_it)
    tag_df_file << map_it->first << ':' << (map_it->second).size() << endl;

  tag_df_file.close();
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


double TagMining::PosWeightFtn(int pos, int length) {
  if (length < 21)
    return 1;
  else {
    if (pos < 11 || pos > length - 10)
      return 1;
    else
      return pow((pos - 0.7 * length) / (0.7 * length), 4);
  }
}


void TagMining::AddBodyWordsToDocVec(const string& body, map<string, double>* doc_vec) {
  int no_of_words = 1;
  for (unsigned i = 0; i < body.size(); ++i)
    if (body[i] == ' ')
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


void TagMining::BuildTrainItemVectors() {
	ifstream train_parsed_file(working_dir + "\\train_parsed.txt", ios_base::in);
	string ln_buffer;
	int item_no = 0;

	ofstream train_item_vectors_file(working_dir + "\\train_item_vectors.txt", ios_base::out);

	if (train_parsed_file.is_open())
  	while (getline(train_parsed_file, ln_buffer)) {
	  	++item_no;
	  	if (item_no % 1000000 == 0)
	  	  cout << item_no << "-th item doc-vector processed.\n";

      map<string, double> doc_vec;
      getline(train_parsed_file, ln_buffer);
      AddTitleWordsToDocVec(ln_buffer, &doc_vec);
      getline(train_parsed_file, ln_buffer);
      AddBodyWordsToDocVec(ln_buffer, &doc_vec);

      string doc_vec_str;
      for (map<string, double>::iterator it = doc_vec.begin(); it != doc_vec.end(); ++it)
        if (df_map.count(it->first))
          doc_vec_str += ' ' + to_string(df_map[it->first].first) + ':' + to_string(it->second);

      train_item_vector_map.insert(make_pair(item_no, doc_vec_str));

      train_item_vectors_file << item_no << endl << doc_vec_str << endl;
	  }
  else
    cout << "parsed data file open error.\n";

	train_parsed_file.close();
	train_item_vectors_file.close();
}


void TagMining::LoadTrainItemVectors() {
  ifstream train_item_vectors_file(working_dir + "\\train_item_vectors.txt", ios_base::in);

  string buffer;
  int item_no;

  while (getline(train_item_vectors_file, buffer)) {
    item_no = stoi(buffer);
    getline(train_item_vectors_file, buffer);
    train_item_vector_map.insert(make_pair(item_no, buffer));
  }

  train_item_vectors_file.close();
}


/*void TagMining::FillNegExs(ofstream& out_file, int tag_id, char t_or_v) {
  set<int> neg_ex_set;
  int no_of_pos_exs = inverted_tag_map[tag_id].size(), breaker = 0;

  while (1) {
    srand(time(NULL));
    int r1 = rand(), r2 = rand();
    int item_id_candidate;

    if (t_or_v == 't') {
      item_id_candidate = (r1 * (RAND_MAX + 1) + r2) % SizeOfTrainingSet + 1;
      ++breaker;
      if (!inverted_tag_map[tag_id].count(item_id_candidate)) {
        neg_ex_set.insert(item_id_candidate);
        if ((neg_ex_set.size() > MAX(20000, 8 * no_of_pos_exs)) || breaker > 200000)
          break;
      }
    }
    else {
      item_id_candidate = (r1 * (RAND_MAX + 1) + r2) % (SizeOfTotalSet - SizeOfTrainingSet) + SizeOfTrainingSet + 1;
      ++breaker;
      if (!inverted_tag_map[tag_id].count(item_id_candidate)) {
        neg_ex_set.insert(item_id_candidate);
        if ((neg_ex_set.size() > MAX(10000, 4 * no_of_pos_exs)) || breaker > 100000)
          break;
      }
    }
  }

  for(set<int>::iterator it = neg_ex_set.begin(); it != neg_ex_set.end(); ++it)
    out_file << -1 << train_item_vector_map[*it] << endl;
}
*/
void TagMining::FillNegExs(ofstream& out_file, int tag_id, char t_or_v) {
  set<int> neg_ex_set;
  int no_of_pos_exs = inverted_tag_map[tag_id].size();

  srand(time(NULL));

  while (1) {
    int r1 = rand(), r2 = rand();
    int item_id_candidate;

    if (t_or_v == 't') {
      item_id_candidate = (r1 * (RAND_MAX + 1) + r2) % SizeOfTrainingSet + 1;
      if (!inverted_tag_map[tag_id].count(item_id_candidate)) {
        neg_ex_set.insert(item_id_candidate);
        if (neg_ex_set.size() > MAX(20000, 8 * no_of_pos_exs))
          break;
      }
    }
    else {
      item_id_candidate = (r1 * (RAND_MAX + 1) + r2) % (SizeOfTotalSet - SizeOfTrainingSet) + SizeOfTrainingSet + 1;
      if (!inverted_tag_map[tag_id].count(item_id_candidate)) {
        neg_ex_set.insert(item_id_candidate);
        if (neg_ex_set.size() > MAX(7500, 3 * no_of_pos_exs))
          break;
      }
    }
  }

  for(set<int>::iterator it = neg_ex_set.begin(); it != neg_ex_set.end(); ++it)
    out_file << -1 << train_item_vector_map[*it] << endl;
}

void TagMining::PrepSVMLearningData() {
  int tag_id;

  for (map< int, set<int> >::iterator map_it = inverted_tag_map.begin();
       map_it != inverted_tag_map.end(); ++map_it) {
    tag_id = map_it->first;
    //if (tag_id % 5000 == 0)
      //cout << tag_id << "-th tag is processed.\n";
    if (tag_id == 30)
      break;

    ofstream out_t(working_dir + "\\training\\" + to_string(tag_id) + "_t.svm", ios_base::out);
    ofstream out_v(working_dir + "\\validation\\" + to_string(tag_id) + "_v.svm", ios_base::out);

    for (set<int>::iterator set_it = (map_it->second).begin(); set_it != (map_it->second).end(); ++set_it) {
      if (*set_it <= SizeOfTrainingSet)
        out_t << 1 << train_item_vector_map[*set_it] << endl;
      if (*set_it > SizeOfTrainingSet)
        out_v << 1 << train_item_vector_map[*set_it] << endl;
    }

    FillNegExs(out_t, map_it->first, 't');
    FillNegExs(out_v, map_it->first, 'v');

    out_t.close();
    out_v.close();
  }
}


void TagMining::PrepSVMTrainingData() {
  int tag_id;

  for (map< int, set<int> >::iterator map_it = inverted_tag_map.begin();
       map_it != inverted_tag_map.end(); ++map_it) {
    tag_id = map_it->first;
    if (tag_id % 5000 == 0)
      cout << tag_id << "-th tag is processed.\n";

    ofstream out_t(working_dir + "\\training\\" + to_string(tag_id) + "_t.svm", ios_base::out);

    for (set<int>::iterator set_it = (map_it->second).begin(); set_it != (map_it->second).end(); ++set_it)
      out_t << 1 << train_item_vector_map[*set_it] << endl;

    FillNegExs(out_t, map_it->first, 't');

    out_t.close();
  }
}


void TagMining::ParseTestData() {
	ifstream raw_data(working_dir + "\\test.csv", ios_base::in);

  string ln_buffer, item_container;
	size_t marker_pos[2];

	getline(raw_data, ln_buffer);

  set<string> t_stopwords;
  PrepStopwords(&t_stopwords, "dictionary_t.txt");
  set<string> b_stopwords;
  PrepStopwords(&b_stopwords, "dictionary_b.txt");

  ofstream test_parsed_file(working_dir + "\\test_parsed.txt", ios_base::out);

	int item_no = SizeOfTotalSet;

	while (getline(raw_data, ln_buffer)) {
		++item_no;
		test_parsed_file << item_no << endl;
		if (item_no % 1000000 == 0)
			cout << item_no << "-th item processed.\n";

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

		string contents;

		contents = item_container.substr(marker_pos[0], marker_pos[1] - marker_pos[0] - 3);
		contents = SiftOff(&contents, &t_stopwords);
		test_parsed_file << contents << endl;

		contents = item_container.substr(marker_pos[1], item_container.size() - 1 - marker_pos[1]);
		contents = SiftOff(&contents, &b_stopwords);
		test_parsed_file << contents << endl;
	}

	raw_data.close();
	test_parsed_file.close();
}


void TagMining::PrepSVMTestData() {
	ifstream test_parsed_file(working_dir + "\\test_parsed.txt", ios_base::in);
	ofstream test_item_vector_file(working_dir + "\\test_item_vectors.txt", ios_base::out);
	string ln_buffer;
	int item_no = SizeOfTotalSet;

	if (test_parsed_file.is_open())
  	while (getline(test_parsed_file, ln_buffer)) {
	  	++item_no;
	  	if (item_no % 1000000 == 0)
	  	  cout << item_no << "-th item doc-vector processed.\n";

      map<string, double> doc_vec;
      getline(test_parsed_file, ln_buffer);
      AddTitleWordsToDocVec(ln_buffer, &doc_vec);
      getline(test_parsed_file, ln_buffer);
      AddBodyWordsToDocVec(ln_buffer, &doc_vec);

      string doc_vec_str;
      for (map<string, double>::iterator it = doc_vec.begin(); it != doc_vec.end(); ++it)
        if (df_map.count(it->first))
          doc_vec_str += " " + to_string(df_map[it->first].first) + ':'
                         + to_string(it->second * log((double)SizeOfTotalSet / df_map[it->first].second));

      test_item_vector_file << item_no << doc_vec_str << endl;
	  }
  else
    cout << "parsed data file open error.\n";

	test_parsed_file.close();
	test_item_vector_file.close();
}


void TagMining::BuildThresholdsMap() {
  for (int tag_id = 1; tag_id <= 42048; ++tag_id) {
    map<double, int> pos, neg;
    ifstream pred_file(working_dir + "\\prediction\\" + to_string(tag_id) + "_p.txt", ios_base::in);
    string num_str;
    int entry_ct = 0, label;
    double dist;

    while (pred_file >> num_str) {
      if (num_str == "Global")
        break;

      ++entry_ct;
      label = (int)stod(num_str);
      pred_file >> num_str;
      dist = stod(num_str);
      if (label == 1)
        pos.insert(make_pair(dist, entry_ct));
      else
        neg.insert(make_pair(dist, entry_ct));
    }

    pred_file.close();

    entry_ct = 0;
    for (map<double, int>::iterator it = pos.begin(); it != pos.end(); ++it) {
      ++entry_ct;
      it->second = entry_ct;
    }

    entry_ct = 0;
    for (map<double, int>::iterator it = neg.begin(); it != neg.end(); ++it) {
      ++entry_ct;
      it->second = entry_ct;
    }

    double pos_start = pos.begin()->first, neg_end = (--(neg.end()))->first;
    if (pos_start >= neg_end) {
      thresholds_map.insert(make_pair(tag_id, (pos_start + neg_end) / 2));
      continue;
    }

    map<double, int>::iterator neg_lower_end_it = neg.begin(), pos_upper_end_it = pos.end();
    for (map<double, int>::iterator it = neg.begin(); it != neg.end(); ++it)
      if (it->first >= pos_start) {
        neg_lower_end_it = it;
        break;
      }
    for (map<double, int>::iterator it = pos.begin(); it != pos.end(); ++it)
      if (it->first > neg_end) {
        pos_upper_end_it = it;
        break;
      }

    set<double> threshold_candidates;
    for (map<double, int>::iterator it = neg_lower_end_it; it != neg.end(); ++it)
      threshold_candidates.insert(it->first);
    for (map<double, int>::iterator it = pos.begin(); it != pos_upper_end_it; ++it)
      threshold_candidates.insert(it->first);

    double f_max = 0, tp = 0, fp = 0, threshold;
    int pos_size = pos.size(), neg_size = neg.size();

    map<double, int>::iterator pos_it = pos.begin(), neg_it = neg_lower_end_it;
    for (set<double>::iterator set_it = threshold_candidates.begin();
         set_it != threshold_candidates.end(); ++set_it) {
      for (map<double, int>::iterator map_it = pos_it; map_it != pos_upper_end_it; ++map_it)
        if (map_it->first >= *set_it) {
          tp = pos_size - map_it->second + 1;
          pos_it = map_it;
          break;
        }
      for (map<double, int>::iterator map_it = neg_it; map_it != neg.end(); ++map_it)
        if (map_it->first >= *set_it) {
          fp = neg_size - map_it->second + 1;
          neg_it = map_it;
          break;
        }
      double f_cur = 2 * tp / (tp + fp + pos_size);

      if (f_cur > f_max) {
        f_max = f_cur;
        threshold = *set_it;
      }
    }

    thresholds_map.insert(make_pair(tag_id, threshold));
  }

  ofstream thresholds_map_file(working_dir + "\\thresholds_map.txt", ios_base::out);

  double min_th = 100;
  for (map<int, double>::iterator it = thresholds_map.begin(); it != thresholds_map.end(); ++it) {
    thresholds_map_file << it->first << " " << it->second << endl;
    if (it->second < min_th)
      min_th = it->second;
  }
  thresholds_map_file << "min : " << min_th;

  thresholds_map_file.close();
}


void TagMining::BuildThresholdsMap_2() {
  ofstream thresholds_map_file(working_dir + "\\thresholds_map.txt", ios_base::out);
  double dist, neg_end;
  int label;
  string num_str;

  for (int tag_id = 1; tag_id <= 42048; ++tag_id) {
    set<double> neg;
    ifstream pred_file(working_dir + "\\prediction\\" + to_string(tag_id) + "_p.txt", ios_base::in);

    while (pred_file >> num_str) {
      if (num_str == "Global")
        break;

      label = (int)stod(num_str);
      pred_file >> num_str;
      dist = stod(num_str);
      if (label == -1)
        neg.insert(dist);
    }

    pred_file.close();

    neg_end = (*(--(neg.end())) > 0 ? *(--(neg.end())) : 0);

    thresholds_map.insert(make_pair(tag_id, neg_end));

    thresholds_map_file << tag_id << " " << neg_end << endl;
  }

  thresholds_map_file.close();
}



void TagMining::DetermineThreshold(int tag_id) {
  map<double, int> pos, neg;
  ifstream pred_file(working_dir + "\\prediction\\" + to_string(tag_id) + "_p.txt", ios_base::in);
  string num_str;
  int entry_ct = 0, label;
  double dist;

  while (pred_file >> num_str) {
    if (num_str == "Global")
      break;

    ++entry_ct;
    label = (int)stod(num_str);
    pred_file >> num_str;
    dist = stod(num_str);
    if (label == 1)
      pos.insert(make_pair(dist, entry_ct));
    else
      neg.insert(make_pair(dist, entry_ct));
  }

  pred_file.close();

  entry_ct = 0;
  for (map<double, int>::iterator it = pos.begin(); it != pos.end(); ++it) {
    ++entry_ct;
    it->second = entry_ct;
  }

  entry_ct = 0;
  for (map<double, int>::iterator it = neg.begin(); it != neg.end(); ++it) {
    ++entry_ct;
    it->second = entry_ct;
  }

  double pos_start = pos.begin()->first, neg_end = (--(neg.end()))->first;
  if (pos_start >= neg_end) {
    cout << 1.0 << "  " << (pos_start + neg_end) / 2 << endl;
    return;
  }

  map<double, int>::iterator neg_lower_end_it = neg.begin(), pos_upper_end_it = pos.end();
  for (map<double, int>::iterator it = neg.begin(); it != neg.end(); ++it)
    if (it->first >= pos_start) {
      neg_lower_end_it = it;
      break;
    }
  for (map<double, int>::iterator it = pos.begin(); it != pos.end(); ++it)
    if (it->first > neg_end) {
      pos_upper_end_it = it;
      break;
    }

  set<double> threshold_candidates;
  for (map<double, int>::iterator it = neg_lower_end_it; it != neg.end(); ++it)
    threshold_candidates.insert(it->first);
  for (map<double, int>::iterator it = pos.begin(); it != pos_upper_end_it; ++it)
    threshold_candidates.insert(it->first);

  double f_max = 0, tp = 0, fp = 0, threshold;
  int pos_size = pos.size(), neg_size = neg.size();

  map<double, int>::iterator pos_it = pos.begin(), neg_it = neg_lower_end_it;
  for (set<double>::iterator set_it = threshold_candidates.begin();
       set_it != threshold_candidates.end(); ++set_it) {
    for (map<double, int>::iterator map_it = pos_it; map_it != pos_upper_end_it; ++map_it)
      if (map_it->first >= *set_it) {
        tp = pos_size - map_it->second + 1;
        pos_it = map_it;
        break;
      }
    for (map<double, int>::iterator map_it = neg_it; map_it != neg.end(); ++map_it)
      if (map_it->first >= *set_it) {
        fp = neg_size - map_it->second + 1;
        neg_it = map_it;
        break;
      }
    double f_cur = 2 * tp / (tp + fp + pos_size);

    if (f_cur > f_max) {
      f_max = f_cur;
      threshold = *set_it;
    }
  }

  cout << tag_id << "  " << f_max << "  " << threshold << endl;
}


void TagMining::LoadThresholdsMap() {
  ifstream thresholds_map_file(working_dir + "\\thresholds_map.txt" ,ios_base::in);
  string buffer;
  int tag_id;
  double threshold;

  while (thresholds_map_file >> buffer) {
    if (buffer == "min")
      break;

    tag_id = stoi(buffer);
    thresholds_map_file >> buffer;
    threshold = stod(buffer);
    thresholds_map.insert(make_pair(tag_id, threshold));
  }

  thresholds_map_file.close();
}


void TagMining::BuildAnswerMap(int start, int how_many_per_run, int how_many_runs) {
  ifstream tag_set_file(working_dir + "\\tag_set.txt", ios_base::in);
  string buffer, tag;
  int tag_id;
  map<int, string> tag_set_rev;

  while (tag_set_file >> buffer) {
    tag = buffer;
    tag_set_file >> buffer;
    tag_id = stoi(buffer);

    tag_set_rev.insert(make_pair(tag_id, tag));
  }

  tag_set_file.close();

  int item_id;
  double dist;

  for (int run = 0; run < how_many_runs; ++run) {
    cout << run << endl;

    ifstream answer_file(working_dir + "\\result\\" + to_string(start) + "_r.txt", ios_base::in);
    string ln_buffer, word_buffer;

    while (getline(answer_file, ln_buffer)) {
      stringstream ln_sstream(ln_buffer);
      ln_sstream >> word_buffer;
      item_id = stoi(word_buffer);

      if (answer_map.count(item_id))
        while (ln_sstream >> word_buffer) {
          tag_id = stoi(word_buffer);
          ln_sstream >> word_buffer;
          dist = stod(word_buffer);

          if (dist > thresholds_map[tag_id])
            answer_map[item_id].insert(make_pair(dist, tag_set_rev[tag_id]));
        }
      else {
        map<double, string> answer_tags;

        while (ln_sstream >> word_buffer) {
          tag_id = stoi(word_buffer);
          ln_sstream >> word_buffer;
          dist = stod(word_buffer);

          if (dist > thresholds_map[tag_id])
            answer_tags.insert(make_pair(dist, tag_set_rev[tag_id]));
        }

        answer_map.insert(make_pair(item_id, answer_tags));
      }
    }

    answer_file.close();

    start += how_many_per_run;
  }
}


void TagMining::WriteSubmitFile() {
  ofstream submit_file(working_dir + "\\submit_Park.csv", ios_base::out);
  submit_file << "\"Id\",\"Tags\"\n";

  for (map< int, map<double, string> >::iterator it = answer_map.begin(); it != answer_map.end(); ++it) {
    submit_file << it->first << ",\"";

    if (!((it->second).empty())) {
      map<double, string>::iterator last_it = --((it->second).end());
      if ((it->second).size() > 3) {
        submit_file << last_it->second << ' ';
        last_it = --last_it;
        submit_file << last_it->second << ' ';
        last_it = --last_it;
        submit_file << last_it->second;
      }
      else if ((it->second).size() == 2) {
        submit_file << last_it->second << ' ';
        last_it = --last_it;
        submit_file << last_it->second;
      }
      else if ((it->second).size() == 1) {
        submit_file << last_it->second;
      }
    }

    submit_file << '\"' << endl;
  }

  submit_file.close();
}







