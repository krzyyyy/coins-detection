#include "stdafx.h"
//#include "ImageBase.h"
#include "ConfMat.h"
#include "MinDistRed.h"
#include "ShowMatches.h"




using namespace std;
using namespace cv;



int main() {
	Mat www;
	double fact, bestfactor;
	vector <vector <vector<DMatch>>> closed;
	vector <unsigned int > przynal;
	//ImageBase x("C:/Users/ASUS/Desktop/BAZA_ZDJEC/NOWE_MONETY/ogolne/", "tif");
	MinDistRed ssss;
	ShowMatches view;
	Ptr<MSER> detect = MSER::create(4, 20, 12000, 0.25);
	Ptr<Feature2D> descript = xfeatures2d::SIFT::create();
	ConfMat por(ImageBase("C:/Users/ASUS/Desktop/BAZA_ZDJEC/NOWE_MONETY/ogolne/", "tif"), ImageBase("C:/Users/ASUS/Desktop/BAZA_ZDJEC/NOWE_MONETY/test/", "tif"), detect, descript);
	
	Ptr<DescriptorMatcher> descmatch = FlannBasedMatcher::create();
	por.matchBases(descmatch, true);
	bestfactor = 0;
	for (int i = 1; i < 21; i++) {
		Mat conf = Mat(9, 9, CV_16UC1, Scalar(0));
		ssss.create(1.5+(0.05*i));
		ssss.reduceVector(por.distances, closed, przynal);
		por.computConf(przynal,fact, conf);
		if (fact > bestfactor) {
			cout << "Best matching:  " << fact << endl;
			cout << "For factor:  " << 1.5 + (0.05*i) << endl;
			bestfactor = fact;
			conf.copyTo(www);
		}
	}

	cout << "Rozmiar:  " << www.size << endl
		<< www << endl;

	getchar();
	return 0;
}