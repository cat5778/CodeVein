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
	explicit CColliderGroup(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjTag,Engine::CTransform* pTransform, CDynamicMesh* pMesh);
	virtual ~CColliderGroup(void);

public:
	HRESULT						Ready_Component(void);
	virtual _int				Update_Component(const _float& fTimeDelta);
	void						Render_Collider();

public:
	CSphereColliderCom*			Add_Collider(Engine::COLLOPTION eOption, wstring wstrBoneTag, _float fRadius = 10.f);
	CSphereColliderCom*			Add_Collider(Engine::COLLOPTION eOption, wstring wstrBoneTag,wstring wstrCollTag,_vec3 vPos , _float fRadius = 10.f, _uint uiIdx=0);
	void						Set_Pos(Engine::COLLOPTION eOption, wstring wstrBoneTag, _vec3 vPos);
	void						Set_Radius(Engine::COLLOPTION eOption, wstring wstrColliderTag, _float fRadius);
	Engine::CSphereColliderCom* Get_SphereColl(wstring wstrCollTag);
	vector<Engine::CSphereColliderCom*>* Get_CollVec(Engine::COLLOPTION eOption);
	
private:

private:
	LPDIRECT3DDEVICE9					m_pGraphicDev;
	Engine::CTransform*					m_pPartentTransform =nullptr;

	vector<Engine::CSphereColliderCom*> m_pHurtCollVec;
	vector<Engine::CSphereColliderCom*> m_pAttackCollVec;
	vector<Engine::CSphereColliderCom*> m_pStepCollVec;
	CDynamicMesh*						m_pDynamicMesh;
	wstring								m_wstrObjTag;
public:
	static CColliderGroup*				Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjTag,Engine::CTransform* pTransform, CDynamicMesh* pMesh);

private:
	virtual void		Free(void);

};

END
#endif // ColliderGroup_h__
