// ../Code/ColliderTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "../Code/ColliderTool.h"
#include "afxdialogex.h"


// CColliderTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CColliderTool, CDialogEx)

CColliderTool::CColliderTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG4, pParent)
{

}

CColliderTool::~CColliderTool()
{
}

void CColliderTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CColliderTool, CDialogEx)
END_MESSAGE_MAP()


// CColliderTool 메시지 처리기입니다.
