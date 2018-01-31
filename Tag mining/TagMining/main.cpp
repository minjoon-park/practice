#include "TagMining.h"
#include <math.h>

using namespace std;
int main() {

	TagMining test;
/*
	test.PrepStopwords("C:\\Users\\Minjoon\\Documents\\dictionary.txt");
	test.BuildTrainingDfMap("C:\\Users\\Minjoon\\Documents\\Train.csv", "C:\\Users\\Minjoon\\Documents");
	test.BuildTestDfMap("C:\\Users\\Minjoon\\Documents\\Test.csv", "C:\\Users\\Minjoon\\Documents");
	test.FinalizeTagSetAndDfMaps("C:\\Users\\Minjoon\\Documents");

  cout << "Tag set size: " << test.GetTagSetSize() << endl;
  cout << "Title df map size: " << test.GetTitleMapSize() << '\t'
       << "Body df map size: " << test.GetBodyMapSize() << endl;

  test.LoadTagSet("C:\\Users\\Minjoon\\Documents");
  test.LoadDfMaps("C:\\Users\\Minjoon\\Documents");
  test.BuildInvertedTagMapAndTrainItemVectors("C:\\Users\\Minjoon\\Documents", "C:\\Users\\Minjoon\\Documents");

  //test.LoadInvertedTagMap("C:\\Users\\Minjoon\\Documents");
  test.LoadItemVectors("C:\\Users\\Minjoon\\Documents\\train_item_vectors.txt");
  //test.PrepBCSVMLearningData("C:\\Users\\Minjoon\\Documents\\SVM_train", 5500000);
  test.PrepBCSVMValData("C:\\Users\\Minjoon\\Documents", 250000);
  //test.PrepBCSVMLearningData("E:\\10000-5", SizeOfTrainingSet);

  test.LoadDfMaps("C:\\Users\\Minjoon\\Documents");
  test.PrepSVMTestData("C:\\Users\\Minjoon\\Documents", "C:\\Users\\Minjoon\\Documents");

  test.BuildAnswerMap("C:\\Users\\Minjoon\\Documents\\10000-5_a", 1, 500, 80);
  test.BuildAnswerMap("C:\\Users\\Minjoon\\Documents\\10000-5_a", 40001, 200, 9);
  test.BuildAnswerMap("C:\\Users\\Minjoon\\Documents\\10000-5_a", 41801, 248, 1);
  test.WriteAnswerMapToFile("C:\\Users\\Minjoon\\Documents");
*/
  test.LoadAnswerMap("C:\\Users\\Minjoon\\Documents");
  test.LoadInvertedTagMap("C:\\Users\\Minjoon\\Documents");
  //test.BuildThresholdMap("C:\\Users\\Minjoon\\Documents\\val_250k-4.svm", "H:\\10000-5_vp-4",
  //                       "C:\\Users\\Minjoon\\Documents\\threshold_map-4.txt");
  test.LoadTagSet("C:\\Users\\Minjoon\\Documents");
  test.WriteSubmitFile(test.LoadThresholdMap(4), "C:\\Users\\Minjoon\\Documents");

	return 0;
}
