#pragma once


#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CStaticMesh;
class CDynamicMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class CColliderGroup;
END


class CDynamicObject : public Engine::CGameObject
{
public:
	explicit				CDynamicObject(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx);
	explicit				CDynamicObject(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx, TRANSFORM_INFO tInfo);
	virtual					~CDynamicObject(void);

public:
	HRESULT					Ready_GameObject();
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			Render_GameObject(void) override;
	
public:
	_uint					Get_AnimationIdx() { return m_uiAni; }
	void					Set_AnimationIdx(_uint uiAnim) { m_uiAni= uiAnim; }
protected:
	virtual HRESULT			Add_Component(void);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

protected:
	virtual void			Set_TransformData();

protected:
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CDynamicMesh*	m_pMeshCom = nullptr;
	Engine::CColliderGroup* m_pColliderGroupCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;

	Engine::CCollider*		m_pColliderCom = nullptr;
	TRANSFORM_INFO			m_tTransformInfo;
	_bool					m_bIsRespawn = false;
	_uint					m_uiAni = 0;
public:
	static CDynamicObject*	Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx = 0);
	static CDynamicObject*	Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx ,  TRANSFORM_INFO tInfo);

protected:
	virtual void			Free(void) override;
};

