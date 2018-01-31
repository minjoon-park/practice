#include "TagMining.h"
#include <math.h>
#include <algorithm>
#include <time.h>


TagMining::TagMining(string working_dir_name) {
  working_dir = working_dir_name;
}


void TagMining::LoadMaps() {
  string ln_buffer, word_buffer, word, tag;

  ifstream df_map_file(working_dir + "\\df_map.txt", ios_base::in);
  int id;
  while (df_map_file >> word_buffer) {
    word = word_buffer;

    df_map_file >> word_buffer;
    id = stoi(word_buffer);

    df_map.insert(make_pair(word, id));

    df_map_file >> word_buffer;
  }
  df_map_file.close();

  ifstream tag_set_file(working_dir + "\\tag_set.txt", ios_base::in);
  int tag_id;
  while (tag_set_file >> word_buffer) {
    tag = word_buffer;
    tag_set_file >> word_buffer;
    tag_id = stoi(word_buffer);

    tag_set.insert(make_pair(tag_id, tag));
  }
  tag_set_file.close();

  ifstream inverted_tag_map_file(working_dir + "\\inverted_tag_map.txt", ios_base::in);
  while (getline(inverted_tag_map_file, ln_buffer)) {
    stringstream ln_sstream(ln_buffer);

    ln_sstream >> word_buffer;
    id = stoi(word_buffer);

    set<int> item_set;
    while (ln_sstream >> word_buffer)
      item_set.insert(stoi(word_buffer));

    inverted_tag_map.insert(make_pair(id, item_set));
  }
  inverted_tag_map_file.close();
}


void TagMining::ReadInDataFile(ifstream& doc_vec_file) {
  string ln_buffer, word_buffer;
  int label, word_id;
  double tf;

  srand(time(NULL));
  while (getline(doc_vec_file, ln_buffer)) {
    stringstream doc_vec_ss(ln_buffer);
  	doc_vec_ss >> word_buffer;
    label = stoi(word_buffer);

    map<int, double> doc_vec;

    while (doc_vec_ss >> word_buffer) {
      int separator = word_buffer.find(':');

      word_id = stoi(word_buffer.substr(0, separator));
      tf = stod(word_buffer.substr(separator + 1, word_buffer.size() - separator - 1));

      doc_vec.insert(make_pair(word_id, tf));

      if (!beta.count(word_id))
        beta.insert(make_pair(word_id, 2.0 * rand() / RAND_MAX - 1));
    }

    if (label == 1)
      p_doc_vec.insert(doc_vec);
    else
      n_doc_vec.insert(doc_vec);
  }
}


double TagMining::CalcLogLikelihoodAndGrad(char p_or_n, map<int, double>* grad) {
  set< map<int, double> > doc_vec;

  if (p_or_n == '+')
    doc_vec = p_doc_vec;
  else
    doc_vec = n_doc_vec;

  double m_log_likelihood = 0;

  for (set< map<int, double> >::iterator dv_it = doc_vec.begin(); dv_it != doc_vec.end(); ++dv_it) {
    double probability = 0;

    for (map<int, double>::const_iterator it = dv_it->begin(); it != dv_it->end(); ++it)
      probability -= beta[it->first] * it->second;

    probability = (p_or_n == '+' ? 1 / (1 + exp(probability)) : 1 - 1 / (1 + exp(probability)));
    if (probability == 0)
      probability = 1.0e-15;

    m_log_likelihood -= log(probability);

    for (map<int, double>::const_iterator it = dv_it->begin(); it != dv_it->end(); ++it) {
      if (p_or_n == '+')
        grad->at(it->first) -= it->second * (1 - probability);
      else
        grad->at(it->first) += it->second * (1 - probability);
    }
  }

  return m_log_likelihood;
}


double TagMining::CalcLogLikelihood(map<int, double>& moving_coordis, char p_or_n) {
  set< map<int, double> > doc_vec;

  if (p_or_n == '+')
    doc_vec = p_doc_vec;
  else
    doc_vec = n_doc_vec;

  double m_log_likelihood = 0;

  for (set< map<int, double> >::iterator dv_it = doc_vec.begin(); dv_it != doc_vec.end(); ++dv_it) {
    double probability = 0;

    for (map<int, double>::const_iterator it = dv_it->begin(); it != dv_it->end(); ++it)
      probability -= moving_coordis[it->first] * it->second;

    probability = (p_or_n == '+' ? 1 / (1 + exp(probability)) : 1 - 1 / (1 + exp(probability)));
    if (probability == 0)
      probability = 1.0e-15;

    m_log_likelihood -= log(probability);
  }

  return m_log_likelihood;
}


bool TagMining::LineSearch(map<int, double>& grad, map<int, double>* start,
                            double* m_log_likelihood_per_doc, double learn_rate) {
  map<int, double> beta_second_to_last = *start, beta_last = *start, moving_coordis = *start;

  double ftn_val_second_to_last = *m_log_likelihood_per_doc, ftn_val_last = *m_log_likelihood_per_doc, ftn_val;

  for (int step = 1; 1; ++step) {
    for (map<int, double>::iterator it = moving_coordis.begin(); it != moving_coordis.end(); ++it)
      moving_coordis[it->first] -= learn_rate * grad[it->first];

    ftn_val = (CalcLogLikelihood(moving_coordis, '+')
                + CalcLogLikelihood(moving_coordis, '-')) / no_of_docs;

cout << "sub: step-" << step << "  " << ftn_val << endl;

    if (ftn_val < ftn_val_last) {
      ftn_val_second_to_last = ftn_val_last;
      ftn_val_last = ftn_val;
      beta_second_to_last = beta_last;
      beta_last = moving_coordis;
    }
    else {
      *m_log_likelihood_per_doc = ftn_val_second_to_last;
      *start = beta_second_to_last;
      beta = beta_last;

      if ((ftn_val - ftn_val_last) / ftn_val_last < 0.0001 && step > 1)
        return true;
      else
        return false;
    }
  }
}


void TagMining::TrainForEachTag_line_search(int tag_id, double lambda) {   // gradient descent
// find the min in the direction of -grad at a given point.
  for (int iteration_count = 0; iteration_count < 500 ; ++iteration_count) {
    cout << iteration_count << '\t';

    map<int, double> grad;
    for (map<int, double>::iterator it = beta.begin(); it != beta.end(); ++it)
      grad.insert(make_pair(it->first, 0));

    double m_log_likelihood_per_doc_cur
             = (CalcLogLikelihoodAndGrad('+', &grad) + CalcLogLikelihoodAndGrad('-', &grad)) / no_of_docs;

    cout << "-log(likelihood) per doc = " << m_log_likelihood_per_doc_cur << endl;

    if (m_log_likelihood_per_doc_cur < 0.0001)
      break;

    for (map<int, double>::iterator it = beta.begin(); it != beta.end(); ++it)
      grad[it->first] += lambda * beta[it->first];

    double grad_norm = 0;
    for (map<int, double>::iterator it = beta.begin(); it != beta.end(); ++it)
      grad_norm += grad[it->first] * grad[it->first];
    grad_norm = sqrt(grad_norm);

    for (map<int, double>::iterator it = beta.begin(); it != beta.end(); ++it)
      grad[it->first] /= grad_norm;

    map<int, double> start = beta;
    double learn_rate = 5;
    while (!LineSearch(grad, &start, &m_log_likelihood_per_doc_cur, learn_rate))
      learn_rate /= 3;
  }

  ofstream model_file(working_dir + "\\LR_model\\" + to_string(tag_id) + "_m.txt", ios_base::out);

  model_file << tag_id << endl;
  for (map<int, double>::iterator it = beta.begin(); it != beta.end(); ++it)
    model_file << it->first << ' ' << it->second << "  ";

  model_file.close();
}


