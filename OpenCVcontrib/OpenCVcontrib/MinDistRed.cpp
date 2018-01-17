#include "stdafx.h"
#include "MinDistRed.h"

using namespace std;
using namespace cv;

MinDistRed::MinDistRed() {};
MinDistRed::MinDistRed(double thres) {
	min.clear();
	mean.clear();
	this->thres = thres;
	maxD = DBL_MIN;
}

void MinDistRed::reduceVector(const vector <vector <vector <DMatch>>> &invector, vector <vector <vector <DMatch>>> &outvector,
	vector <unsigned int> &result) {
	double current, best;
	vector <vector <vector <DMatch>>> reduced;
	for (int i = 0; i < invector.size(); i++) {
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
			}
			mean[i][j] /= reduced.size();

		}
	}
	compResult(reduced, invector, result);
	outvector = reduced;
}
void MinDistRed::compResult(vector <vector <vector<DMatch>>> &reducedVec, vector <vector <vector<DMatch>>> &orginal,
	vector <vector <double>> &mean, vector <vector <double>> &min, vector <unsigned int> &result){
	result.clear();
	result = vector <unsigned int>(reducedVec.size());
	double current , best;
	for (int i = 0; i < reducedVec.size(); i++) {
		best = 0;
		for (int j = 0; j < reducedVec[i].size(); j++) {
			if (reducedVec[i][j].size()>2) {
				current = sqrt(((reducedVec[i][j].size() / orginal[i][j].size())*(reducedVec[i][j].size() / orginal[i][j].size())) +
					((min[i][j] / mean[i][j])*(min[i][j] / mean[i][j])));
				if (best < current) {
					best = current;
					result[i] = j;
				}
			}
		}
	}
}
void MinDistRed::comptVecRes(vector <vector <vector<DMatch>>> &reducedVec, vector <vector <vector<DMatch>>> &orginal,
	vector <vector <double>> &mean, vector <vector <double>> &min, vector <unsigned int> &result, int quant) {
	result.clear();
	result = vector <unsigned int>(reducedVec.size());
	vector <double > best;
	vector <unsigned int> idbest;
	double current, best;
	for (int i = 0; i < reducedVec.size(); i++) {
		best.assign(quant, 0);
		idbest.assign(quant, 0);
		for (int j = 0; j < reducedVec[i].size(); j++) {
			if (reducedVec[i][j].size()>2) {
				current = sqrt(((reducedVec[i][j].size() / orginal[i][j].size())*(reducedVec[i][j].size() / orginal[i][j].size())) +
					((min[i][j] / mean[i][j])*(min[i][j] / mean[i][j])));
				for (int k = 0; k < quant; k++) {
					if (best[k] < current) {
						best[k] = current;
						idbest[k] = j;
					}
				}
			}
		}

	}
}

void MinDistRed::decide(vector <unsigned int> &result, vector <unsigned int> &idbest, vector <double> &best) {

}


vector <vector <vector <DMatch>>> MinDistRed:: composit(vector <vector <vector<DMatch>>> input, vector <unsigned int> idoutput) {
	vector <vector <vector <DMatch>>> composited = vector <vector <vector <DMatch>>>(input.size());
	for (int i = 0; i < composited.size(); i++) {
		composited[i] = vector<vector<DMatch>>(9);
	}
	for (int i = 0; i < input.size(); i++) {
		for (int j = 0; j < input[i].size(); j++) {
			for (int k = 0; k < input[i][j].size(); k++) {
				composited[i][idoutput[j]].push_back(input[i][j][k]);
			}
		}
	}
	return composited;
}
void MinDistRed::compMinAndMean(vector <vector <vector<DMatch>>> input) {
	for (int i = 0; i < input.size(); i++) {
		for (int j = 0; j < input[i].size(); j++) {
			mean[i][j] = 0;
			min[i][j] = DBL_MAX;
			for (int k = 0; k < input[i][j].size(); k++) {
				mean[i][j] += input[i][j][k].distance;
				if (input[i][j][k].distance < min[i][j]) {
					min[i][j] = input[i][j][k].distance;
				}
			}
			mean[i][j] /= input[i][j].size();
		}
	}
}
void MinDistRed::compResult(vector <vector <vector<DMatch>>> reducedVec, vector <vector <vector<DMatch>>> orginal, vector<unsigned int> &result) {
	result = vector<unsigned int>(reducedVec.size());
	double factor, bestfactor;
	for (int i = 0; i < reducedVec.size(); i++) {
		bestfactor = 0;
		for(int j = 0; j < reducedVec[i].size(); j++) {
			if (reducedVec[i][j].size() < 3)factor = 0;
			else {
				factor = sqrt((reducedVec[i][j].size() / orginal[i][j].size())*(reducedVec[i][j].size() / orginal[i][j].size()) +
					((min[i][j] / mean[i][j])*(min[i][j] / mean[i][j])));
				if (factor > bestfactor) {
					bestfactor = factor;
					result[i] = j;
				}
			}

		}
	}
}
















/*void MinDistRed::matchImg(const vector <vector <vector <DMatch>>> &invector, vector <unsigned int> &result) {
	double current, best;
	for (int i = 0; i < invector.size(); i++) {
		best = 0;
		for (int j = 0; j < invector[i].size(); j++) {
			if (invector[i][j].size() > 2) {
				current = sqrt(((invector[i][j].size() / invector[i][j].size())*(reduced[i][j].size() / invector[i][j].size())) +
					((min[i][j] / mean[i][j])*(min[i][j] / mean[i][j])));
				if (best < current) {
					best = current;
					result[i] = j;
				}
			}
		}
	}
}*/
