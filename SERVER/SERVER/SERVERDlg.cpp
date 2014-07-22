
// SERVERDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SERVER.h"
#include "SERVERDlg.h"
#include "afxdialogex.h"
using namespace std;

#include "DATA.h"
#include "BLL_WaitConn.h"
#include "BLL_DataSR.h"
#include "BLL_Dute.h"
#include "BLL_Init.h"


#define THREAD_NUM 10


HANDLE handle[THREAD_NUM];



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSERVERDlg 对话框




CSERVERDlg::CSERVERDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSERVERDlg::IDD, pParent)
	, m_ClientIP(_T(""))
	, m_recvOut(_T(""))
	, m_recvDate(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSERVERDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_ClientIP);
	DDX_Text(pDX, IDC_EDIT2, m_recvOut);
	DDX_Text(pDX, IDC_EDIT3, m_recvDate);
}

BEGIN_MESSAGE_MAP(CSERVERDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CSERVERDlg::OnClicked_refresh)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSERVERDlg 消息处理程序

BOOL CSERVERDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);		
	SetIcon(m_hIcon, FALSE);
	SetTimer(1,1000, NULL);

	int nIndex =1;
	BLL_InitFunc(NULL);

	handle[nIndex] = (HANDLE)_beginthreadex(NULL, 0, main_WaitConn, &nIndex, 0, NULL);
	nIndex++;
	handle[nIndex] = (HANDLE)_beginthreadex(NULL, 0, BLL_DataSRFunc, &nIndex, 0, NULL);
	nIndex++;
	handle[nIndex] = (HANDLE)_beginthreadex(NULL, 0, BLL_DuteFunc, &nIndex, 0, NULL);













	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSERVERDlg::OnPaint()
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
HCURSOR CSERVERDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSERVERDlg::OnClicked_refresh()
{
	UpdateData(TRUE);
	if (pstClientLIST->nNodeCount == 0)
	{
		return;
	}
	else
	{
		EnterCriticalSection(&g_csClientLIST);	// 进入临界区	客户端表
		char* pcIPaddress = (char*)pstClientLIST->pstFirstNode;
		int	 *pnTmp	 =	(int *)pcIPaddress;
		P_ClientAddr_T pstClientNode = (P_ClientAddr_T)(*pnTmp);

		CString CSclientIP;
		CSclientIP.Format("%d",pstClientNode->unIpAddress);

		m_ClientIP.Append(CSclientIP);
		m_ClientIP.Append("\n");

		UpdateData(FALSE);

		LeaveCriticalSection(&g_csClientLIST);		// 离开临界区	客户端表
	}
	
}


void CSERVERDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	UpdateData(TRUE);
	if (pstClientLIST->nNodeCount == 0)
	{
		return;
	}
	else
	{
		PLIST_NODE_T pstTempNode = pstClientLIST->pstFirstNode;

		for (int nIndex = 0;nIndex<pstClientLIST->nNodeCount; nIndex++)
		{
			EnterCriticalSection(&g_csClientLIST);	// 进入临界区	客户端表

			char* pcIPaddress = (char*)pstTempNode;

			int	 *pnTmp	 =	(int *)pcIPaddress;
			P_ClientAddr_T pstClientNode = (P_ClientAddr_T)(*pnTmp);

			CString CSclientIP;
			CSclientIP.Format("%d",pstClientNode->unIpAddress);

			m_ClientIP = CSclientIP;

			UpdateData(FALSE);

			LeaveCriticalSection(&g_csClientLIST);		// 离开临界区	客户端表

			pstTempNode = pstTempNode->pstNexter;

		}
		
	}

	CDialogEx::OnTimer(nIDEvent);
}
