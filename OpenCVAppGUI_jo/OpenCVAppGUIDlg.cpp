
// OpenCVAppGUIDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "OpenCVAppGUI.h"
#include "OpenCVAppGUIDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define NAME_WINDOW_SRC "srcView"
#define NAME_WINDOW_DEBUG "srcDebug"
#define NAME_WINDOW_RESULT "srcResult"

//#define LINE
//#define SCATTER
//#define RECT




// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// COpenCVAppGUIDlg 대화 상자



COpenCVAppGUIDlg::COpenCVAppGUIDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OPENCVAPPGUI_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COpenCVAppGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COpenCVAppGUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_Load, &COpenCVAppGUIDlg::OnBnClickedBtnLoad)
	ON_BN_CLICKED(IDC_BTN_Save, &COpenCVAppGUIDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_INSPECTION, &COpenCVAppGUIDlg::OnBnClickedBtnInspection)
	ON_BN_CLICKED(IDC_BTN_INSPECTION_CV, &COpenCVAppGUIDlg::OnBnClickedBtnInspectionCv)
	ON_BN_CLICKED(IDC_BTN_SampleCode, &COpenCVAppGUIDlg::OnBnClickedBtnSamplecode)
END_MESSAGE_MAP()


// COpenCVAppGUIDlg 메시지 처리기
BOOL COpenCVAppGUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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
	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	//ShowWindow(SW_MAXIMIZE);
	//ShowWindow(SW_MINIMIZE);



	//opencv -> window handle
	namedWindow(NAME_WINDOW_SRC, WINDOW_AUTOSIZE);
	HWND hWnd = (HWND)cvGetWindowHandle(NAME_WINDOW_SRC);
	HWND hParent = (HWND)FindWindow(NULL, NAME_WINDOW_SRC);
	HWND hOrgParent = ::SetParent(hWnd, GetDlgItem(IDC_PICTURE)->m_hWnd);
	::ShowWindow(hOrgParent, SW_HIDE);

	namedWindow(NAME_WINDOW_DEBUG, WINDOW_AUTOSIZE);
	hWnd = (HWND)cvGetWindowHandle(NAME_WINDOW_DEBUG);
	hParent = (HWND)FindWindow(NULL, NAME_WINDOW_DEBUG);
	hOrgParent = ::SetParent(hWnd, GetDlgItem(IDC_PICTURE_DEBUG)->m_hWnd);
	::ShowWindow(hOrgParent, SW_HIDE);

	namedWindow(NAME_WINDOW_RESULT, WINDOW_AUTOSIZE);
	hWnd = (HWND)cvGetWindowHandle(NAME_WINDOW_RESULT);
	hParent = (HWND)FindWindow(NULL, NAME_WINDOW_RESULT);
	hOrgParent = ::SetParent(hWnd, GetDlgItem(IDC_PICTURE_RESULT)->m_hWnd);
	::ShowWindow(hOrgParent, SW_HIDE);

	
	//GetDlgItem(IDC_PICTURE)->GetWindowRect(_rtImageView);
	//ScreenToClient(_rtImageView);

	_mWndImageView.insert(pair<int, CRect>(IDC_PICTURE, CRect(0,0,0,0)));
	_mWndImageView.insert(pair<int, CRect>(IDC_PICTURE_DEBUG, CRect(0, 0, 0, 0)));

	for (auto iter = _mWndImageView.begin(); iter != _mWndImageView.end(); iter++) {
		int resID = (iter->first);
		CRect rtImgView;
		GetDlgItem(resID)->GetWindowRect(rtImgView);
		ScreenToClient(rtImgView);
		iter->second = rtImgView;
	}

	_mMatBuff.clear();
	_mInsps.clear();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void COpenCVAppGUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void COpenCVAppGUIDlg::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.



	if (IsIconic())
	{
		//CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{

		//if (!_imageView.IsNull())
		//{
		//	dc.SetStretchBltMode(COLORONCOLOR);
		//	_imageView.Draw(dc, _rtImageView);
		//}
//		DisplayImage(IDC_PICTURE, _DrawImage);


		//CRect rect;
		//::GetWindowRect(GetDlgItem(IDC_PICTURE)->m_hWnd, rect);
		//ScreenToClient(rect);

		CBrush brush(HS_DIAGCROSS, RGB(200, 200, 200));
		//dc.FillRect(CRect(rect.left + 1, rect.top + 1, rect.right, rect.bottom), &brush);

		for (auto iter = _mWndImageView.begin(); iter != _mWndImageView.end(); iter++) {
			CRect rt = iter->second;
			dc.FillRect(rt, &brush);
		}



		if (!_mMatBuff[eImgSrcColor].empty())
		{
			Mat Img = _mMatBuff[eImgSrcColor];
			Mat DrawResize;
			Size size(int(_dNewScale * Img.cols), int(_dNewScale * Img.rows));
			resize(Img, DrawResize, size);

			resizeWindow(NAME_WINDOW_SRC, size.width, size.height);
			imshow(NAME_WINDOW_SRC, DrawResize);
		}

		if (_bShowDebug)
		{
			if (!_mMatBuff[eImgDebugColor].empty())
			{
				Mat Img = _mMatBuff[eImgDebugColor];
				Mat DrawResize;
				Size size(int(_dNewScale * Img.cols), int(_dNewScale * Img.rows));
				resize(Img, DrawResize, size);

				resizeWindow(NAME_WINDOW_DEBUG, size.width, size.height);
				imshow(NAME_WINDOW_DEBUG, DrawResize);
			}
		}

		if (_bShowResult)
		{
			if (!_mMatBuff[eImgDrawColor].empty())
			{
				Mat Img = _mMatBuff[eImgDrawColor];
				Mat DrawResize;
				Size size(int(_dNewScale * Img.cols), int(_dNewScale * Img.rows));
				resize(Img, DrawResize, size);

				resizeWindow(NAME_WINDOW_RESULT, size.width, size.height);
				imshow(NAME_WINDOW_RESULT, DrawResize);
			}
		}

		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR COpenCVAppGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void COpenCVAppGUIDlg::UpdateDispSrc()
{
	_bShowResult = false;
	CRect rectSrc;
	::GetWindowRect(GetDlgItem(IDC_PICTURE)->m_hWnd, rectSrc);
	double dScaleX = rectSrc.Width() / (double)_SourceImage.cols;
	double dScaleY = rectSrc.Height() / (double)_SourceImage.rows;
	_dSrcScale = min(dScaleX, dScaleY);
	_dNewScale = _dSrcScale;
	//_bShowResult = FALSE;
	//防止顯示不同比例圖片時DC殘留
	CWnd* pWnd = GetDlgItem(IDC_PICTURE);
	pWnd->GetClientRect(&rectSrc);//제어 클라이언트 영역의 좌표 얻기
	pWnd->ClientToScreen(rectSrc);//제어 클라이언트 영역에서 대화 영역으로 영역 좌표 변환
	this->ScreenToClient(rectSrc); //다이얼로그 영역의 영역 좌표를 다이얼로그의 클라이언트 영역 좌표로 변환
	InvalidateRect(rectSrc);
	//비율이 다른 사진을 표시할 때 DC 잔여물 방지

	//Scroll Bar


	//Test
	//double d1 = clock ();
	//Mat matResult;
	//matchTemplate (_DrawImage, _matPtrn, matResult, CV_TM_CCORR);
	//double d2 = clock ();

	////From ImageShop
	//matResult.create (_DrawImage.rows - _matPtrn.rows + 1,
	//	_DrawImage.cols - _matPtrn.cols + 1, CV_32FC1);
	//matResult.setTo (0);
	//cv::Mat& matTemplate = _matPtrn;

	//int  t_r_end = matTemplate.rows, t_r = 0;
	//for (int r = 0; r < matResult.rows; r++)
	//{
	//	float* r_matResult = matResult.ptr<float> (r);
	//	uchar* r_source = _DrawImage.ptr<uchar> (r);
	//	uchar* r_template, *r_sub_source;
	//	for (int c = 0; c < matResult.cols; ++c, ++r_matResult, ++r_source)
	//	{
	//		r_template = matTemplate.ptr<uchar> ();
	//		r_sub_source = r_source;
	//		for (t_r = 0; t_r < t_r_end; ++t_r, r_sub_source += _DrawImage.cols, r_template += matTemplate.cols)
	//		{
	//			*r_matResult = *r_matResult + IM_Conv_SIMD (r_template, r_sub_source, matTemplate.cols);
	//		}
	//	}
	//}
	//double d3 = clock ();

	//double d = (d3 - d2) / (d2 - d1);
	//CString str; str.Format ("%.3f", d); AfxMessageBox (str);
	//
}


void COpenCVAppGUIDlg::OnBnClickedBtnLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_READONLY, _T("image file(*.jpg;*.bmp;*.png;)|*.jpg;*.bmp;*.png;|All Files(*.*)|*.*||"));
	if (fileDlg.DoModal() == IDOK)
	{
		CString path = fileDlg.GetPathName();

		//string fileName = path;
		//_SourceImage = cv::imread(fileName, IMREAD_ANYCOLOR);

		//if (_SourceImage.type() == CV_8UC1)
		//	cvtColor(_SourceImage, _DrawImage, COLOR_GRAY2BGR);
		//else
		//	_SourceImage.copyTo(_DrawImage);

		string fileName = path;
		_SourceImage = cv::imread(fileName, IMREAD_ANYCOLOR);

		OnAllocateBuffer(_SourceImage.cols, _SourceImage.rows);


		UpdateDispSrc();

		InvalidateRect(_rtImageView, FALSE);
		//AfxMessageBox("Image Loaded");

	}	
}

