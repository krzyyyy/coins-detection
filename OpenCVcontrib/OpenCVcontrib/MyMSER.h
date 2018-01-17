#pragma once
#include "stdafx.h"

using namespace std;
using namespace cv;

class MyMSER: public Feature2D
{
public:
	MyMSER();
	~MyMSER();
	void detect(vector <Mat> images, vector<vector<KeyPoint>>& keypoints,
		InputArrayOfArrays masks = noArray()) {
		Ptr<MSER> ss = MSER::create(6, 60,300,0.25);
		namedWindow("im", WINDOW_AUTOSIZE);
		keypoints.clear();
		vector <vector< Point>> sets;
		vector <Rect > boxes;
		for (int i = 0; i < images.size(); i++) {
			keypoints.push_back(vector<KeyPoint>(0));
			ss->detectRegions(images[i], sets, boxes);
			Point2f pkt[4];
			for (int j = 0; j < sets.size(); j++) {
				RotatedRect elip = fitEllipse(sets[j]);
				KeyPoint key;
				key.pt = elip.center;
				key.size = (elip.size.height + elip.size.width) / 3;
				key.angle = elip.angle;
				keypoints[i].push_back(key);
				ellipse(images[i], elip, Scalar(0, 0, 230), 2);
				elip.points(pkt);
				for (int k = 0; k < 4; k++) line(images[i], pkt[k], pkt[(k + 1) % 4], Scalar(0), 2);
			}
			imshow("im", images[i]);
			waitKey(0);

		}
	}
	
private:

};

MyMSER::MyMSER()
{
}

MyMSER::~MyMSER()
{
}