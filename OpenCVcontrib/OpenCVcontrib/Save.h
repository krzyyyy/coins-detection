#pragma once
#include "stdafx.h"
#include "ImageBase.h"

using namespace std;

class Save
{
public:
	Save();
	void saveImageBase(const string dest, ImageBase base);
	ImageBase readImageBase(const string dest);


private:

};
