
// SERVERDlg.h : ͷ�ļ�
//

#pragma once


// CSERVERDlg �Ի���
class CSERVERDlg : public CDialogEx
{
// ����
public:
	CSERVERDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_ClientIP;
	CString m_recvOut;
	CString m_recvDate;
	afx_msg void OnClicked_refresh();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
