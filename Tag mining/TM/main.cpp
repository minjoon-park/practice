/**
no. of items in train.csv: 6034195
no. of items in test.csv: 2013337
no. of tags: 42048
df_map size with DFCUT = 3: 443095, 5: 300157
**/

#include "TagMining.h"

using namespace std;


int main() {
	TagMining test("C:\\Users\\Minjoon\\Documents");

//	test.ParseTrainingDataAndBuildDfMap();
  test.LoadMaps();
	test.ConvertParsedDataFileToDocVectorFile();
//  test.BuildCorrelationMap();
//  test.ExamineCorMap();

/*  test.LoadMaps();
  test.LoadCorMap();

  test.BuildTrainingSet(1000);

  double threshold;
  int num;

  while (1) {
    cout << "threshold: ";
    cin >> threshold;
    cout << "min. no. of common words in a keyword set for a tag: ";
    cin >> num;
    cout << endl;

    if (num == 0)
      break;

    test.Train(threshold, num);
  }
*/
  return 0;
}



/**
  test.BuildTrainingSet(50000);    // 144k tags to predict

"Train(threshold, # of common words)"
  test.Train(1, 3);              // p: 2.215% 4966, r: 0.0764% 110
  test.Train(0.75, 3);           // p: 0.82% 61396, r: 0.349% 502
  test.Train(0.5, 3);            // p: 0.677% 350414, r: 1.65% 2371

  test.Train(1, 1);              // p: 0.25% 8268575, r: 14.2% 20468
  test.Train(0.75, 1);           // p: 0.166% 30581327, r: 35.3% 50893
  test.Train(0.5, 1);            // p: 0.098% 73833362, r: 50.1% 72191

  test.Train(0.75, 2);           // p: 0.43% 1092097, r: 3.25% 4679

  test.Train(0.1, 5);            // p: 0.26% 6027541, r: 10.9% 15759

  test.Train(2, 3);              // p: 18.18% 11, r: .000139% 2
  test.Train(2, 1);              // p: 0.0366% 139520, r: 0.0354% 51


"Train(threshold, n where # of words for a given tag / n)"
  test.Train(1, 5);                 // p: 0.25, r: 14.2
  test.Train(1, 2);                 // p: 0.36, r: 14.2
  test.Train(1, 1);                 // p: 0.85, r: 13.5
  test.Train(0.75, 1);              // p: 1.16, r: 32.7
  test.Train(0.5, 1);               // p: 2.02, r: 39.2
  test.Train(0.5, 2);               // p: 0.3,  r: 50.0
  test.Train(0.5, 5);               // p: 0.13, r: 50.4
  test.Train(0.2, 1);               // p: 8.53, r: 14.3
  test.Train(0.2, 2);               // p: 0.86, r: 54.1
  test.Train(0.2, 5);               // p: 0.096, r: 71.9


  test.BuildTrainingSet(20000);    // 57635 tags to predict

  test.Train(2);              // p: .039% 56K, r: .038% 22
  test.Train(1);              // p: .248% 3.3M, r: 14.2% 8K
  test.Train(.5);             // p: .098% 29.5M, r: 50.1% 28.9K
**/
