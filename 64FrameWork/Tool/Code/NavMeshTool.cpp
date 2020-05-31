// ../Code/NavMeshTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "../Code/NavMeshTool.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "ToolView.h"
#include "MyForm.h"
#include "StaticObject.h"
#include "TestStage.h"
#include "DynamicCamera.h"
// CNavMeshTool ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CNavMeshTool, CDialogEx)

CNavMeshTool::CNavMeshTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{

}

CNavMeshTool::~CNavMeshTool()
{
	//for (auto &pNavMesh : m_pNavMeshVec)
	//{
	//	if (pNavMesh != nullptr)
	//	{
	//		delete pNavMesh;
	//		pNavMesh = nullptr;
	//	}
	//}
	//m_pNavMeshVec.clear();
	//Engine::Safe_Release(m_pDevice);
	//Engine::Safe_Release(m_pDeviceClass);
}

void CNavMeshTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_NavMeshTree);
	DDX_Control(pDX, IDC_EDIT4, m_EditMeshLeaf);
	DDX_Control(pDX, IDC_EDIT1, m_EditPosX);
	DDX_Control(pDX, IDC_EDIT2, m_EditPosY);
	DDX_Control(pDX, IDC_EDIT3, m_EditPosZ);
}

BEGIN_MESSAGE_MAP(CNavMeshTool, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CNavMeshTool::OnNavMeshCreateButton)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CNavMeshTool::OnSelectNavMesh)
	ON_BN_CLICKED(IDC_BUTTON2, &CNavMeshTool::OnBnClickedDeleteButton)
END_MESSAGE_MAP()

void CNavMeshTool::OnNavMeshCreateButton()
{
	_uint uiNavIdx;
	uiNavIdx = m_ppCellVec->size();
	
	if (m_ppCellVec != nullptr)
	{
		Engine::NAVI_DATA* pNaviData = new Engine::NAVI_DATA;
		memset(pNaviData,0, sizeof(Engine::NAVI_DATA));
		//pNaviData->uiIdx = 0;
		//pNaviData->vPosition1 = {0.f,0.f,0.f};
		//pNaviData->vPosition2 = {0.f,0.f,0.f};
		//pNaviData->vPosition3 = {0.f,0.f,0.f};
		//pNaviData->uiIdx=(*m_ppCellVec).size();
		
		pNaviData->uiIdx =(*m_ppCellVec).size();

		CString csTemp;
		csTemp.Format(_T("Nav%d"), pNaviData->uiIdx);

		m_hNavMeshIdx = m_NavMeshTree.InsertItem(csTemp, 0, 0, m_hRoot, TVI_LAST);
		csTemp.Format(_T("%d (X= %f  Y= %f  Z=%f)"), 0, pNaviData->vPosition1.x, pNaviData->vPosition1.y, pNaviData->vPosition1.z);
		m_hNavMeshPoint = m_NavMeshTree.InsertItem(csTemp, 0, 0, m_hNavMeshIdx, TVI_LAST);

		csTemp.Format(_T("%d (X= %f  Y= %f  Z=%f)"), 1, pNaviData->vPosition2.x, pNaviData->vPosition2.y, pNaviData->vPosition2.z);
		m_hNavMeshPoint = m_NavMeshTree.InsertItem(csTemp, 0, 0, m_hNavMeshIdx, TVI_LAST);
		
		csTemp.Format(_T("%d (X= %f  Y= %f  Z=%f)"), 2, pNaviData->vPosition3.x, pNaviData->vPosition3.y, pNaviData->vPosition3.z);
		m_hNavMeshPoint = m_NavMeshTree.InsertItem(csTemp, 0, 0, m_hNavMeshIdx, TVI_LAST);

		m_pNavDataVec.push_back(pNaviData);
		m_pNaviCom->Add_Cell(pNaviData);
	}

	//csTemp.Format(_T("Nav%d"), pNavMesh->uiNavMeshIdx);
	//wstring wstrNavName = csTemp;
	//m_pScene->Get_Layer(L"GameLogic")->Add_GameObject(wstrNavName.c_str(), pCell);
	//m_pNavMap.insert(make_pair(wstrNavName, pCell));

	//TODO: �Ž� ������ Ʈ�� ��Ʈ�꿡 �־ ǥ������ Ŭ���ϸ� Ȯ���ϱ� ����  create �ϸ� m_pNavMap �ȿ� ��  
	//NAV_MESH *pNavMesh= new NAV_MESH;
	//ZeroMemory(pNavMesh, sizeof(NAV_MESH));
	//uiNavIdx = m_pNavMeshVec.size();
	//pNavMesh->uiNavMeshIdx = uiNavIdx;
	//
	//m_pCell = CCell::Create(m_pDevice);

	////m_pNavMeshVec.push_back(pNavMesh);
	//
}

