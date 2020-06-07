#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CColliderTool 대화 상자입니다.
class CDynamicCamera;
class CMyForm;
class CKeyMgr;
class CDynamicObject;
class CSphereCollider;
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
	virtual void						DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL						OnInitDialog();

public:
	afx_msg void						OnBnClickedCollideleteButton();
	afx_msg void						OnBnClickedClliderCreateButton();
	afx_msg void						OnBnClickedColliderSaveButton();
	afx_msg void						OnBnClickedColliderLoadButton();
	afx_msg void						OnTvnSelchangedDymeshTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void						OnTvnSelchangedBoneTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void						OnDeltaposSpinPosX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void						OnDeltaposSpinPosY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void						OnDeltaposSpinPosZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void						OnDeltaposSpinRadius(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void						OnDeltaposSpinAnim(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void						OnEnChangeEditRadius();
	virtual void						OnOK();
	virtual void						OnCancel();
public:
	HRESULT								Update(const _float& fTimeDelta);
private:
	CTreeCtrl							m_ColliderTree;
	CTreeCtrl							m_ObjectTree;
	CTreeCtrl							m_BoneTree;
	HTREEITEM							m_hDynamicMesh, m_hStaticMesh;
	HTREEITEM							m_hStaticRoot, m_hDynamicRoot, m_hInstDynamic, m_hInstStatic, m_hSelectBone;

	CEdit								m_EditAnim;
	CEdit								m_EditPositionX;
	CEdit								m_EditPositionY;
	CEdit								m_EditPositionZ;
	CEdit								m_EditRadius;

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
	Engine::CDynamicMesh*				m_pDynamicMesh = nullptr;
	CSphereCollider*					m_pSphereColl = nullptr;
	_uint								m_uiMapSize=0;
	CString								m_csSelectMesh;
	wstring								m_wstrSelectObject;
	wstring								m_wstrSelectBone;

	CString								m_csAnim;
	_uint								m_uiAnim = 0;

	CString								m_csRadius;
	_float								m_fRadius = 10.f;
	CString								m_csPosition[3];
	_vec3								m_vPosition = { INIT_VEC3 };

	
	map<wstring, Engine::CGameObject*>*	m_ppGameObjectMap = nullptr;

};
