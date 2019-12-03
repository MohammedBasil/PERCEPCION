// ConsoleApplication2.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include "opencv/cv.hpp"
#include "pch.h"
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
	double calibracion[9] = { 9.1334451621431538e+02, 0, 640, 0, 9.1334451621431538e+02, 360, 0, 0, 1 };
	double distorsion[5] = { 5.9933687464190685e-02, -2.5927601997166805e-01, 0, 0, 1.4176938948850759e-01 };
	Mat calib;
	Mat distor;
	Mat homogra;
	Mat transfor(Size(600, 600), CV_16SC1, Scalar(0, 0, 0));
	Mat transforgris;
	Mat transforumbral;

	calib = Mat(3, 3, DataType<double>::type, calibracion);
	distor = Mat(3, 3, DataType<double>::type, distorsion);
	int perimetro;
	int lado;
	double to;
	double tf=0;
	unsigned fps;
	vector<Point2d> vector_objetivo;
	Point2d p1 = Point2d(0, 0);
	Point2d p2 = Point2d(0, 600);
	Point2d p3 = Point2d(600, 0);
	Point2d p4 = Point2d(600, 600);
	vector_objetivo.push_back(p1);
	vector_objetivo.push_back(p2);
	vector_objetivo.push_back(p3);
	vector_objetivo.push_back(p4);

	/*int diagonal1x[6];
	int diagonal2x[6];
	int diagonal1y[6];
	int diagonal2y[6];
	int linea1x[4];
	int linea2x[4];
	int linea3x[4];
	int linea4x[4];
	int linea1y[4];
	int linea2y[4];
	int linea3y[4];
	int linea4y[4];
	
	Point esquina1, esquina2, esquina3, esquina4;
	*/
	int esquinazo = -1;
	
	vector < vector < Point>> contor;
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
		while (pressedkey != ESCAPE)
		{
			
			
			success = capture.read(hola);
			if (success == false) {
				cout << "no se puede leer" << endl;
				return 1;
			}
			cvtColor(hola, grises, CV_BGR2GRAY);
			threshold(grises, umbral, 150, 255, THRESH_BINARY);
			findContours(umbral, contor, RETR_LIST, CHAIN_APPROX_NONE);
			//drawContours(hola, contor, -1, Scalar(255, 0, 255), 2);
			for (int i = 0; i < contor.size(); i++) {
				approxPolyDP(contor[i], contor[i], 0.1*arcLength(contor[i], 1), 1);
				if (contor[i].size() == 4) {
					if (contourArea(contor[i], 1) > 1000) {
						perimetro = arcLength(contor[i], 1);
						lado = perimetro / 4;
						drawContours(hola, contor, i, Scalar(255, 0, 255), 2);
						circle(hola, contor[i][0], 6, Scalar(255, 0, 0), -1);
						circle(hola, contor[i][1], 6, Scalar(255, 0, 0), -1);
						circle(hola, contor[i][2], 6, Scalar(255, 0, 0), -1);
						circle(hola, contor[i][3], 6, Scalar(255, 0, 0), -1);
						homogra = findHomography(contor[i], vector_objetivo);
						warpPerspective(hola, transfor, homogra, Size(600, 600));
						cvtColor(transfor, transforgris, CV_BGR2GRAY);
						threshold(transforgris, transforumbral, 150, 255, THRESH_BINARY);



					}
					

				}


			} 
			   /* esquina1 = contor[esquinazo][0];
				esquina2 = contor[esquinazo][1];
				esquina3 = contor[esquinazo][2];
				esquina4 = contor[esquinazo][3];
				diagonal1x[0] = esquina1.x;
				diagonal1x[5] = esquina3.x;
				diagonal1y[0] = esquina1.y;
				diagonal1y[5] = esquina3.y;
				diagonal2x[0] = esquina2.x;
				diagonal2x[5] = esquina4.x;
				diagonal2y[0] = esquina2.y;
				diagonal2y[5] = esquina4.y;

				for (int j = 1; j < 5; j++) {
					int inicio;
					if (diagonal1x[0] <= diagonal1x[5]) {
						inicio = diagonal1x[0];
					}
					else inicio = diagonal1x[5];
					diagonal1x[j] = inicio + (abs((diagonal1x[5] - diagonal1x[0]) / 12)*(2 * j + 1));

				}
				for (int j = 1; j < 5; j++) {
					int inicio;
					if (diagonal1y[0] <= diagonal1y[5]) {
						inicio = diagonal1y[0];
					}
					else inicio = diagonal1y[5];
					diagonal1y[j] = inicio + (abs((diagonal1y[5] - diagonal1y[0]) / 12)*(2 * j + 1));

				}
				for (int j = 1; j < 5; j++) {
					int inicio;
					if (diagonal2x[0] <= diagonal2x[5]) {
						inicio = diagonal2x[0];
					}
					else inicio = diagonal2x[5];
					diagonal2x[j] = inicio + (abs((diagonal2x[5] - diagonal2x[0]) / 12)*(2 * j + 1));

				}
				for (int j = 1; j < 5; j++) {
					int inicio;
					if (diagonal2y[0] <= diagonal2y[5]) {
						inicio = diagonal2y[0];
					}
					else inicio = diagonal2y[5];
					diagonal2y[j] = inicio + (abs((diagonal2y[5] - diagonal2y[0]) / 12)*(2 * j + 1));

				}
					Point diago1 (diagonal1x[1], diagonal1y[1]);
					Point diago2 (diagonal1x[2], diagonal1y[2]);
					Point diago3 (diagonal1x[3], diagonal1y[3]);
					Point diago4 (diagonal1x[4], diagonal1y[4]);
				circle(hola,diago1, 6, Scalar(255, 0, 0), -1);
				circle(hola,diago2, 6, Scalar(255, 0, 0), -1);
				circle(hola,diago3, 6, Scalar(255, 0, 0), -1);
				circle(hola,diago4, 6, Scalar(255, 0, 0), -1);
			
				 linea1x[0] = diagonal1x[1];
				linea1x[3] = diagonal2x[1];
				linea1y[0] = diagonal1y[1];
				linea1y[3] = diagonal2y[1];

				for (int j = 1; j < 3; j++) {

					linea1x[j] = linea1x[0] + abs((linea1x[3] - linea1x[0]) / 3)*j;
					

				}
				*/
				
			imshow("LO1_EO3", hola);
			//imshow("grises1", grises);
			imshow("umbral1", umbral);
			
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
