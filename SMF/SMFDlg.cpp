
// SMFDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SMF.h"
#include "SMFDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
	DDX_Control(pDX, IDC_BUTTON_FROM_VIDEO, m_bFromVideo);
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

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	op = OpenCVOp();

	bindCVWindow(IDC_STATIC_PIC_L, WIND_L);
	bindCVWindow(IDC_STATIC_PIC_R, WIND_R);
	bindCVWindow(IDC_STATIC_PIC_V, WIND_V);
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

void CSMFDlg::bindCVWindow(int nID, const char * winname)
{
	CRect rect;
	GetDlgItem(nID)->GetClientRect(rect);
	HWND hWnd = op.bindWindow(winname, rect.Width(), rect.Height());
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(nID)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);
}

void CSMFDlg::OnBnClickedButtonOpen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!openingCamera) {
		if (openCamera(true)) {
			openingCamera = true;
			m_bOpen.SetWindowTextW(TEXT_CLOSE);
		}
		else {
			MessageBox(TEXT_OPEN_FAILED);
		}
	} else {
		openCamera(false);
		openingCamera = false;
		m_bOpen.SetWindowTextW(TEXT_OPEN);
	}
}


void CSMFDlg::OnBnClickedButtonCalibrate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CSMFDlg::OnBnClickedButtonFromCamera()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CSMFDlg::OnBnClickedButtonFromPic()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CSMFDlg::OnBnClickedButtonFromVideo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
