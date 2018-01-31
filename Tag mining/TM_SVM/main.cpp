/**
no. of items in train.csv: 6034195
no. of items in test.csv: 2013337
no. of tags: 42048
df_map size with DFCUT = 3: 443095, 5: 300157. 10: 170905
**/

#include "TagMining.h"

using namespace std;


int main() {

	TagMining test("C:\\Users\\Minjoon\\Documents");

	test.ParseLearningDataAndBuildMaps();
	//test.LoadMaps();
	//test.CheckTagsDist();
  test.BuildTrainItemVectors();
  //test.LoadTrainItemVectors();
  test.PrepSVMTrainingData();
  //test.ParseTestData();
  //test.PrepSVMTestData();

/*  test.DetermineThreshold(18070);

  test.BuildThresholdsMap();

  test.LoadThresholdsMap();

  test.BuildAnswerMap(1, 1000, 42);
  test.BuildAnswerMap(42001, 48, 1);
  test.WriteSubmitFile();
*/
  return 0;
}

