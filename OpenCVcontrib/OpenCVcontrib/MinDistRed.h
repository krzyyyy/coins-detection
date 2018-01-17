#include "stdafx.h"
#include "NormVector.h"

using namespace std;
using namespace cv;

class MinDistRed
{
public:
	double thres;
	double maxD;
	MinDistRed();
	MinDistRed(double thres);
	void reduceVector(const vector <vector <vector <DMatch>>> &invector, vector <vector <vector <DMatch>>> &outvector, vector <unsigned int> &result);
	vector <vector <vector <DMatch>>> composit(vector <vector <vector<DMatch>>> input,  vector <unsigned int> idoutput);
	void compMinAndMean(vector <vector <vector<DMatch>>> input);
	void compResult(vector <vector <vector<DMatch>>> reducedVec, vector <vector <vector<DMatch>>> orginal, vector<unsigned int> &result);
private:
	void compResult(vector <vector <vector<DMatch>>> &reducedVec, vector <vector <vector<DMatch>>> &orginal,
		vector <vector <double>> &mean, vector <vector <double>> &min, vector <unsigned int> &result);
	void decide(vector <unsigned int> &result, vector <unsigned int> &idbest, vector <double> &best);
	vector <vector <double>> mean;
	vector <vector <double>> min;
};
