#ifndef TAGMINING_H
#define TAGMINING_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <map>

#define DFCUT 3
#define SizeOfTrainingSet 6034195
#define SizeOfTotalSet 8047532
#define no_of_tags 42048
#define no_of_words 300157
#define size_of_learning_set 5000000
#define l_bfgs 7

using namespace std;


class TagMining {
 public:
	TagMining(string working_dir_name);

  void LoadMaps();
  void TrainForEachTag(int tag_id, double lambda);
  void ValidateForEachTag(int tag_id);
  void Validate();
  void Test(int start, int fin);
  void WriteAnswerMap(int start);

  void BuildAnswerMap(int start);
  void WriteSubmitFile();

 private:
  string working_dir;

  map<int, string> tag_set;
  map<string, int> df_map;
  map< int, set<int> > inverted_tag_map;

  map<int, double> beta;
  set< map<int, double> > p_doc_vec, n_doc_vec;
  int no_of_docs;

  void ReadInDataFile(ifstream& doc_vec_file);
  double CalcLogLikelihoodAndGrad(char p_or_n, map<int, double>* grad);
  double CalcLogLikelihood(map<int, double>& moving_coordis, char p_or_n);
  bool LineSearch(map<int, double>& grad, map<int, double>* start, double* m_log_likelihood_per_doc, double learn_rate);
  void TrainForEachTag_line_search(int tag_id, double lambda);
  void TrainForEachTag_simple_grad_descent(int tag_id, double lambda);

  map<int, double> thresholds_map;
  map< int, vector< pair<string, double> > > answer_map;


};



#endif // TAGMINING_H
