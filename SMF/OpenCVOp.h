#pragma once
#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

class OpenCVOp
{
protected:


public:
	bool isRectify;

	OpenCVOp();
	~OpenCVOp();

	int idL, idR;
	int boardWidth, boardHeight, imgNumber, squareSize;

	VideoCapture captureL, captureR;
	Mat frameL, frameR, frameV;
	Size resizeL, resizeR, resizeV, imgSize, boardSize;
	Mat intrinsicL, distortion_coeffL, intrinsicR, distortion_coeffR;
	Mat cameraMatrixL, distCoeffL, cameraMatrixR, distCoeffR;
	Mat R, T, E, F, Q, Rl, Rr, Pl, Pr, XYZ;
	Mat mapLx, mapLy, mapRx, mapRy;
	Rect validROIL, validROIR, validROI;
	vector<Mat> rvecsL, tvecsL, rvecsR, tvecsR;
	vector<vector<Point2f>> cornersL, cornersR;

	Ptr<StereoBM> sbm;

	int frameCount;
	int countCameras();
	bool openCamera(int l, int r);
	void closeCamera();
	void nextFrame();
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
	bool getPointClouds(cv::Mat & disparity, cv::Mat & pointClouds);
	double getDistance(int x, int y);

};