void COpenCVAppGUIDlg::OnBnClickedBtnSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//string fileName = "jetson_clone.bmp";
	//_imageView.Save(fileName.c_str());

	CFileDialog dlgFileSave(FALSE, "*.*", NULL,//화일명 없음
		OFN_FILEMUSTEXIST,
		_T("image file(*.jpg;*.bmp;*.png;)|*.jpg;*.bmp;*.png;|All Files(*.*)|*.*||"), NULL);

	//옵션 선택 부분.
	if (dlgFileSave.DoModal() == IDOK)
	{
		string str = dlgFileSave.GetPathName();
		imwrite(str, _SourceImage);		
	}

	//AfxMessageBox("Image Saved")
}

void COpenCVAppGUIDlg::OnBnClickedBtnInspection()
{
	_bShowResult = false;
	_bShowDebug = false;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

//input
	//OnInspection(InputArray src, OutputArray dst);
	Mat src = _mMatBuff[eImgSrcGray];
	

//fucntion
	Mat dst = _mMatBuff[eImgDebugGray];

	// using pointer to data
	OnInspection(src.data, src.cols, src.rows, dst.data);

//result & display
	/*
	_bShowResult = true;
	cvtColor(dst, _mMatBuff[eImgResultColor], COLOR_GRAY2BGR);
	*/
	_bShowResult = true;
	_bShowDebug = true;
	dst.copyTo(_mMatBuff[eImgDebugGray]);
	cvtColor(dst, _mMatBuff[eImgDebugColor], COLOR_GRAY2BGR);

	uchar* pData = _mMatBuff[eImgDebugGray].data;
	int width = _mMatBuff[eImgDebugGray].cols;
	int height = _mMatBuff[eImgDebugGray].rows;

	Mat draw = _mMatBuff[eImgDrawColor];

#ifdef LINE
	int row1 = height / 2 + 1;
	int row2 = height / 2 - 1;
	int col1 = 0;
	int col2 = 0;	
	for (int col = 1; col < width; col++) {
		int index = row1 * width + col;
		if (pData[index] > 128) {
			col1 = col;
			break;
		}			
	}
	for (int col = 1; col < width; col++) {
		int index = row2 * width + col;
		if (pData[index] > 128) {
			col2 = col;
			break;
		}			
	}
	
	line(draw, Point(col1, row1), Point(col2, row2), Scalar(0, 0, 255), 2, LINE_8);
	line(draw, _pt1, _pt2, Scalar(0, 0, 255), 2, LINE_8);
#endif
		
#ifdef SCATTER
	for (int row = 1; row < height; row++) {
		for (int col = 1; col < width; col++) {
			int index = row * width + col;
			int old_index = row * width + col - 1;

			if (abs(pData[index] - pData[old_index]) > 200)
			{
				line(draw, Point(col, row), Point(col, row), Scalar(0, 0, 255), 2, LINE_8);
			}
		}
	}	
#endif

#ifdef RECT
	line(draw, Point(x1, y1), Point(x1, y2), Scalar(100, 0, 255), 5, LINE_8);
	line(draw, Point(x2, y1), Point(x2, y2), Scalar(0, 100, 255), 5, LINE_8);
	line(draw, Point(x1, y1), Point(x2, y1), Scalar(100, 100, 255), 5, LINE_8);
	line(draw, Point(x1, y2), Point(x2, y2), Scalar(100, 100, 0), 5, LINE_8);
	Xpoint(_pt_tl, draw);
	Xpoint(_pt_tr, draw);
	Xpoint(_pt_bl, draw);
	Xpoint(_pt_br, draw);

	draw = _mMatBuff[eImgDebugColor];
	line(draw, Point(x1, 0), Point(x1, height), Scalar(100, 0, 255), 8, LINE_8);
	line(draw, Point(x2, 0), Point(x2, height), Scalar(0, 100, 255), 8, LINE_8);
	line(draw, Point(0, y1), Point(width, y1), Scalar(100, 100, 255), 8, LINE_8);
	line(draw, Point(0, y2), Point(width, y2), Scalar(100, 100, 0), 8, LINE_8);

#endif
	Invalidate(FALSE);
}


