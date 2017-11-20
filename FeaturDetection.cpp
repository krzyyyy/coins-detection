// FeaturDetection.cpp : Defines the entry point for the console application.
//
#include <io.h>
#include <iostream>
#include <vector>
#include "opencv2/opencv.hpp"
#include <Windows.h>

using namespace cv;
using namespace std;
vector <Mat> listaObrazow;
vector <Mat> listaObrazowM;
/*Mat src,src2,rys,rys2;
Mat doc,doc2 , dst2;
int maxVar = 25, minDiv=20;
int treshmin = 0,treshmax=1;
int maxReg = 150, minReg = 50, delta = 200;
int min_margin = 3, blure=5;
Mat myMSER(int delta, int minReg, int maxReg, Mat org) {
	Mat r, t,wyy,wyy2;
	org.copyTo(t);
	vector < vector <Point> > obsz1;
	vector <RotatedRect> obr1;
	vector <Rect> pros1;
	Ptr<MSER> wsk = MSER::create(0, minReg, maxReg, 0,(minDiv/100.),delta,1.+(maxVar/100.),(min_margin/1000.),5);
	wsk->detectRegions(t, obsz1, pros1);
	//cout << "Start"<<endl;
	for (int i = 0; i < obsz1.size(); i++) {
		for (int j = 0; j < obsz1[i].size(); j++) {
			t.at<Vec3b>(obsz1[i][j]) = Vec3b(234453 * i % 90, 345234 * i % 90, 5677657 * i % 80);
		}
		//cout << i << "   "<<obsz1[i].size()<<endl;
	}
	//imwrite("C:/Users/ASUS/Desktop/BazaZdjec/klockiColor.jpg", t);
	return t;
}
void naPas(int, void*) {
	//rys = myMSER(delta, minReg, maxReg, src);
	//rys2= myMSER(delta, minReg, maxReg, src2);
	imshow("mon", myMSER(delta, minReg+10, maxReg ,src));
	//imshow("mon2", myMSER(delta, minReg + 10, maxReg, doc2, src2));
}
int main() {
	namedWindow("mon", WINDOW_NORMAL);
	//namedWindow("mon2", WINDOW_AUTOSIZE);
	createTrackbar("minimalny region", "mon", &minReg, 400, naPas);
	createTrackbar("maksymalny  region", "mon", &maxReg, 1000, naPas);
	createTrackbar("max evolution", "mon", &delta, 255, naPas);
	createTrackbar("area Tresholidng", "mon", &maxVar, 50, naPas);
	createTrackbar("minDiversity", "mon", &minDiv, 100, naPas);
	createTrackbar("blure ", "mon", &blure, 6, naPas);
	createTrackbar("min Margin", "mon", &min_margin, 300, naPas);
	src = imread("C:/Users/ASUS/Desktop/BAZA_ZDJEC/PIERWOTNE/IMG_20171004_120932.jpg");
	//src2 = imread("C:/Users/ASUS/Desktop/BazaZdjec/punkt.jpg");
	//resize(src2, src2, Size(src.cols*4,src.rows*4));
	if (src.empty() == true ) return -1;
	
	
	waitKey(0);
	return 0;
}
*/
int main()
{
	namedWindow("pokaz", WINDOW_NORMAL);
	namedWindow("pokaz2", WINDOW_NORMAL);
	_finddata_t danePliku;
	long uchwyt =_findfirst("C:/Users/ASUS/Desktop/BAZA_ZDJEC/PIERWOTNE/*.jpg", &danePliku);
	if (uchwyt != -1) {
		string sciezka = "C:/Users/ASUS/Desktop/BAZA_ZDJEC/PIERWOTNE/";
		listaObrazow.push_back(imread(sciezka + danePliku.name));
		cout << danePliku.name << endl;
		while (int szykDal = _findnext(uchwyt, &danePliku)!=-1) {
			listaObrazow.push_back(imread(sciezka + danePliku.name));
			cout << danePliku.name << endl;
		}
		_findclose(uchwyt);
		cout << listaObrazow.size() << endl;
		string nazwaZ = "MONETY_";
		string nazwaF = "C:/Users/ASUS/Desktop/BAZA_ZDJEC/POMNIEJSZONE/";

		/*Mat gray, kopia,kopia2;
		listaObrazow[0].copyTo(kopia);
		vector <Vec3f> circles;
		//pow(kopia, 2, kopia);
		cvtColor(kopia, gray, CV_BGR2GRAY);
		
		medianBlur(gray, gray, 3);
		HoughCircles(gray, circles, HOUGH_GRADIENT, 1, gray.rows / 100, 100, 60, 220 / 2, 640 / 2);
		for (size_t j = 0; j < circles.size(); j++) {
			Vec3i pom = circles[j];
			circle(kopia, Point(pom[0], pom[1]), pom[2], Scalar(200, 22, 22), 3, LINE_AA);
		}
		imshow("pokaz", kopia);
		waitKey(500);*/
		
		for (int i = 0; i < listaObrazow.size(); i++) {
			Mat gray,kopia ;
			listaObrazow[i].copyTo(kopia);
			vector <Vec3f> circles;
			cvtColor(listaObrazow[i], gray, CV_BGR2GRAY);
			medianBlur(gray, gray, 3);
			GaussianBlur(gray, gray, Size(3,3),1);
			HoughCircles(gray, circles, HOUGH_GRADIENT, 1, gray.rows/100 ,100 ,60 ,220/2,640/2);
			for (size_t j = 0; j < circles.size(); j++) {
				Vec3i pom = circles[j];

				circle(kopia, Point(pom[0], pom[1]),pom[2],Scalar(200,22,22),3,LINE_AA);
			}
			imshow("pokaz", kopia);
			imshow("pokaz2", gray);
			waitKey(500);

		}
		CreateDirectoryA("C:/Users/ASUS/Desktop/BAZA_ZDJEC/POMNIEJSZONE", NULL);
		/*for (int i = 0; i < listaObrazow.size(); i++) {
			stringstream ss;
			Mat pom;
			ss << i+1;
			resize(listaObrazow[i], pom, Size(listaObrazow[0].cols / 5, listaObrazow[0].rows / 5));
			listaObrazowM.push_back(pom);
			imwrite(nazwaF + "Pom" + ss.str()+".jpg", listaObrazowM[i]);
			//cout << nazwaF + "Pom" + ss.str() << endl;
			imshow("pokaz", listaObrazowM[i]);
			waitKey(500);
			
		}*/
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	waitKey(0);
	return 0;
}

/*
int main() {
	namedWindow("fram", WINDOW_AUTOSIZE);
	//namedWindow("wynik", WINDOW_AUTOSIZE);
	//namedWindow("wynik2", WINDOW_AUTOSIZE);
	Mat obr , lenaMat , wynik;
	obr = imread("C:/Users/ASUS/Desktop/BazaZdjec/klocki.jpg");
	obr.copyTo(src);
	cvtColor(obr, doc, CV_BGR2GRAY);
	vector < vector <Point> > obsz1;
	vector <RotatedRect> obr1;
	vector <Rect> pros1;
	Ptr<MSER> wsk = MSER::create(4, 50, 1000, 0.15);
	wsk->detectRegions(doc, obsz1, pros1);
	for (int i = 0; i < obsz1.size(); i++) {
		for (int j = 0; j < obsz1[i].size(); j++) {
			src.at<Vec3b>(obsz1[i][j]) = Vec3b(0 * i % 90, 0 * i % 90, 0 * i % 80);
		}
		//drawContours(src, obsz1, i, Scalar(2626637*i%256, 244243*i%255, 29382382*i%255));
		//cout << i << "   "<<obsz1[i].size()<<endl;
	}
	imshow("fram", src);
	imwrite("C:/Users/ASUS/Desktop/BazaZdjec/klockiOpenCVMSER.jpg", src);
	/*
	lenaMat = imread("C:/Users/ASUS/Desktop/BazaZdjec/lenaMatlab.bmp");
	absdiff(src, lenaMat, wynik);
	imshow("wynik", wynik);
	imshow("wynik2", lenaMat);
	imwrite("C:/Users/ASUS/Desktop/BazaZdjec/lenaOpenCV.bmp", src);
	imwrite("C:/Users/ASUS/Desktop/BazaZdjec/lenaroznica.bmp", wynik);
	
	waitKey();
	return 0;
}*/