void TagMining::TrainForEachTag_simple_grad_descent(int tag_id, double lambda) {   // gradient descent
// reduce learn_rate by 5%, if -log(likelihood) doesn't decrease.
// kick back up learn_rate by factor of 2, if -log(likelihood) decreases by less than 0.1%.
  double m_log_likelihood_per_doc = 10, learn_rate = no_of_docs;

  for (int iteration_count = 0; iteration_count < 1000 ; ++iteration_count) {
    cout << iteration_count << '\t';

    map<int, double> grad;
    for (map<int, double>::iterator it = beta.begin(); it != beta.end(); ++it)
      grad.insert(make_pair(it->first, 0));

    double m_log_likelihood_per_doc_cur
             = (CalcLogLikelihoodAndGrad('+', &grad) + CalcLogLikelihoodAndGrad('-', &grad)) / no_of_docs;

    for (map<int, double>::iterator it = beta.begin(); it != beta.end(); ++it)
      grad[it->first] += lambda * beta[it->first];

    cout << "-log(likelihood) per doc = " << m_log_likelihood_per_doc_cur << endl;

    if (m_log_likelihood_per_doc_cur < 0.0001 && iteration_count > 50)
      break;

    if (m_log_likelihood_per_doc <= m_log_likelihood_per_doc_cur)
      learn_rate *= 0.9;
    else if ((m_log_likelihood_per_doc - m_log_likelihood_per_doc_cur) / m_log_likelihood_per_doc < 0.001)
      learn_rate *= 2;

    m_log_likelihood_per_doc = m_log_likelihood_per_doc_cur;

    double grad_norm = 0;
    for (map<int, double>::iterator it = beta.begin(); it != beta.end(); ++it)
      grad_norm += grad[it->first] * grad[it->first];
    grad_norm = sqrt(grad_norm);

    for (map<int, double>::iterator it = beta.begin(); it != beta.end(); ++it)
      beta[it->first] -= learn_rate * grad[it->first] / grad_norm;
  }

  ofstream model_file(working_dir + "\\LR_model\\" + to_string(tag_id) + "_m.txt", ios_base::out);

  model_file << tag_id << endl;
  for (map<int, double>::iterator it = beta.begin(); it != beta.end(); ++it)
    model_file << it->first << ' ' << it->second << "  ";

  model_file.close();
}


void TagMining::TrainForEachTag(int tag_id, double lambda) {
  cout << tag_id << endl;

  beta.clear();
  p_doc_vec.clear();
  n_doc_vec.clear();

  ifstream doc_vec_file(working_dir + "\\training\\" + to_string(tag_id) + "_t.svm", ios_base::in);

  ReadInDataFile(doc_vec_file);

  doc_vec_file.close();

  no_of_docs = p_doc_vec.size() + n_doc_vec.size();

cout << "# of docs: " << no_of_docs << ", dim of beta: " << beta.size() << endl;

  //if (no_of_docs > 50000)
    //TrainForEachTag_line_search(tag_id, lambda);
  //else
    TrainForEachTag_simple_grad_descent(tag_id, lambda);
}


void TagMining::ValidateForEachTag(int tag_id) {
  ifstream model_file(working_dir + "\\LR_model\\" + to_string(tag_id) + "_m.txt", ios_base::in);
  ofstream threshold_file(working_dir + "\\thresholds_" + to_string(tag_id) + ".txt", ios_base::out);

  map<int, double> beta_cur;
  for (unsigned i = 1; i <= df_map.size(); ++i)
    beta_cur.insert(make_pair(i, 0));

  string ln_buffer, word_buffer;
  getline(model_file, ln_buffer);
  if (stoi(ln_buffer) != tag_id) {
    cout << "model file doesn't match.\n";
    return;
  }
  getline(model_file, ln_buffer);
  stringstream weight_vector_ss(ln_buffer);
  int word_id;
  double weight;
  while (weight_vector_ss >> word_buffer) {
    word_id = stoi(word_buffer);
    weight_vector_ss >> word_buffer;
    weight = stod(word_buffer);
    beta_cur[word_id] = weight;
  }

  ifstream doc_vec_file(working_dir + "\\validation\\" + to_string(tag_id) + "_t.svm", ios_base::in);

  int label;
  double tf;
  set< map<int, double> > p_doc_vec, n_doc_vec;

  while (getline(doc_vec_file, ln_buffer)) {
    stringstream doc_vec_ss(ln_buffer);
  	doc_vec_ss >> word_buffer;
    label = stoi(word_buffer);

    map<int, double> doc_vec;

    while (doc_vec_ss >> word_buffer) {
      int separator = word_buffer.find(':');

      word_id = stoi(word_buffer.substr(0, separator));
      tf = stod(word_buffer.substr(separator + 1, word_buffer.size() - separator - 1));

      doc_vec.insert(make_pair(word_id, tf));
    }

    if (label == 1)
      p_doc_vec.insert(doc_vec);
    else
      n_doc_vec.insert(doc_vec);
  }

  doc_vec_file.close();

  set<double> p_pred, n_pred;

  for (set< map<int, double> >::iterator pdv_it = p_doc_vec.begin(); pdv_it != p_doc_vec.end(); ++pdv_it) {
    double probability = 0;

    for (map<int, double>::const_iterator it = pdv_it->begin(); it != pdv_it->end(); ++it)
      probability -= beta_cur[it->first] * it->second;

    probability = 1 / (1 + exp(probability));

    p_pred.insert(probability);
  }

  for (set< map<int, double> >::iterator ndv_it = n_doc_vec.begin(); ndv_it != n_doc_vec.end(); ++ndv_it) {
    double probability = 0;

    for (map<int, double>::const_iterator it = ndv_it->begin(); it != ndv_it->end(); ++it)
      probability -= beta_cur[it->first] * it->second;

    probability = 1 / (1 + exp(probability));

    n_pred.insert(probability);
  }

  threshold_file << tag_id << ' ' << "total # of pos : " << p_doc_vec.size() << endl
                 << "pos_pred range: " << *(p_pred.begin()) << " ~ " << *(--p_pred.end()) << endl
                 << "neg_pred range: " << *(n_pred.begin()) << " ~ " << *(--n_pred.end()) << endl;

  model_file.close();
  threshold_file.close();
}


