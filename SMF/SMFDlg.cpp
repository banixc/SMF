
// SMFDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMF.h"
#include "SMFDlg.h"
#include "afxdialogex.h"
#include "Cvvimage.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ID_TIMER_LOOP_LRV 10

static UINT BASED_CODE indicators[] = {
	IDS_STRING_STATUS,
	IDS_STRING_STATUS_V
};

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSMFDlg �Ի���



CSMFDlg::CSMFDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SMF_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSMFDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SELECT_L, m_cSelectL);
	DDX_Control(pDX, IDC_COMBO_SELECT_R, m_cSelectR);
	DDX_Control(pDX, IDC_EDIT_GRID_H, m_eGridH);
	DDX_Control(pDX, IDC_EDIT_GRID_W, m_eGridW);
	DDX_Control(pDX, IDC_EDIT_REALLY, m_eReally);
	DDX_Control(pDX, IDC_BUTTON_OPEN, m_bOpen);
	DDX_Control(pDX, IDC_BUTTON_CALIBRATE, m_bCalibrate);
	DDX_Control(pDX, IDC_BUTTON_FROM_CAMERA, m_bFromCamera);
	DDX_Control(pDX, IDC_BUTTON_FROM_PIC, m_bFromPic);
	DDX_Control(pDX, IDC_BUTTON_FROM_FILE, m_bFromFile);
	DDX_Control(pDX, IDC_EDIT_NUM, m_eNum);
	DDX_Control(pDX, IDC_BUTTON_CUT, m_bCut);
}

BEGIN_MESSAGE_MAP(CSMFDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CSMFDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_FROM_CAMERA, &CSMFDlg::OnBnClickedButtonFromCamera)
	ON_BN_CLICKED(IDC_BUTTON_FROM_PIC, &CSMFDlg::OnBnClickedButtonFromPic)
	ON_BN_CLICKED(IDC_BUTTON_FROM_FILE, &CSMFDlg::OnBnClickedButtonFromFile)
	ON_BN_CLICKED(IDC_BUTTON_CUT, &CSMFDlg::OnBnClickedButtonCut)
	ON_WM_LBUTTONDOWN()

	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSMFDlg ��Ϣ�������

BOOL CSMFDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	initStatusBar();

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	op = OpenCVOp();

	initCombox();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CSMFDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSMFDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSMFDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSMFDlg::initCombox()
{
	int num = op.countCameras();
	for (int i = 0; i < num; i++) {
		CString s;
		s.Format(L"%d", i);
		m_cSelectL.AddString(s);
		m_cSelectR.AddString(s);
	}
}

void CSMFDlg::initStatusBar()
{
	m_bar.Create(this);
	m_bar.SetIndicators(indicators, 2);
	CRect rect;
	GetClientRect(&rect);
	m_bar.SetPaneInfo(0, IDS_STRING_STATUS, SBPS_NORMAL, rect.Width() - 100);
	m_bar.SetPaneInfo(1, IDS_STRING_STATUS_V, SBPS_STRETCH, 0);
	m_bar.GetStatusBarCtrl().SetBkColor(RGB(180, 180, 180));
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, AFX_IDW_CONTROLBAR_FIRST);
}

bool CSMFDlg::validParam()
{
	int w = getInt(IDC_EDIT_GRID_W);
	int h = getInt(IDC_EDIT_GRID_H);
	int num = getInt(IDC_EDIT_NUM);
	int size = getInt(IDC_EDIT_RELALLY);
	op.initCalibrateParam(w, h, size, num);
	return w > 0 && h > 0 && num > 0 && size >0;
}

int CSMFDlg::getInt(int nID)
{
	CString t;
	GetDlgItem(nID)->GetWindowText(t);
	return _ttoi(t);
}

void CSMFDlg::showNextFrame()
{
	if (!op.frameL.empty())
		ShowMat(op.frameL, IDC_STATIC_PIC_L);
	if (!op.frameR.empty())
		ShowMat(op.frameR, IDC_STATIC_PIC_R);
	if (!op.frameV.empty())
		ShowMat(op.frameV, IDC_STATIC_PIC_V);
}

void CSMFDlg::OnBnClickedButtonOpen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (op.openCamera(getInt(IDC_COMBO_SELECT_L), getInt(IDC_COMBO_SELECT_R))) {
		openingCamera = true;
		lockCameraButton();
		unlockCalibrateButton();
		m_bar.SetPaneText(0, L"������ͷ�ɹ���");
		SetTimer(ID_TIMER_LOOP_LRV, 10, NULL);

	}
	else {
		m_bar.SetPaneText(0, L"������ͷʧ�ܣ�");
	}
}

void CSMFDlg::OnBnClickedButtonFromCamera()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!validParam())
		m_bar.SetPaneText(0, L"��������ȷ��");
	else {
		m_bar.SetPaneText(0, L"��ʼ����ɣ�");
		m_bFromCamera.EnableWindow(false);
		m_bCut.EnableWindow(true);
	}
}


