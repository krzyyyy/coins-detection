#include "stdafx.h"
#include "MinDistRed.h"

using namespace std;
using namespace cv;


void MinDistRed::create(double thres) {
	this->thres = thres;
	minD = DBL_MAX;
	maxD = DBL_MIN;
}
void MinDistRed::reduceVector(const vector <vector <vector <DMatch>>> &invector, vector <vector <vector <DMatch>>> &outvector) {
	vector <vector <vector <DMatch>>> reduced;
	for (int i = 0; i < invector.size(); i++) {
		reduced.push_back(vector<vector<DMatch>>(0));
		for (int j = 0; j < invector[i].size(); j++) {
			reduced[i].push_back(vector <DMatch>(0));
			for (int k = 0; k < invector[i][j].size(); k++) {
				if (invector[i][j][k].distance < minD)minD = invector[i][j][k].distance;
				if (invector[i][j][k].distance > maxD)maxD = invector[i][j][k].distance;
			}
			for (int k = 0; k < invector[i][j].size(); k++) {
				if (invector[i][j][k].distance < minD*thres)reduced[i][j].push_back(invector[i][j][k]);
			}
		}
	}
	outvector = reduced;
}
void MinDistRed::choseImag(const vector <vector <vector <DMatch>>> &invector, vector <unsigned int> &result) {

}