void TagMining::Validate() {
  ofstream threshold_file(working_dir + "\\thresholds.txt", ios_base::out);

  for (int tag_id = 1; tag_id <= 42048; ++tag_id) {
    if (tag_id % 1000 == 0)
      cout << tag_id << "  ";

    ifstream model_file(working_dir + "\\LR_model\\" + to_string(tag_id) + "_m.txt", ios_base::in);

    map<int, double> beta_cur;
    for (unsigned i = 1; i <= df_map.size(); ++i)
      beta_cur.insert(make_pair(i, 0));

    string ln_buffer, word_buffer;
    getline(model_file, ln_buffer);
    if (stoi(ln_buffer) != tag_id) {
      cout << "model file doesn't match.\n";
      return;
    }

    getline(model_file, ln_buffer);
    stringstream weight_vector_ss(ln_buffer);
    int word_id;
    double weight;
    while (weight_vector_ss >> word_buffer) {
      word_id = stoi(word_buffer);
      weight_vector_ss >> word_buffer;
      weight = stod(word_buffer);
      beta_cur[word_id] = weight;
    }

    model_file.close();

    ifstream doc_vec_file(working_dir + "\\validation\\" + to_string(tag_id) + "_v.svm", ios_base::in);

    int label;
    double tf;
    set< map<int, double> > p_doc_vec, n_doc_vec;

    while (getline(doc_vec_file, ln_buffer)) {
      stringstream doc_vec_ss(ln_buffer);
  	  doc_vec_ss >> word_buffer;
      label = stoi(word_buffer);

      map<int, double> doc_vec;

      while (doc_vec_ss >> word_buffer) {
        int separator = word_buffer.find(':');

        word_id = stoi(word_buffer.substr(0, separator));
        tf = stod(word_buffer.substr(separator + 1, word_buffer.size() - separator - 1));

        doc_vec.insert(make_pair(word_id, tf));
      }

      if (label == 1)
        p_doc_vec.insert(doc_vec);
      else
        n_doc_vec.insert(doc_vec);
    }

    doc_vec_file.close();

    map<double, int> p_pred, n_pred;
    set<double> threshold_candidates;

    int cnt = 0;
    for (set< map<int, double> >::iterator pdv_it = p_doc_vec.begin(); pdv_it != p_doc_vec.end(); ++pdv_it) {
      ++cnt;
      double probability = 0;

      for (map<int, double>::const_iterator it = pdv_it->begin(); it != pdv_it->end(); ++it)
        probability -= beta_cur[it->first] * it->second;

      probability = 1 / (1 + exp(probability));

      p_pred.insert(make_pair(probability, cnt));
      threshold_candidates.insert(probability);
    }

    cnt = 0;
    for (map<double, int>::iterator it = p_pred.begin(); it != p_pred.end(); ++it) {
      ++cnt;
      it->second = cnt;
    }

    cnt = 0;
    for (set< map<int, double> >::iterator ndv_it = n_doc_vec.begin(); ndv_it != n_doc_vec.end(); ++ndv_it) {
      ++cnt;
      double probability = 0;

      for (map<int, double>::const_iterator it = ndv_it->begin(); it != ndv_it->end(); ++it)
        probability -= beta_cur[it->first] * it->second;

      probability = 1 / (1 + exp(probability));

      n_pred.insert(make_pair(probability, cnt));
      threshold_candidates.insert(probability);
    }

    cnt = 0;
    for (map<double, int>::iterator it = n_pred.begin(); it != n_pred.end(); ++it) {
      ++cnt;
      it->second = cnt;
    }

    double f_max = 0, threshold = 0.5;
    int pos_size = p_pred.size(), neg_size = n_pred.size(), tp = 0, fp = 0, fn_opt = 0, fp_opt = 0;

    map<double, int>::iterator pos_it = p_pred.begin(), neg_it = n_pred.begin();

    for (set<double>::iterator set_it = threshold_candidates.begin();
         set_it != threshold_candidates.end(); ++set_it) {
      for (map<double, int>::iterator map_it = pos_it; map_it != p_pred.end(); ++map_it)
        if (map_it->first >= *set_it) {
          tp = pos_size - map_it->second + 1;
          pos_it = map_it;
          break;
        }
      for (map<double, int>::iterator map_it = neg_it; map_it != n_pred.end(); ++map_it)
        if (map_it->first >= *set_it) {
          fp = neg_size - map_it->second + 1;
          neg_it = map_it;
          break;
        }
      double f_cur = 2.0 * tp / (tp + fp + pos_size);

      if (f_cur > f_max) {
        f_max = f_cur;
        threshold = *set_it;
        fn_opt = pos_size - tp;
        fp_opt = fp;
      }
    }

    if (threshold == 0)
      threshold = 0.5;

    threshold_file << tag_id << ' ' << threshold << endl
                   << "F_max : " << f_max << ", total # of pos : " << pos_size
                   << ", fn : " << fn_opt << ", fp : " << fp_opt << endl;
  }

  threshold_file.close();
}


void TagMining::Test(int start, int fin) {
  string ln_buffer, word_buffer;
  int id;

  ifstream threshold_file(working_dir + "\\thresholds.txt", ios_base::in);
  double threshold;

  while (getline(threshold_file, ln_buffer)) {
    stringstream threshold_ss(ln_buffer);
    threshold_ss >> word_buffer;
    id = stoi(word_buffer);
    threshold_ss >> word_buffer;
    threshold = stod(word_buffer);

    thresholds_map.insert(make_pair(id, threshold));

    getline(threshold_file, ln_buffer);
  }

  threshold_file.close();

  ifstream test_item_vectors_file(working_dir + "\\test_item_vectors.txt", ios_base::in);

  while (!(test_item_vectors_file.eof())) {
    int item_no;
    double tf;
    map< int, map<int, double> > test_doc_vector_map;

    while (getline(test_item_vectors_file, ln_buffer)) {
      stringstream doc_vec_ss(ln_buffer);
  	  doc_vec_ss >> word_buffer;
      item_no = stoi(word_buffer);

      map<int, double> doc_vec;

      while (doc_vec_ss >> word_buffer) {
        int separator = word_buffer.find(':');

        id = stoi(word_buffer.substr(0, separator));
        tf = stod(word_buffer.substr(separator + 1, word_buffer.size() - separator - 1));

        doc_vec.insert(make_pair(id, tf));
      }

      test_doc_vector_map.insert(make_pair(item_no, doc_vec));

      if (item_no % 500000 == 0)
        break;
    }

    for (int tag_id = start; tag_id <= fin; ++tag_id) {
      if (tag_id % 1000 == 0)
        cout << tag_id << endl;

      string tag = tag_set[tag_id];
      threshold = thresholds_map[tag_id];

      ifstream model_file(working_dir + "\\LR_model\\" + to_string(tag_id) + "_m.txt", ios_base::in);

      map<int, double> beta_cur;
      for (unsigned i = 1; i <= df_map.size(); ++i)
        beta_cur.insert(make_pair(i, 0));

      getline(model_file, ln_buffer);
      if (stoi(ln_buffer) != tag_id) {
        cout << "model file doesn't match.\n";
        return;
      }

      getline(model_file, ln_buffer);
      stringstream weight_vector_ss(ln_buffer);
      double weight;
      while (weight_vector_ss >> word_buffer) {
        id = stoi(word_buffer);
        weight_vector_ss >> word_buffer;
        weight = stod(word_buffer);
        beta_cur[id] = weight;
      }

      model_file.close();

      for (map< int, map<int, double> >::iterator tdvm_it = test_doc_vector_map.begin();
           tdvm_it != test_doc_vector_map.end(); ++tdvm_it) {
        item_no = tdvm_it->first;
        double probability = 0;

        for (map<int, double>::iterator it = (tdvm_it->second).begin(); it != (tdvm_it->second).end(); ++it)
          probability -= beta_cur[it->first] * it->second;

        probability = 1 / (1 + exp(probability));

        if (probability > threshold) {
          if (answer_map.count(item_no))
            answer_map[item_no].push_back(make_pair(tag, probability));
          else {
            vector< pair<string, double> > temp_tag_set;
            temp_tag_set.push_back(make_pair(tag, probability));
            answer_map.insert(make_pair(item_no, temp_tag_set));
          }
        }
      }
    }
  }

  test_item_vectors_file.close();
}


