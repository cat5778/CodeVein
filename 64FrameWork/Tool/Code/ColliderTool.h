#pragma once


// CColliderTool ��ȭ �����Դϴ�.

class CColliderTool : public CDialogEx
{
	DECLARE_DYNAMIC(CColliderTool)

public:
	CColliderTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CColliderTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};