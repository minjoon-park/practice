#include "TagMining.h"
#include <math.h>
#include <algorithm>



TagMining::TagMining(string working_dir_name) {
  working_dir = working_dir_name;
  m_log_likelihood = 1e+12;
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


/*void TagMining::TrainForEachTag(int start, int fin, double lambda, double learn_rate) {
  ofstream model_file(working_dir + "\\LR_model\\" + to_string(start) + '-' + to_string(fin)
                           + "_m.txt", ios_base::out);

  for (int tag_id = start; tag_id <= fin; ++tag_id) {
    cout << endl << tag_id << endl;

    map<int, double> beta_cur;
    for (int i = 1; i <= df_map.size(); ++i)
      beta_cur.insert(make_pair(i, 0));

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
      }

      if (label == 1)
        p_doc_vec.insert(doc_vec);
      else
        n_doc_vec.insert(doc_vec);
    }

    doc_vec_file.close();

    for (int iteration_count = 1; 1 ; ++iteration_count) {
      cout << iteration_count << '\t';

      double beta_L2_norm = 0;
      for (int i = 0; i < beta_cur.size(); ++i)
        beta_L2_norm += beta_cur[i] * beta_cur[i];

      map<int, double> grad_vector;
      for (int i = 1; i <= df_map.size(); ++i)
        grad_vector.insert(make_pair(i, 0));

      double m_log_likelihood_cur = 0;

      for (set< map<int, double> >::iterator pdv_it = p_doc_vec.begin(); pdv_it != p_doc_vec.end(); ++pdv_it) {
        double probability = 0;

        for (map<int, double>::const_iterator it = pdv_it->begin(); it != pdv_it->end(); ++it)
          probability -= beta_cur[it->first] * it->second;

        probability = 1 / (1 + exp(probability));

        for (map<int, double>::const_iterator it = pdv_it->begin(); it != pdv_it->end(); ++it)
          grad_vector[it->first] += it->second / probability * (1 - probability) * probability;

        m_log_likelihood_cur -= log(probability);
      }

      for (set< map<int, double> >::iterator ndv_it = n_doc_vec.begin(); ndv_it != n_doc_vec.end(); ++ndv_it) {
        double probability = 0;

        for (map<int, double>::const_iterator it = ndv_it->begin(); it != ndv_it->end(); ++it)
          probability -= beta_cur[it->first] * it->second;

        probability = 1 - 1 / (1 + exp(probability));

        for (map<int, double>::const_iterator it = ndv_it->begin(); it != ndv_it->end(); ++it)
          grad_vector[it->first] -= it->second / probability * (1 - probability) * probability;

        m_log_likelihood_cur -= log(probability);
      }

      m_log_likelihood_cur += lambda / 2 * beta_L2_norm;
      cout << "- log(likelihood) = " << m_log_likelihood_cur << endl;

      if ((m_log_likelihood - m_log_likelihood_cur) / m_log_likelihood < .000001
          && m_log_likelihood - m_log_likelihood_cur > 0)
        break;
      if (((m_log_likelihood - m_log_likelihood_cur) / m_log_likelihood < .01
          && m_log_likelihood - m_log_likelihood_cur > 0)
          || m_log_likelihood - m_log_likelihood_cur < 0)
        learn_rate /= 2;
      m_log_likelihood = m_log_likelihood_cur;

      for (int i = 1; i <= df_map.size(); ++i)
        grad_vector[i] = grad_vector[i] - lambda * beta_cur[i];

      double grad_vector_L2_norm = 0;
      for (int i = 1; i <= df_map.size(); ++i)
        grad_vector_L2_norm += grad_vector[i] * grad_vector[i];

      for (int i = 1; i <= df_map.size(); ++i)
        beta_cur[i] += learn_rate * grad_vector[i] / sqrt(grad_vector_L2_norm);
    }

    model_file << tag_id << endl;
    for (map<int, double>::iterator it = beta_cur.begin(); it != beta_cur.end(); ++it)
      if (it->second != 0)
        model_file << it->first << ' ' << it->second << "  ";
    model_file << endl;
  }

  model_file.close();
}


void TagMining::ValidateForEachTag(int start, int fin) {
  ifstream model_file(working_dir + "\\LR_model\\" + to_string(start) + '-' + to_string(fin)
                           + "_m.txt", ios_base::in);
  ofstream threshold_file(working_dir + "\\LR_threshold\\" + to_string(start) + '-' + to_string(fin)
                           + "_th.txt", ios_base::out);

  for (int tag_id = start; tag_id <= fin; ++tag_id) {
    map<int, double> beta_cur;
    for (int i = 1; i <= df_map.size(); ++i)
      beta_cur.insert(make_pair(i, 0));

    string ln_buffer, word_buffer;
    getline(model_file, ln_buffer);
    if (stoi(ln_buffer) != tag_id) {
      cout << "model file doesn't match.\n";
      break;
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

    double f_max = 0, threshold;
    int pos_size = p_pred.size(), neg_size = n_pred.size(), tp = 0, fp = 0, fn_opt, fp_opt;

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

    threshold_file << tag_id << ' ' << threshold << endl
                   << "F_max : " << f_max << ", total # of pos : " << pos_size
                   << ", fn : " << fn_opt << ", fp : " << fp_opt << endl;
  }

  model_file.close();
  threshold_file.close();
}
*/


/*void TagMining::TrainForEachTag(int tag_id, double lambda) {
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

  double learn_rate = 1;

  for (int iteration_count = 1; 1 ; ++iteration_count) {
    cout << iteration_count << '\t';

    map<int, double> grad_vector;
    for (map<int, double>::iterator it = beta_cur.begin(); it != beta_cur.end(); ++it)
      grad_vector.insert(make_pair(it->first, 0));

    double m_log_likelihood_cur = 0;

    for (set< map<int, double> >::iterator pdv_it = p_doc_vec.begin(); pdv_it != p_doc_vec.end(); ++pdv_it) {
      double probability = 0;

      for (map<int, double>::const_iterator it = pdv_it->begin(); it != pdv_it->end(); ++it)
        probability -= beta_cur[it->first] * it->second;

      probability = 1 / (1 + exp(probability));

      if (probability == 0)
        probability = 1.0e-15;

      for (map<int, double>::const_iterator it = pdv_it->begin(); it != pdv_it->end(); ++it)
        grad_vector[it->first] += it->second / probability * (1 - probability) * probability;

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
        grad_vector[it->first] -= it->second / probability * (1 - probability) * probability;

      m_log_likelihood_cur -= log(probability);
    }

    double beta_L2_norm = 0;
    for (map<int, double>::iterator it = beta_cur.begin(); it != beta_cur.end(); ++it)
      beta_L2_norm += it->second * it->second;

    m_log_likelihood_cur += lambda / 2 * beta_L2_norm;

    cout << "-log(likelihood) per doc = " << (m_log_likelihood_cur - lambda / 2 * beta_L2_norm) / no_of_docs << endl;

    if (iteration_count == 1)
      learn_rate = m_log_likelihood_cur / 300;

    if ((m_log_likelihood_cur - lambda / 2 * beta_L2_norm < no_of_docs * .0001 && m_log_likelihood < m_log_likelihood_cur)
        || iteration_count > 500)
      break;
    if (m_log_likelihood < m_log_likelihood_cur)
      learn_rate *= 0.95;

    m_log_likelihood = m_log_likelihood_cur;

    for (map<int, double>::iterator it = beta_cur.begin(); it != beta_cur.end(); ++it)
      grad_vector[it->first] = grad_vector[it->first] - lambda * beta_cur[it->first];

    double grad_vector_L2_norm = 0;
    for (map<int, double>::iterator it = beta_cur.begin(); it != beta_cur.end(); ++it)
      grad_vector_L2_norm += grad_vector[it->first] * grad_vector[it->first];

    for (map<int, double>::iterator it = beta_cur.begin(); it != beta_cur.end(); ++it)
      beta_cur[it->first] += learn_rate * grad_vector[it->first] / sqrt(grad_vector_L2_norm);
  }

  ofstream model_file(working_dir + "\\LR_model\\" + to_string(tag_id) + "_m.txt", ios_base::out);

  model_file << tag_id << endl;
  for (map<int, double>::iterator it = beta_cur.begin(); it != beta_cur.end(); ++it)
    model_file << it->first << ' ' << it->second << "  ";

  model_file.close();
}
*/

void TagMining::TrainForEachTag(int tag_id, double lambda) {
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

    map<int, double> grad_vector;
    for (map<int, double>::iterator it = beta_cur.begin(); it != beta_cur.end(); ++it)
      grad_vector.insert(make_pair(it->first, 0));

    map< int, map<int, double> > hessian;
    for (map<int, double>::iterator row_it = beta_cur.begin(); row_it != beta_cur.end(); ++row_it) {
      map<int, double> temp_row;
      for (map<int, double>::iterator col_it = beta_cur.begin(); col_it != beta_cur.end(); ++col_it)
        temp_row.insert(make_pair(col_it->first, 0));
      hessian.insert(make_pair(row_it->first, temp_row));
    }

    double m_log_likelihood_cur = 0;

    for (set< map<int, double> >::iterator pdv_it = p_doc_vec.begin(); pdv_it != p_doc_vec.end(); ++pdv_it) {
      double probability = 0;

      for (map<int, double>::const_iterator it = pdv_it->begin(); it != pdv_it->end(); ++it)
        probability -= beta_cur[it->first] * it->second;

      probability = 1 / (1 + exp(probability));

      if (probability == 0)
        probability = 1.0e-15;

      for (map<int, double>::const_iterator it = pdv_it->begin(); it != pdv_it->end(); ++it)
        grad_vector[it->first] -= it->second * (1 - probability);

      for (map<int, double>::const_iterator row_it = pdv_it->begin(); row_it != pdv_it->end(); ++row_it)
        for (map<int, double>::const_iterator col_it = pdv_it->begin(); col_it != pdv_it->end(); ++col_it)
          hessian[row_it->first][col_it->first] += row_it->second * col_it->second * probability * (1 - probability);

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
        grad_vector[it->first] += it->second * (1 - probability);

      for (map<int, double>::const_iterator row_it = ndv_it->begin(); row_it != ndv_it->end(); ++row_it)
        for (map<int, double>::const_iterator col_it = ndv_it->begin(); col_it != ndv_it->end(); ++col_it)
          hessian[row_it->first][col_it->first] += row_it->second * col_it->second * probability * (1 - probability);

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
      grad_vector[it->first] = grad_vector[it->first] + lambda * beta_cur[it->first];

    double denom = 0;
    for (map<int, double>::iterator row_it = beta_cur.begin(); row_it != beta_cur.end(); ++row_it)
      for (map<int, double>::iterator col_it = beta_cur.begin(); col_it != beta_cur.end(); ++col_it)
        denom += grad_vector[row_it->first] * hessian[row_it->first][col_it->first] * grad_vector[col_it->first];

    double learn_rate = 0;
    for (map<int, double>::iterator it = beta_cur.begin(); it != beta_cur.end(); ++it)
      learn_rate += grad_vector[it->first] * grad_vector[it->first];
    learn_rate /= (denom + lambda * learn_rate);

    for (map<int, double>::iterator it = beta_cur.begin(); it != beta_cur.end(); ++it)
      beta_cur[it->first] -= learn_rate * grad_vector[it->first];
  }

  ofstream model_file(working_dir + "\\LR_model\\" + to_string(tag_id) + "_m.txt", ios_base::out);

  model_file << tag_id << endl;
  for (map<int, double>::iterator it = beta_cur.begin(); it != beta_cur.end(); ++it)
    model_file << it->first << ' ' << it->second << "  ";

  model_file.close();
}



void TagMining::ValidateForEachTag(int tag_id) {
  ifstream model_file(working_dir + "\\LR_model\\" + to_string(tag_id) + "_m.txt", ios_base::in);
  ofstream threshold_file(working_dir + "\\thresholds_" + to_string(tag_id) + ".txt", ios_base::app);

  map<int, double> beta_cur;
  for (int i = 1; i <= df_map.size(); ++i)
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

  threshold_file << tag_id << ' ' << threshold << endl
                 << "F_max : " << f_max << ", total # of pos : " << pos_size
                 << ", fn : " << fn_opt << ", fp : " << fp_opt << endl;

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
    for (int i = 1; i <= df_map.size(); ++i)
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
      for (int i = 1; i <= df_map.size(); ++i)
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
      for (int i = 0; i < (it->second).size(); ++i)
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