void TagMining::WriteAnswerMap(int start) {
  ofstream full_answer_map_file(working_dir + "\\fam_" + to_string(start) + ".txt", ios_base::out);

  for (map< int, vector< pair<string, double> > >::iterator it = answer_map.begin(); it != answer_map.end(); ++it) {
    full_answer_map_file << it->first << "  ";

    if (!(it->second).empty())
      for (unsigned i = 0; i < (it->second).size(); ++i)
        full_answer_map_file << (it->second)[i].first << ' ' << (it->second)[i].second << ' ';

    full_answer_map_file << endl;
  }

  full_answer_map_file.close();
}


void TagMining::BuildAnswerMap(int start) {
  ifstream answer_map_file(working_dir + "\\fam_" + to_string(start) + ".txt", ios_base::in);
  string ln_buffer, word_buffer, tag;
  int item_no;
  double probability;;

  while(getline(answer_map_file, ln_buffer)) {
    stringstream answer_ss(ln_buffer);

    answer_ss >> word_buffer;
    item_no = stoi(word_buffer);

    if (answer_map.count(item_no))
      while (answer_ss >> word_buffer) {
        tag = word_buffer;
        answer_ss >> word_buffer;
        probability = stod(word_buffer);

        answer_map[item_no].push_back(make_pair(tag, probability));
      }
    else {
      vector< pair<string, double> > temp_tag_set;

      while (answer_ss >> word_buffer) {
        tag = word_buffer;
        answer_ss >> word_buffer;
        probability = stod(word_buffer);

        temp_tag_set.push_back(make_pair(tag, probability));
      }

      answer_map.insert(make_pair(item_no, temp_tag_set));
    }
  }

  answer_map_file.close();

  cout << start << endl;
}


bool SortByProbDecr(const pair<string, double> &lhs, const pair<string, double> &rhs) {
  return lhs.second > rhs.second;
}


void TagMining::WriteSubmitFile() {
  ofstream submit_file(working_dir + "\\submit_Park.csv", ios_base::out);
  submit_file << "\"Id\",\"Tags\"\n";

  int check = 6034196;

  for (map< int, vector< pair<string, double> > >::iterator am_it = answer_map.begin();
       am_it != answer_map.end(); ++am_it) {
    int item_no = am_it->first;

    if (item_no != check)
      for ( ; check < item_no; ++check)
        submit_file << check << ",\"\"\n";

    ++check;

    submit_file << item_no << ",\"";

    vector< pair<string, double> > candidate_tags = am_it->second;

    if (!candidate_tags.empty()) {
      int no_of_candidate_tags = candidate_tags.size();

      if (no_of_candidate_tags <= 3) {
        for (int i = 0; i < no_of_candidate_tags - 1; ++i)
          submit_file << candidate_tags[i].first << ' ';
        submit_file << candidate_tags[no_of_candidate_tags - 1].first;
      }
      else {
        sort(candidate_tags.begin(), candidate_tags.end(), SortByProbDecr);

        for (int i = 0; i < 2; ++i)
          submit_file << candidate_tags[i].first << ' ';
        submit_file << candidate_tags[2].first;

        for (int i = 3; i < no_of_candidate_tags; ++i) {
          if (candidate_tags[i].second == 1)
            submit_file << ' ' << candidate_tags[i].first;
          else
            break;
        }
      }
    }

    submit_file << '\"' << endl;
  }

  submit_file.close();
}




/*void TagMining::TrainForEachTag(int tag_id, double lambda) {   // gradient descent + L-BFGS
  cout << tag_id << endl;

  map<int, double> beta[l_bfgs], grad[l_bfgs];
  ifstream doc_vec_file(working_dir + "\\training\\" + to_string(tag_id) + "_t.svm", ios_base::in);
  set< map<int, double> > p_doc_vec, n_doc_vec;
  ReadInDataFile(doc_vec_file, &p_doc_vec, &n_doc_vec, &(beta[0]));
  doc_vec_file.close();

  int no_of_docs = p_doc_vec.size() + n_doc_vec.size();

  srand(time(NULL));
  for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it)
    for (int i = 1; i < l_bfgs; ++i) {
      beta[i].insert(make_pair(it->first, 2.0 * rand() / RAND_MAX - 1));
      grad[i].insert(make_pair(it->first, 2.0 * rand() / RAND_MAX - 1));
    }

  double m_log_likelihood = 10, learn_rate = 100;

  for (int iteration_count = 1; 1 ; ++iteration_count) {
    cout << iteration_count << '\t';

    grad[0].clear();
    for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it)
      grad[0].insert(make_pair(it->first, 0));

    double m_log_likelihood_cur = 0;

    CalcLogLikelihood(p_doc_vec, beta[0], '+', &m_log_likelihood_cur, &(grad[0]));
    CalcLogLikelihood(n_doc_vec, beta[0], '-', &m_log_likelihood_cur, &(grad[0]));

    for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it)
      grad[0][it->first] += lambda * beta[0][it->first];

    double beta_L2_norm = 0;
    for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it)
      beta_L2_norm += it->second * it->second;

    m_log_likelihood_cur += lambda / 2 * beta_L2_norm;

    cout << "-log(likelihood) per doc = " << (m_log_likelihood_cur - lambda / 2 * beta_L2_norm) / no_of_docs << endl;

    if (m_log_likelihood_cur - lambda / 2 * beta_L2_norm < no_of_docs * .0001 && iteration_count > 30)
      break;

    if (iteration_count == 1)
      learn_rate = m_log_likelihood_cur / 500;

    map<int, double> excursion_dir;

    if (((m_log_likelihood - m_log_likelihood_cur) / m_log_likelihood > 0.01
          || (m_log_likelihood - m_log_likelihood_cur) / m_log_likelihood < -0.01)
        && iteration_count < 150) {
      if (m_log_likelihood < m_log_likelihood_cur)
        learn_rate *= 0.5;

      m_log_likelihood = m_log_likelihood_cur;

      double grad_cur_L2_norm = 0;
      for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it)
        grad_cur_L2_norm += grad[0][it->first] * grad[0][it->first];

      for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it)
        excursion_dir[it->first] = learn_rate * grad[0][it->first] / sqrt(grad_cur_L2_norm);
    }
    else {
      map<int, double> q;
      q = grad[0];
      double rho_inv[l_bfgs - 1] = {0}, alpha[l_bfgs - 1] = {0};
      for (int i = 0; i < l_bfgs - 1; ++i) {
        for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it) {
          rho_inv[i] += (grad[i][it->first] - grad[i + 1][it->first]) * (beta[i][it->first] - beta[i + 1][it->first]);
          alpha[i] += (beta[i][it->first] - beta[i + 1][it->first]) * q[it->first];
        }
        alpha[i] /= rho_inv[i];
        for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it)
          q[it->first] -= alpha[i] * (grad[i][it->first] - grad[i + 1][it->first]);
      }

      double grad_beta = 0, grad_grad = 0;
      for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it) {
        grad_beta += (grad[0][it->first] - grad[1][it->first]) * (beta[0][it->first] - beta[1][it->first]);
        grad_grad += (grad[0][it->first] - grad[1][it->first]) * (grad[0][it->first] - grad[1][it->first]);
      }
      for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it)
        excursion_dir[it->first] = grad_beta / grad_grad * q[it->first];

      for (int i = l_bfgs - 2; i >= 0; --i) {
        double omega = 0;
        for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it)
          omega += (grad[i][it->first] - grad[i + 1][it->first]) * excursion_dir[it->first];
        omega /= rho_inv[i];
        for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it)
          excursion_dir[it->first] += (alpha[i] - omega) * (beta[i][it->first] - beta[i + 1][it->first]);
      }
    }

    for (int i = l_bfgs - 1; i > 0; --i) {
      beta[i] = beta[i - 1];
      grad[i] = grad[i - 1];
    }

    for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it)
      beta[0][it->first] -= excursion_dir[it->first];
  }

  ofstream model_file(working_dir + "\\LR_model\\" + to_string(tag_id) + "_m.txt", ios_base::out);

  model_file << tag_id << endl;
  for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it)
    model_file << it->first << ' ' << it->second << "  ";

  model_file.close();
}
*/

