#include "stdafx.h"

using namespace std;
using namespace cv;

class ImageBase{
public:
	vector <string> name;
	vector <vector <KeyPoint>> keys;
	vector < Mat> images;
	vector <Mat> desc;
	ImageBase();
	ImageBase(const string path, const string fileExt);
	void computDesc(Ptr<Feature2D> detector, Ptr<Feature2D> descriptor);
	//bool isEmpty();
private:
	bool empty;
	

};


