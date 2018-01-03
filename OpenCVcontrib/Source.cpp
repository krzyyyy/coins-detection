#include "stdafx.h"
#include "ImageBase.h"
#include "ConfMat.h"




using namespace std;
using namespace cv;



int main() {
	ImageBase x("C:/Users/ASUS/Desktop/BAZA_ZDJEC/NOWE_MONETY/ogolne/", "tif");
	ConfMat por(x, x);
	Ptr<MSER> detect = MSER::create(4, 20, 12000, 0.25);
	Ptr<Feature2D> descript = xfeatures2d::SIFT::create();
	x.computDesc(detect, descript);
	for (int i = 0; i < por.inputbase.name.size(); i++)cout << por.inputbase.name[i]  << endl;
	getchar();
	return 0;
}