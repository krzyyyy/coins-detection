#include "stdafx.h"
#include "ConfMat.h"

using namespace std;
using namespace cv;

class MinDistRed
{
public:
	double thres;
	double maxD;
	MinDistRed();
	MinDistRed(double thres);
	void reduceVector(ConfMat &bases);
	//void compMinAndMean(vector <vector <vector<DMatch>>> input);
	//void compResult(vector <vector <vector<DMatch>>> reducedVec, vector <vector <vector<DMatch>>> orginal, vector<unsigned int> &result);
	void computVecRes(vector <vector <vector<DMatch>>> &reducedVec, ConfMat &bases,
						vector <vector <double>> &mean, vector <vector <double>> &min, int quant);
	void assignPatt(ConfMat &bases, vector <unsigned int> &idbest, vector <double> &best, unsigned int idtest);
private:
	void shiftVector(vector <double> &best, vector <unsigned int> &idbest, int k);
	void compResult(vector <vector <vector<DMatch>>> &reducedVec, vector <vector <vector<DMatch>>> &orginal,
		vector <vector <double>> &mean, vector <vector <double>> &min, vector <unsigned int> &result);
	vector <vector <double>> mean;
	vector <vector <double>> min;
};
