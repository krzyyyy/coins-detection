#include "stdafx.h"
#include "ImageBase.h"
#include "ConfMat.h"

using namespace std;
using namespace cv;
void ConfMat::matchBases( Ptr<DescriptorMatcher> matcher, bool show) {

	for (int i = 0; i < inputbase.desc.size(); i++) {
		for (int j = 0; j < outputbase.desc.size(); j++) {
			matcher->match(inputbase.desc[i], outputbase.desc[j], distances[i][j]);
			if (show == true) {
				namedWindow("image", WINDOW_AUTOSIZE);
				Mat img;
				drawMatches(inputbase.images[i],inputbase.keys[i], outputbase.images[j], outputbase.keys[j], distances[i][j], img);
				imshow("image", img);
				waitKey(200);
			}

		}
	}
}
void ConfMat::matchSelfBase(ImageBase &in, Ptr<DescriptorMatcher> matcher, bool show) {
	for (int i = 0; i < in.desc.size(); i++) {
		for (int j = i; j < in.desc.size(); j++) {
			matcher->match(in.desc[i], in.desc[j], distances[i][j]);
		}
	}
}
ConfMat::ConfMat(ImageBase &inputbase, ImageBase &outputbase) {
	this->inputbase = inputbase;
	this->outputbase = outputbase;
	string names[] = { "1gr", "2gr", "5gr", "10gr", "20gr", "50gr", "1zl", "2zl", "5zl" };
	nominal = vector <string>(names, names + sizeof(names) / sizeof(string));
	setNames(nominal, inputbase, idinput);
	setNames(nominal, outputbase, idoutput);
}
void ConfMat::setNames(const vector <string> &names, ImageBase &base, vector < vector<unsigned int>> &id) {
	for (int i = 0; i < base.name.size(); i++) {
		for (int j = 0; j < names.size(); j++) {
			id.push_back(vector<unsigned int> (0));
			if (base.name[i].find(names[j]) != string::npos) {
				id[j].push_back(i);
				j = names.size();
			}
		}
	}
}