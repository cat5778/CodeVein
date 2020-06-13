#ifndef ColliderGroup_h__
#define ColliderGroup_h__

#include "Component.h"

BEGIN(Engine)
class CTransform;
class CSphereColliderCom;
class CDynamicMesh;

class ENGINE_DLL CColliderGroup : public CComponent
{

private:
	explicit CColliderGroup(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CTransform* pTransform, CDynamicMesh* pMesh);
	virtual ~CColliderGroup(void);

public:
	HRESULT						Ready_Component(void);
	virtual _int				Update_Component(const _float& fTimeDelta);
	void						Render_Collider();
public:
	void						Add_Collider(Engine::COLLOPTION eOption, wstring wstrObjTag, wstring wstrBoneTag,_float fRadius=10.f);
	void						Set_Pos(Engine::COLLOPTION eOption, wstring wstrBoneTag, _vec3 vPos);
	void						Set_Radius(Engine::COLLOPTION eOption, wstring wstrColliderTag, _float fRadius);

private:

private:
	LPDIRECT3DDEVICE9					m_pGraphicDev;
	Engine::CTransform*					m_pPartentTransform =nullptr;
	vector<Engine::CSphereColliderCom*> m_pHurtCollVec;
	vector<Engine::CSphereColliderCom*> m_pAttackCollVec;
	vector<Engine::CSphereColliderCom*> m_pStepCollVec;
	CDynamicMesh*						m_pDynamicMesh;

public:
	static CColliderGroup*				Create(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CTransform* pTransform, CDynamicMesh* pMesh);

private:
	virtual void		Free(void);

};

END
#endif // ColliderGroup_h__

