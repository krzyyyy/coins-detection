
#include "stdafx.h"
#include "ShowMatches.h"


ShowMatches::ShowMatches() {
}

void ShowMatches::showMatches(const ConfMat &x, const vector <vector <vector<DMatch>>> &matches) {
	namedWindow("image", WINDOW_AUTOSIZE);
	Mat img;
	for (int i = 0; i < x.inputbase.images.size(); i++) {
		for (int j = 0; j < x.outputbase.images.size(); j++) {
			drawMatches(x.inputbase.images[i], x.inputbase.keys[i], x.outputbase.images[j], x.outputbase.keys[j], matches[i][j], img);
			imshow("image", img);
			waitKey(300);
		}
	}
}