
#include "stdafx.h"
#include "ImageBase.h"
//#include <Windows.h>
#include <io.h>



ImageBase::ImageBase(const string path, const string fileExt) {
	string fullname = path+ "*." + fileExt;
	_finddata_t objects;
	long objHand;
	objHand = _findfirst(fullname.c_str(), &objects);
	if (objHand == -1) empty=true;
	else {
		empty = false;
		do {
			name.push_back(objects.name);
			 images.push_back(imread(path + objects.name,0));
		} while (_findnext(objHand, &objects) != -1);
		_findclose(objHand);
		//string naz[] = { "5zl", "2zl", "1zl", "50gr", "20gr", "10gr", "5gr", "2gr", "1gr" };
		//for (int i = 0; i < 9; i++)this->naz[i] = naz[i];
	}
}
ImageBase::ImageBase() {};
void ImageBase::computDesc(Ptr<Feature2D> detector , Ptr<Feature2D> descriptor) {
	detector->detect(images, keys );
	descriptor->compute(images, keys, desc);
}
void ImageBase::view() {
	namedWindow("images", WINDOW_AUTOSIZE);
	for (int i = 0; i < images.size(); i++) {
		imshow("images", images[i]);
		waitKey(0);
	}
}
/*void ImageBase::savePatterns(const vector <Mat> img, const vector < Vec3f[6][3]> coins, const string path, string ext) {
	Mat dupl;
	//CreateDirectoryA(path.c_str(), NULL);
	for (int i = 0; i < 9; i++) {
		string destpath = path + naz[i];
		//CreateDirectoryA(destpath.c_str(), NULL);
	}
	for (int i = 0; i < img.size(); i++) {
		for (int k = 0; k < 3; k++) {
			for (int j = 0; j < 3; j++) {
				int x, y, szer, wys;
				stringstream ss;
				ss << i;
				if (int(coins[i][k][j][0]) - 300 > 0) {
					x = int(coins[i][k][j][0]) - 300;
					szer = 600;

				}
				else {
					x = 0;
					szer = 2 * int(coins[i][k][j][0]);
				}
				if (int(coins[i][k][j][1]) - 300 > 0) {
					y = int(coins[i][k][j][1]) - 300;
					wys = 600;

				}
				else {
					y = 0;
					wys = 2 * int(coins[i][k][j][1]);
				}

				img[i](Rect(x, y, szer, wys)).copyTo(dupl);
				//imwrite(path + naz[k][j] + "/" + "M-"+ naz[k][j]+"-" + ss.str() + ".tif",dupl);
				imshow("pokaz3", dupl);
				waitKey(100);
			}
		}
	}
}

*/