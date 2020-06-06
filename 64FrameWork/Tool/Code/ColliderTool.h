#pragma once
#include "afxcmn.h"


// CColliderTool ��ȭ �����Դϴ�.
class CDynamicCamera;
class CMyForm;
class CKeyMgr;
class CDynamicObject;

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

private:
	Engine::CGraphicDev*				m_pDeviceClass = nullptr;
	LPDIRECT3DDEVICE9					m_pDevice = nullptr;
	CMyForm*							m_pMyform = nullptr;
	CKeyMgr*							m_pKeyManager = nullptr;
	CDynamicCamera*						m_pCamera = nullptr;
	Engine::CScene*						m_pScene = nullptr;
	CDynamicObject*						m_pDynamicObject=nullptr;


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

};
