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
	ON_BN_CLICKED(IDC_ColliDeleteButton, &CColliderTool::OnBnClickedCollideleteButton)
	ON_BN_CLICKED(IDC_ClliderCreateButton, &CColliderTool::OnBnClickedClliderCreateButton)
	ON_BN_CLICKED(IDC_ColliderSaveButton, &CColliderTool::OnBnClickedColliderSaveButton)
	ON_BN_CLICKED(IDC_ColliderLoadButton, &CColliderTool::OnBnClickedColliderLoadButton)
END_MESSAGE_MAP()


// CColliderTool 메시지 처리기입니다.



void CColliderTool::OnBnClickedCollideleteButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CColliderTool::OnBnClickedClliderCreateButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CColliderTool::OnBnClickedColliderSaveButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CColliderTool::OnBnClickedColliderLoadButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