void CSMFDlg::OnBnClickedButtonFromPic()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TCHAR szFilter[] = _T("ͼƬ|*.jpg|�����ļ�|*.*||");
	vector<string> fileNameListL, fileNameListR;
	CFileDialog fileDlgL(TRUE, NULL, NULL, OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT, NULL, this);
	if (fileDlgL.DoModal() == IDOK) {
		POSITION pos;
		pos = fileDlgL.GetStartPosition();//��ʼ�����û�ѡ���ļ��б�  
		while (pos != NULL)
		{
			CString filename = fileDlgL.GetNextPathName(pos);
			//m_ctlList.AddString(filename);//���ļ�����ӵ��б��  
			USES_CONVERSION;
			string tmpstr(W2A(filename));
			fileNameListL.push_back(tmpstr);
		}
	}

	CFileDialog fileDlgR(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT, NULL, this);
	if (fileDlgR.DoModal() == IDOK) {
		POSITION pos;
		pos = fileDlgR.GetStartPosition();//��ʼ�����û�ѡ���ļ��б�  
		while (pos != NULL)
		{
			CString filename = fileDlgR.GetNextPathName(pos);
			USES_CONVERSION;
			string tmpstr(W2A(filename));
			fileNameListR.push_back(tmpstr);
		}
	}
	if (op.calibrateFromPic(fileNameListL, fileNameListR)) {
		m_bar.SetPaneText(0, L"ͼƬ�궨�ɹ���");
		lockCalibrateButton();
		return;
	}
	m_bar.SetPaneText(0, L"ͼƬ�궨ʧ�ܣ������ԣ�");

}


void CSMFDlg::OnBnClickedButtonFromFile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST , NULL, this);

	if (fileDlg.DoModal() == IDOK) {
		POSITION pos;
		pos = fileDlg.GetStartPosition();//��ʼ�����û�ѡ���ļ��б�  
		if (pos != NULL)
		{
			CString filename = fileDlg.GetNextPathName(pos);
			USES_CONVERSION;
			string tmpstr(W2A(filename));
			if (op.loadCameraParam(tmpstr)) {
				m_bar.SetPaneText(0, L"�Ѵ��ļ��м��ر궨������");
				lockCalibrateButton();
				return;
			}
		}
	}
	m_bar.SetPaneText(0, L"����δ���سɹ��������ԣ�");

}


void CSMFDlg::OnBnClickedButtonCut()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bar.SetPaneText(0, L"���ڽ�ȡ...");
	CString s;
	s.Format(L"%d/%d", op.frameCount, op.imgNumber);
	m_bar.SetPaneText(1, s);
	if (op.cutPic()) {
		m_bar.SetPaneText(0, L"��ȡ�ɹ�");
		s.Format(L"%d/%d", op.frameCount, op.imgNumber);
		m_bar.SetPaneText(1, s);
	}
	else {
		m_bar.SetPaneText(0, L"��ȡʧ��");
	}
}




void CSMFDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	convertPoint(point);
	if (point.x == -1 && point.y == -1)
		return;
	double d = op.getDistance(point.x, point.y);
	CString s;
	s.Format(L"���룺%d", d);
	m_bar.SetPaneText(1, s);
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CSMFDlg::ShowMat(cv::Mat& image, int IDC)
{
	CDC* pDC = GetDlgItem(IDC)->GetDC();			//����ID��ô���ָ���ٻ�ȡ��ô��ڹ�����������ָ��  
	HDC hDC = pDC->GetSafeHdc();					// ��ȡ�豸�����ľ��  
	CRect rect;
	GetDlgItem(IDC)->GetClientRect(&rect);			//��ȡ��ʾ��  
	CvvImage cimg;
	IplImage ipl_img(image);
	cimg.CopyOf(&ipl_img);							// ����ͼƬ
	cimg.DrawToHDC(hDC, &rect);						// ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������
	ReleaseDC(pDC);

}

void CSMFDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (nIDEvent) {
		case ID_TIMER_LOOP_LRV:
			op.nextFrame();
			showNextFrame();
			break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CSMFDlg::convertPoint(CPoint& p) {
	CRect rect;
	GetDlgItem(IDC_STATIC_PIC_V)->GetWindowRect(&rect);			//��ȡ��ʾ��  
	ScreenToClient(&rect);
	if(PtInRect(&rect, p)) {
		p.x -= rect.left;
		p.y -= rect.top;
		p.x = p.x * op.imgSize.width / rect.Width();
		p.y = p.y * op.imgSize.height / rect.Height();
	}
	else {
		p.x = -1;
		p.y = -1;
	}
}

void CSMFDlg::lockCameraButton() {
	m_bOpen.EnableWindow(false);
	m_cSelectL.EnableWindow(false);
	m_cSelectR.EnableWindow(false);
}

void CSMFDlg::lockCalibrateButton() {
	lockCalibrateButton(false);
}

void CSMFDlg::unlockCalibrateButton() {
	lockCalibrateButton(true);
}

void CSMFDlg::lockCalibrateButton(boolean lock) {
	m_eGridH.EnableWindow(lock);
	m_eGridW.EnableWindow(lock);
	m_eReally.EnableWindow(lock);
	m_eNum.EnableWindow(lock);
	m_bFromCamera.EnableWindow(lock);
	m_bFromPic.EnableWindow(lock);
}

