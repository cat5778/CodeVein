#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CColliderTool 대화 상자입니다.
class CDynamicCamera;
class CMyForm;
class CKeyMgr;
class CDynamicObject;

class CColliderTool : public CDialogEx
{
	DECLARE_DYNAMIC(CColliderTool)

public:
	CColliderTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CColliderTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	afx_msg void OnBnClickedCollideleteButton();
	afx_msg void OnBnClickedClliderCreateButton();
	afx_msg void OnBnClickedColliderSaveButton();
	afx_msg void OnBnClickedColliderLoadButton();
	virtual void OnOK();
public:
	HRESULT								Update(const _float& fTimeDelta);
private:
	HTREEITEM							m_hDynamicMesh, m_hStaticMesh;
	HTREEITEM							m_hStaticRoot, m_hDynamicRoot, m_hInstDynamic, m_hInstStatic;

public:
	CTreeCtrl							m_ObjectTree;
	CTreeCtrl							m_BoneTree;

public:
	void								Get_BoneName();

private:
	Engine::CGraphicDev*				m_pDeviceClass = nullptr;
	LPDIRECT3DDEVICE9					m_pDevice = nullptr;
	CMyForm*							m_pMyform = nullptr;
	CKeyMgr*							m_pKeyManager = nullptr;
	CDynamicCamera*						m_pCamera = nullptr;
	Engine::CScene*						m_pScene = nullptr;
	CDynamicObject*						m_pDynamicObject = nullptr;
	CDynamicObject*						m_pCurSelectObj= nullptr;


	_uint								m_uiMapSize=0;
	CString								m_csSelectMesh;
	wstring								m_wstrSelectObject;
	CString								m_csScale[3];
	CString								m_csRotation[3];
	CString								m_csPosition[3];
	_vec3								m_vScale = { INIT_VEC3 };
	_vec3								m_vRotation = { INIT_VEC3 };
	_vec3								m_vPosition = { INIT_VEC3 };

	
	map<wstring, Engine::CGameObject*>*	m_ppGameObjectMap = nullptr;

public:
	afx_msg void OnTvnSelchangedDymeshTree(NMHDR *pNMHDR, LRESULT *pResult);
	CTreeCtrl m_ColliderTree;
	CEdit m_EditPositionX;
	CEdit m_EditPositionY;
};
