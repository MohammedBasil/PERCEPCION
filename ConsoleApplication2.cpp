// ConsoleApplication2.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include "opencv/cv.hpp"
#include <string>								//librería usada para convertir ints en strings 

//#include "pch.h"
#include <iostream>
#include <ctime>								//librería usada para el control de fps




using namespace std;
using namespace cv;

#define ESCAPE 27
#define SPACE 32

int main()
{

	Mat hola;									//matriz original en la que se dibuja todo
	Mat grises;									//matriz hola en blanco y negro
	Mat umbral;									// matriz de grises umbralizada 
	bool f;                                  // variable para combio de cubo a piramide
	double calibracion[9] = { 4.5930409526695178e+02, 0, 320, 0, 4.5930409526695178e+02, 240, 0, 0, 1 };  //array de calibracion de la camara
	double distorsion[5] = { -1.1559474367321274e-01, 1.6127813266167305e-02, 0, 0, 2.4356166478716794e-02 };  //array de distorsion de la camara
	Mat calib = Mat(3, 3, DataType<double>::type, calibracion);					// conversión del array de calibración a una matriz 3x3
	Mat distor = Mat(5, 1, DataType<double>::type, distorsion);					// conversión del array de distorsión a una matriz 5x1
	unsigned t0, t1;                                                            // variables de tiempo inicial y tiempo final
	Mat homogra;																// matriz de cambio de 3d a 2d 
	Mat inversa;
	Mat transfor(Size(600, 600), CV_16SC1, Scalar(0, 0, 0));                    //matriz 600x600 en la que aplicamos la transformacion 2 d al contorno
	Mat transforgris(Size(600, 600), CV_16SC1, Scalar(0, 0, 0));				//matriz 600x600 en la que aplicamos escala de grises
	Mat transforumbral(Size(600, 600), CV_16SC1, Scalar(0, 0, 0));				//matriz 600x600 en la que aplicamos umbralización


	int matriz[10][16] = {                                                      //matriz que almacena codificado todos los arucos 
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

	int correcto = 0;															//variale que usamos para detectar que si sigue siendo 0 ha detectado algun aruco en alguna posición
	int correcto2 = 4;
	int id = 0;																	//variable que nos indica el id
	int posicion = 0;															//variable que almacena en que rotación hemos detectado el aruco
	int perimetro;																// variable del perimetro del aruco detectado
	int lado;																	// lado del aruco detectado
	int arrayp[16];																// array que almacena el identificador del aruco de la matriz 600x600

	double fps;																	//variable usada para contar fps
	int r;																		//variable que tiene el fin de contar las rotaciones
	vector<Point2d> vector_objetivo;											//vector que tiene los puntos de las esquinas de la matriz 600x600
	Point2d p1 = Point2d(0, 0);													//asignación de las esquinas a los puntos
	Point2d p2 = Point2d(0, 600);
	Point2d p3 = Point2d(600, 600);
	Point2d p4 = Point2d(600, 0);
	vector_objetivo.push_back(p4);												//almacenamiento de los puntos en el vector
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
	Point3d p6_3dp = Point3d(300, 300, 0);

	vector_proy_3d.push_back(p4_3dp);
	vector_proy_3d.push_back(p3_3dp);
	vector_proy_3d.push_back(p2_3dp);
	vector_proy_3d.push_back(p1_3dp);
	vector_proy_3d.push_back(p6_3dp);


	vector<Point3d> vector_proy_3d_pir;
	Point3d p1_3dpp = Point3d(0, 0, -600);
	Point3d p2_3dpp = Point3d(0, 600, -600);
	Point3d p3_3dpp = Point3d(600, 600, -600);
	Point3d p4_3dpp = Point3d(600, 0, -600);
	Point3d p5_3dpp = Point3d(300, 300, -600);
	Point3d p6_3dpp = Point3d(300, 300, 0);

	vector_proy_3d_pir.push_back(p5_3dpp);
	vector_proy_3d_pir.push_back(p4_3dpp);
	vector_proy_3d_pir.push_back(p3_3dpp);
	vector_proy_3d_pir.push_back(p2_3dpp);
	vector_proy_3d_pir.push_back(p1_3dpp);
	vector_proy_3d_pir.push_back(p6_3dpp);


	Mat rvec(3, 1, cv::DataType<double>::type);
	Mat tvec(3, 1, cv::DataType<double>::type);

	vector < Point2d> projectedPoints;
	vector < Point> projectedPointsp;

	Mat cannyImg(hola.size(), CV_16SC1, Scalar(0, 0, 0));

	vector < vector < Point>> contor;    //vector de vectores de puntos que almacenan los puntos de las 4 esquinas de cada aruco detectado
	vector<Point> contour;
	vector<Point2f> contour2f;

	VideoCapture capture(0);
	char pressedkey = 0;

	bool success;
	f = 1;
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
			t0 = clock();																//comienzo de la cuenta para fps

			success = capture.read(hola);
			if (success == false) {
				cout << "no se puede leer" << endl;
				return 1;
			}
			cvtColor(hola, grises, CV_BGR2GRAY);										//tranformación de la mmatriz original a escala de grises
			//Canny(grises, cannyImg, 100, 300);
			threshold(grises, umbral, 100, 255, THRESH_BINARY);							//umbralización de la matriz de grises
			findContours(umbral, contor, RETR_LIST, CHAIN_APPROX_NONE);                 //encontramos contornos en la matriz umbral

			for (int i = 0; i < contor.size(); i++) {									//bucle en lo que tenemos todos los contornos detectados
				approxPolyDP(contor[i], contour, 0.1 * arcLength(contor[i], 1)*0.1, 1);     //filtramos los contornos por figuras poligonales
				if (contour.size() == 4) {												// condición que tenga 4 lados
					if (contourArea(contour, 1) > 1000) {								// filtrado del contorno por el area
						Mat(contour).convertTo(contour2f, Mat(contour2f).type());
						perimetro = arcLength(contour, 1);
						lado = perimetro / 4;
						drawContours(hola, contor, i, Scalar(255, 0, 255), 2);			//dibujamos los contornos resultantes en la matriz hola
						homogra = findHomography(contour, vector_objetivo);				//guardamos la metriz de transformacion de los contornos a vector objetivo
						warpPerspective(hola, transfor, homogra, Size(600, 600));       //pasamos la imagen del contorno a 2d en tranfor
						cvtColor(transfor, transforgris, CV_BGR2GRAY);					// conversión a grises de transfor
						threshold(transforgris, transforumbral, 150, 255, THRESH_BINARY);  //umbralización de transforgrises
						for (r = 0; r < 4; r++) {									    //bucle para recorrer todas las rotaciones
							for (int n = 0; n < 10; n++) {								//bucle para recorrer la metriz de arucos
								int p = 0;												//inicializar la variable que nos da la posición de arrayp para poder esribir encima de arrayp
								for (int j = 0; j < 4; j++) {							//estos dos bucles sirven para poder recorrer la imagen para detectar la codificación del aruco
									for (int x = 0; x < 4; x++, p++) {
										arrayp[p] = transforumbral.at<uchar>(150 + 100 * j, 150 + 100 * x);  // asignación a arrayp de los valores de color de los puntos seleccionados
									}
								}
								for (int x = 0; x < 16; x++) {							//bucle que hace que el valor 1 sea el equivalente al 255 y se sobreescribe en arrayp
									if (arrayp[x] == 255) {
										arrayp[x] = 1;
									}
								}
								correcto = 0;
								for (int x = 0; x < 16; x++) {

									if (arrayp[x] != matriz[n][x]) correcto = 1;		// si correcto = 1 quiere decir que no ha encontrado un aruco

								}
								if (correcto == 0) {									// si coincide la orientación y el número del aruco entra aquí
									id = n;												// se asigna el id al número del aruco
									posicion = r;										// se asigna pa posicion al número de rotación del aruco
									solvePnP(vector_objetivo_3d, contour2f, calib, distor, rvec, tvec); // obtenemos la representación espacial. Obtenemos las matrices de rotación y traslación para usarlos en project points.

									string ids;											//creación de variable auxiliar ya que puttext no permite enteros
									ids.push_back(id + '0');							// se guarda id en el string

									if (f) { // si f es 1 dibujamos la pirámide
										projectPoints(vector_proy_3d_pir, rvec, tvec, calib, distor, projectedPoints); // Obtenemos las proyecciones de los puntos del cubo en el espacio
										line(hola, contour[0], projectedPoints[0], Scalar(255, 0, 255), 2); // dibujamos las aristas de la pirámide
										line(hola, contour[1], projectedPoints[0], Scalar(255, 0, 255), 2);
										line(hola, contour[2], projectedPoints[0], Scalar(255, 0, 255), 2);
										line(hola, contour[3], projectedPoints[0], Scalar(255, 0, 255), 2);
										circle(hola, projectedPoints[5], 4, Scalar(255, 0, 0), -1);

										putText(hola, ids, projectedPoints[5], FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255)); //imprimimos en hola en el centro de cada aruco el id correspondiente




									}
									else {
										projectPoints(vector_proy_3d, rvec, tvec, calib, distor, projectedPoints);// Obtenemos las proyecciones de los puntos del cubo en el espacio
										line(hola, contour[0], projectedPoints[0], Scalar(255, 0, 255), 2); // dibujamos las aristas del cubo
										line(hola, contour[1], projectedPoints[1], Scalar(255, 0, 255), 2);
										line(hola, contour[2], projectedPoints[2], Scalar(255, 0, 255), 2);
										line(hola, contour[3], projectedPoints[3], Scalar(255, 0, 255), 2);
										line(hola, projectedPoints[0], projectedPoints[1], Scalar(255, 0, 255), 2);
										line(hola, projectedPoints[1], projectedPoints[2], Scalar(255, 0, 255), 2);
										line(hola, projectedPoints[2], projectedPoints[3], Scalar(255, 0, 255), 2);
										line(hola, projectedPoints[3], projectedPoints[0], Scalar(255, 0, 255), 2);


										putText(hola, ids, projectedPoints[4], FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255));
									}

								}

							}

							rotate(transforumbral, transforumbral, 0);  //rota la matriz transforumbral 90 grados

						}


					}


				}


			}


			//cout << id << endl;
			imshow("LO1_EO3", hola);
			//imshow("grises1", grises);
			imshow("umbral1", umbral);
			imshow("umbral2", transforumbral);
			t1 = clock();											//tiempo de fin de ejecución
			fps = (1.0 / (double(t1 - t0) / CLOCKS_PER_SEC));	    //se restan los 2 tiempos, se divide entre los ciclos por segundo y 1 entre eso son los fps
			//cout << fps << endl;
			pressedkey = waitKey(1);
			if (pressedkey == 'd') {
				f = !f;
			}
			//cout << pressedkey << endl;


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