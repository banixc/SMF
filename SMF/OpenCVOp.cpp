#include "stdafx.h"
#include "OpenCVOp.h"

using namespace cv;
using namespace std;

void OpenCVOp::show(int flag)
{
	Mat resized;
	double sf;
	int w, h;
	sf = 640. / MAX(resizeL.width, resizeR.height);

	if (PL == (flag & PL) && !frameL.empty()) {
		if (isRectify) {

			Rect vroiL(cvRound(validROIL.x*sf), cvRound(validROIL.y*sf),					//获得被截取的区域	
				cvRound(validROIL.width*sf), cvRound(validROIL.height*sf));
			remap(frameL, frameL, mapLx, mapLy, INTER_LINEAR);
			rectangle(resized, vroiL, Scalar(0, 0, 255), 3, 8);
		}
		resize(frameL, resized, resizeL);
		imshow(WIND_L, resized);
	} 
	if (PR == (flag & PR) && !frameR.empty()) {
		if (isRectify) {
			Rect vroiR(cvRound(validROIR.x*sf), cvRound(validROIR.y*sf),					//获得被截取的区域	
				cvRound(validROIR.width*sf), cvRound(validROIR.height*sf));
			remap(frameR, frameR, mapRx, mapRy, INTER_LINEAR);
			rectangle(resized, vroiR, Scalar(0, 0, 255), 3, 8);
		}
		resize(frameR, resized, resizeR);
		imshow(WIND_R, resized);
	}
	if (PV == (flag & PV) && !frameV.empty()) {
		resize(frameV, resized, resizeV);
		imshow(WIND_V, resized);
	}
}

void OpenCVOp::showBlack(int flag)
{
	Mat black;
	if (PL == (flag & PL)) {
		black.copyTo(frameL);
	}
	if (PR == (flag & PR)) {
		black.copyTo(frameR);
	}
	if (PV == (flag & PV)) {
		black.copyTo(frameV);
	}
	show(flag);
}

OpenCVOp::OpenCVOp()
{
	captureL = VideoCapture();
	captureR = VideoCapture();
	black = Mat(2, 2, CV_8UC3, Scalar(0, 0, 255));
	isRectify = false;
}

OpenCVOp::~OpenCVOp()
{
}

int OpenCVOp::countCameras()
{
	VideoCapture temp_camera;
	int maxTested = 10;
	for (int i = 0; i < maxTested; i++) {
		VideoCapture temp_camera(i);
		bool res = (!temp_camera.isOpened());
		temp_camera.release();
		if (res)
		{
			return i;
		}
	}
	return maxTested;
}

bool OpenCVOp::openCamera(int l, int r)
{
	idL = l;
	idR = r;
	captureL.open(idL);
	captureR.open(idR);
	return captureL.isOpened() && captureR.isOpened();
}

void OpenCVOp::showCamera(bool flag)
{
	if (flag) {
		hThreadCamera = CreateThread(NULL, 0, ShowCameraThread, (LPVOID*)this, NULL, 0);
	}
	else {
		CloseHandle(hThreadCamera);
		captureL.release();
		captureR.release();
	}
}

void OpenCVOp::showVision(bool flag)
{
	if (flag) {
		hThreadVision = CreateThread(NULL, 0, ShowVisionThread, (LPVOID*)this, NULL, 0);
	}
	else {
		CloseHandle(hThreadVision);
	}
}

void OpenCVOp::initCalibrateParam(int w, int h, int size, int number)
{
	boardWidth = w;
	boardHeight = h;
	squareSize = size;
	imgNumber = number;
	boardSize = Size(w, h);
	frameCount = 0;
}

bool OpenCVOp::startCalibrate()
{
	if (frameCount < imgNumber)
		return false;
	//单目标定
	calibrate();
	//双目标定
	double rms = calibrateStereo();
	//重映射
	rectify();
	//计算映射矩阵
	culRemap();
}

void OpenCVOp::calRealPoint(vector<vector<Point3f>>& obj)
{
	vector<Point3f> imgpoint;
	for (int rowIndex = 0; rowIndex < boardHeight; rowIndex++)
	{
		for (int colIndex = 0; colIndex < boardWidth; colIndex++)
		{
			imgpoint.push_back(Point3f(rowIndex * squareSize, colIndex * squareSize, 0));
		}
	}
	for (int imgIndex = 0; imgIndex < imgNumber; imgIndex++)
	{
		obj.push_back(imgpoint);
	}
}

