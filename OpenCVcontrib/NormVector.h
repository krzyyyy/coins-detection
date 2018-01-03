#include "stdafx.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

class NormVector
{
public:
	virtual void create()=0;
	virtual void reduceVector(const vector <vector <vector <DMatch>>> &invector, vector <vector <vector <DMatch>>> &outvector)=0;
	virtual void choseImag(const vector <vector <vector <DMatch>>> &invector,vector <unsigned int> result)=0;
};