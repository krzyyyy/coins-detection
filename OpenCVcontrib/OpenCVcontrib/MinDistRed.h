#include "stdafx.h"
#include "NormVector.h"

using namespace std;
using namespace cv;

class MinDistRed: public NormVector
{
public:
	double thres;
	double maxD;
	MinDistRed();
	void create(double thres);
	void reduceVector(const vector <vector <vector <DMatch>>> &invector, vector <vector <vector <DMatch>>> &outvector, vector <unsigned int> &result);
	
private:
	vector <vector <double>> mean;
	vector <vector <double>> min;
};
