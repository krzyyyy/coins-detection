#include "stdafx.h"
#include "ImageBase.h"
#include "ConfMat.h"
#include "MinDistRed.h"




using namespace std;
using namespace cv;



int main() {
	vector <vector <vector<DMatch>>> closed;
	vector <unsigned int > przynal;
	//ImageBase x("C:/Users/ASUS/Desktop/BAZA_ZDJEC/NOWE_MONETY/ogolne/", "tif");
	MinDistRed ssss;
	Ptr<MSER> detect = MSER::create(4, 20, 12000, 0.25);
	Ptr<Feature2D> descript = xfeatures2d::SIFT::create();
	ConfMat por(ImageBase("C:/Users/ASUS/Desktop/BAZA_ZDJEC/NOWE_MONETY/ogolne/", "tif"), ImageBase("C:/Users/ASUS/Desktop/BAZA_ZDJEC/NOWE_MONETY/ogolne/", "tif"), detect, descript);
	Ptr<DescriptorMatcher> descmatch = FlannBasedMatcher::create();
	por.matchBases(descmatch, true);
	ssss.create(2);
	ssss.reduceVector(por.distances, closed, przynal);
	por.computConf(przynal);
	cout << "Rozmiar:  " << por.confmatrix.size << endl
		<< por.confmatrix << endl;
	cout << "dopasowania:  " << endl;
	for (int i = 0; i < przynal.size(); i++)
		cout << por.idinput[i] << "  -->  " << por.idoutput[przynal[i]] << endl;
	getchar();
	return 0;
}