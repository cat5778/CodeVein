#ifndef Player_h__
#define Player_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CNaviMesh;
class CDynamicMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class CShader;

END

class CPlayer : public Engine::CGameObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer(void);

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);
	void		Key_Input(const _float& fTimeDelta);
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
	HRESULT		Load_Text(const TCHAR * pFilePath);
	wchar_t*	CharToWChar(const char* pstrSrc);

	//void		SetUp_OnTerrain(void);
	//_vec3		PickUp_OnTerrain(void);

private:
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CDynamicMesh*	m_pMeshCom = nullptr;
	Engine::CNaviMesh*		m_pNaviCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;
	CKeyMgr*				m_pKeyMgr = nullptr;
	_int					m_iAnim = 0;
	_vec3					m_vDir;
	_float					m_fSpeed = 20.f;
public:
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};

#endif // Player_h__