/*void TagMining::TrainForEachTag(int tag_id, double lambda) {   // gradient descent + L-BFGS, fixed # of iterations
  cout << tag_id << endl;

  map<int, double> beta[l_bfgs], grad[l_bfgs];
  ifstream doc_vec_file(working_dir + "\\training\\" + to_string(tag_id) + "_t.svm", ios_base::in);
  set< map<int, double> > p_doc_vec, n_doc_vec;
  ReadInDataFile(doc_vec_file, &p_doc_vec, &n_doc_vec, &(beta[0]));
  doc_vec_file.close();

  int no_of_docs = p_doc_vec.size() + n_doc_vec.size();

  srand(time(NULL));
  for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it)
    for (int i = 1; i < l_bfgs; ++i) {
      beta[i].insert(make_pair(it->first, 2.0 * rand() / RAND_MAX - 1));
      grad[i].insert(make_pair(it->first, 2.0 * rand() / RAND_MAX - 1));
    }

  double m_log_likelihood_per_doc_min = 10, m_log_likelihood_per_doc = 10, learn_rate = 100;
  map<int, double> beta_sol;

  for (int iteration_count = 1; iteration_count < 500 ; ++iteration_count) {
    cout << iteration_count << '\t';

    grad[0].clear();
    for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it)
      grad[0].insert(make_pair(it->first, 0));

    double m_log_likelihood_cur = 0;

    CalcLogLikelihood(p_doc_vec, beta[0], '+', &m_log_likelihood_cur, &(grad[0]));
    CalcLogLikelihood(n_doc_vec, beta[0], '-', &m_log_likelihood_cur, &(grad[0]));

    for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it)
      grad[0][it->first] += lambda * beta[0][it->first];

    double beta_L2_norm = 0;
    for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it)
      beta_L2_norm += it->second * it->second;

    m_log_likelihood_cur += lambda / 2 * beta_L2_norm;
    double m_log_likelihood_per_doc_cur = (m_log_likelihood_cur - lambda / 2 * beta_L2_norm) / no_of_docs;

    cout << "-log(likelihood) per doc = " << m_log_likelihood_per_doc_cur << endl;

    if (m_log_likelihood_per_doc_cur < 0.0001 && iteration_count > 30) {
      beta_sol = beta[0];
      break;
    }

    if (m_log_likelihood_per_doc_cur < m_log_likelihood_per_doc_min && iteration_count > 100) {
      m_log_likelihood_per_doc_min = m_log_likelihood_per_doc_cur;
      beta_sol = beta[0];
    }

    if (iteration_count == 1)
      learn_rate = beta[0].size() / 10;

    map<int, double> excursion_dir;

    if (((m_log_likelihood_per_doc - m_log_likelihood_per_doc_cur) / m_log_likelihood_per_doc > 0.01
          || (m_log_likelihood_per_doc - m_log_likelihood_per_doc_cur) / m_log_likelihood_per_doc < -0.01)
        && iteration_count < 100) {
      if (m_log_likelihood_per_doc < m_log_likelihood_per_doc_cur)
        learn_rate *= 0.7;

      m_log_likelihood_per_doc = m_log_likelihood_per_doc_cur;

      double grad_cur_L2_norm = 0;
      for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it)
        grad_cur_L2_norm += grad[0][it->first] * grad[0][it->first];

      for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it)
        excursion_dir[it->first] = learn_rate * grad[0][it->first] / sqrt(grad_cur_L2_norm);
    }
    else {
      map<int, double> q;
      q = grad[0];
      double rho_inv[l_bfgs - 1] = {0}, alpha[l_bfgs - 1] = {0};
      for (int i = 0; i < l_bfgs - 1; ++i) {
        for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it) {
          rho_inv[i] += (grad[i][it->first] - grad[i + 1][it->first]) * (beta[i][it->first] - beta[i + 1][it->first]);
          alpha[i] += (beta[i][it->first] - beta[i + 1][it->first]) * q[it->first];
        }
        alpha[i] /= rho_inv[i];
        for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it)
          q[it->first] -= alpha[i] * (grad[i][it->first] - grad[i + 1][it->first]);
      }

      double grad_beta = 0, grad_grad = 0;
      for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it) {
        grad_beta += (grad[0][it->first] - grad[1][it->first]) * (beta[0][it->first] - beta[1][it->first]);
        grad_grad += (grad[0][it->first] - grad[1][it->first]) * (grad[0][it->first] - grad[1][it->first]);
      }
      for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it)
        excursion_dir[it->first] = grad_beta / grad_grad * q[it->first];

      for (int i = l_bfgs - 2; i >= 0; --i) {
        double omega = 0;
        for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it)
          omega += (grad[i][it->first] - grad[i + 1][it->first]) * excursion_dir[it->first];
        omega /= rho_inv[i];
        for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it)
          excursion_dir[it->first] += (alpha[i] - omega) * (beta[i][it->first] - beta[i + 1][it->first]);
      }
    }

    for (int i = l_bfgs - 1; i > 0; --i) {
      beta[i] = beta[i - 1];
      grad[i] = grad[i - 1];
    }

    for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it)
      beta[0][it->first] -= excursion_dir[it->first];
  }

  cout << endl << "min -log(likelihood) per doc = " << m_log_likelihood_per_doc_min << endl;

  ofstream model_file(working_dir + "\\LR_model\\" + to_string(tag_id) + "_m.txt", ios_base::out);

  model_file << tag_id << endl;
  for (map<int, double>::iterator it = beta_sol.begin(); it != beta_sol.end(); ++it)
    model_file << it->first << ' ' << it->second << "  ";

  model_file.close();
}
*/

