// OpenCVcontrib.cpp : Defines the entry point for the console application.
//
#include <vector>
#include "stdafx.h"
#include <io.h>
#include <iomanip>
#include "opencv2\features2d.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\xfeatures2d.hpp"
#include <iostream>
#include <math.h>
//#include "opencv2\xfeatures2d.hpp"
//#include "opencv2\xfeatures2d\nonfree.hpp"
#include <Windows.h>



using namespace std;
using namespace cv;
//using namespace xfeatures2d;

string naz[3][3] = { { "5zl","2zl","1zl" },{ "50gr","20gr","10gr" },{ "5gr","2gr","1gr" } };
double compfactor(vector <DMatch> in, vector <KeyPoint> keys) {
	if (2*keys.size() < in.size())return 0;
	double mean = 0, min = 10000 , factor=0;
	for each (DMatch i in in) {
		mean += i.distance;
		if (i.distance < min)min = i.distance;
	}
	mean/= in.size();
	return  (in.size() / keys.size()) + (min / mean);

}
void retRotRect(const RotatedRect rect, Mat  inPut, Mat & outPut) {
	Mat rotMatrix = getRotationMatrix2D(rect.center, rect.angle,1);
	Mat rotatedInPut;
	warpAffine(inPut, rotatedInPut, rotMatrix, Size(inPut.cols, inPut.rows));
	rotatedInPut(Rect(rect.center.x - (rect.size.width / 2), rect.center.y - (rect.size.height / 2), rect.size.width, rect.size.height)).copyTo(outPut);
	
}
void pokaz(const Mat &x ) {
	for (int i = 0; i < x.rows; i++) {
		cout << "[  ";
		for (int j = 0; j < x.cols&&i<5; j++) {
			cout << fixed << setprecision(3) << x.at<double>(Point(j,i)) << ",  ";
		}
		cout <<"  ]" << endl;
	}
}
void wysw(Mat &obr, vector<vector<Point>> klu) {
	for (int i = 0; i < klu.size(); i++) {
		for (int j = 0; j < klu[i].size(); j++) {
			obr.at<Vec3b>(klu[i][j]) = Vec3b(i*24234%256, i*10123%256, i*23490%256);
		}
	}
}
void wysw(Mat &obr, vector <KeyPoint> klu) {
	for (int i = 0; i < klu.size(); i++) {
		circle(obr, klu[i].pt, 4, Scalar(100, 120, 0), 1);
	}
}
void mySIFT(Mat &inPut,vector <KeyPoint> keys, const Mat &inPut2, vector <KeyPoint> keys2, double thres, vector <DMatch> &bestDist) {
	Mat descriptors, descriptors2;
	Mat inPutG, inPutG2;
	double minD=100000, maxD=0;
	Ptr<Feature2D> desc = xfeatures2d::SIFT::create();
	desc->compute(inPut, keys, descriptors);
	desc->compute(inPut2, keys2, descriptors2);
	BFMatcher matcher(NORM_L1);
	vector <DMatch> distances ;
	matcher.match(descriptors, descriptors2, distances);
	for (int i = 0; i < distances.size(); i++) {
		if (distances[i].distance > maxD)maxD = distances[i].distance;
		if (distances[i].distance < minD)minD = distances[i].distance;
	}

	for (int i = 0; i < distances.size(); i++) {
		if (distances[i].distance < thres* minD) bestDist.push_back(distances[i]);
	}

}
void normalizeMSER(const Mat &inPut, vector <vector<Point>> blobs, Mat &outDescriptor, vector <KeyPoint> &keys) {
	float *minSize, *maxSize;
	vector<KeyPoint> key(1);
	Ptr<Feature2D> ind = xfeatures2d::SIFT::create();
	Point2f center;
	vector <Mat> paths;
	Mat rotatedPath, normRotPath, copy;
	RotatedRect rotRect;
	Mat desc(0, 128, CV_32FC1);
	Mat rowOfDesc;

	for (int i = 0; i<blobs.size(); i++)
	{
		rotRect = minAreaRect(blobs[i]);
		retRotRect(rotRect, inPut, rotatedPath);
		if (rotRect.size.height > rotRect.size.width) {
			maxSize = &rotRect.size.height;
			minSize = &rotRect.size.width;
		}
		else {
			maxSize = &rotRect.size.width;
			minSize = &rotRect.size.height;
		}
		resize(rotatedPath, normRotPath, Size(*minSize, *minSize));
		key[0].pt = Point2f(*minSize / 2, *minSize / 2);
		key[0].size =  *minSize/2;
		key[0].class_id = -1;
		key[0].angle =  -rotRect.angle;
		ind->compute(normRotPath, key, rowOfDesc);
		key[0].pt =  rotRect.center;
		keys.push_back(key[0]);
		for (int i = 0; i < rowOfDesc.cols; i++)cout << rowOfDesc.at<float>(0, i) << "  ,";
		cout << "Koniec lini" << endl;
		vconcat(desc, rowOfDesc, desc);
		//imshow("obraz4", normRotPath);
		//waitKey(100);
	}
	desc.copyTo(outDescriptor);

}
void myMoment( vector <vector<Point>> blobs1, vector <vector< Point>> blobs2, double thres, vector <DMatch> &bestDist) {
	vector <Moments> mom1, mom2;
	vector <KeyPoint> keys1, keys2;
	Mat desc1(0,7, CV_64FC1), desc2(0, 7, CV_64FC1);
	Mat huMom1, huMom2;
	double minD=1000, maxD=0;
	for (int i = 0; i < blobs1.size(); i++) {
		mom1.push_back(moments(blobs1[i]));
		RotatedRect rect = fitEllipse(blobs1[i]);
		keys1.push_back(KeyPoint(rect.center,10));
	}
	for (int i = 0; i < blobs2.size(); i++) {
		mom2.push_back(moments(blobs2[i]));
		RotatedRect rect = fitEllipse(blobs2[i]);
		keys2.push_back(KeyPoint(rect.center, 10));
	}
	for (int i = 0; i < mom1.size(); i++) {
		HuMoments(mom1[i], huMom1);
		transpose(huMom1, huMom1);
		vconcat(desc1, huMom1, desc1);
	}
	for (int i = 0; i < mom2.size(); i++) {
		HuMoments(mom2[i], huMom2);
		transpose(huMom2,huMom2);
		vconcat(desc2, huMom2, desc2);
	}
	desc1.convertTo(desc1, CV_32FC1);
	desc2.convertTo(desc2, CV_32FC1);

	
	BFMatcher matcher;
	
	vector <DMatch> distances;
	matcher.match(desc1, desc2,distances);
	//for (int i = 0; i < distances.size(); i++)cout << distances[i].distance << endl;
	
	for (int i = 0; i < distances.size(); i++) {
		if (distances[i].distance > maxD)maxD = distances[i].distance;
		if (distances[i].distance < minD)minD = distances[i].distance;
	}
	for (int i = 0; i < distances.size(); i++) {
		if (distances[i].distance < thres*minD)bestDist.push_back(distances[i]);
	}
	
	
}
void myNormalizedSIFT(const Mat &inPut1, vector <vector<Point>> blobs1, const Mat &inPut2, vector <vector< Point>> blobs2, double thres, Mat &outPut) {
	namedWindow("obraz5", WINDOW_NORMAL);
	namedWindow("obraz6", WINDOW_NORMAL);
	vector <KeyPoint> keys5, keys2;
	vector <DMatch> distances, disProp;
	double min=1000000, max=0;
	Mat inPutG, inPutG2;
	Mat desc, desc2;
	normalizeMSER(inPut1, blobs1, desc, keys5);
	normalizeMSER(inPut2, blobs2, desc2, keys2);
	drawKeypoints(inPut1, keys5, inPutG);
	drawKeypoints(inPut2, keys2, inPutG2);
	FlannBasedMatcher macher;
	macher.match(desc, desc2, distances);
	for (int i = 0; i < distances.size(); i++) {
		//cout << distances[i].distance << endl;
		if (distances[i].distance < min)min = distances[i].distance;
		if (distances[i].distance > max)max = distances[i].distance;
	}
	for (int i = 0; i < distances.size(); i++) {
		if (distances[i].distance < 30)disProp.push_back(distances[i]);
	}
	drawMatches(inPut1, keys5, inPut2, keys2, disProp, outPut);
	imshow("obraz5", inPutG);
	imshow("obraz6", inPutG2);
	waitKey(0);
}
/*
int main()
{
	string pathObj, pathPat;
	long patHand, objHand;
	vector <DMatch> bestDist(1000);
	Mat bestIm, matchedIm,test;
	vector <KeyPoint> bestKey;
	namedWindow("obraz", WINDOW_NORMAL);
	namedWindow("obraz2", WINDOW_NORMAL);
	vector <Rect> prost;
	vector <Rect> prost2;
	Mat obr, obrG, obr2, obr2G;
	Ptr<MSER> wsk=MSER::create(4,30,15000,0.25);
	pathObj = "C:/Users/ASUS/Desktop/BAZA_ZDJEC/NOWE_MONETY/test/";
	pathPat = "C:/Users/ASUS/Desktop/BAZA_ZDJEC/NOWE_MONETY/ogolne/";
	_finddata_t pattern, object;
	objHand = _findfirst("C:/Users/ASUS/Desktop/BAZA_ZDJEC/NOWE_MONETY/test/*.tif", &object);
	if ( objHand == -1) return -1;
	
	do {
		vector <KeyPoint> keys;
		int licz = 0;
		patHand = _findfirst("C:/Users/ASUS/Desktop/BAZA_ZDJEC/NOWE_MONETY/ogolne/*.tif", &pattern);
		if (patHand == -1)return -1;
		obr = imread(pathObj + object.name);
		cvtColor(obr, obrG, CV_BGR2GRAY);
		wsk->detect(obrG, keys);
		do {
			vector <DMatch> dist;
			vector <KeyPoint> keys2;
			obr2 = imread(pathPat + pattern.name);
			cvtColor(obr2, obr2G, CV_BGR2GRAY);
			wsk->detect(obr2G, keys2);
			mySIFT(obr, keys, obr2, keys2, 1.4, dist);
			
			if (compfactor(dist,keys2) > compfactor(bestDist,bestKey)||licz==0) {
				bestDist = dist;
				obr2.copyTo(bestIm);
				bestKey = keys2;
				licz++;
			}
			drawMatches(obr, keys, obr2, keys2, dist, matchedIm);
			imshow("obraz2", matchedIm);
			waitKey(400);

		} while (_findnext(patHand,&pattern)!=-1);
		drawMatches(obr, keys, bestIm, bestKey, bestDist, matchedIm);
		imshow("obraz", matchedIm);
		waitKey(1000);
		_findclose(patHand);

	} while (_findnext(objHand,&object)!=-1);
	_findclose(objHand);






    return 0;
}
*/
