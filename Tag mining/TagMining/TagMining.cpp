#include "TagMining.h"
#include <math.h>
#include <time.h>
#include <vector>
#include <algorithm>

#define MAX(x, y) ((x) < (y) ? (y) : (x))
#define MIN(x, y) ((x) >= (y) ? (y) : (x))

Item::Item() {
}
Item::~Item() {
}



TagMining::TagMining() {
}
TagMining::~TagMining() {
}



void TagMining::PrepStopwords(const string& dic_filename) {
	ifstream dictionary(dic_filename, ios_base::in);
	string stopword_buffer;
	while (dictionary >> stopword_buffer)
		stopwords.insert(stopword_buffer);
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

void TagMining::CodeRemoval(string* raw_body) {
  BlockRemoval(raw_body, "<pre>", "</pre>");
  BlockRemoval(raw_body, "<pre", "</pre>");
  BlockRemoval(raw_body, "<code>", "</code>");
	BlockRemoval(raw_body, "<a ", ">");
	BlockRemoval(raw_body, "<img", ">");

  ModifierRemoval(raw_body, "</a>");
  ModifierRemoval(raw_body, "<b>");
  ModifierRemoval(raw_body, "</b>");
  ModifierRemoval(raw_body, "<blockquote>");
  ModifierRemoval(raw_body, "</blockquote>");
  ModifierRemoval(raw_body, "<br>");
  ModifierRemoval(raw_body, "</br>");
  ModifierRemoval(raw_body, "<br/>");
  ModifierRemoval(raw_body, "<code>");
  ModifierRemoval(raw_body, "</code>");
  ModifierRemoval(raw_body, "<cr>");
  ModifierRemoval(raw_body, "<del>");
  ModifierRemoval(raw_body, "</del>");
  ModifierRemoval(raw_body, "<em>");
  ModifierRemoval(raw_body, "</em>");
  ModifierRemoval(raw_body, "<esc>");
  ModifierRemoval(raw_body, "<h1>");
  ModifierRemoval(raw_body, "</h1>");
  ModifierRemoval(raw_body, "<h2>");
  ModifierRemoval(raw_body, "</h2>");
  ModifierRemoval(raw_body, "<h3>");
  ModifierRemoval(raw_body, "</h3>");
  ModifierRemoval(raw_body, "<hr>");
  ModifierRemoval(raw_body, "<i>");
  ModifierRemoval(raw_body, "</i>");
  ModifierRemoval(raw_body, "<kbd>");
  ModifierRemoval(raw_body, "</kbd>");
  ModifierRemoval(raw_body, "<li>");
  ModifierRemoval(raw_body, "</li>");
  ModifierRemoval(raw_body, "<ol>");
  ModifierRemoval(raw_body, "</ol>");
  ModifierRemoval(raw_body, "<p>");
  ModifierRemoval(raw_body, "</p>");
  ModifierRemoval(raw_body, "<span>");
  ModifierRemoval(raw_body, "</span>");
  ModifierRemoval(raw_body, "<strike>");
  ModifierRemoval(raw_body, "</strike>");
  ModifierRemoval(raw_body, "<strong>");
  ModifierRemoval(raw_body, "</strong>");
  ModifierRemoval(raw_body, "<sub>");
  ModifierRemoval(raw_body, "</sub>");
  ModifierRemoval(raw_body, "<sup>");
  ModifierRemoval(raw_body, "</sup>");
  ModifierRemoval(raw_body, "<ul>");
  ModifierRemoval(raw_body, "</ul>");
  ModifierRemoval(raw_body, ".............................................................");
  ModifierRemoval(raw_body, "...........................................................");
  ModifierRemoval(raw_body, ".....................................................");
  ModifierRemoval(raw_body, "...............................................");
  ModifierRemoval(raw_body, "...........................................");
  ModifierRemoval(raw_body, ".........................................");
  ModifierRemoval(raw_body, ".....................................");
  ModifierRemoval(raw_body, "...............................");
  ModifierRemoval(raw_body, ".............................");
  ModifierRemoval(raw_body, ".......................");
  ModifierRemoval(raw_body, "...................");
  ModifierRemoval(raw_body, ".................");
  ModifierRemoval(raw_body, ".............");
  ModifierRemoval(raw_body, "...........");
  ModifierRemoval(raw_body, ".......");
  ModifierRemoval(raw_body, ".....");
  ModifierRemoval(raw_body, "...");
  ModifierRemoval(raw_body, "..");
}

string TagMining::SiftOff(string* contents) {
	for (unsigned i = 0; i < contents->size(); ++i)
	if ((*contents)[i] >= 'A' && (*contents)[i] <= 'Z')
		(*contents)[i] += 32;

	stringstream contents_stream(*contents);
	string raw, output;
	bool common;
	while (contents_stream >> raw) {
    if (raw.find("http://") != string:: npos || raw.find("https://") != string:: npos || raw.find("horse") != string:: npos
        || raw.find("regard") != string:: npos || raw.find("cheer") != string:: npos)
      continue;

		if (raw.back() == '.')
			raw.erase(raw.size() - 1, 1);

		size_t x;
		while ((x = raw.find('\"')) != string::npos)
			raw.erase(x, 1);
		while ((x = raw.find('`')) != string::npos)
			raw.erase(x, 1);
		while ((x = raw.find('\?')) != string::npos)
			raw.erase(x, 1);
		while ((x = raw.find('!')) != string::npos)
			raw.erase(x, 1);
		while ((x = raw.find('\'')) != string::npos)
			raw.erase(x, 1);
		while ((x = raw.find('(')) != string::npos)
			raw.replace(x, 1, " ");
		while ((x = raw.find(')')) != string::npos)
			raw.replace(x, 1, " ");
		while ((x = raw.find('{')) != string::npos)
			raw.replace(x, 1, " ");
		while ((x = raw.find('}')) != string::npos)
			raw.replace(x, 1, " ");
		while ((x = raw.find(',')) != string::npos)
			raw.replace(x, 1, " ");
		while ((x = raw.find(';')) != string::npos)
			raw.replace(x, 1, " ");
		while ((x = raw.find(':')) != string::npos)
			raw.replace(x, 1, " ");

    common = false;
		if (stopwords.find(raw) != stopwords.end())
			common = true;
		if (common == false)
			output += raw + " ";
	}

	return output;
}

void TagMining::InsertToDfMap(Item* item, char where) {
	stringstream temp_sstream;
	set<string> words_wo_duplicity;
	string word;

	if (where == 't') {
		temp_sstream << item->title;
		while (temp_sstream >> word) {
		  if ((word[0] == '.') || (word[0] >= '0' && word[0] <= '9') || (word[0] >= 'a' && word[0] <= 'z'))
		    words_wo_duplicity.insert(word);
    }
    for (set<string>::iterator it = words_wo_duplicity.begin(); it != words_wo_duplicity.end(); ++it) {
      if (title_df_map.count(*it))
        ++((title_df_map.find(*it)->second).second);
      else
        title_df_map.insert(make_pair(*it, make_pair(0, 1)));
		}
	}
	else {
    temp_sstream << item->body;
    while (temp_sstream >> word) {
		  if (word[0] >= '%' && word[0] <= 'z')
		    words_wo_duplicity.insert(word);
    }
    for (set<string>::iterator it = words_wo_duplicity.begin(); it != words_wo_duplicity.end(); ++it) {
      if (body_df_map.count(*it))
        ++((body_df_map.find(*it)->second).second);
      else
        body_df_map.insert(make_pair(*it, make_pair(0, 1)));
    }
	}
}

void TagMining::BuildTrainingDfMap(const string& in_filename, string out_file_dir) {
	ifstream raw_data(in_filename, ios_base::in);
	string ln_buffer, item_container;
	size_t marker_pos[4];

	ofstream out(out_file_dir + "\\train_parsed.txt", ios_base::out);

	getline(raw_data, ln_buffer);

	int prog = 0;
	while (getline(raw_data, ln_buffer)) {
		++prog;
		if (prog % 100000 == 0)
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
		CodeRemoval(&temp_item.title);
		temp_item.title = SiftOff(&temp_item.title);

		temp_item.body = item_container.substr(marker_pos[1], marker_pos[2] - marker_pos[1] - 3);
		CodeRemoval(&temp_item.body);
		temp_item.body = SiftOff(&temp_item.body);

		stringstream tag_sstream(item_container.substr(marker_pos[2], marker_pos[3] - marker_pos[2] + 1));
		string indv_tags;
		while (tag_sstream >> indv_tags) {
			temp_item.tags.insert(indv_tags);
			if(!tag_set.count(indv_tags))
			  tag_set.insert(make_pair(indv_tags, 0));
		}

		out << temp_item.item_no << endl;
		for (set<string>::iterator it = temp_item.tags.begin(); it != temp_item.tags.end(); ++it)
			out << *it << " ";
		out << endl;
		out << temp_item.title << endl;
		out << temp_item.body << endl;

		InsertToDfMap(&temp_item, 't');
		InsertToDfMap(&temp_item, 'b');
	}

	raw_data.close();
	out.close();
}

void TagMining::BuildTestDfMap(const string& in_filename, string out_file_dir) {
	fstream raw_data(in_filename, ios_base::in);
	string ln_buffer, item_container;
	size_t marker_pos[3];

	ofstream out(out_file_dir + "\\test_parsed.txt", ios_base::out);

	getline(raw_data, ln_buffer);

	int prog = SizeOfTrainingSet;
	while (getline(raw_data, ln_buffer)) {
		++prog;
		if (prog % 100000 == 0)
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

		InsertToDfMap(&temp_item, 't');
		InsertToDfMap(&temp_item, 'b');
	}

	raw_data.close();
	out.close();
}

void TagMining::FinalizeTagSetAndDfMaps(string out_file_dir) {
  int i = 0;

  for (map<string, int>::iterator it = tag_set.begin(); it != tag_set.end(); ++it) {
    ++i;
    it->second = i;
  }
	ofstream tag_set_file(out_file_dir + "\\tags.txt", ios_base::out);
	for (map<string, int>::iterator it = tag_set.begin(); it != tag_set.end(); ++it)
		tag_set_file << it->first << '\t' << it->second << endl;
	tag_set_file.close();

  i = 0;
  for (map< string, pair<int, int> >::iterator it = title_df_map.begin(); it != title_df_map.end(); ) {
    if ((it->second).second <= IDFCUT)
      it = title_df_map.erase(it);
    else {
      ++i;
      (it->second).first = i;
      ++it;
    }
  }
  ofstream title_df_map_file(out_file_dir + "\\title_df_map.txt", ios_base::out);
  for (map< string, pair<int, int> >::iterator it = title_df_map.begin(); it != title_df_map.end(); ++it)
    title_df_map_file << it->first << '\t' << (it->second).first << '\t' << (it->second).second << endl;
  title_df_map_file.close();

  for (map< string, pair<int, int> >::iterator it = body_df_map.begin(); it != body_df_map.end(); ) {
    if ((it->second).second <= IDFCUT)
      it = body_df_map.erase(it);
    else {
      ++i;
      (it->second).first = i;
      ++it;
    }
  }
  ofstream body_df_map_file(out_file_dir + "\\body_df_map.txt", ios_base::out);
  for (map< string, pair<int, int> >::iterator it = body_df_map.begin(); it != body_df_map.end(); ++it)
    body_df_map_file << it->first << '\t' << (it->second).first << '\t' << (it->second).second << endl;
  body_df_map_file.close();
}



int TagMining::GetTagSetSize() {
	return tag_set.size();
}

int TagMining::GetTitleMapSize() {
	return title_df_map.size();
}

int TagMining::GetBodyMapSize() {
	return body_df_map.size();
}



void TagMining::LoadTagSet(const string& in_file_dir) {
  string word, id;

  ifstream tag_set_file(in_file_dir + "\\tags.txt", ios_base::in);
  while (tag_set_file >> word) {
    tag_set_file >> id;
    tag_set.insert(make_pair(word, stoi(id)));
  }
  tag_set_file.close();
}

void TagMining::LoadDfMaps(const string& in_file_dir) {
  string word, id, df;

  ifstream title_df_map_file(in_file_dir + "\\title_df_map.txt", ios_base::in);
  while (title_df_map_file >> word) {
    title_df_map_file >> id;
    title_df_map_file >> df;
    title_df_map.insert(make_pair(word, make_pair(stoi(id), stoi(df))));
  }
  title_df_map_file.close();

  ifstream body_df_map_file(in_file_dir + "\\body_df_map.txt", ios_base::in);
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

int TagMining::ProcessText(int item_no, string* data_ln_buffer, char where) {
  if(*data_ln_buffer == "")
    return -1;

  stringstream contents(*data_ln_buffer);
  string word;
  map<string, int> tf_map;

  while (contents >> word) {
    if (tf_map.count(word))
      ++(tf_map[word]);
    else
      tf_map.insert(make_pair(word, 1));
  }

  map< string, pair<int, int> >::iterator df_map_it;
  string doc_vec;
  if (where == 't') {
    for (map<string, int>::iterator it = tf_map.begin(); it != tf_map.end(); ++it) {
      df_map_it = title_df_map.find(it->first);
      if (df_map_it != title_df_map.end()) {
        int id = (df_map_it->second).first;
        int df = (df_map_it->second).second;
        doc_vec += " " + to_string(id) + ':' + to_string(it->second * log((double)SizeOfTotalSet / (double)df));
      }
    }
    item_vector_map.insert(make_pair(item_no, doc_vec));
  }
  else {
    for (map<string, int>::iterator it = tf_map.begin(); it != tf_map.end(); ++it) {
      df_map_it = body_df_map.find(it->first);
      if (df_map_it != body_df_map.end()) {
        int id = (df_map_it->second).first;
        int df = (df_map_it->second).second;
        doc_vec += " " + to_string(id) + ':' + to_string(it->second * log((double)SizeOfTotalSet / (double)df));
      }
    }
    if (item_vector_map.count(item_no))
      item_vector_map[item_no] += doc_vec;
    else
      item_vector_map.insert(make_pair(item_no, doc_vec));
  }

  return 0;
}

void TagMining::BuildInvertedTagMapAndTrainItemVectors(const string& parsed_data_file_dir, string out_file_dir) {
  ifstream parsed_data(parsed_data_file_dir + "\\train_parsed.txt", ios_base::in);
  string data_ln_buffer;
  item_vector_map.clear();

  int item_count = 0, title_flag, body_flag;
  while (getline(parsed_data, data_ln_buffer)) {
    ++item_count;
    if (item_count % 100000 == 0)
      cout << item_count << "-th item processed.\n";

    getline(parsed_data, data_ln_buffer);
    ProcessTags(item_count, &data_ln_buffer);

    getline(parsed_data, data_ln_buffer);
    title_flag = ProcessText(item_count, &data_ln_buffer, 't');

    getline(parsed_data, data_ln_buffer);
    body_flag = ProcessText(item_count, &data_ln_buffer, 'b');

    if (title_flag + body_flag == -2)
      item_vector_map.insert(make_pair(item_count, ""));
  }

  parsed_data.close();

  ofstream inverted_tag_map_file(out_file_dir + "\\inverted_tag_map.txt", ios_base::out);
  for (map< int, set<int> >::iterator it = inverted_tag_map.begin(); it != inverted_tag_map.end(); ++it) {
    inverted_tag_map_file << it->first << " :";
    for (set<int>::iterator set_it = (it->second).begin(); set_it != (it->second).end(); ++set_it)
      inverted_tag_map_file << " " << *set_it;
    inverted_tag_map_file << endl;
  }
  inverted_tag_map_file.close();

  ofstream train_item_vector_file(out_file_dir + "\\train_item_vectors.txt", ios_base::out);
  for (map<int, string>::iterator it = item_vector_map.begin(); it != item_vector_map.end(); ++it) {
    train_item_vector_file << it->first << " " << it->second << endl;
  }
  train_item_vector_file.close();
}

void TagMining::LoadInvertedTagMap(const string& in_file_dir) {
  string ln_buffer, buffer;
  int id;

  ifstream inverted_tag_map_file(in_file_dir + "\\inverted_tag_map.txt", ios_base::in);
  while (getline(inverted_tag_map_file, ln_buffer)) {
    stringstream ln_sstream(ln_buffer);
    ln_sstream >> buffer;
    id = stoi(buffer);

    ln_sstream >> buffer;

    set<int> item_no_set;
    while (ln_sstream >> buffer)
      item_no_set.insert(stoi(buffer));

    inverted_tag_map.insert(make_pair(id, item_no_set));
  }
  inverted_tag_map_file.close();
}

void TagMining::LoadItemVectors(const string& in_filename) {
  string ln_buffer, buffer;
  int id;

  ifstream item_vectors_file(in_filename, ios_base::in);
  while (getline(item_vectors_file, ln_buffer)) {
    stringstream ln_sstream(ln_buffer);
    ln_sstream >> buffer;
    id = stoi(buffer);
    string item_vector_buffer;
    while (ln_sstream >> buffer)
      item_vector_buffer += " " + buffer;
    item_vector_map.insert(make_pair(id, item_vector_buffer));
  }
  item_vectors_file.close();
}

void TagMining::FillNegExs(ofstream& out_file, int tag_id, int size_of_learning_ex, char t_or_v) {
  set<int> neg_ex_set;
  int breaker = 0;

  srand(time(NULL));
  while (1) {
    ++breaker;
    int r1 = rand(), r2 = rand();
    int item_id_candidate = (r1 * (RAND_MAX + 1) + r2) % SizeOfTrainingSet + 1;
    if (t_or_v == 't') {
      if (item_id_candidate <= size_of_learning_ex) {
        if (inverted_tag_map[tag_id].count(item_id_candidate))
          continue;
        neg_ex_set.insert(item_id_candidate);
        if (neg_ex_set.size() > MAX(10000, 5 * (unsigned)(inverted_tag_map[tag_id].size())))
          break;
      }
    }
    else {
      if (item_id_candidate > size_of_learning_ex) {
        if (inverted_tag_map[tag_id].count(item_id_candidate))
          continue;
        neg_ex_set.insert(item_id_candidate);
        if (neg_ex_set.size() > MAX(1000, 2 * (unsigned)(inverted_tag_map[tag_id].size())) || breaker > 10000)
          break;
      }
    }
  }

  for(set<int>::iterator it = neg_ex_set.begin(); it != neg_ex_set.end(); ++it)
    out_file << -1 << item_vector_map[*it] << endl;
}

void TagMining::PrepBCSVMLearningData(string out_file_dir, int size_of_learning_ex) {
  for (map< int, set<int> >::iterator it = inverted_tag_map.begin(); it != inverted_tag_map.end(); ++it) {
    if (it->first % 5000 == 0)
      cout << it->first << "-th tag is processed.\n";

    ofstream out(out_file_dir + "\\" + to_string(it->first) + ".svm", ios_base::out);

    for (set<int>::iterator set_it = (it->second).begin(); set_it != (it->second).end(); ++set_it)
      if (*set_it <= size_of_learning_ex)
        out << 1 << item_vector_map[*set_it] << endl;
    FillNegExs(out, it->first, size_of_learning_ex, 't');

    out.close();
  }
}

void TagMining::PrepBCSVMValData(string out_file_dir, int size_of_validation_set) {
  set<int> item_no_for_val_set;

  srand(time(NULL));
  while (1) {
    int r1 = rand(), r2 = rand();
    int item_id_candidate = (r1 * (RAND_MAX + 1) + r2) % SizeOfTrainingSet + 1;

    item_no_for_val_set.insert(item_id_candidate);
    if (item_no_for_val_set.size() % 50000 == 0)
      cout << "piling...  ";
    if (item_no_for_val_set.size() == size_of_validation_set)
      break;
  }

  ofstream out(out_file_dir + "\\" + "val.svm", ios_base::out);
  for (set<int>::iterator it = item_no_for_val_set.begin(); it != item_no_for_val_set.end(); ++it)
    out << *it << item_vector_map[*it] << endl;
  out.close();
}



void TagMining::PrepSVMTestData(const string& parsed_data_file_dir, string out_file_dir) {
  ifstream parsed_data(parsed_data_file_dir + "\\test_parsed.txt", ios_base::in);
  string data_ln_buffer;
  item_vector_map.clear();

  int item_no = SizeOfTrainingSet, title_flag, body_flag;
  while (getline(parsed_data, data_ln_buffer)) {
    ++item_no;
    if (item_no % 100000 == 0)
      cout << item_no << "-th item processed.\n";

    getline(parsed_data, data_ln_buffer);
    title_flag = ProcessText(item_no, &data_ln_buffer, 't');

    getline(parsed_data, data_ln_buffer);
    body_flag = ProcessText(item_no, &data_ln_buffer, 'b');

    if (title_flag + body_flag == -2)
      item_vector_map.insert(make_pair(item_no, ""));
  }

  parsed_data.close();

  ofstream test_item_vector_file(out_file_dir + "\\test_item_vectors.txt", ios_base::out);
  for (map<int, string>::iterator it = item_vector_map.begin(); it != item_vector_map.end(); ++it) {
    test_item_vector_file << it->first << " " << it->second << endl;
  }
  test_item_vector_file.close();
}



void TagMining::BuildAnswerMap(const string& answer_file_dir, int start, int how_many_per_run, int how_many_runs) {
  int item_id, tag_id;
  double dist;

  for (int run = 0; run < how_many_runs; ++run) {
    ifstream answer_file(answer_file_dir + "\\p_" + to_string(start) + ".txt", ios_base::in);
    string ln_buffer, word_buffer;

    while (getline(answer_file, ln_buffer)) {
      stringstream ln_sstream(ln_buffer);
      ln_sstream >> word_buffer;
      item_id = stoi(word_buffer);

      if (answer_map.count(item_id))
        while (ln_sstream >> word_buffer) {
          dist = stod(word_buffer);

          ln_sstream >> word_buffer;
          tag_id = stoi(word_buffer);

          (answer_map.find(item_id)->second).insert(make_pair(dist, tag_id));
        }
      else {
        set< pair<double, int> > tags_for_item;

        while (ln_sstream >> word_buffer) {
          dist = stod(word_buffer);

          ln_sstream >> word_buffer;
          tag_id = stoi(word_buffer);

          tags_for_item.insert(make_pair(dist, tag_id));
        }

        answer_map.insert(make_pair(item_id, tags_for_item));
      }
    }

    answer_file.close();

    start += how_many_per_run;
  }
}

void TagMining::WriteAnswerMapToFile(string out_file_dir) {
  ofstream out(out_file_dir + "\\answer_map.txt", ios_base::out);

  for (map< int, set< pair<double, int> > >::iterator it = answer_map.begin(); it != answer_map.end(); ++it) {
    out << it->first << '\t';
    for (set< pair<double, int> >::iterator set_it = (it->second).begin(); set_it != (it->second).end(); ++set_it)
      out << "  " << set_it->first << "  " << set_it->second << "  ";
    out << endl;
  }

  out.close();
}



void TagMining::LoadAnswerMap(const string& in_file_dir) {
  int item_id, tag_id;
  double dist;

  ifstream answer_map_file(in_file_dir + "\\answer_map.txt", ios_base::in);
  string ln_buffer, word_buffer;

  while (getline(answer_map_file, ln_buffer)) {

    stringstream ln_sstream(ln_buffer);
    ln_sstream >> word_buffer;
    item_id = stoi(word_buffer);

    set< pair<double, int> > tags_for_item;

    while (ln_sstream >> word_buffer) {
      dist = stod(word_buffer);

      ln_sstream >> word_buffer;
      tag_id = stoi(word_buffer);

      tags_for_item.insert(make_pair(dist, tag_id));
    }

    answer_map.insert(make_pair(item_id, tags_for_item));
  }

  answer_map_file.close();
}

/*void TagMining::BuildThresholdMap(const string& in_dir) {
  for (int tag_id = 1; tag_id <= 42048; ++tag_id) {
    double threshold;
    ifstream val_file(in_dir + "\\" + to_string(tag_id) + "_p.txt", ios_base::in);
    stringstream val_file_ss;
    val_file_ss << val_file.rdbuf();

    string val_file_str(val_file_ss.str());
    size_t pos = val_file_str.rfind("**") + 3;
    int positives = stoi(val_file_str.substr(pos, val_file_str.size() - pos));

    if (positives > 0) {
      string word_buffer;
      set<double> pos_dist_values;

      int count = 0;
      double dist;
      while (val_file_ss >> word_buffer) {
        dist = stod(word_buffer);
        if (dist > 0) {
          ++count;
          pos_dist_values.insert(dist);
          if (count == positives)
            break;
        }
      }

      int expected_appearance_no = 250000 * (inverted_tag_map[tag_id].size() / (double)SizeOfTrainingSet);

      if (expected_appearance_no <= positives) {
        set<double>::iterator it = pos_dist_values.end();
        for (int i = 0; i < expected_appearance_no; ++i)
          --it;
        threshold = *it;
      }
      else
        threshold = 0;
    }
    else
      threshold = 0;

    threshold_map.insert(make_pair(tag_id, threshold));
    val_file.close();
  }

  ofstream out("C:\\Users\\Minjoon\\Documents\\threshold_map.txt", ios_base::out);
  for (map<int, double>::iterator it = threshold_map.begin(); it != threshold_map.end(); ++it)
    out << it->first << '\t' << it->second << endl;
  out.close();
}
*/

void TagMining::BuildThresholdMap(const string& val_set_filename, const string& in_dir, string out_filename) {
  ifstream val_set_file(val_set_filename, ios_base::in);
  string ln_buffer, item_id_buffer;
  set<int> val_item_list;
  while (getline(val_set_file, ln_buffer)) {
    stringstream ln_sstream(ln_buffer);
    ln_sstream >> item_id_buffer;
    val_item_list.insert(stoi(item_id_buffer));
  }
  val_set_file.close();

  map<int, int> tag_appearance_map;
  for (map< int, set<int> >::iterator it = inverted_tag_map.begin(); it != inverted_tag_map.end(); ++it) {
    vector<int> common;
    set_intersection((it->second).begin(), (it->second).end(),
                     val_item_list.begin(), val_item_list.end(), back_inserter(common));

    tag_appearance_map.insert(make_pair(it->first, common.size()));
  }

  for (int tag_id = 1; tag_id <= 42048; ++tag_id) {
    if(tag_id % 3000 == 0)
      cout << tag_id << '\t';

    double threshold;
    ifstream val_file(in_dir + "\\" + to_string(tag_id) + "_p.txt", ios_base::in);
    stringstream val_file_ss;
    val_file_ss << val_file.rdbuf();

    string val_file_str(val_file_ss.str());
    size_t pos = val_file_str.rfind("**") + 3;
    int positives = stoi(val_file_str.substr(pos, val_file_str.size() - pos));

    if (positives > 0) {
      string word_buffer;
      set<double> pos_dist_values;

      int count = 0;
      double dist;
      while (val_file_ss >> word_buffer) {
        dist = stod(word_buffer);
        if (dist > 0) {
          ++count;
          pos_dist_values.insert(dist);
          if (count == positives)
            break;
        }
      }

      int expected_appearance_no = tag_appearance_map[tag_id];

      if (expected_appearance_no <= positives) {
        set<double>::iterator it = pos_dist_values.end();
        for (int i = 0; i <= expected_appearance_no; ++i)
          --it;
        threshold = *it;
      }
      else
        threshold = 0;
    }
    else
      threshold = 0;

    threshold_map.insert(make_pair(tag_id, threshold));
    val_file.close();
  }

  ofstream out(out_filename, ios_base::out);
  for (map<int, double>::iterator it = threshold_map.begin(); it != threshold_map.end(); ++it)
    out << it->first << '\t' << it->second << endl;
  out.close();
}

double TagMining::LoadThresholdMap(int how_many_val_sets) {
  for (int i = 1; i <= how_many_val_sets; ++i) {
    ifstream threshold_map_file("C:\\Users\\Minjoon\\Documents\\threshold_map-" + to_string(i) + ".txt", ios_base::in);
    string word_buffer;
    int tag_id;
    double threshold;

    while (threshold_map_file >> word_buffer) {
      tag_id = stoi(word_buffer);

      threshold_map_file >> word_buffer;
      if (word_buffer.find("e-") != string::npos)
        threshold = 0;
      else
        threshold = stod(word_buffer);

      if (threshold_map.count(tag_id))
        threshold_map.find(tag_id)->second += threshold;
      else
        threshold_map.insert(make_pair(tag_id, threshold));
    }

    threshold_map_file.close();
  }

  double avg_thrshd = 0;
  for (map<int, double>::iterator it = threshold_map.begin(); it != threshold_map.end(); ++it) {
    it->second /= how_many_val_sets;
    avg_thrshd += it->second;
  }
  avg_thrshd /= 42048;

  cout << avg_thrshd << endl;

  ofstream out("C:\\Users\\Minjoon\\Documents\\threshold_map_avg.txt", ios_base::out);
  for (map<int, double>::iterator it = threshold_map.begin(); it != threshold_map.end(); ++it)
    out << it->first << '\t' << it->second << endl;
  out.close();

  return avg_thrshd;
}


/*void TagMining::WriteSubmitFile(string submit_file_dir) {
  ofstream out(submit_file_dir + "\\check-2.csv", ios_base::out);

  for (map<string, int>::iterator it = tag_set.begin(); it != tag_set.end(); ++it)
    out << it->first << '\t' << inverted_tag_map[it->second].size() << endl;

  out.close();
}
*/

/*void TagMining::WriteSubmitFile(string submit_file_dir) {
  map<int, string> reversed_tag_set;
  string tag;
  int tag_id;
  for (map<string, int>::iterator it = tag_set.begin(); it != tag_set.end(); ++it) {
    tag = it->first;
    tag_id = it->second;
    reversed_tag_set.insert(make_pair(tag_id, tag));
  }

  ofstream out(submit_file_dir + "\\check.csv", ios_base::out);
  out << "\"Id\",\"Tags\"\n";

  for (map< int, set< pair<double, int> > >::iterator it = answer_map.begin(); it != answer_map.end(); ++it) {
    out << it->first << '\t';

    if (!(it->second).empty()) {
      set< pair<double, int> > dist_from_threshold;
      for (set< pair<double, int> >::iterator set_it = (it->second).begin(); set_it != (it->second).end(); ++set_it)
        dist_from_threshold.insert(make_pair(set_it->first - threshold_map[set_it->second], set_it->second));

      for (set< pair<double, int> >::iterator dft_it = dist_from_threshold.begin();
           dft_it != dist_from_threshold.end(); ++dft_it)
        out << reversed_tag_set[dft_it->second] << " : " << dft_it->first << "  ";
    }

    out << '\"' << endl;
  }

  out.close();
}
*/



void TagMining::WriteSubmitFile(double avg_thrshd, string submit_file_dir) {
  map<int, string> reversed_tag_set;
  string tag;
  int tag_id;
  for (map<string, int>::iterator it = tag_set.begin(); it != tag_set.end(); ++it) {
    tag = it->first;
    tag_id = it->second;
    reversed_tag_set.insert(make_pair(tag_id, tag));
  }

  ofstream out(submit_file_dir + "\\submit_Park.csv", ios_base::out);
  out << "\"Id\",\"Tags\"\n";

  for (map< int, set< pair<double, int> > >::iterator it = answer_map.begin(); it != answer_map.end(); ++it) {
    out << it->first << ",\"";

    if (!(it->second).empty()) {
      set< pair<double, int> > dist_from_threshold;
      for (set< pair<double, int> >::iterator set_it = (it->second).begin(); set_it != (it->second).end(); ++set_it)
        dist_from_threshold.insert(make_pair((set_it->first) - threshold_map[set_it->second], set_it->second));

      set< pair<double, int> >::iterator dft_it = dist_from_threshold.end();
      --dft_it;

      if (dft_it->first <= 0) {
        out << reversed_tag_set[dft_it->second] << " ";
        --dft_it;
        out << reversed_tag_set[dft_it->second];
      }
      else {
        int how_many_pos = 0;
        while (dft_it->first > 0 && how_many_pos < dist_from_threshold.size()) {
          ++how_many_pos;
          --dft_it;
        }

        dft_it = dist_from_threshold.end();
        --dft_it;
        for (int i = 0; i < MIN(4, how_many_pos - 1); ++i, --dft_it)
          out << reversed_tag_set[dft_it->second] << " ";
        out << reversed_tag_set[dft_it->second];
      }
    }

    out << '\"' << endl;
  }

  out.close();
}

/*
void TagMining::WriteSubmitFile(double avg_thrshd, string submit_file_dir) {
  map<int, string> reversed_tag_set;
  string tag;
  int tag_id;
  for (map<string, int>::iterator it = tag_set.begin(); it != tag_set.end(); ++it) {
    tag = it->first;
    tag_id = it->second;
    reversed_tag_set.insert(make_pair(tag_id, tag));
  }

  ofstream out(submit_file_dir + "\\submit_Park.csv", ios_base::out);
  out << "\"Id\",\"Tags\"\n";

  for (map< int, set< pair<double, int> > >::iterator it = answer_map.begin(); it != answer_map.end(); ++it) {
    out << it->first << ",\"";

    vector<string> temp_tag_holder;
    for (set< pair<double, int> >::iterator set_it = (it->second).begin(); set_it != (it->second).end(); ++set_it)
      if ((set_it->first) + avg_thrshd * tanh(.5 * log10(1000.0 / (inverted_tag_map[set_it->second].size()))) > threshold_map[set_it->second])
        temp_tag_holder.push_back(reversed_tag_set[set_it->second]);
    int how_many_tags = temp_tag_holder.size();

    if (how_many_tags > 0) {
      for (int i = 0; i < how_many_tags - 1; ++i)
        out << temp_tag_holder[i] << " ";
      out << temp_tag_holder[how_many_tags - 1];
    }

    out << '\"' << endl;
  }

  out.close();
}
*/


/*

void TagMining::WriteSubmitFile(string submit_file_dir) {
  map<int, string> reversed_tag_set;
  string tag;
  int tag_id;
  for (map<string, int>::iterator it = tag_set.begin(); it != tag_set.end(); ++it) {
    tag = it->first;
    tag_id = it->second;
    reversed_tag_set.insert(make_pair(tag_id, tag));
  }

  ofstream out(submit_file_dir + "\\submit_Park.csv", ios_base::out);
  out << "\"Id\",\"Tags\"\n";

  for (map< int, set< pair<double, int> > >::iterator it = answer_map.begin(); it != answer_map.end(); ++it) {
    if (it->first % 100000 == 0)
      cout << it->first << '\t';

    out << it->first << ",\"";

    vector<string> temp_tag_holder;
    for (set< pair<double, int> >::iterator set_it = (it->second).begin(); set_it != (it->second).end(); ++set_it)
      if (set_it->first > cutoff)
        temp_tag_holder.push_back(reversed_tag_set[set_it->second]);
    int how_many_tags = temp_tag_holder.size();

    if (how_many_tags > 0) {
      for (int i = 0; i < how_many_tags - 1; ++i)
        out << temp_tag_holder[i] << " ";
      out << temp_tag_holder[how_many_tags - 1];
    }


    if ((it->second).size() <= 3) {
      for (set< pair<double, int> >::iterator set_it = (it->second).begin(); set_it != (it->second).end(); ++set_it) {
        out << reversed_tag_set[set_it->second];
        if (set_it != --((it->second).end()))
          out << " ";
      }
    }
    else {
      set< pair<double, int> >::iterator set_it = (it->second).end();
      --set_it;
      for (int i = 0; i < 2; ++i) {
        out << reversed_tag_set[set_it->second] << " ";
        --set_it;
      }
      out << reversed_tag_set[set_it->second];
    }

    out << '\"' << endl;
  }

  out.close();
}









void TagMining::BuildAnswerMap(const string& answer_file_dir, int start, int how_many_per_run, int how_many_runs) {
  int item_id, tag_id;
  double dist;

  for (int run = 0; run < how_many_runs; ++run) {
    ifstream answer_file(answer_file_dir + "\\p_" + to_string(start) + ".txt", ios_base::in);
    string ln_buffer, word_buffer;

    while (getline(answer_file, ln_buffer)) {
      stringstream ln_sstream(ln_buffer);
      ln_sstream >> word_buffer;
      item_id = stoi(word_buffer);

      if (answer_map.count(item_id))
        while (ln_sstream >> word_buffer) {
          dist = stod(word_buffer);

          ln_sstream >> word_buffer;
          tag_id = stoi(word_buffer);

          (answer_map.find(item_id)->second).insert(make_pair(dist * log(SizeOfTrainingSet / inverted_tag_map[tag_id].size()),
                                                              tag_id));
        }
      else {
        set< pair<double, int> > tags_for_item;

        while (ln_sstream >> word_buffer) {
          dist = stod(word_buffer);

          ln_sstream >> word_buffer;
          tag_id = stoi(word_buffer);

          tags_for_item.insert(make_pair(dist * log(SizeOfTrainingSet / inverted_tag_map[tag_id].size()), tag_id));
        }

        answer_map.insert(make_pair(item_id, tags_for_item));
      }
    }

    answer_file.close();

    start += how_many_per_run;
  }
}











void TagMining::PrepBCSVMValData(string out_file_dir, int size_of_learning_ex) {
  for (map< int, set<int> >::iterator it = inverted_tag_map.begin(); it != inverted_tag_map.end(); ++it) {
    if (it->first % 5000 == 0)
      cout << it->first << "-th tag is processed.\n";

    ofstream out(out_file_dir + "\\" + to_string(it->first) + "_val.svm", ios_base::out);

    for (set<int>::iterator set_it = (it->second).begin(); set_it != (it->second).end(); ++set_it)
      if (*set_it > size_of_learning_ex)
        out << 1 << item_vector_map[*set_it] << endl;
    FillNegExs(out, it->first, size_of_learning_ex, 'v');

    out.close();
  }
}









void TagMining::PrepBCSVMLearningDataWithCutOff(string out_file_dir, int size_of_learning_ex) {
  for (map< int, set<int> >::iterator it = inverted_tag_map.begin(); it != inverted_tag_map.end(); ++it) {
    if (it->first % 5000 == 0)
      cout << it->first << "-th tag is processed.\n";

    ofstream out;

    if ((it->second).size() < 50)
      out.open(out_file_dir + "\\no_learn\\" + to_string(it->first) + ".svm", ios_base::out);
    else
      out.open(out_file_dir + "\\" + to_string(it->first) + ".svm", ios_base::out);

    for (set<int>::iterator set_it = (it->second).begin(); set_it != (it->second).end(); ++set_it)
      if (*set_it <= size_of_learning_ex)
        out << 1 << item_vector_map[*set_it] << endl;
    FillNegExs(out, it->first, size_of_learning_ex, 't');

    out.close();
  }
}











void TagMining::PrepBCSVMValData(string out_file_dir, int SizeOfTrainingSet) {
  for (map< int, set<int> >::iterator it = inverted_tag_map.begin(); it != inverted_tag_map.end(); ++it) {
    ofstream out(out_file_dir + "\\" + to_string(it->first) + "_val.svm", ios_base::out);

    for (int i = SizeOfTrainingSet + 1; i <= SizeOfTrainingSet; ++i) {
      if ((it->second).count(i))
        out << 1 << text_map[i] << endl;
      else
        out << -1 << text_map[i] << endl;
    }

    out.close();
  }
}


// prep a single file for multi-label SVM
void TagMining::WriteTagIdsToFile(string* data_ln_buffer, ofstream& svm_feed_file) {
  stringstream local_tags(*data_ln_buffer);
  string tag_buffer;
  set<int> local_tag_id_set;
  while (local_tags >> tag_buffer)
    local_tag_id_set.insert(tag_set.find(tag_buffer)->second);
  set<int>::iterator it_fin = --local_tag_id_set.end();
  for(set<int>::iterator it = local_tag_id_set.begin(); it != it_fin; ++it)
    svm_feed_file << *it << ',';
  svm_feed_file << *it_fin << " ";
}

void TagMining::WriteItemVectorsToFile(string* data_ln_buffer, char where, ofstream& svm_feed_file) {
  if(*data_ln_buffer == "")
    return;

  stringstream contents(*data_ln_buffer);
  string word;
  map<string, int> tf_map;

  while (contents >> word) {
    if (tf_map.count(word))
      ++(tf_map.find(word)->second);
    else
      tf_map.insert(make_pair(word, 1));
  }

  if (where == 't') {
    map< string, pair<int, int> >::iterator title_map_it;
    for (map<string, int>::iterator it = tf_map.begin(); it != tf_map.end(); ++it) {
      title_map_it = title_df_map.find(it->first);
      if (title_map_it != title_df_map.end()) {
        int id = (title_map_it->second).first;
        int df = (title_map_it->second).second;
        svm_feed_file << " " << id << ':' << it->second * log(SizeOfTotalSet / df);
      }
    }
  }
  else {
    map< string, pair<int, double> >::iterator body_map_it;
    for (map<string, int>::iterator it = tf_map.begin(); it != tf_map.end(); ++it) {
      body_map_it = body_idf_map.find(it->first);
      if (body_map_it != body_idf_map.end()) {
        int id = (body_map_it->second).first;
        double idf = (body_map_it->second).second;
        svm_feed_file << " " << id << ':' << it->second * idf;
      }
    }
  }
}





void TagMining::PrepMLSVMLearningData(const string& parsed_data_filename, int SizeOfTrainingSet, string out_file_dir) {
  ifstream parsed_data(parsed_data_filename, ios_base::in);
  ofstream training_file(out_file_dir + "\\training.svm", ios_base::out);
  string data_ln_buffer;

  int item_count = 0;
  while (getline(parsed_data, data_ln_buffer)) {
    ++item_count;
    if (item_count % 500000 == 0)
      cout << item_count << "-th item processed.\n";
    if (item_count > SizeOfTrainingSet)
      break;

    getline(parsed_data, data_ln_buffer);
    WriteTagIdsToFile(&data_ln_buffer, training_file);

    getline(parsed_data, data_ln_buffer);
    WriteItemVectorsToFile(&data_ln_buffer, 't', training_file);

    getline(parsed_data, data_ln_buffer);
    WriteItemVectorsToFile(&data_ln_buffer, 'b', training_file);

    training_file << endl;
  }
  training_file.close();


  if (SizeOfTrainingSet < SizeOfTrainingSet) {
    ofstream valid_Q_file(out_file_dir + "\\valid_Q.svm", ios_base::out);
    ofstream valid_A_file(out_file_dir + "\\valid_A.svm", ios_base::out);

    while (getline(parsed_data, data_ln_buffer)) {
      ++item_count;
      if (item_count % 100000 == 0)
        cout << item_count << "-th item processed.\n";

      getline(parsed_data, data_ln_buffer);
      WriteTagIdsToFile(&data_ln_buffer, valid_A_file);
      valid_A_file << endl;

      getline(parsed_data, data_ln_buffer);
      WriteItemVectorsToFile(&data_ln_buffer, 't', valid_Q_file);
      getline(parsed_data, data_ln_buffer);
      WriteItemVectorsToFile(&data_ln_buffer, 'b', valid_Q_file);
      valid_Q_file << endl;
    }
    valid_Q_file.close();
    valid_A_file.close();
  }

  parsed_data.close();
}




void TagMining::PrepFileForIndvTagFull(const string& in_filename, int which_tag, string out_file_dir) {
  ifstream training_file(in_filename, ios_base::in);
  string item_buffer;
  ofstream out_file_for_which_tag(out_file_dir + "\\" + to_string(which_tag) + ".svm", ios_base::out);

  while (getline(training_file, item_buffer)) {
    int loc = 0, tag_id = 0, is_there_this_tag = -1;

    for (char c : item_buffer) {
      ++loc;
      if (c == ' ')
        break;
      if (c != ',')
        tag_id = 10 * tag_id + c - '0';
      else {
        if (tag_id == which_tag)
          is_there_this_tag = 1;
        tag_id = 0;
      }
    }

    item_buffer.erase(0, loc);
    out_file_for_which_tag << is_there_this_tag << item_buffer << endl;
  }

  training_file.close();
  out_file_for_which_tag.close();
}

void TagMining::PrepFileForIndvTag(const string& in_filename, int which_tag, string out_file_dir) {
  ifstream training_file(in_filename, ios_base::in);
  string item_buffer;

  string tag_id = to_string(which_tag);
  ofstream out_file_for_which_tag(out_file_dir + "\\" + tag_id + ".svm", ios_base::out);

  while(getline(training_file, item_buffer)) {
    if (item_buffer.find(tag_id + ",") != string::npos || item_buffer.find(tag_id + " ") != string::npos) {
      size_t loc_separator = item_buffer.find(" ");
      out_file_for_which_tag << 1 << item_buffer.substr(loc_separator, item_buffer.size() - loc_separator) << endl;
    }
  }

  training_file.close();
  out_file_for_which_tag.close();
}


void TagMining::PrepFileForIndvTag(const string& in_filename, int which_tag, string out_file_dir) {
  ifstream training_file(in_filename, ios_base::in);
  stringstream file_buffer_ss;
  file_buffer_ss << training_file.rdbuf();
  string file_buffer_str(file_buffer_ss.str());

  string tag_id = to_string(which_tag);
  ofstream out_file_for_which_tag(out_file_dir + "\\" + tag_id + ".svm", ios_base::out);

  size_t loc_mid = file_buffer_str.find(tag_id + ","), loc_end = file_buffer_str.find(tag_id + " ");
  while (loc_mid != string::npos) {
    size_t loc_separator = file_buffer_str.find(" ", loc_mid), loc_nl = file_buffer_str.find('\n', loc_mid);
    out_file_for_which_tag << 1 << file_buffer_str.substr(loc_separator, loc_nl - loc_separator + 1);
    loc_mid = file_buffer_str.find(tag_id + ",", loc_nl);
  }
  while (loc_end != string::npos) {
    size_t loc_separator = loc_end + tag_id.size(), loc_nl = file_buffer_str.find('\n', loc_end);
    out_file_for_which_tag << 1 << file_buffer_str.substr(loc_separator, loc_nl - loc_separator + 1);
    loc_end = file_buffer_str.find(tag_id + " ", loc_nl);
  }

  training_file.close();
  out_file_for_which_tag.close();
}



WordAttributes::WordAttributes() {
}
WordAttributes::~WordAttributes() {
}

void WordAttributes::AddEntry(int item_no) {
	if (items_with_given_word_map.count(item_no))
		++(items_with_given_word_map.find(item_no)->second);
	else
		items_with_given_word_map.insert(make_pair(item_no, 1));
}


int WordAttributes::Get_tf(int item_no) {
	return items_with_given_word_map.find(item_no)->second;
}


int WordAttributes::Get_idf() {
	return items_with_given_word_map.size();
}


map<int, int> WordAttributes::GetMemberMap() {
	return items_with_given_word_map;
}






void TagMining::BuildWordMap(Item item, char where) {
	stringstream temp_sstream;
	string word;
	if (where == 't') {
		temp_sstream << item.title;
		while (temp_sstream >> word) {
			if (word_map_title.count(word))
				(word_map_title.find(word)->second).AddEntry(item.item_no);
			else {
				WordAttributes temp_word_attributes;
				temp_word_attributes.AddEntry(item.item_no);
				word_map_title.insert(make_pair(word, temp_word_attributes));
			}
		}
	}
	else {
		temp_sstream << item.body;
		while (temp_sstream >> word) {
			if (word_map_body.count(word))
				(word_map_body.find(word)->second).AddEntry(item.item_no);
			else {
				WordAttributes temp_word_attributes;
				temp_word_attributes.AddEntry(item.item_no);
				word_map_body.insert(make_pair(word, temp_word_attributes));
			}
		}
	}
}


void TagMining::BuildInvertedIndexTraining(const string& in_filename) {
	ifstream raw_data(in_filename, ios_base::in);
	string ln_buffer, item_container;
	size_t marker_pos[4];

	getline(raw_data, ln_buffer);

	int prog = 0;
	while (getline(raw_data, ln_buffer)) {
		++prog;

		if (prog % 50000 == 0)
			cout << prog << "-th item processed.\n";
	//	if(prog > 1000)
	//	  break;

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
		temp_item.title = SiftOff(item_container.substr(marker_pos[0], marker_pos[1] - marker_pos[0] - 3));
		temp_item.body = SiftOff(CodeRemovalFromBody(item_container.substr(marker_pos[1], marker_pos[2] - marker_pos[1] - 3)));

		stringstream tag_sstream(item_container.substr(marker_pos[2], marker_pos[3] - marker_pos[2] + 1));
		string indv_tags;
		while (tag_sstream >> indv_tags) {
			temp_item.tags.push_back(indv_tags);
			tag_set.insert(indv_tags);
		}

		BuildWordMap(temp_item, 't');
		BuildWordMap(temp_item, 'b');
	}

	raw_data.close();
}


void TagMining::BuildInvertedIndexTest(const string& in_filename) {
	fstream raw_data(in_filename, ios_base::in);
	string ln_buffer, item_container;
	size_t marker_pos[3];

	getline(raw_data, ln_buffer);

	int prog = 6034195;
	while (getline(raw_data, ln_buffer)) {
		++prog;

		if (prog % 50000 == 0)
			cout << prog << "-th item parsed.\n";
	//	if(prog > 6034195 + 150000)
	//	   break;

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
		temp_item.title = SiftOff(item_container.substr(marker_pos[0], marker_pos[1] - marker_pos[0] - 3));
		temp_item.body = SiftOff(CodeRemovalFromBody(item_container.substr(marker_pos[1], marker_pos[2] - marker_pos[1] + 1)));

		BuildWordMap(temp_item, 't');
		BuildWordMap(temp_item, 'b');
	}

	raw_data.close();
}


void TagMining::BuildInvertedIndexTrainingWithFile(const string& in_filename, string out_filename) {
	ifstream raw_data(in_filename, ios_base::in);
	string ln_buffer, item_container;
	size_t marker_pos[4];

	ofstream out(out_filename, ios_base::out);

	getline(raw_data, ln_buffer);

	int prog = 0;
	while (getline(raw_data, ln_buffer)) {
		++prog;

		if (prog % 50000 == 0)
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
		temp_item.title = SiftOff(item_container.substr(marker_pos[0], marker_pos[1] - marker_pos[0] - 3));
		temp_item.body = SiftOff(CodeRemovalFromBody(item_container.substr(marker_pos[1], marker_pos[2] - marker_pos[1] - 3)));

		stringstream tag_sstream(item_container.substr(marker_pos[2], marker_pos[3] - marker_pos[2] + 1));
		string indv_tags;
		while (tag_sstream >> indv_tags) {
			temp_item.tags.push_back(indv_tags);
			tag_set.insert(indv_tags);
		}

		out << "### " << temp_item.item_no << endl;
		out << "$$$ " << temp_item.title << endl;
		out << "%%% " << temp_item.body << endl;
		out << "&&& ";
		for (unsigned i = 0; i < temp_item.tags.size(); ++i)
			out << (temp_item.tags)[i] << " ";
		out << endl;

		BuildWordMap(temp_item, 't');
		BuildWordMap(temp_item, 'b');
	}

	raw_data.close();
	out.close();
}


void TagMining::BuildInvertedIndexTestWithFile(const string& in_filename, string out_filename) {
	fstream raw_data(in_filename, ios_base::in);
	string ln_buffer, item_container;
	size_t marker_pos[3];

	ofstream out(out_filename, ios_base::out);

	getline(raw_data, ln_buffer);

	int prog = 6034195;
	while (getline(raw_data, ln_buffer)) {
		++prog;

		if (prog % 50000 == 0)
			cout << prog << "-th item parsed.\n";
	//	if (prog > 6034195 + 10000)
	//		break;

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
		temp_item.title = SiftOff(item_container.substr(marker_pos[0], marker_pos[1] - marker_pos[0] - 3));
		temp_item.body = SiftOff(CodeRemovalFromBody(item_container.substr(marker_pos[1], marker_pos[2] - marker_pos[1] + 1)));

		out << "### " << temp_item.item_no << endl;
		out << "$$$ " << temp_item.title << endl;
		out << "%%% " << temp_item.body << endl;
		out << endl;

		BuildWordMap(temp_item, 't');
		BuildWordMap(temp_item, 'b');
	}

	raw_data.close();
	out.close();
}


int TagMining::GetTitleMapSize() {
	return word_map_title.size();
}


int TagMining::GetBodyMapSize() {
	return word_map_body.size();
}


int TagMining::GetTagSetSize() {
	return tag_set.size();
}



int TagMining::VectorSpaceSearch() {
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
cout << "and there is(are) " << best_matches.size() << " of them." << endl;

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








void TagMining::PrepInputFileForSVM(string parsed_items, string tags_filename) {
ifstream in_file(parsed_items, ios_base::in);
ifstream tags_file(tags_filename, ios_base::in);
string tag, item_contents;
int processed_item[FEATURESIZE + 1];

int count_tag = 0;
while(tags_file >> tag) {
++count_tag;
cout << count_tag << "-th tag is being processed.\n";

ofstream out_file("C:\\Users\\Minjoon\\Documents\\prepped_for_SVM\\"
+ to_string(count_tag) + ".txt", ios_base::out);
in_file.seekg(11, in_file.beg);

processed_item[1] = 0;
while(in_file >> item_contents) {
if(item_contents == tag)
processed_item[1] += 2;
else if(item_contents == "%%%") {
processed_item[2] = 0;
while(in_file >> item_contents) {
if(item_contents == tag)
processed_item[2] += 1;
else if(item_contents == "&&&") {
processed_item[0] = -1;
while(in_file >> item_contents) {
if(item_contents == tag) {
processed_item[0] = 1;
break;
}
else if(item_contents == "###")
break;
}
out_file << processed_item[0] << " ";
for(int i = 1; i < FEATURESIZE + 1; ++i)
out_file << i << ":" << processed_item[i] << " ";
out_file << endl;
while(in_file >> item_contents)
if(item_contents == "$$$")
break;
processed_item[1] = 0;
break;
}
}
}
}
out_file.close();
in_file.clear();
}
in_file.close();
tags_file.close();
}
*/











