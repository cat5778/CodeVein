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
#include "SphereCollider.h"

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
	DDX_Control(pDX, IDC_TREE1, m_ColliderTree);
	DDX_Control(pDX, IDC_EditPosX, m_EditPositionX);
	DDX_Control(pDX, IDC_EditPosY, m_EditPositionY);
	DDX_Control(pDX, IDC_EditPosZ, m_EditPositionZ);
	DDX_Control(pDX, IDC_EditRadius, m_EditRadius);
	DDX_Control(pDX, IDC_EDITAnim, m_EditAnim);
}


BEGIN_MESSAGE_MAP(CColliderTool, CDialogEx)
	ON_BN_CLICKED(IDC_ColliDeleteButton, &CColliderTool::OnBnClickedCollideleteButton)
	ON_BN_CLICKED(IDC_ClliderCreateButton, &CColliderTool::OnBnClickedClliderCreateButton)
	ON_BN_CLICKED(IDC_ColliderSaveButton, &CColliderTool::OnBnClickedColliderSaveButton)
	ON_BN_CLICKED(IDC_ColliderLoadButton, &CColliderTool::OnBnClickedColliderLoadButton)
	ON_NOTIFY(TVN_SELCHANGED, IDC_DyMeshTREE, &CColliderTool::OnTvnSelchangedDymeshTree)
	ON_NOTIFY(TVN_SELCHANGED, IDC_BoneTREE, &CColliderTool::OnTvnSelchangedBoneTree)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINPosX, &CColliderTool::OnDeltaposSpinPosX)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINPosY, &CColliderTool::OnDeltaposSpinPosY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINPosZ, &CColliderTool::OnDeltaposSpinPosZ)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINRadius, &CColliderTool::OnDeltaposSpinRadius)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINAnim, &CColliderTool::OnDeltaposSpinAnim)
	ON_EN_CHANGE(IDC_EditRadius, &CColliderTool::OnEnChangeEditRadius)
END_MESSAGE_MAP()


// CColliderTool 메시지 처리기입니다.



void CColliderTool::OnBnClickedCollideleteButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CColliderTool::OnBnClickedClliderCreateButton()
{
	Engine::CGameObject* pGameObject=nullptr;
	pGameObject=m_pSphereColl =CSphereCollider::Create(m_pDevice, m_wstrSelectObject, m_wstrSelectBone);
	wstring wstrColl = m_wstrSelectBone + L"_SphereCollider";

	(*m_ppGameObjectMap).insert(make_pair(wstrColl, pGameObject));

	m_vPosition =dynamic_cast<Engine::CTransform*>(m_pSphereColl->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC))->m_vInfo[Engine::INFO_POS];

	m_csPosition[0].Format(_T("%f"),m_vPosition.x);
	m_csPosition[1].Format(_T("%f"),m_vPosition.y);
	m_csPosition[2].Format(_T("%f"),m_vPosition.z);

	SetDlgItemTextW(IDC_EditPosX, m_csPosition[0]);
	SetDlgItemTextW(IDC_EditPosY, m_csPosition[1]);
	SetDlgItemTextW(IDC_EditPosZ, m_csPosition[2]);


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
	//CDialogEx::OnOK();
}
void CColliderTool::OnCancel()
{
	//CDialogEx::OnCancel();
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


					if (!bIsCompare)
					{
						m_hInstDynamic = m_ObjectTree.InsertItem(wstrObjName.c_str(), 0, 0, m_hDynamicRoot, TVI_LAST);
						m_wstrSelectObject= pObject.first;
					}
				}


			}

		}

	}

	return S_OK;
}

void CColliderTool::Get_BoneName()
{
	m_pDynamicMesh = dynamic_cast<Engine::CDynamicMesh*>(m_pCurSelectObj->Get_Component(L"Com_Mesh", Engine::ID_STATIC));
	if (m_pDynamicMesh == nullptr)
	{
		DE_COUT("Collider Tool 141 Error DynamicMesh is nullptr");

		return;
	}
	m_BoneTree.DeleteAllItems();

	list<Engine::D3DXMESHCONTAINER_DERIVED*>* list_Mesh = m_pDynamicMesh->Get_MeshContainerList();
	list<Engine::D3DXMESHCONTAINER_DERIVED*>::iterator iter = list_Mesh->begin();
	list<Engine::D3DXMESHCONTAINER_DERIVED*>::iterator end_iter = list_Mesh->end();

	//const Engine::D3DXFRAME_DERIVED* ptr = nullptr;
	//ptr = pDynamicMesh->Get_FrameByName("RightHandAttach");


	for (iter; iter != end_iter; ++iter)
	{
		//DWORD iNum= (*iter)->dwNumBones;

		DWORD iNum = (*iter)->pSkinInfo->GetNumBones();

		for (size_t i = 0; i < iNum; ++i)
		{
			string strBone= (*iter)->pSkinInfo->GetBoneName(i);
			wstring wstrBone;
			wstrBone.assign(strBone.begin(), strBone.end());
			//Engine::D3DXFRAME_DERIVED*	pBone = (Engine::D3DXFRAME_DERIVED*)D3DXFrameFind(pDynamicMesh->Get_RootFrame(), ch);

			m_BoneTree.InsertItem(wstrBone.c_str(), 0, 0, TVI_ROOT, TVI_SORT); //<-TODO: 고치는중 
		}
	}


}


