#include "stdafx.h"
//#include "ImageBase.h"
#include "ConfMat.h"

using namespace std;
using namespace cv;
void ConfMat::matchBases( Ptr<DescriptorMatcher> matcher, bool show) {
	for (int i = 0; i < inputbase.desc.size(); i++) {
		for (int j = 0; j < outputbase.desc.size(); j++) {
			if(inputbase.name[i]!=outputbase.name[j])
				matcher->match(inputbase.desc[i], outputbase.desc[j], distances[i][j]);
		}
	}
}
void ConfMat::matchSelfBase(ImageBase &in, Ptr<DescriptorMatcher> matcher, bool show) {
	for (int i = 0; i < in.desc.size(); i++) {
		for (int j = i+1; j < in.desc.size(); j++) {
			matcher->match(in.desc[i], in.desc[j], distances[i][j]);
		}
	}
}
ConfMat::ConfMat(ImageBase &inputbase, ImageBase &outputbase, Ptr<Feature2D> detector, Ptr<Feature2D> descriptor) {
	this->inputbase = inputbase;
	this->outputbase = outputbase;
	this->inputbase.computDesc(detector, descriptor);
	this->outputbase.computDesc(detector, descriptor);
	distances = vector <vector <vector<DMatch>>>(inputbase.images.size());
	for (int i = 0; i < inputbase.images.size(); i++)
		distances[i] = vector<vector<DMatch>>(outputbase.images.size());
	idinput = vector <unsigned int >(inputbase.images.size());
	idoutput = vector <unsigned int >(outputbase.images.size());
	string names[] = { "1gr", "2gr", "5gr", "10gr", "20gr", "50gr", "1zl", "2zl", "5zl" };
	nominal = vector <string>(names, names + sizeof(names) / sizeof(string));
	confmatrix = Mat(9, 9, CV_16UC1, Scalar(0));
	setNames(nominal, inputbase, idinput);
	setNames(nominal, outputbase, idoutput);
}
void ConfMat::setNames(const vector <string> &names, ImageBase &base, vector<unsigned int> &id) {
	for (int i = 0; i < base.name.size(); i++) {
		for (int j = 0; j < names.size(); j++) {
			if (base.name[i].find(names[j]) != string::npos) {
				id[i]=j;
				j = names.size();
			}
		}
	}
}
void ConfMat::computConf(const vector <unsigned int> & resalt) {
	for each (unsigned int i in resalt) {
		confmatrix.at<unsigned short>(idinput[i],idoutput[resalt[i]])++;
	}
}