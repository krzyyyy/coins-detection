#include "stdafx.h"
//#include "ImageBase.h"

using namespace std;
using namespace cv;


class ConfMat {
public:
	ImageBase inputbase;
	ImageBase outputbase;
	vector <vector <unsigned int>> idinput;
	vector <vector <unsigned int>> idoutput;
	vector <string> nominal;
	vector<vector <vector <DMatch>>> distances;
	Mat confuseMat;
	ConfMat(ImageBase &inputbase, ImageBase &outputbase);
	void matchBases( Ptr<DescriptorMatcher> matcher, bool show);
	void matchSelfBase(ImageBase &in, Ptr<DescriptorMatcher> matcher, bool show);
	void setNames(const vector <string> &names, ImageBase &base, vector < vector<unsigned int>> &id);
	void normMatchMatrix(double thres);
	//void compConfMat();


};