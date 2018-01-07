#include "stdafx.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

class NormVector
{
public:
	virtual void create(double thres)=0;
	virtual void reduceVector(const vector <vector <vector <DMatch>>> &invector, vector <vector <vector <DMatch>>> &outvector, vector <unsigned int> &result)=0;
	//virtual void choseImag(const vector <vector <vector <DMatch>>> &invector,vector <unsigned int> result)=0;
};