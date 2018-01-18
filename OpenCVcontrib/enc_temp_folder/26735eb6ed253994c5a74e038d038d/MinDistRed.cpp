#include "stdafx.h"
#include "MinDistRed.h"


using namespace std;
using namespace cv;

MinDistRed::MinDistRed(){
	this->thres = 2;
}
MinDistRed::MinDistRed(double thres) {
	min.clear();
	mean.clear();
	this->thres = thres;
	maxD = DBL_MIN;
}

void MinDistRed::reduceVector(ConfMat &bases) {
	vector <vector <double>> min, mean;
	double current, best;
	vector <vector <vector <DMatch>>> reduced;
	for (int i = 0; i < bases.distances.size(); i++) {
		min.push_back(vector <double>(0));
		mean.push_back(vector <double>(0));
		reduced.push_back(vector<vector<DMatch>>(0));
		for (int j = 0; j < bases.distances[i].size(); j++) {
			reduced[i].push_back(vector <DMatch>(0));
			min[i].push_back(DBL_MAX);
			mean[i].push_back(0);
			for (int k = 0; k < bases.distances[i][j].size(); k++) {
				if (bases.distances[i][j][k].distance < min[i][j])min[i][j] = bases.distances[i][j][k].distance;
				if (bases.distances[i][j][k].distance > maxD)maxD = bases.distances[i][j][k].distance;
			}
			for (int k = 0; k < bases.distances[i][j].size(); k++) {
				if (bases.distances[i][j][k].distance < min[i][j]*thres) {
					reduced[i][j].push_back(bases.distances[i][j][k]);
					mean[i][j] += bases.distances[i][j][k].distance;
				}
			}
			mean[i][j] /= reduced.size();

		}
	}
	computVecRes(reduced,  bases, mean, min,4);
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
void MinDistRed::shiftVector(vector <double> &best, vector <unsigned int> &idbest, int k) {
	vector <double> tempbest(best.size());
	vector <unsigned int> tempid(idbest.size());
	for (int i = 0; i < k;i++) {
		tempbest[i] = best[i];
		tempid[i] = idbest[i];
	}

	for (int i = k + 1; i < best.size(); i++) {
		tempbest[i] = best[i - 1];
		tempid[i] = idbest[i - 1];
	}
	best = tempbest;
	idbest = tempid;


}
void MinDistRed::computVecRes(vector <vector <vector<DMatch>>> &reducedVec, ConfMat &bases,
	vector <vector <double>> &mean, vector <vector <double>> &min,  int quant) {
	vector <double > best;
	vector <unsigned int> idbest;
	double current;
	for (int i = 0; i < reducedVec.size(); i++) {
		best.assign(quant, 0);
		idbest.assign(quant, 0);
		for (int j = 0; j < reducedVec[i].size(); j++) {
			if (reducedVec[i][j].size()>2) {
				
				//current = sqrt( ((min[i][j] / mean[i][j])*(min[i][j] / mean[i][j])));
				current = 1 / (min[i][j] * mean[i][j]);
				for (int k = 0; k < quant; k++) {
					if (best[k] < current) {
						if(k!=0)shiftVector(best, idbest, k);
						best[k] = current;
						idbest[k] = j;
						k = quant;
					}
				}
			}
		}
		assignPatt(bases, idbest, best, i);
	}
}

void MinDistRed::assignPatt(ConfMat &bases, vector <unsigned int> &idbest, vector <double> &best, unsigned int idtest) {

	vector <double> truedistance(idbest.size());
	double thebest;
	unsigned int idthebest;
	for (int i = 0; i < idbest.size(); i++) {
		for (int j = i + 1; j < idbest.size(); j++) {
			if (idbest[i] == idbest[j]) {
				best[i] += best[j];
				idbest.erase(idbest.begin() + j);
				best.erase(best.begin() + j);
				j--;
			}
		}
	}
	thebest = 0;
	for (int i = 0; i < idbest.size(); i++) {
		if (thebest < best[i]) {
			thebest = best[i];
			idthebest = idbest[i];
		}
	}

	bases.result[idtest] = idthebest;
}


/*vector <vector <vector <DMatch>>> MinDistRed:: composit(vector <vector <vector<DMatch>>> input, vector <unsigned int> idoutput) {
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
*/















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