int COpenCVAppGUIDlg::OnAllocateBuffer(int cols, int rows)
{

	_mMatBuff.clear();
	
	//KEY...enum의 값들 Value...Mat의 size와 Channel_Type만 형성
	_mMatBuff.insert(make_pair<int, cv::Mat>(eImgSrcColor, Mat(rows, cols, CV_8UC3)));
	_mMatBuff.insert(make_pair<int, cv::Mat>(eImgSrcGray, Mat(rows, cols, CV_8UC1)));
	_mMatBuff.insert(make_pair<int, cv::Mat>(eImgDebugGray, Mat(rows, cols, CV_8UC1)));
	_mMatBuff.insert(make_pair<int, cv::Mat>(eImgDrawColor, Mat(rows, cols, CV_8UC3)));
	_mMatBuff.insert(make_pair<int, cv::Mat>(eImgResultColor, Mat(rows, cols, CV_8UC3)));
	_mMatBuff.insert(make_pair<int, cv::Mat>(eImgDebugColor, Mat(rows, cols, CV_8UC3)));


	
	//source to source of map
	//_SourceImage.copyTo(_mMatBuff[eImgSrc]);
	
	//source to draw of map
	if (_SourceImage.type() == CV_8UC1)
	{
		cvtColor(_SourceImage, _mMatBuff[eImgSrcColor], COLOR_GRAY2BGR); 
		_SourceImage.copyTo(_mMatBuff[eImgSrcGray]);		
	}
	else
	{
		_SourceImage.copyTo(_mMatBuff[eImgSrcColor]);
		cvtColor(_SourceImage, _mMatBuff[eImgSrcGray], COLOR_BGR2GRAY);
	}

	Mat vi = _mMatBuff[eImgSrcColor];



	_mMatBuff[eImgSrcColor].copyTo(_mMatBuff[eImgDrawColor]);

	//debug of map ... just create buffer as initilized value
	_mMatBuff[eImgDebugGray] = 0;

	//result of map ... just create buffer as initilized value
	_mMatBuff[eImgResultColor] = 255;

	return 0;
}


