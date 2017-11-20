// FeaturDetection.cpp : Defines the entry point for the console application.
//
#include <fstream>
#include <io.h>
#include <iostream>
#include <vector>
#include "opencv2/opencv.hpp"
#include <Windows.h>
#define W 0
#define K 1
#define P 2
using namespace cv;
using namespace std;
vector <Mat> listaObrazow;
vector <Mat> listaObrazowM;
Mat kopia,gray,kopia2;
int i=0;
int wsk1 = 58, wsk2 = 50, rozm = 3;
Vec3f tablica[6][3];

void sortuj(vector<Vec3f> & obiekt, int wym) {
	if (wym > 2 || wym < 0) return;
	Vec3f pom;
	for (int i = 0; i < obiekt.size(); i++) {
		for (int j = 1; j < obiekt.size(); j++) {
			if (obiekt[j - 1][wym] > obiekt[j][wym]) {
				pom = obiekt[j - 1];
				obiekt[j - 1]= obiekt[j ];

				obiekt[j ] = pom;
			}
		}
	}
}
void klasyfikacja(const vector<Vec3f> & obiekt, Vec3f tab[6][3], float wier[]) {
	Vec3f obecny,nastepny;
	bool czyWier;

	obecny = obiekt[0];
	for (int i = 0,liczWier=0,liczKol=0; i < obiekt.size()-1;i=i) {
		
		tab[liczWier][liczKol] = obecny;
		liczKol++;
		czyWier = true;
		while (czyWier==true&&i<obiekt.size()-1){
			i++;
			nastepny = obiekt[i];
			if (obecny[1] + 400 > nastepny[1] && obecny[1] - 400 < nastepny[1]) {
				tab[liczWier][liczKol] = nastepny;
				liczKol++;
				if (i == obiekt.size() - 1) {
					float sred = 0;
					for (int k = 0; k < liczKol; k++) {
						sred += tab[liczWier][k][1];
					}
					sred /= liczKol;
					wier[liczWier] = sred;
				}
			}
			else {
				float sred = 0;
				for (int k = 0; k < liczKol; k++) {
					sred+= tab[liczWier][k][1];
				}
				sred /= liczKol;
				wier[liczWier]=sred;
				czyWier = false;
				obecny = nastepny;
				liczWier++;
				liczKol = 0;
			}
		}

	}
}
void klasyfikacja2(const vector<Vec3f> & obiekt, Vec3f tab[6][3],float kol[]) {
	Vec3f obecny, nastepny;
	bool czyWier;

	obecny = obiekt[0];
	for (int i = 0, liczWier = 0, liczKol = 0; i < obiekt.size() - 1; i = i) {

		tab[liczKol][liczWier] = obecny;
		liczKol++;
		czyWier = true;
		while (czyWier == true && i<obiekt.size() - 1) {
			i++;
			nastepny = obiekt[i];
			if (obecny[0] + 500 > nastepny[0] && obecny[0] - 500 < nastepny[0]) {
				tab[liczKol][liczWier] = nastepny;
				liczKol++;
				if (i == obiekt.size() - 1) {
					float sred = 0;
					for (int k = 0; k < liczKol; k++) {
						sred += tab[k][liczWier][0];
					}
					sred /= liczKol;
					kol[liczWier] = sred;
				}
			}
			else {
				float sred = 0;
				for (int k = 0; k < liczKol; k++) {
					sred += tab[k][liczWier][0];
				}
				sred /= liczKol;
				kol[liczWier] = sred;
				czyWier = false;
				obecny = nastepny;
				liczWier++;
				liczKol = 0;
			}
		}

	}

}
void kopi(Vec3f tab[6][3]) {
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 3; j++)
			tablica[i][j] = tab[i][j];
	}
}
void wysw(vector <Vec3f> circ, int wym) {
	for (int i = 0; i < circ.size(); i++)cout << circ[i][wym] << endl;
}
void wysw(Vec3f tab[6][3]) {
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 3; j++) {
			cout << "[ ";
			cout.width(8);
			cout <<left<<tab[i][j][0];
			cout << " , ";
			cout.width(8);
			cout << left << tab[i][j][1];
			cout << " ]";
		}
		cout << endl;
	}
}
void wyswKW(float wier[], float kol[]) {
	for (int i = 0; i < 3; i++) cout << kol[i] << "   ";
	for (int i = 0; i < 6; i++) cout << wier[i] << endl;
}
void uzup(Vec3f tab[6][3]) {
	float rzad[6];
	float kol[3];
	for (int i = 0; i < 6; i++) {
		int licz = 0;
		rzad[i] = 0;
		for (int j = 0; j < 3; j++) {
			if (tab[i][j][1] != 0.) {
				rzad[i] += tab[i][j][1];
				licz++;
			}
		}
		rzad[i] /= licz;
	}
	for (int i = 0; i < 3; i++) {
		int licz = 0;
		kol[i] = 0;
		for (int j = 0; j < 6; j++) {
			if (tab[j][i][0] != 0.) {
				kol[i] += tab[j][i][0];
				licz++;
			}	
		}
		kol[i] /= licz;
	}
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 3; j++) {
			if (tab[i][j][0] == 0.) {
				tab[i][j][0] = kol[j];
				tab[i][j][1] = rzad[i];
				tab[i][j][2] = 300;
			}
		}
	}
}
void sort2(Vec3f tab[6][3]) {
	Vec3f pom;
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 2; k++) {
				if (tab[i][k][0] > tab[i][k + 1][0]&&tab[i][k+1][0]!=0) {
					pom = tab[i][k];
					tab[i][k] = tab[i][k + 1];
					tab[i][k + 1] = pom;
				}
			}
		}
	}
}
void sort3(Vec3f tab[6][3], float wier[], float kol[]) {
	bool czyUzy[6][3];
	for (int i = 0; i < 6; i++)
		for (int j = 0; j < 3; j++)czyUzy[i][j] = false;

	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				if (tab[i][j][0]!=0.&&(tab[i][j][0] > kol[k] - 400 && tab[i][j][0] < kol[k] + 400)) {
					czyUzy[i][k] = true;
					k = 3;
				}
			}
		}
		for (int j = 0; j < 3; j++) {
			if (tab[i][j][0] == 0.) {
				for (int k = 0; k < 3; k++) {
					if (czyUzy[i][k] == false) {
						czyUzy[i][k] = true;
						tab[i][j][0] = kol[k];
						tab[i][j][1]= wier[i];
						k = 3;
					}
				}
			}
			
		}
	}

}
void zer(Vec3f tab[6][3]) {
	for(int i=0;i<6;i++)
		for (int j = 0; j < 3; j++) {
			tab[i][j][0] = 0.;
			tab[i][j][1] = 0.;
			tab[i][j][2] = 0.;
		}
}
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

	imshow("mon", myMSER(delta, minReg+10, maxReg ,src));
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

