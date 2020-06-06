// ../Code/ColliderTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "../Code/ColliderTool.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "MyForm.h"
#include "StaticObject.h"
#include "DynamicObject.h"
#include "TestStage.h"
#include "DynamicCamera.h"


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
	DDX_Control(pDX, IDC_DyMeshTREE, m_ObjectTree);
	DDX_Control(pDX, IDC_BoneTREE, m_BoneTree);
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


void CColliderTool::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	//CDialogEx::OnOK();
}

HRESULT CColliderTool::Update(const _float & fTimeDelta)
{
	if (m_pScene != nullptr)
	{
		int iMapSize = 0;
		if (m_ppGameObjectMap == nullptr)
			m_ppGameObjectMap = &dynamic_cast<CTestStage*>(m_pScene)->Get_Layer(L"GameLogic")->Get_ObjectMap();
		else
			iMapSize = (*m_ppGameObjectMap).size();
		if (0 < iMapSize)
			m_pCamera = dynamic_cast<CDynamicCamera*>(((*m_ppGameObjectMap).find(L"DynamicCamera")->second));

		if (iMapSize != m_uiMapSize)
		{
			m_uiMapSize = iMapSize;
			for (auto pObject : (*m_ppGameObjectMap))
			{
				_bool bIsCompare = false;
				m_pDynamicObject =dynamic_cast<CDynamicObject*>(pObject.second);
				wstring wstrObjName= pObject.second->Get_ObjName();

				if (m_pDynamicObject != nullptr)
				{
					HTREEITEM hChild = m_ObjectTree.GetChildItem(m_hDynamicRoot);
					CString csText = m_ObjectTree.GetItemText(hChild);

					if (csText.Find(wstrObjName.c_str()) != -1) //있음
						bIsCompare = true;
					while (hChild = m_ObjectTree.GetNextSiblingItem(hChild))//
					{
						CString csText = m_ObjectTree.GetItemText(hChild);
						if (csText.Find(wstrObjName.c_str()) != -1)
							bIsCompare = true;
					}


					if(!bIsCompare)
						m_hInstDynamic = m_ObjectTree.InsertItem(wstrObjName.c_str(), 0, 0, m_hDynamicRoot, TVI_LAST);
				}


			}

		}



	}

	return S_OK;
}


BOOL CColliderTool::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//m_hStaticMesh = m_StaticTree.InsertItem(TEXT("StaticMesh"), 0, 0, TVI_ROOT, TVI_LAST);
	//m_hDynamicMesh = m_StaticTree.InsertItem(TEXT("DynamicMesh"), 0, 0, TVI_ROOT, TVI_LAST);


	if (m_pDeviceClass == nullptr)
	{
		CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		NULL_CHECK_RETURN(pFrameWnd, L"df");

		m_pMyform = dynamic_cast<CMyForm*>(pFrameWnd->m_MainSplitter.GetPane(0, 0));
		m_pDeviceClass = m_pMyform->Get_DeviceClass();
		m_pDevice = m_pMyform->Get_Device();
		m_pScene = m_pMyform->Get_Scene();
		m_pKeyManager = CKeyMgr::GetInstance();
	}
	//m_hStaticRoot	=	m_ObjectTree.InsertItem(TEXT("StaticObject"), 0, 0, TVI_ROOT, TVI_LAST);
	m_hDynamicRoot = m_ObjectTree.InsertItem(TEXT("DynamicObject"), 0, 0, TVI_ROOT, TVI_LAST);



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