HRESULT CNavMeshTool::Update(const _float & fTimeDelta)
{
	if (m_pScene != nullptr)
	{
		dynamic_cast<CTestStage*>(m_pScene)->Get_Layer(L"GameLogic")->Get_ObjectMap();
		
		int iMapSize = dynamic_cast<CTestStage*>(m_pScene)->Get_Layer(L"GameLogic")->Get_ObjectMap().size();
		if (m_pNaviCom == nullptr)
		{
			m_pNaviCom = dynamic_cast<Engine::CNaviMesh*>(m_pScene->Get_GameObject(L"GameLogic", L"Player")->Get_Component(L"Com_Navi", Engine::ID_STATIC));
			m_ppCellVec = &m_pNaviCom->Get_CellVec();
		}
		if (0 < iMapSize)
			m_pCamera = dynamic_cast<CDynamicCamera*>(dynamic_cast<CTestStage*>(m_pScene)->Get_Layer(L"GameLogic")->Get_ObjectMap().find(L"DynamicCamera")->second);
	}


	if (m_pCamera != nullptr)
	{
		if (m_pCamera->IsPick())
		{
			m_vPos = m_pCamera->Get_PickPos();
			m_bIsColl = true;
	
		}
		else if (m_pCamera->IsNavPick())
		{
			m_vPos = m_pCamera->Get_PickPos();
			m_bIsColl = true;

		}

		if (m_bIsColl)
		{
			Set_NavMeshData();
			if (m_uiChangeIdx < 2)
				m_uiChangeIdx++;
			else
				m_uiChangeIdx = 0;
		}
		

	}

	return S_OK;
}

void CNavMeshTool::Set_NavMeshData()
{
	if (m_csCurParentItem.Compare(L"NavMesh") != 0)
		return;
	m_bIsColl = false;
	CString csTemp;

	HTREEITEM hCurITem = m_NavMeshTree.GetSelectedItem();
	HTREEITEM hChildItem = m_NavMeshTree.GetNextItem(hCurITem, TVGN_CHILD);

	for (int i = 0; i < m_uiChangeIdx; i++)
		hChildItem = m_NavMeshTree.GetNextItem(hChildItem, TVGN_NEXT);

	switch (m_uiChangeIdx)
	{
	case 0:
		m_pNavDataVec[m_uiSelectNavIdx]->vPosition1 = m_vPos;
		csTemp.Format(_T("%d (X= %f  Y= %f  Z=%f)"), 0, m_pNavDataVec[m_uiSelectNavIdx]->vPosition1.x,
														m_pNavDataVec[m_uiSelectNavIdx]->vPosition1.y, 
														m_pNavDataVec[m_uiSelectNavIdx]->vPosition1.z);
		break;
	case 1:
		m_pNavDataVec[m_uiSelectNavIdx]->vPosition2 = m_vPos;
		csTemp.Format(_T("%d (X= %f  Y= %f  Z=%f)"), 1, m_pNavDataVec[m_uiSelectNavIdx]->vPosition2.x,
														m_pNavDataVec[m_uiSelectNavIdx]->vPosition2.y,
														m_pNavDataVec[m_uiSelectNavIdx]->vPosition2.z);
		break;
	case 2:
		m_pNavDataVec[m_uiSelectNavIdx]->vPosition3 = m_vPos;
		csTemp.Format(_T("%d (X= %f  Y= %f  Z=%f)"), 2, m_pNavDataVec[m_uiSelectNavIdx]->vPosition3.x,
														m_pNavDataVec[m_uiSelectNavIdx]->vPosition3.y,
														m_pNavDataVec[m_uiSelectNavIdx]->vPosition3.z);
		break;
	}
	m_NavMeshTree.SetItemText(hChildItem, csTemp);
	
	if (m_pCamera->IsNavPick())
		(*m_ppCellVec)[m_uiSelectNavIdx]->Set_NaviData((Engine::CCell::POINT) m_uiChangeIdx, m_vPos);
	if(m_pCamera->IsPick())
		(*m_ppCellVec)[m_uiSelectNavIdx]->Set_NaviData(*m_pNavDataVec[m_uiSelectNavIdx]);

	m_pNaviCom->Link_Cell();
	//dynamic_cast<CTestStage*>(m_pScene)->Set_MeshVec(m_pNavDataVec);
			
}

//vector<NAV_MESH*>& CNavMeshTool::Get_NavMeshVec()
//{
//	return m_pNavMeshVec;
//}


