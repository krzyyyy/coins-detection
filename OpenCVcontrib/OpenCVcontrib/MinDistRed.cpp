#include "stdafx.h"
#include "MinDistRed.h"

using namespace std;
using namespace cv;

MinDistRed::MinDistRed() {};
void MinDistRed::create(double thres) {
	this->thres = thres;
	maxD = DBL_MIN;
}
void MinDistRed::reduceVector(const vector <vector <vector <DMatch>>> &invector, vector <vector <vector <DMatch>>> &outvector,
	vector <unsigned int> &result) {
	result.clear();
	double current, best;
	vector <vector <vector <DMatch>>> reduced;
	for (int i = 0; i < invector.size(); i++) {
		result.push_back(0);
		min.push_back(vector <double>(0));
		mean.push_back(vector <double>(0));
		reduced.push_back(vector<vector<DMatch>>(0));
		for (int j = 0; j < invector[i].size(); j++) {
			reduced[i].push_back(vector <DMatch>(0));
			min[i].push_back(DBL_MAX);
			mean[i].push_back(0);
			for (int k = 0; k < invector[i][j].size(); k++) {
				if (invector[i][j][k].distance < min[i][j])min[i][j] = invector[i][j][k].distance;
				if (invector[i][j][k].distance > maxD)maxD = invector[i][j][k].distance;
			}
			for (int k = 0; k < invector[i][j].size(); k++) {

				if (invector[i][j][k].distance < min[i][j]*thres) {
					reduced[i][j].push_back(invector[i][j][k]);
					mean[i][j] += invector[i][j][k].distance;
				}
				mean[i][j] /= reduced.size();
			}

		}
		best = 0;
		for (int j = 0; j < invector[i].size(); j++) {
			if(reduced[i][j].size()>2){
					current = sqrt(((reduced[i][j].size() / invector[i][j].size())*(reduced[i][j].size() / invector[i][j].size())) +
					((min[i][j] / mean[i][j])*(min[i][j] / mean[i][j])));
					if (best < current) {
						best = current;
						result[i] = j;
					}
			}
		}
	}
	outvector = reduced;
}