/*void TagMining::TrainForEachTag(int tag_id, double lambda) {   // gradient descent + L-BFGS, fixed # of iterations
                                                                    // normalized log-likelihood
  cout << tag_id << endl;

  map<int, double> beta[l_bfgs], grad[l_bfgs];
  ifstream doc_vec_file(working_dir + "\\training\\" + to_string(tag_id) + "_t.svm", ios_base::in);
  set< map<int, double> > p_doc_vec, n_doc_vec;

  ReadInDataFile(doc_vec_file, &p_doc_vec, &n_doc_vec, &(beta[0]));

  doc_vec_file.close();

  int no_of_docs = p_doc_vec.size() + n_doc_vec.size();

  srand(time(NULL));
  for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it)
    for (int i = 1; i < l_bfgs; ++i) {
      beta[i].insert(make_pair(it->first, 2.0 * rand() / RAND_MAX - 1));
      grad[i].insert(make_pair(it->first, 2.0 * rand() / RAND_MAX - 1));
    }

  double m_log_likelihood_per_doc_min = 10, m_log_likelihood_per_doc = 10, learn_rate = 100;
  map<int, double> beta_sol;

  for (int iteration_count = 1; iteration_count < 500 ; ++iteration_count) {
    cout << iteration_count << '\t';

    grad[0].clear();
    for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it)
      grad[0].insert(make_pair(it->first, 0));

    double m_log_likelihood_per_doc_cur = 0;
    CalcLogLikelihood(p_doc_vec, beta[0], '+', &m_log_likelihood_per_doc_cur, &(grad[0]));
    CalcLogLikelihood(n_doc_vec, beta[0], '-', &m_log_likelihood_per_doc_cur, &(grad[0]));
    m_log_likelihood_per_doc_cur /= no_of_docs;

    for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it) {
      grad[0][it->first] /= no_of_docs;
      grad[0][it->first] += lambda * beta[0][it->first];
    }

    cout << "-log(likelihood) per doc = " << m_log_likelihood_per_doc_cur << endl;

    if (m_log_likelihood_per_doc_cur < 0.0001 && iteration_count > 30) {
      beta_sol = beta[0];
      break;
    }

    if (m_log_likelihood_per_doc_cur < m_log_likelihood_per_doc_min && iteration_count > 100) {
      m_log_likelihood_per_doc_min = m_log_likelihood_per_doc_cur;
      beta_sol = beta[0];
    }

    if (iteration_count == 1)
      learn_rate = no_of_docs;

    map<int, double> excursion_dir;

    if (((m_log_likelihood_per_doc - m_log_likelihood_per_doc_cur) / m_log_likelihood_per_doc > 0.01
          || (m_log_likelihood_per_doc - m_log_likelihood_per_doc_cur) / m_log_likelihood_per_doc < -0.01)
        && iteration_count < 100) {
      if (m_log_likelihood_per_doc < m_log_likelihood_per_doc_cur)
        learn_rate *= 0.7;

      m_log_likelihood_per_doc = m_log_likelihood_per_doc_cur;

      double grad_cur_L2_norm = 0;
      for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it)
        grad_cur_L2_norm += grad[0][it->first] * grad[0][it->first];

      for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it)
        excursion_dir[it->first] = learn_rate * grad[0][it->first] / sqrt(grad_cur_L2_norm);
    }
    else {
      map<int, double> q;
      q = grad[0];
      double rho_inv[l_bfgs - 1] = {0}, alpha[l_bfgs - 1] = {0};
      for (int i = 0; i < l_bfgs - 1; ++i) {
        for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it) {
          rho_inv[i] += (grad[i][it->first] - grad[i + 1][it->first]) * (beta[i][it->first] - beta[i + 1][it->first]);
          alpha[i] += (beta[i][it->first] - beta[i + 1][it->first]) * q[it->first];
        }
        alpha[i] /= rho_inv[i];
        for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it)
          q[it->first] -= alpha[i] * (grad[i][it->first] - grad[i + 1][it->first]);
      }

      double grad_beta = 0, grad_grad = 0;
      for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it) {
        grad_beta += (grad[0][it->first] - grad[1][it->first]) * (beta[0][it->first] - beta[1][it->first]);
        grad_grad += (grad[0][it->first] - grad[1][it->first]) * (grad[0][it->first] - grad[1][it->first]);
      }
      for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it)
        excursion_dir[it->first] = grad_beta / grad_grad * q[it->first];

      for (int i = l_bfgs - 2; i >= 0; --i) {
        double omega = 0;
        for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it)
          omega += (grad[i][it->first] - grad[i + 1][it->first]) * excursion_dir[it->first];
        omega /= rho_inv[i];
        for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it)
          excursion_dir[it->first] += (alpha[i] - omega) * (beta[i][it->first] - beta[i + 1][it->first]);
      }
    }

    for (int i = l_bfgs - 1; i > 0; --i) {
      beta[i] = beta[i - 1];
      grad[i] = grad[i - 1];
    }

    for (map<int, double>::iterator it = beta[0].begin(); it != beta[0].end(); ++it)
      beta[0][it->first] -= excursion_dir[it->first];
  }

  cout << endl << "min -log(likelihood) per doc = " << m_log_likelihood_per_doc_min << endl;

  ofstream model_file(working_dir + "\\LR_model\\" + to_string(tag_id) + "_m.txt", ios_base::out);

  model_file << tag_id << endl;
  for (map<int, double>::iterator it = beta_sol.begin(); it != beta_sol.end(); ++it)
    model_file << it->first << ' ' << it->second << "  ";

  model_file.close();
}
*/

