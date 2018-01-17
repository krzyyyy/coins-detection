#include "stdafx.h"

using namespace std;
using namespace cv;

class ImageBase{
public:
	string naz[9]; 
	vector <string> name;
	vector <vector <KeyPoint>> keys;
	vector < Mat> images;
	vector <Mat> desc;
	ImageBase();
	ImageBase(const string path, const string fileExt);
	void computDesc(Ptr<Feature2D> detector, Ptr<Feature2D> descriptor);
	void view();
	//void savePatterns(const vector <Mat> img, const vector < Vec3f[6][3]> coins, const string path, string ext);
private:
	bool empty;
	

};