bool OpenCVOp::cutPic() {
	Mat grayL, grayR;
	cvtColor(frameL, grayL, CV_BGR2GRAY);
	cvtColor(frameR, grayR, CV_BGR2GRAY);
	vector<Point2f> cornerL, cornerR;
	bool isFindL = findChessboardCorners(grayL, boardSize, cornerL);
	bool isFindR = findChessboardCorners(grayR, boardSize, cornerR);
	if (!(isFindL && isFindR))
		return false;
	cornerSubPix(grayL, cornerL, Size(5, 5), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 20, 0.1));
	cornerSubPix(grayR, cornerR, Size(5, 5), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 20, 0.1));
	cornersL.push_back(cornerL);
	cornersR.push_back(cornerR);
	frameCount++;
	
	startCalibrate();
	
	return true;
}

void OpenCVOp::calibrate()
{
	vector<vector<Point3f>> objL,objR;
	calRealPoint(objL);
	calRealPoint(objR);
	calibrateCamera(objL, cornersL, resizeL, intrinsicL, distortion_coeffL, rvecsL, tvecsL, 0);
	calibrateCamera(objR, cornersR, resizeR, intrinsicR, distortion_coeffR, rvecsR, tvecsR, 0);
}

double OpenCVOp::calibrateStereo()
{
	vector<vector<Point3f>> obj;
	calRealPoint(obj);
	return stereoCalibrate(obj, cornersL, cornersR,
		cameraMatrixL, distCoeffL,
		cameraMatrixR, distCoeffR,
		resizeL, R, T, E, F,
		CALIB_USE_INTRINSIC_GUESS,
		TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 100, 1e-5));
}

void OpenCVOp::rectify()
{
	stereoRectify(cameraMatrixL, distCoeffL, cameraMatrixR, distCoeffR, resizeL, R, T, Rl, Rr, Pl, Pr, Q,
		CALIB_ZERO_DISPARITY, 0, resizeL, &validROIL, &validROIR);
}

void OpenCVOp::culRemap()
{
	mapLx = Mat(resizeL, CV_32FC1);
	mapLy = Mat(resizeL, CV_32FC1);
	mapRx = Mat(resizeL, CV_32FC1);
	mapRy = Mat(resizeL, CV_32FC1);
	initUndistortRectifyMap(cameraMatrixL, distCoeffL, Rl, Pl, resizeL, CV_32FC1, mapLx, mapLy);
	initUndistortRectifyMap(cameraMatrixR, distCoeffR, Rr, Pr, resizeL, CV_32FC1, mapRx, mapRy);
	isRectify = true;
}

bool OpenCVOp::loadCameraParam() {
	FileStorage fs("CameraID.yml", FileStorage::READ);
	if (!fs.isOpened())
		return false;

	fs["cameraMatrixL"] >> cameraMatrixL;
	fs["cameraDistcoeffL"] >> distCoeffL;
	fs["cameraMatrixR"] >> cameraMatrixR;
	fs["cameraDistcoeffR"] >> distCoeffR;
	fs["R"] >> R;
	fs["T"] >> T;
	fs["Rl"] >> Rl;
	fs["Rr"] >> Rr;
	fs["Pl"] >> Pl;
	fs["Pr"] >> Pr;
	fs["Q"] >> Q;
	fs["mapLx"] >> mapLx;
	fs["mapLy"] >> mapLy;
	fs["mapRx"] >> mapRx;
	fs["mapRy"] >> mapRy;

	fs.release();
	isRectify = true;
	return true;
}

bool OpenCVOp::outPutCameraParam() {
	FileStorage fs("CameraParams.yml", FileStorage::WRITE);
	if (!fs.isOpened())
		return false;
	fs << "cameraMatrixL" << cameraMatrixL << "cameraDistcoeffL" << distCoeffL << "cameraMatrixR" << cameraMatrixR << "cameraDistcoeffR" << distCoeffR;
	fs << "R" << R << "T" << T << "Rl" << Rl << "Rr" << Rr << "Pl" << Pl << "Pr" << Pr << "Q" << Q;
	fs << "mapLx" << mapLx << "mapLy" << mapLy << "mapRx" << mapRx << "mapRy" << mapRy;
	fs.release();
	return true;
}



HWND OpenCVOp::bindWindow(const char * winname, int width, int height)
{
	namedWindow(winname);
	resizeWindow(winname, width, height);
	if (winname == WIND_L) {
		resizeL = Size(width, height);
	} 
	else if (winname == WIND_R) {
		resizeR = Size(width, height);
	}
	else if (winname == WIND_V) {
		resizeV = Size(width, height);
	}
	return (HWND)cvGetWindowHandle(winname);
}

DWORD OpenCVOp::ShowCameraThread(void * pArg)
{
	OpenCVOp* t = (OpenCVOp*)pArg;
	while (true) {
		t->captureL >> t->frameL;
		t->captureR >> t->frameR;
		t->show(PL|PR);
	}
	return 0;
}

DWORD OpenCVOp::ShowVisionThread(void * pArg)
{
	OpenCVOp* t = (OpenCVOp*)pArg;
	while (true) {

	}
	return 0;
}