int COpenCVAppGUIDlg::UpdateInspList()
{
	_mInsps.insert(make_pair("OnInspFindcontourSample", &COpenCVAppGUIDlg::CallInspFindcontourSample));
	_mInsps.insert(make_pair("OnInspFindShapes", COpenCVAppGUIDlg::CallInspFindShape));
	return 0;
}

int COpenCVAppGUIDlg::OnInspection(InputArray src, OutputArray dst)
{
	
	return 0;
}

int COpenCVAppGUIDlg::OnInspection(Mat src, Mat dst)
{
	return 0;
}

int COpenCVAppGUIDlg::OnInspection(uchar* pSrc, size_t cols, size_t rows, uchar* pDst)
{
	/*
	pSrc = pointer 2d
	cols = width
	rows = height
	pDst = result data, pointer 2d
	*/
	/* todo*/
	// 조건 pSrc = GrayScale
	// pSrc > 128 ? pDst = 255 : pDst = 0
	// 코드를 작성하세요

	for (size_t i = 0; i < cols*rows; i++)
	{
		pSrc[i] > 128 ? pDst[i] = 255 : pDst[i] = 0;
	}
	/*
	//LINE
	_pt1 = cv::Point(0, 0);
	_pt2 = cv::Point(0, 0);

	for (int row = 0; row < rows; row++)
	{	
		for (int col = 0; col < cols; col++)
		{
			int index = row * cols + col;
			if (pDst[index] > 128)
			{
				if (row == 0)
				{
					_pt1 = Point(col, row);
				}
				else if (row == rows-1)
				{
					_pt2 = Point(col, row);
				}
				break;
			}
		}
	}
	*/
	_pt_tl = cv::Point(0, 0);
	_pt_tr = cv::Point(0, 0);
	_pt_bl = cv::Point(0, 0);
	_pt_br = cv::Point(0, 0);

	y1 = 0;
	y2 = 0;
	x1 = 0;
	x2 = 0;

	for (size_t row = 100; row < rows; row = row + 3)
	{
		size_t col = cols / 2;
		int index = row * cols + col;
		if (pDst[index] > 128)
		{
			y1 = row;
			break;
		}
	}

	for (size_t row = 350; row > 0; row=row-3)
	{
		size_t col = cols / 2;
		int index = row * cols + col;
		if (pDst[index] > 128)
		{
			y2 = row;
			break;
		}
	}

	for (size_t col = 100; col < cols; col = col + 3)
	{
		size_t row = rows / 2;
		int index = row * cols + col;
		if (pDst[index] > 128)
		{
			x1 = col;
			break;
		}
	}
	for (size_t col = 350; col > 0; col=col-3)
	{
		size_t row = rows / 2;
		int index = row * cols + col;
		if (pDst[index] > 128)
		{
			x2 = col;
			break;
		}
	}

	_pt_tl = Point(x1, y1);
	_pt_tr = Point(x2, y1);
	_pt_bl = Point(x1, y2);
	_pt_br = Point(x2, y2);


	return 0;
}

