#pragma once
#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

class OpenCVOp
{
protected:
	HANDLE hThreadCamera;
	HANDLE hThreadVision;
	Mat black;
	bool isRectify;

public:
	OpenCVOp();
	~OpenCVOp();

	int idL, idR;
	VideoCapture captureL, captureR;
	Mat frameL, frameR, frameV;
	Size resizeL, resizeR, resizeV;
	Mat intrinsicL, distortion_coeffL, intrinsicR, distortion_coeffR;
	Mat cameraMatrixL, distCoeffL, cameraMatrixR, distCoeffR;
	Mat R, T, E, F, Q, Rl, Rr, Pl, Pr;
	Rect validROIL, validROIR, validROI;
	vector<Mat> rvecsL, tvecsL, rvecsR, tvecsR;
	Mat mapLx, mapLy, mapRx, mapRy;
	int boardWidth, boardHeight, imgNumber, squareSize;
	Size boardSize;
	vector<vector<Point2f>> cornersL, cornersR;


	int frameCount;
	int countCameras();
	bool openCamera(int l, int r);
	void showCamera(bool flag);
	void showVision(bool flag);
	void initCalibrateParam(int w, int h, int size, int number);
	bool startCalibrate();
	void calRealPoint(vector<vector<Point3f>>& obj);
	bool cutPic();
	void calibrate();
	double calibrateStereo();
	void rectify();
	void culRemap();
	bool loadCameraParam();
	bool outPutCameraParam();
	HWND bindWindow(const char * winname, int width, int height);


	void show(int flag);
	void showBlack(int flag);

	static DWORD WINAPI ShowCameraThread(void *pArg);	//显示摄像机
	static DWORD WINAPI ShowVisionThread(void *pArg);	//显示视差图
};