BOOL CColliderTool::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	for(int i=0; i<3; i++)
		m_csPosition[i].Format(_T("%f"), 0.f);
	m_csRadius.Format(_T("%f"), m_fRadius);

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


void CColliderTool::OnTvnSelchangedDymeshTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	HTREEITEM hCurITem = m_ObjectTree.GetSelectedItem();
	m_csSelectMesh = m_ObjectTree.GetItemText(hCurITem);
	
	
	wstring wstrSelectMesh = m_csSelectMesh;
	wstrSelectMesh += L"_0";
	if (wstrSelectMesh.find(L"Object") == wstring::npos)
	{
		if (m_ppGameObjectMap != nullptr)
		{
			m_pCurSelectObj = dynamic_cast<CDynamicObject*>((*(*m_ppGameObjectMap).find(wstrSelectMesh)).second);
			if (m_pDynamicObject != nullptr)
				Get_BoneName();
		}
	}

	*pResult = 0;
}




void CColliderTool::OnTvnSelchangedBoneTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	m_hSelectBone =m_BoneTree.GetSelectedItem();
	CString csBone= m_BoneTree.GetItemText(m_hSelectBone);

	m_wstrSelectBone = csBone;

	CStringA strA(csBone);
	const char* pszBone = (char *)(LPWSTR)(LPCSTR)strA;
	m_csPosition[0].Format(_T("%f"), m_pDynamicMesh->Get_FrameByName(pszBone)->TransformationMatrix(4, 1));
	m_csPosition[1].Format(_T("%f"), m_pDynamicMesh->Get_FrameByName(pszBone)->TransformationMatrix(4, 2));
	m_csPosition[2].Format(_T("%f"), m_pDynamicMesh->Get_FrameByName(pszBone)->TransformationMatrix(4, 3));
	
	SetDlgItemTextW(IDC_EditPosX, m_csPosition[0]);
	SetDlgItemTextW(IDC_EditPosY, m_csPosition[1]);
	SetDlgItemTextW(IDC_EditPosZ, m_csPosition[2]);


	*pResult = 0;
}


void CColliderTool::OnDeltaposSpinPosX(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	m_vPosition.x = _tstof(m_csPosition[0]);

	if (pNMUpDown->iDelta < 0)
		m_vPosition.x += 1.0f;
	else
		m_vPosition.x -= 1.0f;

	m_csPosition[0].Format(_T("%f"), m_vPosition.x);
	SetDlgItemTextW(IDC_EditPosX, m_csPosition[0]);
	if (m_pSphereColl!=nullptr)
		dynamic_cast<Engine::CTransform*>(m_pSphereColl->Get_Component(L"Com_Transform",Engine::ID_DYNAMIC))->m_vInfo[Engine::INFO_POS].x = m_vPosition.x;

	*pResult = 0;
}


void CColliderTool::OnDeltaposSpinPosY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	m_vPosition.y = _tstof(m_csPosition[1]);

	if (pNMUpDown->iDelta < 0)
		m_vPosition.y += 1.0f;
	else
		m_vPosition.y -= 1.0f;

	m_csPosition[1].Format(_T("%f"), m_vPosition.y);
	SetDlgItemTextW(IDC_EditPosY, m_csPosition[1]);

	if (m_pSphereColl != nullptr)
		dynamic_cast<Engine::CTransform*>(m_pSphereColl->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC))->m_vInfo[Engine::INFO_POS].y = m_vPosition.y;



	*pResult = 0;
}


void CColliderTool::OnDeltaposSpinPosZ(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	m_vPosition.z = _tstof(m_csPosition[2]);

	if (pNMUpDown->iDelta < 0)
		m_vPosition.z += 1.0f;
	else
		m_vPosition.z -= 1.0f;

	m_csPosition[2].Format(_T("%f"), m_vPosition.z);
	SetDlgItemTextW(IDC_EditPosZ, m_csPosition[2]);
	if (m_pSphereColl != nullptr)
		dynamic_cast<Engine::CTransform*>(m_pSphereColl->Get_Component(L"Com_Transform", Engine::ID_DYNAMIC))->m_vInfo[Engine::INFO_POS].z = m_vPosition.z;

	*pResult = 0;
}


void CColliderTool::OnDeltaposSpinRadius(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	m_fRadius = _tstof(m_csRadius);

	if (pNMUpDown->iDelta < 0)
		m_fRadius += 1.f;
	else
		m_fRadius -= 1.f;

	m_csRadius.Format(_T("%f"), m_fRadius);
	SetDlgItemTextW(IDC_EditRadius, m_csRadius);

	if (m_pSphereColl != nullptr)
		m_pSphereColl->Set_Radius(m_fRadius);


	*pResult = 0;
}


void CColliderTool::OnDeltaposSpinAnim(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	m_uiAnim = _tstoi(m_csAnim);

	if (pNMUpDown->iDelta < 0)
		m_uiAnim += 1;
	else
	{
		if (m_uiAnim > 0)
			m_uiAnim -= 1;
	}
	m_csAnim.Format(_T("%d"), m_uiAnim);
	SetDlgItemTextW(IDC_EDITAnim, m_csAnim);

	if (m_pSphereColl != nullptr)
		m_pCurSelectObj->Set_AnimationIdx(m_uiAnim);
	
	
	*pResult = 0;
}


void CColliderTool::OnEnChangeEditRadius()
{
	GetDlgItemText(IDC_EditRadius, m_csRadius);
	if (m_pSphereColl != nullptr)
		m_pSphereColl->Set_Radius(_tstof(m_csRadius));

}
