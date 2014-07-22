
// SERVERDlg.cpp : ʵ���ļ�
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


// CSERVERDlg �Ի���




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


// CSERVERDlg ��Ϣ�������

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













	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSERVERDlg::OnPaint()
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
		EnterCriticalSection(&g_csClientLIST);	// �����ٽ���	�ͻ��˱�
		char* pcIPaddress = (char*)pstClientLIST->pstFirstNode;
		int	 *pnTmp	 =	(int *)pcIPaddress;
		P_ClientAddr_T pstClientNode = (P_ClientAddr_T)(*pnTmp);

		CString CSclientIP;
		CSclientIP.Format("%d",pstClientNode->unIpAddress);

		m_ClientIP.Append(CSclientIP);
		m_ClientIP.Append("\n");

		UpdateData(FALSE);

		LeaveCriticalSection(&g_csClientLIST);		// �뿪�ٽ���	�ͻ��˱�
	}
	
}


void CSERVERDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	UpdateData(TRUE);
	if (pstClientLIST->nNodeCount == 0)
	{
		return;
	}
	else
	{
		m_ClientIP = "";
		EnterCriticalSection(&g_csClientLIST);	// �����ٽ���	�ͻ��˱�

		PLIST_NODE_T pstTempNode = pstClientLIST->pstFirstNode;

		for (int nIndex = 0;nIndex<pstClientLIST->nNodeCount; nIndex++)
		{


			char* pcIPaddress = (char*)pstTempNode;

			int	 *pnTmp	 =	(int *)pcIPaddress;
			P_ClientAddr_T pstClientNode = (P_ClientAddr_T)(*pnTmp);

			CString CSclientIP;
			CSclientIP = (CString)pstClientNode->acIPstr;
			m_ClientIP.Append("\n");
			m_ClientIP.Append(CSclientIP);

			pstTempNode = pstTempNode->pstNexter;

		}

		LeaveCriticalSection(&g_csClientLIST);		// �뿪�ٽ���	�ͻ��˱�
	
	}
	UpdateData(FALSE);
	CDialogEx::OnTimer(nIDEvent);
}