BOOL CNavMeshTool::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_hRoot = m_NavMeshTree.InsertItem(TEXT("NavMesh"), 0, 0, TVI_ROOT, TVI_LAST);

	if (m_pDeviceClass == nullptr)
	{
		CMainFrame* pFrameWnd = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		if (pFrameWnd == nullptr)
			return false;


		m_pMyform = dynamic_cast<CMyForm*>(pFrameWnd->m_MainSplitter.GetPane(0, 0));
		m_pDeviceClass = m_pMyform->Get_DeviceClass();
		m_pDevice = m_pMyform->Get_Device();
		m_pScene = m_pMyform->Get_Scene();
	}


	return TRUE;  
}


void CNavMeshTool::OnSelectNavMesh(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	m_uiChangeIdx = 0;

	HTREEITEM hCurITem = m_NavMeshTree.GetSelectedItem();
	HTREEITEM hParentItem = m_NavMeshTree.GetNextItem(hCurITem, TVGN_PARENT);
	CString csCurText, csCurTextIdx, csNaviDataIdx,csSub;

	m_csCurNav= m_csCurParentItem = m_NavMeshTree.GetItemText(hParentItem);

	csCurText = m_NavMeshTree.GetItemText(hCurITem);
	m_uiSelectNavIdx = csCurText.Find('v');
	
	csCurTextIdx = csCurText.Right(csCurText.GetLength()- m_uiSelectNavIdx-1);
	m_uiSelectNavIdx = _ttoi(csCurTextIdx);


	_uint uiChildIdx = csCurText.Find('(');
	csSub = csCurText.Left(uiChildIdx - 1);

	//uiChildIdx = _ttoi(csSub);
	if (m_csCurNav.Compare(L"NavMesh") < 0)
		m_csCurNav = m_csCurNav + "_" + csSub;
	else
		m_csCurNav= csCurText;

	m_EditMeshLeaf.SetWindowTextW(m_csCurNav);


	*pResult = 0;
}


void CNavMeshTool::OnOK()
{


	//CDialogEx::OnOK();
}


void CNavMeshTool::OnCancel()
{
	//CDialogEx::OnCancel();
}


void CNavMeshTool::OnBnClickedDeleteButton()
{
	int i = 0;
	for (auto itr = (*m_ppCellVec).begin(); itr != (*m_ppCellVec).end();i++)
	{
		if (m_uiSelectNavIdx == i)
		{
			(*itr)->Release();
			itr = (*m_ppCellVec).erase(itr);
			break;
		}
		else
			itr++;
	}

	_uint uiNavIdx;
	uiNavIdx = m_ppCellVec->size();
	m_NavMeshTree.DeleteAllItems();
	m_hRoot = m_NavMeshTree.InsertItem(TEXT("NavMesh"), 0, 0, TVI_ROOT, TVI_LAST);

	if (m_ppCellVec != nullptr)
	{
		_uint uiCellIdx= 0;
		for (auto pCell : (*m_ppCellVec))
		{
			Engine::NAVI_DATA* pNaviData = &pCell->Get_NaviData();
			memset(pNaviData, 0, sizeof(Engine::NAVI_DATA));
			//pNaviData->uiIdx = 0;
			//pNaviData->vPosition1 = {0.f,0.f,0.f};
			//pNaviData->vPosition2 = {0.f,0.f,0.f};
			//pNaviData->vPosition3 = {0.f,0.f,0.f};
			//pNaviData->uiIdx=(*m_ppCellVec).size();

			pNaviData->uiIdx = uiCellIdx;

			CString csTemp;
			csTemp.Format(_T("Nav%d"), pNaviData->uiIdx);

			m_hNavMeshIdx = m_NavMeshTree.InsertItem(csTemp, 0, 0, m_hRoot, TVI_LAST);
			csTemp.Format(_T("%d (X= %f  Y= %f  Z=%f)"), 0, pNaviData->vPosition1.x, pNaviData->vPosition1.y, pNaviData->vPosition1.z);
			m_hNavMeshPoint = m_NavMeshTree.InsertItem(csTemp, 0, 0, m_hNavMeshIdx, TVI_LAST);

			csTemp.Format(_T("%d (X= %f  Y= %f  Z=%f)"), 1, pNaviData->vPosition2.x, pNaviData->vPosition2.y, pNaviData->vPosition2.z);
			m_hNavMeshPoint = m_NavMeshTree.InsertItem(csTemp, 0, 0, m_hNavMeshIdx, TVI_LAST);

			csTemp.Format(_T("%d (X= %f  Y= %f  Z=%f)"), 2, pNaviData->vPosition3.x, pNaviData->vPosition3.y, pNaviData->vPosition3.z);
			m_hNavMeshPoint = m_NavMeshTree.InsertItem(csTemp, 0, 0, m_hNavMeshIdx, TVI_LAST);
			uiCellIdx++;
		}
	}



}