/*void TagMining::TrainForEachTag(int tag_id, double lambda) {      // line-search method with Hessian
  cout << tag_id << endl;

  map<int, double> beta_cur;

  ifstream doc_vec_file(working_dir + "\\training\\" + to_string(tag_id) + "_t.svm", ios_base::in);

  string ln_buffer, word_buffer;
  int label, word_id;
  double tf;
  set< map<int, double> > p_doc_vec, n_doc_vec;

  while (getline(doc_vec_file, ln_buffer)) {
    stringstream doc_vec_ss(ln_buffer);
  	doc_vec_ss >> word_buffer;
    label = stoi(word_buffer);

    map<int, double> doc_vec;

    while (doc_vec_ss >> word_buffer) {
      int separator = word_buffer.find(':');

      word_id = stoi(word_buffer.substr(0, separator));
      tf = stod(word_buffer.substr(separator + 1, word_buffer.size() - separator - 1));

      doc_vec.insert(make_pair(word_id, tf));

      if (!beta_cur.count(word_id))
        beta_cur.insert(make_pair(word_id, 0));
    }

    if (label == 1)
      p_doc_vec.insert(doc_vec);
    else
      n_doc_vec.insert(doc_vec);
  }

  doc_vec_file.close();

  int no_of_docs = p_doc_vec.size() + n_doc_vec.size();

  for (int iteration_count = 1; 1 ; ++iteration_count) {
    cout << iteration_count << '\t';

    map<int, double> grad_cur;
    for (map<int, double>::iterator it = beta_cur.begin(); it != beta_cur.end(); ++it)
      grad_cur.insert(make_pair(it->first, 0));

    double m_log_likelihood_cur = 0;

    vector<double> p_probabilities;
    for (set< map<int, double> >::iterator pdv_it = p_doc_vec.begin(); pdv_it != p_doc_vec.end(); ++pdv_it) {
      double probability = 0;

      for (map<int, double>::const_iterator it = pdv_it->begin(); it != pdv_it->end(); ++it)
        probability -= beta_cur[it->first] * it->second;

      probability = 1 / (1 + exp(probability));

      if (probability == 0)
        probability = 1.0e-15;

      p_probabilities.push_back(probability);

      for (map<int, double>::const_iterator it = pdv_it->begin(); it != pdv_it->end(); ++it)
        grad_cur[it->first] -= it->second * (1 - probability);

      m_log_likelihood_cur -= log(probability);
    }

    vector<double> n_probabilities;
    for (set< map<int, double> >::iterator ndv_it = n_doc_vec.begin(); ndv_it != n_doc_vec.end(); ++ndv_it) {
      double probability = 0;

      for (map<int, double>::const_iterator it = ndv_it->begin(); it != ndv_it->end(); ++it)
        probability -= beta_cur[it->first] * it->second;

      probability = 1 - 1 / (1 + exp(probability));

      if (probability == 0)
        probability = 1.0e-15;

      n_probabilities.push_back(probability);

      for (map<int, double>::const_iterator it = ndv_it->begin(); it != ndv_it->end(); ++it)
        grad_cur[it->first] += it->second * (1 - probability);

      m_log_likelihood_cur -= log(probability);
    }

    double beta_L2_norm = 0;
    for (map<int, double>::iterator it = beta_cur.begin(); it != beta_cur.end(); ++it)
      beta_L2_norm += it->second * it->second;

    m_log_likelihood_cur += lambda / 2 * beta_L2_norm;

    cout << "-log(likelihood) per doc = " << (m_log_likelihood_cur - lambda / 2 * beta_L2_norm) / no_of_docs << endl;

    if ((m_log_likelihood_cur - lambda / 2 * beta_L2_norm < no_of_docs * .0001 && m_log_likelihood < m_log_likelihood_cur)
        || iteration_count > 500)
      break;

    m_log_likelihood = m_log_likelihood_cur;

    for (map<int, double>::iterator it = beta_cur.begin(); it != beta_cur.end(); ++it)
      grad_cur[it->first] = grad_cur[it->first] + lambda * beta_cur[it->first];

    double learn_rate = 0;
    for (map<int, double>::iterator it = beta_cur.begin(); it != beta_cur.end(); ++it)
      learn_rate += grad_cur[it->first] * grad_cur[it->first];

    double denom = 0;

for (map<int, double>::iterator row_it = beta_cur.begin(); row_it != beta_cur.end(); ++row_it)
        for (map<int, double>::iterator col_it = beta_cur.begin(); col_it != beta_cur.end(); ++col_it) {

    int doc_id = 0;
    for (set< map<int, double> >::iterator pdv_it = p_doc_vec.begin(); pdv_it != p_doc_vec.end(); ++pdv_it) {
          if (pdv_it->count(row_it->first) && pdv_it->count(col_it->first))
            denom += grad_cur[row_it->first] * grad_cur[col_it->first]
                     * pdv_it->at(row_it->first) * pdv_it->at(col_it->first)
                     * p_probabilities[doc_id] * (1 - p_probabilities[doc_id]);
      ++doc_id;
    }
    doc_id = 0;
    for (set< map<int, double> >::iterator ndv_it = n_doc_vec.begin(); ndv_it != n_doc_vec.end(); ++ndv_it) {
          if (ndv_it->count(row_it->first) && ndv_it->count(col_it->first))
            denom += grad_cur[row_it->first] * grad_cur[col_it->first]
                     * ndv_it->at(row_it->first) * ndv_it->at(col_it->first)
                     * n_probabilities[doc_id] * (1 - n_probabilities[doc_id]);
      ++doc_id;
    }

    }

    learn_rate /= (denom + lambda * learn_rate);

    for (map<int, double>::iterator it = beta_cur.begin(); it != beta_cur.end(); ++it)
      beta_cur[it->first] -= learn_rate * grad_cur[it->first];
  }

  ofstream model_file(working_dir + "\\LR_model\\" + to_string(tag_id) + "_m.txt", ios_base::out);

  model_file << tag_id << endl;
  for (map<int, double>::iterator it = beta_cur.begin(); it != beta_cur.end(); ++it)
    model_file << it->first << ' ' << it->second << "  ";

  model_file.close();
}
*/

/*void TagMining::TrainForEachTag(int tag_id, double lambda) {      // BB method
  cout << tag_id << endl;

  map<int, double> beta_cur, beta_prev, grad_prev;

  ifstream doc_vec_file(working_dir + "\\training\\" + to_string(tag_id) + "_t.svm", ios_base::in);

  string ln_buffer, word_buffer;
  int label, word_id;
  double tf;
  set< map<int, double> > p_doc_vec, n_doc_vec;

  while (getline(doc_vec_file, ln_buffer)) {
    stringstream doc_vec_ss(ln_buffer);
  	doc_vec_ss >> word_buffer;
    label = stoi(word_buffer);

    map<int, double> doc_vec;

    while (doc_vec_ss >> word_buffer) {
      int separator = word_buffer.find(':');

      word_id = stoi(word_buffer.substr(0, separator));
      tf = stod(word_buffer.substr(separator + 1, word_buffer.size() - separator - 1));

      doc_vec.insert(make_pair(word_id, tf));

      if (!beta_cur.count(word_id))
        beta_cur.insert(make_pair(word_id, 0));
    }

    if (label == 1)
      p_doc_vec.insert(doc_vec);
    else
      n_doc_vec.insert(doc_vec);
  }

  doc_vec_file.close();

  int no_of_docs = p_doc_vec.size() + n_doc_vec.size();

  double m_log_likelihood = 10;

  for (int iteration_count = 1; 1 ; ++iteration_count) {
    cout << iteration_count << '\t';

    map<int, double> grad_cur;
    for (map<int, double>::iterator it = beta_cur.begin(); it != beta_cur.end(); ++it)
      grad_cur.insert(make_pair(it->first, 0));

    double m_log_likelihood_cur = 0;

    for (set< map<int, double> >::iterator pdv_it = p_doc_vec.begin(); pdv_it != p_doc_vec.end(); ++pdv_it) {
      double probability = 0;

      for (map<int, double>::const_iterator it = pdv_it->begin(); it != pdv_it->end(); ++it)
        probability -= beta_cur[it->first] * it->second;

      probability = 1 / (1 + exp(probability));

      if (probability == 0)
        probability = 1.0e-15;

      for (map<int, double>::const_iterator it = pdv_it->begin(); it != pdv_it->end(); ++it)
        grad_cur[it->first] -= it->second * (1 - probability);

      m_log_likelihood_cur -= log(probability);
    }

    for (set< map<int, double> >::iterator ndv_it = n_doc_vec.begin(); ndv_it != n_doc_vec.end(); ++ndv_it) {
      double probability = 0;

      for (map<int, double>::const_iterator it = ndv_it->begin(); it != ndv_it->end(); ++it)
        probability -= beta_cur[it->first] * it->second;

      probability = 1 - 1 / (1 + exp(probability));

      if (probability == 0)
        probability = 1.0e-15;

      for (map<int, double>::const_iterator it = ndv_it->begin(); it != ndv_it->end(); ++it)
        grad_cur[it->first] += it->second * (1 - probability);

      m_log_likelihood_cur -= log(probability);
    }

    double beta_L2_norm = 0;
    for (map<int, double>::iterator it = beta_cur.begin(); it != beta_cur.end(); ++it)
      beta_L2_norm += it->second * it->second;

    m_log_likelihood_cur += lambda / 2 * beta_L2_norm;

    cout << "-log(likelihood) per doc = " << (m_log_likelihood_cur - lambda / 2 * beta_L2_norm) / no_of_docs << endl;

    if ((m_log_likelihood_cur - lambda / 2 * beta_L2_norm < no_of_docs * .0001 && m_log_likelihood < m_log_likelihood_cur)
        || iteration_count > 500)
      break;

    m_log_likelihood = m_log_likelihood_cur;

    for (map<int, double>::iterator it = beta_cur.begin(); it != beta_cur.end(); ++it)
      grad_cur[it->first] = grad_cur[it->first] + lambda * beta_cur[it->first];

    double learn_rate;
    if (iteration_count == 1)
      learn_rate = 5;
    else {
      double beta_grad = 0;
      for (map<int, double>::iterator it = beta_cur.begin(); it != beta_cur.end(); ++it)
        beta_grad += (grad_cur[it->first] - grad_prev[it->first]) * (beta_cur[it->first] - beta_prev[it->first]);

      double grad_grad = 0;
      for (map<int, double>::iterator it = beta_cur.begin(); it != beta_cur.end(); ++it)
        grad_grad += (grad_cur[it->first] - grad_prev[it->first]) * (grad_cur[it->first] - grad_prev[it->first]);

      double beta_beta = 0;
      for (map<int, double>::iterator it = beta_cur.begin(); it != beta_cur.end(); ++it)
        beta_beta += (beta_cur[it->first] - beta_prev[it->first]) * (beta_cur[it->first] - beta_prev[it->first]);

      learn_rate = (beta_grad / grad_grad + beta_beta / beta_grad) / 2;
    }

    beta_prev = beta_cur;
    grad_prev = grad_cur;

    for (map<int, double>::iterator it = beta_cur.begin(); it != beta_cur.end(); ++it)
      beta_cur[it->first] -= learn_rate * grad_cur[it->first];
  }

  ofstream model_file(working_dir + "\\LR_model\\" + to_string(tag_id) + "_m.txt", ios_base::out);

  model_file << tag_id << endl;
  for (map<int, double>::iterator it = beta_cur.begin(); it != beta_cur.end(); ++it)
    model_file << it->first << ' ' << it->second << "  ";

  model_file.close();
}
*/

