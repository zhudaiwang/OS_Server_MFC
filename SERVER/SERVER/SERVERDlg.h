
// SERVERDlg.h : 头文件
//

#pragma once


// CSERVERDlg 对话框
class CSERVERDlg : public CDialogEx
{
// 构造
public:
	CSERVERDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