int COpenCVAppGUIDlg::Xpoint(Point _pt, Mat draw)
{
	line(draw,Point(_pt.x - 10, _pt.y - 10), Point(_pt.x + 10, _pt.y + 10), Scalar(100, 100, 100), 2, LINE_8);
	line(draw, Point(_pt.x - 10, _pt.y + 10), Point(_pt.x + 10, _pt.y - 10), Scalar(100, 100, 100), 2, LINE_8);
	return 0;
}

int COpenCVAppGUIDlg::OnInspFindShapes()
{
	Mat src_gray = _mMatBuff[eImgSrcGray];
	vector<Scalar> vScalar = { Scalar(255,0,0), Scalar(0,255,0), Scalar(0,0,255), Scalar(100,150,200), Scalar(200,150,100), Scalar(50,150,50) };
	int thresh = 50;

	Mat canny_output;
	Canny(src_gray, canny_output, thresh, thresh * 2);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
	//findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_NONE);

	canny_output.copyTo(_mMatBuff[eImgDebugGray]);
	cvtColor(_mMatBuff[eImgDebugGray], _mMatBuff[eImgDebugColor], COLOR_GRAY2BGR);

	int circle_index = 0, triangle_index = 0, rectangle_index = 0;
	for (size_t i = 0; i < contours.size(); i++)
	{
		if (contours[i].size() >= 100 && contours[i].size() <= 150)
		{
			circle_index = i;
			continue;
		}
		else if (contours[i].size() >= 4 && contours[i].size() <= 8)
		{
			rectangle_index = i;
			continue;
		}
		else if (contours[i].size() >= 150)
		{
			triangle_index = i;
			continue;
		}
	}

	Mat drawing = _mMatBuff[eImgDebugColor];
	/*
	for (size_t i = 0; i < contours.size(); i++)
	{
		drawContours(drawing, contours, (int)i, Scalar(0,0,255), 8, LINE_8, hierarchy, 0);
	}
	*/
	for (size_t i = 0; i < contours.size() / 2; i++)
	{
		drawContours(drawing, contours, (int)(2 * i + 1), vScalar[i], 8, LINE_8, hierarchy, 0);
	}

	drawing = _mMatBuff[eImgDrawColor];
	//contour.Line
	if (0)
	{
		for (size_t i = 0; i < contours.size() / 2; i++)
		{
			drawContours(drawing, contours, (int)2 * i + 1, vScalar[i], 4, LINE_8, hierarchy, 0);
		}
	}

	//contour.Point
	if (1)
	{
		for (size_t row = 0; row < contours.size(); row++)
		{
			for (size_t col = 0; col < contours[row].size(); col++)
			{
				Point _pt = contours[row][col];
				//line(drawing, _pt, _pt, vScalar[row], 8, LINE_8);
				Xpoint(_pt, drawing);
			}
		}
	}

	Mat mask = _mMatBuff[eImgDrawColor].clone();
	mask = 0;
	drawContours(mask, contours, circle_index, Scalar(255, 255, 255), CV_FILLED, 8, hierarchy);
	drawing = drawing & mask;

	//imshow("Contours", drawing);
	_bShowDebug = true;
	_bShowResult = true;
	Invalidate(FALSE);
	return 0;
}

