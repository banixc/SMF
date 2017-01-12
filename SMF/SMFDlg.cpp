
// SMFDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SMF.h"
#include "SMFDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static UINT BASED_CODE indicators[] = {
	IDS_STRING_STATUS,
	IDS_STRING_STATUS_V
};

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CSMFDlg 对话框



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
	DDX_Control(pDX, IDC_BUTTON_FROM_VIDEO, m_bFromVideo);
	DDX_Control(pDX, IDC_EDIT_NUM, m_eNum);
	DDX_Control(pDX, IDC_BUTTON_CUT, m_bCut);
}

BEGIN_MESSAGE_MAP(CSMFDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CSMFDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_CALIBRATE, &CSMFDlg::OnBnClickedButtonCalibrate)
	ON_BN_CLICKED(IDC_BUTTON_FROM_CAMERA, &CSMFDlg::OnBnClickedButtonFromCamera)
	ON_BN_CLICKED(IDC_BUTTON_FROM_PIC, &CSMFDlg::OnBnClickedButtonFromPic)
	ON_BN_CLICKED(IDC_BUTTON_FROM_VIDEO, &CSMFDlg::OnBnClickedButtonFromVideo)
	ON_BN_CLICKED(IDC_BUTTON_CUT, &CSMFDlg::OnBnClickedButtonCut)
END_MESSAGE_MAP()


// CSMFDlg 消息处理程序

BOOL CSMFDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	initStatusBar();

	// TODO: 在此添加额外的初始化代码
	op = OpenCVOp();

	bindCVWindow(IDC_STATIC_PIC_L, WIND_L);
	bindCVWindow(IDC_STATIC_PIC_R, WIND_R);
	bindCVWindow(IDC_STATIC_PIC_V, WIND_V);
	initCombox();
	op.showBlack(PL|PR|PV);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSMFDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSMFDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool CSMFDlg::openCamera(bool flag)
{
	if (flag) {
		CString t;
		m_cSelectL.GetWindowText(t);
		int idL = _ttoi(t);
		m_cSelectR.GetWindowText(t);
		int idR = _ttoi(t);
		flag = op.openCamera(idL, idR);
		if (flag) {
			op.showCamera(true);
			return true;
		} else {
			return false;
		}
	}
	else {
		op.showCamera(false);
		return true;
	}
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

void CSMFDlg::bindCVWindow(int nID, const char * winname)
{
	CRect rect;
	GetDlgItem(nID)->GetClientRect(rect);
	HWND hWnd = op.bindWindow(winname, rect.Width(), rect.Height());
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(nID)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);
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

void CSMFDlg::OnBnClickedButtonOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!openingCamera) {
		if (openCamera(true)) {
			openingCamera = true;
			m_bOpen.SetWindowTextW(TEXT_CLOSE);
			m_eGridH.EnableWindow(true);
			m_eGridW.EnableWindow(true);
			m_eReally.EnableWindow(true);
			m_eNum.EnableWindow(true);
			m_bFromCamera.EnableWindow(true);
			m_bar.SetPaneText(0, L"打开摄像头成功！");
		}
		else {
			m_bar.SetPaneText(0, L"打开摄像头失败！");
		}
	} else {
		openCamera(false);
		openingCamera = false;
		m_bOpen.SetWindowTextW(TEXT_OPEN);
	}
}


void CSMFDlg::OnBnClickedButtonCalibrate()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CSMFDlg::OnBnClickedButtonFromCamera()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!validParam())
		m_bar.SetPaneText(0, L"参数不正确！");
	else {
		m_bar.SetPaneText(0, L"初始化完成！");
		m_bFromCamera.EnableWindow(false);
		m_bCut.EnableWindow(true);
	}
}


void CSMFDlg::OnBnClickedButtonFromPic()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CSMFDlg::OnBnClickedButtonFromVideo()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CSMFDlg::OnBnClickedButtonCut()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bar.SetPaneText(0, L"正在截取...");
	CString s;
	s.Format(L"%d/%d", op.frameCount, op.imgNumber);
	m_bar.SetPaneText(1, s);
	if (op.cutPic()) {
		m_bar.SetPaneText(0, L"截取成功");
		s.Format(L"%d/%d", op.frameCount, op.imgNumber);
		m_bar.SetPaneText(1, s);
	}
	else {
		m_bar.SetPaneText(0, L"截取失败");
	}
}