void wykryj(int, void*) {
	float wier[6];
	float kol[3];
	Vec3f tab[6][3];
	Vec3f tab2[6][3];
	listaObrazow[i].copyTo(kopia);
	kopia.copyTo(kopia2);
	vector <Vec3f> circles;
	cvtColor(listaObrazow[i], gray, CV_BGR2GRAY);
	medianBlur(gray, gray, 2*rozm+1);
	HoughCircles(gray, circles, HOUGH_GRADIENT, 1, gray.rows / 9, wsk1, wsk2, 220 / 2, 700 / 2);
	sortuj(circles, 0);
	wysw(circles, 0);
	klasyfikacja2(circles, tab,kol);
	cout << "Po klasyfikacji kolumn: " << endl;
	wysw(tab);
	zer(tab);
	sortuj(circles, 1);
	klasyfikacja(circles, tab, wier);
	cout << "Po klasyfikacji wierszy:" << endl;
	wysw(tab);
	sort3(tab, wier, kol);
	cout << "Po uzupelnieniu: " << endl;
	wysw(tab);
	sort2(tab);
	for (size_t j = 0; j < circles.size(); j++) {
		stringstream ss;
		ss << j + 1;
		Vec3i pom = circles[j];
		circle(kopia, Point(pom[0], pom[1]), pom[2], Scalar(200, 22, 22), 3, LINE_AA);
		putText(kopia,ss.str(), Point(pom[0], pom[1]), FONT_HERSHEY_COMPLEX, 3, Scalar(3, 21, 140), 6);
	}
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 3; j++) {
			stringstream ss;
			ss << "[ " << i << " , " << j << " ]";
			rectangle(kopia2, Point(tab[i][j][0] - 300, tab[i][j][1] - 300), Point(tab[i][j][0] + 300, tab[i][j][1] + 300),Scalar(20,150,30),6);
			putText(kopia2, ss.str(), Point(tab[i][j][0], tab[i][j][1]), FONT_HERSHEY_COMPLEX, 2, Scalar(200, 1, 150), 6);
		}
	}
	kopi(tab);
	imshow("pokaz", kopia);
	imshow("pokaz2", kopia2);
}

void wykryj2(int, void*) {
	listaObrazow[i].copyTo(kopia);
	vector <Mat> kanaly;
	Mat B, G, R;
	cvtColor(kopia, kopia, CV_BGR2HSV);
	split(kopia, kanaly);
	threshold(kanaly[0], B, wsk1, 255, THRESH_BINARY);
	threshold(kanaly[1], G, wsk2, 255, THRESH_BINARY);
	threshold(kanaly[2], R, rozm, 255, THRESH_BINARY);
	imshow("pokaz", B);
	imshow("pokaz2", G);
	imshow("pokaz3", R);
}