/*void TagMining::TrainForEachTag(int tag_id, double lambda) {      // fractionally reduce step size with roll-back option
  cout << tag_id << endl;

  map<int, double> beta_cur, beta_prev, grad_prev;

  ifstream doc_vec_file(working_dir + "\\training\\" + to_string(tag_id) + "_t.svm", ios_base::in);

  string ln_buffer, word_buffer;
  int label, word_id;
  double tf;
  set< map<int, double> > p_doc_vec, n_doc_vec;

  srand(time(NULL));
  while (getline(doc_vec_file, ln_buffer)) {
    stringstream doc_vec_ss(ln_buffer);
  	doc_vec_ss >> word_buffer;
    label = stoi(word_buffer);

    map<int, double> doc_vec;

    while (doc_vec_ss >> word_buffer) {
      int separator = word_buffer.find(':');

      word_id = stoi(word_buffer.substr(0, separator));
      tf = stod(word_buffer.substr(separator + 1, word_buffer.size() - separator - 1));

      doc_vec.insert(make_pair(word_id, tf));

      if (!beta_cur.count(word_id))
        beta_cur.insert(make_pair(word_id, 2.0 * rand() / RAND_MAX - 1));
    }

    if (label == 1)
      p_doc_vec.insert(doc_vec);
    else
      n_doc_vec.insert(doc_vec);
  }

  doc_vec_file.close();

  int no_of_docs = p_doc_vec.size() + n_doc_vec.size();

  double m_log_likelihood_per_doc_prev = 100;

  double learn_rate = beta_cur.size() / 10;
  int roll_back_count = 0;

  for (int iteration_count = 1; iteration_count <= 500 ; ++iteration_count) {
    cout << iteration_count << '\t';

    map<int, double> grad_cur;
    for (map<int, double>::iterator it = beta_cur.begin(); it != beta_cur.end(); ++it)
      grad_cur.insert(make_pair(it->first, 0));

    double m_log_likelihood_per_doc_cur = 0;

    for (set< map<int, double> >::iterator pdv_it = p_doc_vec.begin(); pdv_it != p_doc_vec.end(); ++pdv_it) {
      double probability = 0;

      for (map<int, double>::const_iterator it = pdv_it->begin(); it != pdv_it->end(); ++it)
        probability -= beta_cur[it->first] * it->second;

      probability = 1 / (1 + exp(probability));

      if (probability == 0)
        probability = 1.0e-15;

      for (map<int, double>::const_iterator it = pdv_it->begin(); it != pdv_it->end(); ++it)
        grad_cur[it->first] -= it->second * (1 - probability);

      m_log_likelihood_per_doc_cur -= log(probability);
    }

    for (set< map<int, double> >::iterator ndv_it = n_doc_vec.begin(); ndv_it != n_doc_vec.end(); ++ndv_it) {
      double probability = 0;

      for (map<int, double>::const_iterator it = ndv_it->begin(); it != ndv_it->end(); ++it)
        probability -= beta_cur[it->first] * it->second;

      probability = 1 - 1 / (1 + exp(probability));

      if (probability == 0)
        probability = 1.0e-15;

      for (map<int, double>::const_iterator it = ndv_it->begin(); it != ndv_it->end(); ++it)
        grad_cur[it->first] += it->second * (1 - probability);

      m_log_likelihood_per_doc_cur -= log(probability);
    }

    for (map<int, double>::iterator it = beta_cur.begin(); it != beta_cur.end(); ++it)
      grad_cur[it->first] = grad_cur[it->first] + lambda * beta_cur[it->first];

    m_log_likelihood_per_doc_cur /= no_of_docs;

    cout << "-log(likelihood) per doc = " << m_log_likelihood_per_doc_cur << endl
         << "learn rate = " << learn_rate << endl;

    if (m_log_likelihood_per_doc_cur < 0.0001 && iteration_count > 30)
      break;

    if (m_log_likelihood_per_doc_prev <= m_log_likelihood_per_doc_cur) {
      ++roll_back_count;
      learn_rate *= 0.95;

      beta_cur = beta_prev;
      grad_cur = grad_prev;
    }
    else {
      //for ( ; roll_back_count > 1; --roll_back_count)
        //learn_rate *= 2;

      beta_prev = beta_cur;
      grad_prev = grad_cur;
      m_log_likelihood_per_doc_prev = m_log_likelihood_per_doc_cur;
    }

    double grad_cur_L2_norm = 0;
    for (map<int, double>::iterator it = beta_cur.begin(); it != beta_cur.end(); ++it)
      grad_cur_L2_norm += grad_cur[it->first] * grad_cur[it->first];

    for (map<int, double>::iterator it = beta_cur.begin(); it != beta_cur.end(); ++it)
      beta_cur[it->first] -= learn_rate * grad_cur[it->first] / sqrt(grad_cur_L2_norm);
  }

  ofstream model_file(working_dir + "\\LR_model\\" + to_string(tag_id) + "_m.txt", ios_base::out);

  model_file << tag_id << endl;
  for (map<int, double>::iterator it = beta_cur.begin(); it != beta_cur.end(); ++it)
    model_file << it->first << ' ' << it->second << "  ";

  model_file.close();
}
*/