int COpenCVAppGUIDlg::OnInspFindcontourSample()
{
	Mat src_gray = _mMatBuff[eImgSrcGray];
	int thresh = 128;
	RNG rng(12345);

	//Mat canny_output;
	//Canny(src_gray, canny_output, thresh, thresh * 2);

	Mat thr_img;
	threshold(src_gray, thr_img, thresh, 255, THRESH_BINARY);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(thr_img, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

	//canny_output.copyTo(_mMatBuff[eImgDebugGray]);
	cvtColor(_mMatBuff[eImgDebugGray], _mMatBuff[eImgDebugColor], COLOR_GRAY2BGR);
	Mat drawing = _mMatBuff[eImgDebugColor];

	for (size_t i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
		drawContours(drawing, contours, (int)i, color, 2, LINE_8, hierarchy, 0);
	}
	drawing = _mMatBuff[eImgDrawColor];

	for (size_t i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
		drawContours(drawing, contours, (int)i, color, 2, LINE_8, hierarchy, 0);
	}

	int thickness = 5;
	int lineType = LINE_8;
	for (size_t i = 0; i < contours.size(); i++)
	{
		for (size_t k = 0; k < contours[i].size(); k++)
		{
			Point pt = contours[i][k];
			drawing = _mMatBuff[eImgDrawColor];
			line(drawing, Point(pt.x - 10, pt.y - 10), Point(pt.x + 10, pt.y + 10), Scalar(255, 255, 0), thickness, lineType);
			line(drawing, Point(pt.x + 10, pt.y - 10), Point(pt.x - 10, pt.y + 10), Scalar(255, 255, 0), thickness, lineType);
			drawing = _mMatBuff[eImgDebugColor];
			line(drawing, Point(pt.x - 10, pt.y - 10), Point(pt.x + 10, pt.y + 10), Scalar(255, 255, 0), thickness, lineType);
			line(drawing, Point(pt.x + 10, pt.y - 10), Point(pt.x - 10, pt.y + 10), Scalar(255, 255, 0), thickness, lineType);
		}
	}
	//imshow("Contours", drawing);
	_bShowDebug = true;
	_bShowResult = true;
	Invalidate(FALSE);
	return 0;
}



int COpenCVAppGUIDlg::CallInspFindcontourSample(void* lpUserData)
{
	COpenCVAppGUIDlg* pDlg = reinterpret_cast<COpenCVAppGUIDlg*>(lpUserData);
	//reinterpret_cast  
	return pDlg->OnInspFindcontourSample();
}


int COpenCVAppGUIDlg::CallInspFindShape(void* lpUserData)
{
	COpenCVAppGUIDlg* pDlg = reinterpret_cast<COpenCVAppGUIDlg*>(lpUserData);
	return pDlg->OnInspFindShapes();
	return 0;
}

void COpenCVAppGUIDlg::OnBnClickedBtnInspectionCv()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	auto f = _mInsps["OnInspFindShapes"];
	auto ret = f(this);
}


void COpenCVAppGUIDlg::OnBnClickedBtnSamplecode()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	vector<vector<Point>> contours;

	vector<Point> contour;
	//contour #1...object1
	contour.clear();
	contour.push_back(Point(10, 10));
	contour.push_back(Point(20, 10));
	contour.push_back(Point(10, 20));
	contour.push_back(Point(20, 20));
	contours.push_back(contour);

	//contour #2...object2
	contour.clear();
	contour.push_back(Point(110, 110));
	contour.push_back(Point(120, 110));
	contour.push_back(Point(110, 120));
	contour.push_back(Point(120, 120));
	contours.push_back(contour);

	Point a = contours[0][0];
	Point b = contours[1][0];
}
