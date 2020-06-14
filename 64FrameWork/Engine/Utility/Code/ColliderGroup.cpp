#include "ColliderGroup.h"
#include "SphereColliderCom.h"
#include "DynamicMesh.h"
USING(Engine)

CColliderGroup::CColliderGroup(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjTag,Engine::CTransform* pTransform, CDynamicMesh* pMesh)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
	m_pPartentTransform = pTransform;
	m_pDynamicMesh = pMesh;
	m_wstrObjTag = wstrObjTag;

}

CColliderGroup::~CColliderGroup(void)
{
}


HRESULT CColliderGroup::Ready_Component(void)
{
	return S_OK;
}

_int CColliderGroup::Update_Component(const _float & fTimeDelta)
{
	for (auto &pColl : m_pHurtCollVec)
		pColl->Update_Component(fTimeDelta);
	for (auto &pColl : m_pAttackCollVec)
		pColl->Update_Component(fTimeDelta);
	for (auto &pColl : m_pStepCollVec)
		pColl->Update_Component(fTimeDelta);

		
	return 0;

}

void CColliderGroup::Render_Collider()
{
	for (auto &pColl : m_pHurtCollVec)
		pColl->Render_Collider();
	for (auto &pColl : m_pAttackCollVec)
		pColl->Render_Collider();
	for (auto &pColl : m_pStepCollVec)
		pColl->Render_Collider();

}


CSphereColliderCom* CColliderGroup::Add_Collider(Engine::COLLOPTION eOption,wstring wstrBoneTag, _float fRadius)
{
	string strBone;
	strBone.assign(wstrBoneTag.begin(), wstrBoneTag.end());
	
	const Engine::D3DXFRAME_DERIVED* pBone = m_pDynamicMesh->Get_FrameByName(strBone.c_str());
	
	_uint uiIdx = 0;
	for (auto &pColl : m_pHurtCollVec)
		if (pColl->Get_CollTag().find(wstrBoneTag) != wstring::npos)
			uiIdx++;
	for (auto &pColl : m_pAttackCollVec)
		if (pColl->Get_CollTag().find(wstrBoneTag) != wstring::npos)
			uiIdx++;
	for (auto &pColl : m_pStepCollVec)
		if (pColl->Get_CollTag().find(wstrBoneTag) != wstring::npos)
			uiIdx++;

	
	CSphereColliderCom* pSphereCollCom = CSphereColliderCom::Create(m_pGraphicDev,m_wstrObjTag, wstrBoneTag, m_pPartentTransform, pBone,uiIdx);
	if (pSphereCollCom == nullptr)
		return nullptr;
	pSphereCollCom->Set_Radius(fRadius);

	switch (eOption)
	{
	case Engine::COLOPT_STEP:
		m_pStepCollVec.push_back(pSphereCollCom);
		break;
	case Engine::COLOPT_HURT:
		m_pHurtCollVec.push_back(pSphereCollCom);
		break;
	case Engine::COLOPT_ATTACK:
		m_pAttackCollVec.push_back(pSphereCollCom);
		break;
	case Engine::COLOPT_END:
		break;
	default:
		break;
	}

	return pSphereCollCom;
}

CSphereColliderCom * CColliderGroup::Add_Collider(Engine::COLLOPTION eOption, wstring wstrBoneTag, wstring wstrCollTag, _vec3 vPos, _float fRadius ,_uint uiIdx)
{
	string strBone;
	strBone.assign(wstrBoneTag.begin(), wstrBoneTag.end());

	const Engine::D3DXFRAME_DERIVED* pBone = m_pDynamicMesh->Get_FrameByName(strBone.c_str());

	CSphereColliderCom* pSphereCollCom = CSphereColliderCom::Create(m_pGraphicDev, 
																	m_wstrObjTag, 
																	wstrBoneTag, 
																	m_pPartentTransform, 
																	pBone, 
																	uiIdx);
	if (pSphereCollCom == nullptr)
		return nullptr;

	pSphereCollCom->Set_Radius(fRadius);
	pSphereCollCom->Set_Pos(vPos);

	switch (eOption)
	{
	case Engine::COLOPT_STEP:
		m_pStepCollVec.push_back(pSphereCollCom);
		break;
	case Engine::COLOPT_HURT:
		m_pHurtCollVec.push_back(pSphereCollCom);
		break;
	case Engine::COLOPT_ATTACK:
		m_pAttackCollVec.push_back(pSphereCollCom);
		break;
	case Engine::COLOPT_END:
		break;
	default:
		break;
	}

	return pSphereCollCom;
}

