#include "stdafx.h"
//#include "ImageBase.h"
#include "ConfMat.h"
#include "MinDistRed.h"
#include "ShowMatches.h"




using namespace std;
using namespace cv;



int main() {
	Mat www;
	double fact, bestfactor, meanbestfactor, meanfact;
	vector <unsigned int > przynal, meanprzynal;
	/*Ptr<MyMSER> mymser;
	Ptr<Feature2D> mser = MSER::create(4, 32, 12000, 0.4);
	Ptr<Feature2D> sift = xfeatures2d::SIFT::create();
	
	ImageBase x("C:/Users/ASUS/Desktop/NOWA_BAZA/TESTS/", "tif");
	mymser->detect(x.images, x.keys);
	//mymser->detect(x.images, x.keys ,'a');
	//x.view();*/


	
	ShowMatches view;
	Ptr<MSER> mser = MSER::create(4, 40, 1000, 0.25);
	Ptr<Feature2D> sift = xfeatures2d::SIFT::create();
	Ptr<Feature2D> brisk = BRISK::create();
	Ptr<Feature2D> orb = ORB::create();
	Ptr<Feature2D> surf = xfeatures2d::SURF::create();
	Ptr<Feature2D> brief = xfeatures2d::BriefDescriptorExtractor::create();

	Ptr<Feature2D> wsk[7];
	wsk[0] = mser;
	wsk[1] =sift;
	wsk[2] = brisk;
	wsk[3] = orb;
	wsk[4] = surf;
	wsk[5] = brief;
	string detectors[] = { "mser","sift","brisk", "orb" , "surf", "brief" };
	Ptr<DescriptorMatcher> descmatch = BFMatcher::create();
	for (int k = 0; k < 5; k++) {
		ConfMat por("C:/Users/ASUS/Desktop/NOWA_BAZA/TESTS/", "tif", "C:/Users/ASUS/Desktop/NOWA_BAZA/PATTERNS/", "tif", wsk[k], sift);
 		por.matchBases(descmatch, false);
		bestfactor = 0;
		meanbestfactor = 0;
		for (int i = 1; i < 31; i++) {
			vector <vector <vector<DMatch>>> closed,redcomp,inputcomp;
			MinDistRed ssss = MinDistRed(1.2 + (0.05*i));
			Mat conf = Mat(9, 9, CV_16UC1, Scalar(0));
			ssss.reduceVector(por.distances, closed, przynal);
			redcomp = ssss.composit(closed, por.idoutput);
			inputcomp = ssss.composit(por.distances, por.idoutput);
			
			por.computConf(przynal, fact, conf);
			if (fact > bestfactor) {
				cout << "detector:  " << detectors[k] << endl;
				cout << "Best matching:  " << fact << endl;
				cout << "For factor:  " << 1.2 + (0.05*i) << endl;
				cout << "Matrix:   \n" << conf << endl;
				bestfactor = fact;
				conf.copyTo(www);
			}
		}
	}
		
	cout << "Rozmiar:  " << www.size << endl
		<< www << endl;

	getchar();
	return 0;
}