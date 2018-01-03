#include "stdafx.h"
#include "NormVector.h"

using namespace std;
using namespace cv;

class MinDistRed: public NormVector
{
public:
	double thres;
	double minD;
	double maxD;
	void create(double thres);
	void reduceVector(const vector <vector <vector <DMatch>>> &invector, vector <vector <vector <DMatch>>> &outvector);
	void choseImag(const vector <vector <vector <DMatch>>> &invector, vector <unsigned int> &result)


};