void CColliderGroup::Set_Pos(Engine::COLLOPTION eOption, wstring wstrColliderTag, _vec3 vPos)
{
	switch (eOption)
	{
	case Engine::COLOPT_STEP:
		for (auto &pColl : m_pStepCollVec)
		{
			if (pColl->Get_BoneTag().compare(wstrColliderTag)==0)
				pColl->Set_Pos(vPos);
		}
		break;
	case Engine::COLOPT_HURT:
		for (auto &pColl : m_pHurtCollVec)
		{
			if (pColl->Get_BoneTag().compare(wstrColliderTag) == 0)
				pColl->Set_Pos(vPos);
		}

		break;
	case Engine::COLOPT_ATTACK:
		for (auto &pColl : m_pAttackCollVec)
		{
			if (pColl->Get_BoneTag().compare(wstrColliderTag) == 0)
				pColl->Set_Pos(vPos);
		}
		break;
	case Engine::COLOPT_END:
		break;
	default:
		break;
	}



}

void CColliderGroup::Set_Radius(Engine::COLLOPTION eOption, wstring wstrColliderTag,_float fRadius)
{
	switch (eOption)
	{
	case Engine::COLOPT_STEP:
		for (auto &pColl : m_pStepCollVec)
		{
			if (pColl->Get_BoneTag().compare(wstrColliderTag) == 0)
				pColl->Set_Radius(fRadius);
		}
		break;
	case Engine::COLOPT_HURT:
		for (auto &pColl : m_pHurtCollVec)
		{
			if (pColl->Get_BoneTag().compare(wstrColliderTag) == 0)
				pColl->Set_Radius(fRadius);
		}

		break;
	case Engine::COLOPT_ATTACK:
		for (auto &pColl : m_pAttackCollVec)
		{
			if (pColl->Get_BoneTag().compare(wstrColliderTag) == 0)
				pColl->Set_Radius(fRadius);
		}
		break;
	case Engine::COLOPT_END:
		break;
	default:
		break;
	}
}

Engine::CSphereColliderCom * CColliderGroup::Get_SphereColl(wstring wstrCollTag)
{
	Engine::CSphereColliderCom* pSphereColl = nullptr;
	
	for (auto &pColl : m_pStepCollVec)
	{
		if (pColl->Get_CollTag().compare(wstrCollTag) == 0)
			pSphereColl = pColl;
	}
	if (pSphereColl == nullptr)
	{
		for (auto &pColl : m_pHurtCollVec)
		{
			if (pColl->Get_CollTag().compare(wstrCollTag) == 0)
				pSphereColl = pColl;
		}
	}
	else if (pSphereColl == nullptr)
	{
		for (auto &pColl : m_pAttackCollVec)
		{
			if (pColl->Get_CollTag().compare(wstrCollTag) == 0)
				pSphereColl = pColl;
		}
	}

	return pSphereColl;
}

vector<Engine::CSphereColliderCom*>* CColliderGroup::Get_CollVec(Engine::COLLOPTION eOption)
{
	switch (eOption)
	{
	case Engine::COLOPT_STEP:
		return &m_pStepCollVec;
		break;
	case Engine::COLOPT_HURT:
		return &m_pHurtCollVec;
		break;
	case Engine::COLOPT_ATTACK:
		return &m_pAttackCollVec;
		break;
	case Engine::COLOPT_END:
		break;
	default:
		break;
	}
	return nullptr;
}

CColliderGroup * CColliderGroup::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjTag, Engine::CTransform* pTransform, CDynamicMesh * pMesh)
{
	CColliderGroup*	pInstance = new CColliderGroup(pGraphicDev, wstrObjTag, pTransform, pMesh);

	if (FAILED(pInstance->Ready_Component()))
		Engine::Safe_Release(pInstance);

	return pInstance;

}

void CColliderGroup::Free(void)
{
	for (auto &pColl : m_pHurtCollVec)
	{
		if (pColl != nullptr)
			pColl->Release();
	}
	for (auto &pColl : m_pAttackCollVec)
	{
		if (pColl != nullptr)
			pColl->Release();
	}

	for (auto &pColl : m_pStepCollVec)
	{
		if (pColl != nullptr)
			pColl->Release();
	}


		
	Safe_Release(m_pGraphicDev);

}
