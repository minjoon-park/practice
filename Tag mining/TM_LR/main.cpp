#include "TagMining.h"


using namespace std;


int main() {
	TagMining test("D:\\Work");

  test.LoadMaps();

  for (int tag_id = 32025; tag_id <= 32025; ++tag_id) {
    test.TrainForEachTag(tag_id, 0.01);
    test.ValidateForEachTag(tag_id);
  }

/*  test.Test(18001, 24000);
  test.WriteAnswerMap(18001);

  test.BuildAnswerMap(1);
  test.BuildAnswerMap(6001);
  test.BuildAnswerMap(12001);
  test.BuildAnswerMap(18001);
  test.BuildAnswerMap(24001);
  test.BuildAnswerMap(28501);
  test.BuildAnswerMap(33001);
  test.BuildAnswerMap(0);

//  test.WriteAnswerMap(0);

  test.WriteSubmitFile();*/

  return 0;
}
