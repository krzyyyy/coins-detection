#pragma once
#include "stdafx.h"
#include "ImageBase.h"

using namespace std;
using namespace cv;


class ConfMat {
public:
	ImageBase inputbase;
	ImageBase outputbase;
	vector <unsigned int> idinput;
	vector <unsigned int> idoutput;
	vector <string> nominal;
	vector<vector <vector <DMatch>>> distances;
	ConfMat(const string & name1, const string &ext1, const string & name2, const string &ext2, Ptr<Feature2D> detector, Ptr<Feature2D> descriptor);
	void matchBases( Ptr<DescriptorMatcher> matcher, bool show);
	void matchSelfBase(ImageBase &in, Ptr<DescriptorMatcher> matcher, bool show);
	void computConf(const vector <unsigned int> &resalt, double &fac, Mat &confmatrix);
	void computMeanConf(const vector <unsigned int> &resalt, double &fac, Mat &confmatrix);
	
	//void compConfMat();
private:
	void setNames(const vector <string> &names, ImageBase &base,  vector<unsigned int> &id);
	vector <vector<unsigned int >> result;

};