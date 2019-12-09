// ConsoleApplication2.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include "opencv/cv.hpp"
//#include "pch.h"
#include <iostream>
#include <ctime>




using namespace std;
using namespace cv;

#define ESCAPE 27

int main()
{

	Mat hola;
	Mat grises;
	Mat umbral;
	int a = 0;
	double calibracion[9] = { 4.5930409526695178e+02, 0, 320, 0, 4.5930409526695178e+02, 240, 0, 0, 1 };
	double distorsion[5] = { -1.1559474367321274e-01, 1.6127813266167305e-02, 0, 0, 2.4356166478716794e-02 };
	Mat calib = Mat(3, 3, DataType<double>::type, calibracion);
	Mat distor = Mat(5, 1, DataType<double>::type, distorsion);
	unsigned t0, t1;
	Mat homogra;
	Mat inversa;
	Mat transfor(Size(600, 600), CV_16SC1, Scalar(0, 0, 0));
	Mat transforgris(Size(600, 600), CV_16SC1, Scalar(0, 0, 0));
	Mat transforumbral(Size(600, 600), CV_16SC1, Scalar(0, 0, 0));

	int matriz[10][16] = {
		{1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0},
		{0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0},
		{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1},
		{1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0},
		{0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0},
		{0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1},
		{1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0},
		{1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0},
		{1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0},
		{1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0},
	};

	int correcto = 0;
	int correcto2 = 4;
	int id = 0;
	int posicion = 0;
	int perimetro;
	int lado;
	int arrayp[16];
	double to;
	double tf = 0;
	double fps;
	int r;
	vector<Point2d> vector_objetivo;
	Point2d p1 = Point2d(0, 0);
	Point2d p2 = Point2d(0, 600);
	Point2d p3 = Point2d(600, 600);
	Point2d p4 = Point2d(600, 0);
	vector_objetivo.push_back(p4);
	vector_objetivo.push_back(p3);
	vector_objetivo.push_back(p2);
	vector_objetivo.push_back(p1);

	vector<Point3d> vector_objetivo_3d;
	Point3d p1_3d = Point3d(0, 0, 0);
	Point3d p2_3d = Point3d(0, 600, 0);
	Point3d p3_3d = Point3d(600, 600, 0);
	Point3d p4_3d = Point3d(600, 0, 0);
	vector_objetivo_3d.push_back(p4_3d);
	vector_objetivo_3d.push_back(p3_3d);
	vector_objetivo_3d.push_back(p2_3d);
	vector_objetivo_3d.push_back(p1_3d);

	vector<Point3d> vector_proy_3d;
	Point3d p1_3dp = Point3d(0, 0, -600);
	Point3d p2_3dp = Point3d(0, 600, -600);
	Point3d p3_3dp = Point3d(600, 600, -600);
	Point3d p4_3dp = Point3d(600, 0, -600);
	vector_proy_3d.push_back(p4_3dp);
	vector_proy_3d.push_back(p3_3dp);
	vector_proy_3d.push_back(p2_3dp);
	vector_proy_3d.push_back(p1_3dp);

	vector<Point3d> vector_proy_3d_pir;
	Point3d p1_3dpp = Point3d(300, 300, -600);
	Point3d p2_3dpp = Point3d(0, 600, -600);
	Point3d p3_3dpp = Point3d(600, 600, -600);
	Point3d p4_3dpp = Point3d(600, 0, -600);
	Point3d p5_3dpp = Point3d(300, 300, -600);

	vector_proy_3d_pir.push_back(p5_3dpp);
	vector_proy_3d_pir.push_back(p4_3dpp);
	vector_proy_3d_pir.push_back(p3_3dpp);
	vector_proy_3d_pir.push_back(p2_3dpp);
	vector_proy_3d_pir.push_back(p1_3dpp);

	Mat rvec(3, 1, cv::DataType<double>::type);
	Mat tvec(3, 1, cv::DataType<double>::type);

	vector < Point2d> projectedPoints;
	vector < Point> projectedPointsp;

	Mat cannyImg(hola.size(), CV_16SC1, Scalar(0, 0, 0));

	vector < vector < Point>> contor;
	vector<Point> contour;
	vector<Point2f> contour2f;

	VideoCapture capture(0);
	char pressedkey = 0;

	bool success;

	if (!capture.isOpened())
	{
		cout << "Error " << endl;
	}
	else {
		namedWindow("L01_E03", CV_WINDOW_AUTOSIZE);
		namedWindow("grises1", CV_WINDOW_AUTOSIZE);
		namedWindow("umbral1", CV_WINDOW_AUTOSIZE);
		namedWindow("umbral2", CV_WINDOW_AUTOSIZE);
		while (pressedkey != ESCAPE)
		{
			t0 = clock();


			success = capture.read(hola);
			if (success == false) {
				cout << "no se puede leer" << endl;
				return 1;
			}
			cvtColor(hola, grises, CV_BGR2GRAY);
			Canny(grises, cannyImg, 100, 300);
			threshold(cannyImg, umbral, 150, 255, THRESH_BINARY);
			findContours(umbral, contor, RETR_LIST, CHAIN_APPROX_NONE);
			for (int i = 0; i < contor.size(); i++) {
				approxPolyDP(contor[i], contour, 0.1 * arcLength(contor[i], 1), 1);
				if (contour.size() == 4) {
					if (contourArea(contour, 1) > 1000) {
						Mat(contour).convertTo(contour2f, Mat(contour2f).type());
						perimetro = arcLength(contour, 1);
						lado = perimetro / 4;
						drawContours(hola, contor, i, Scalar(255, 0, 255), 2);
						homogra = findHomography(contour, vector_objetivo);
						warpPerspective(hola, transfor, homogra, Size(600, 600));
						cvtColor(transfor, transforgris, CV_BGR2GRAY);
						threshold(transforgris, transforumbral, 150, 255, THRESH_BINARY);
						for (r = 0; r < 4; r++) {
							for (int n = 0; n < 10; n++) {
								int p = 0;
								for (int j = 0; j < 4; j++) {
									for (int x = 0; x < 4; x++, p++) {
										arrayp[p] = transforumbral.at<uchar>(150 + 100 * j, 150 + 100 * x);
									}
								}
								for (int x = 0; x < 16; x++) {
									if (arrayp[x] == 255) {
										arrayp[x] = 1;
									}
								}
								correcto = 0;
								for (int x = 0; x < 16; x++) {

									if (arrayp[x] != matriz[n][x]) correcto = 1;

								}
								if (correcto == 0) {
									id = n;
									posicion = r;
									solvePnP(vector_objetivo_3d, contour2f, calib, distor, rvec, tvec);
									cout << id << endl;

									if (a == 1) {
										projectPoints(vector_proy_3d_pir, rvec, tvec, calib, distor, projectedPoints);
										line(hola, contour[0], projectedPoints[0], Scalar(255, 0, 255), 2);
										line(hola, contour[1], projectedPoints[0], Scalar(255, 0, 255), 2);
										line(hola, contour[2], projectedPoints[0], Scalar(255, 0, 255), 2);
										line(hola, contour[3], projectedPoints[0], Scalar(255, 0, 255), 2);
										
									}
									else {
										projectPoints(vector_proy_3d, rvec, tvec, calib, distor, projectedPoints);
										line(hola, contour[0], projectedPoints[0], Scalar(255, 0, 255), 2);
										line(hola, contour[1], projectedPoints[1], Scalar(255, 0, 255), 2);
										line(hola, contour[2], projectedPoints[2], Scalar(255, 0, 255), 2);
										line(hola, contour[3], projectedPoints[3], Scalar(255, 0, 255), 2);
										line(hola, projectedPoints[0], projectedPoints[1], Scalar(255, 0, 255), 2);
										line(hola, projectedPoints[1], projectedPoints[2], Scalar(255, 0, 255), 2);
										line(hola, projectedPoints[2], projectedPoints[3], Scalar(255, 0, 255), 2);
										line(hola, projectedPoints[3], projectedPoints[0], Scalar(255, 0, 255), 2);

									}

								}

							}

							rotate(transforumbral, transforumbral, 0);

						}
						
						
					}


				}


			}
			

			//cout << id << endl;
			imshow("LO1_EO3", hola);
			//imshow("grises1", grises);
			imshow("umbral1", umbral);
			imshow("umbral2", transforumbral);
			t1 = clock();
			fps = (1.0 / (double(t1 - t0) / CLOCKS_PER_SEC));
			//cout << fps << endl;
			pressedkey = waitKey(1);


		}
		destroyWindow("L01_E03");
		capture.release();

	}
}

// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln