#ifndef SphereColliderCom_h__
#define SphereColliderCom_h__

#include "Component.h"

BEGIN(Engine)
class CTransform;
class ENGINE_DLL CSphereColliderCom : public CComponent
{

private:
	explicit CSphereColliderCom(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjTag, wstring wstrBoneTag, Engine::CTransform* pObjTransform, const Engine::D3DXFRAME_DERIVED * pBone);
	virtual ~CSphereColliderCom(void);

public:
	void						Render_Collider();
	HRESULT						Ready_Component(void);
	virtual _int				Update_Component(const _float& fTimeDelta);

public:
	const _matrix*				Get_ColliderMatrix(void) { return &m_matWorld; }
	void						Set_Radius(_float fRadius) { m_fRadius = fRadius; }
	HRESULT						Set_DMParentMatrix(const Engine::D3DXFRAME_DERIVED* pBone);
	void						Set_Pos(_vec3 vPos) { m_vPos = vPos; }
	_bool						IsColl() { return m_bIsColl; }
	void						Set_bIsColl(_bool bIsColl) {m_bIsColl = bIsColl;}
	wstring						Get_ObjTag() { return m_wstrObjTag; }
	wstring						Get_BoneTag() { return m_wstrBoneTag; }
private:
	void						Ready_SphereMesh();
	void						Debug_IsColl();



private:
	_float						m_fRadius			= 1.0f;
	_vec3						m_vPos				= { 0.f,0.f,0.f };
	_matrix						m_matWorld;
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	wstring						m_wstrObjTag = L"";
	wstring						m_wstrBoneTag = L"";

	wstring						m_wstrBone = L"";
	_bool						m_bIsColl = false;
	const	_matrix*			m_pParentBoneMatrix = nullptr;
	const	_matrix*			m_pParentWorldMatrix= nullptr;

#ifdef _DEBUG
	LPDIRECT3DTEXTURE9			m_pTexture[COL_END];
	LPD3DXMESH					m_pSphereMesh = nullptr;
#endif

public:
	static CSphereColliderCom*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjTag, wstring wstrBoneTag,Engine::CTransform* pObjTransform, const Engine::D3DXFRAME_DERIVED * pBone);

private:
	virtual void		Free(void);

};

END
#endif // SphereColliderCom_h__

