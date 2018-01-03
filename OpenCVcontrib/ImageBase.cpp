#include "stdafx.h"
#include "ImageBase.h"
//#include <Windows.h>
#include <io.h>
using namespace std;


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

	}
}
ImageBase::ImageBase() {};
void ImageBase::computDesc(Ptr<Feature2D> detector , Ptr<Feature2D> descriptor) {
	detector->detect(images, keys);
	descriptor->compute(images, keys, desc);
}

