#ifndef TAGMINING_H
#define TAGMINING_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <map>

#define IDFCUT 5
#define SizeOfTrainingSet 6034195
#define SizeOfTotalSet 8047532

using namespace std;

class Item{
public:
	Item();
	~Item();

	int item_no;
	string title;
	string body;
	set<string> tags;
};


class TagMining {
 public:
	TagMining();
	~TagMining();

  void PrepStopwords(const string& dic_filename);
  void BlockRemoval(string* contents, string modifier1, string modifier2);
  void ModifierRemoval(string* contents, string modifier);
  void CodeRemoval(string* raw_body);
  string SiftOff(string* contents);
  void InsertToDfMap(Item* item, char where);
  void BuildTrainingDfMap(const string& in_filename, string out_file_dir);
  void BuildTestDfMap(const string& in_filename, string out_file_dir);
  void FinalizeTagSetAndDfMaps(string out_file_dir);

  int GetTagSetSize();
  int GetTitleMapSize();
  int GetBodyMapSize();

  void LoadTagSet(const string& in_file_dir);
  void LoadDfMaps(const string& in_file_dir);
  void ProcessTags(int item_no, string* data_ln_buffer);
  int ProcessText(int item_no, string* data_ln_buffer, char where);
  void BuildInvertedTagMapAndTrainItemVectors(const string& parsed_data_file_dir, string out_file_dir);

  void LoadInvertedTagMap(const string& in_file_dir);
  void LoadItemVectors(const string& in_filename);
  void FillNegExs(ofstream& out_file, int tag_id, int size_of_learning_ex, char t_or_v);
  void PrepBCSVMLearningData(string out_file_dir, int size_of_learning_ex);
  void PrepBCSVMValData(string out_file_dir, int size_of_validation_set);

  void PrepSVMTestData(const string& parsed_data_file_dir, string out_file_dir);

  void BuildAnswerMap(const string& answer_file_dir, int start, int how_many_per_run, int how_many_runs);
  void WriteAnswerMapToFile(string out_file_dir);
  void LoadAnswerMap(const string& in_file_dir);
  void BuildThresholdMap(const string& val_set_filename, const string& in_dir, string out_filename);
  double LoadThresholdMap(int how_many_val_sets);
  void WriteSubmitFile(double avg_thrshd, string submit_file_dir);

 private:
  set<string> stopwords;
  map<string, int> tag_set;
  map< string, pair<int, int> > title_df_map;
  map< string, pair<int, int> > body_df_map;
  map< int, set<int> > inverted_tag_map;
  map<int, string> item_vector_map;
  map< int, set< pair<double, int> > > answer_map;
  map<int, double> threshold_map;

};


#endif // TAGMINING_H