int main()
{
	int a;
	string naz[] = { "5zl","2zl","1zl","50gr","20gr","10gr","5gr","2gr","1gr" };


	namedWindow("pokaz", WINDOW_NORMAL);
	namedWindow("pokaz2", WINDOW_NORMAL);
	namedWindow("pokaz3", WINDOW_NORMAL);
	createTrackbar("wsk1", "pokaz", &wsk1, 255, wykryj);
	createTrackbar("wsk2", "pokaz", &wsk2, 255, wykryj);
	createTrackbar("rozm1", "pokaz", &rozm, 255, wykryj);
	_finddata_t danePliku;
	long uchwyt =_findfirst("C:/Users/ASUS/Desktop/BAZA_ZDJEC/BAZA_ZDJEC_TIFF/*.tif", &danePliku);
	if (uchwyt != -1) {
		string sciezka = "C:/Users/ASUS/Desktop/BAZA_ZDJEC/BAZA_ZDJEC_TIFF/";
		listaObrazow.push_back(imread(sciezka + danePliku.name));
		cout << danePliku.name << endl;
		int licz = 0;
		while (int szykDal = _findnext(uchwyt, &danePliku) != -1) {
			listaObrazow.push_back(imread(sciezka + danePliku.name));
			cout << danePliku.name << endl;
			licz++;
		}
		_findclose(uchwyt);
		cout << listaObrazow.size() << endl;
		string nazwaZ = "MONETY_";
		string nazwaF = "C:/Users/ASUS/Desktop/BAZA_ZDJEC/POMNIEJSZONE/";
		string nazwaF2 = "C:/Users/ASUS/Desktop/BAZA_ZDJEC/POMNIEJSZONE2/";
		CreateDirectoryA("C:/Users/ASUS/Desktop/BAZA_ZDJEC/POMNIEJSZONE2", NULL);
			CreateDirectoryA("C:/Users/ASUS/Desktop/BAZA_ZDJEC/POMNIEJSZONE2/5zl", NULL);
			CreateDirectoryA("C:/Users/ASUS/Desktop/BAZA_ZDJEC/POMNIEJSZONE2/2zl", NULL);
			CreateDirectoryA("C:/Users/ASUS/Desktop/BAZA_ZDJEC/POMNIEJSZONE2/1zl", NULL);
			CreateDirectoryA("C:/Users/ASUS/Desktop/BAZA_ZDJEC/POMNIEJSZONE2/50gr", NULL);
			CreateDirectoryA("C:/Users/ASUS/Desktop/BAZA_ZDJEC/POMNIEJSZONE2/20gr", NULL);
			CreateDirectoryA("C:/Users/ASUS/Desktop/BAZA_ZDJEC/POMNIEJSZONE2/10gr", NULL);
			CreateDirectoryA("C:/Users/ASUS/Desktop/BAZA_ZDJEC/POMNIEJSZONE2/5gr", NULL);
			CreateDirectoryA("C:/Users/ASUS/Desktop/BAZA_ZDJEC/POMNIEJSZONE2/2gr", NULL);
			CreateDirectoryA("C:/Users/ASUS/Desktop/BAZA_ZDJEC/POMNIEJSZONE2/1gr", NULL);

		for ( i = 0; i < listaObrazow.size(); i++) {
			Mat dupl,dupl2,dupl3;
			listaObrazow[i].copyTo(dupl);
			wykryj(NULL,NULL);
			waitKey(1500);
			for (int k = 0; k < 3; k++) {
				for (int j = 0; j < 3; j++) {
					int x, y,szer,wys;
					stringstream ss;
					ss << i;
					if (int(tablica[k][j][0]) - 300 > 0) {
						x = int(tablica[k][j][0]) - 300;
						szer = 600;
						
					}
					else {
						x = 0;
						szer = 2 * int(tablica[k][j][0]);
					}
					if (int(tablica[k][j][1]) - 300 > 0) {
						y = int(tablica[k][j][1]) - 300;
						wys = 600;
						
					}
					else {
						y = 0;
						wys = 2 * int(tablica[k][j][1]);
					}

					dupl(Rect(x, y, szer, wys)).copyTo(dupl2);
					resize(dupl2, dupl3, Size(dupl2.cols / 5, dupl2.rows / 5));
					imwrite(nazwaF2 + naz[j*(k + 1)] + "/" + "M" + ss.str() + ".tif",dupl3);
					imshow("pokaz3", dupl3);
					waitKey(300);
				}
			}
		}


	}
	getchar();
	return 0;
}

