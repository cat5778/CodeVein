#include "stdafx.h"
#include "RussianHat.h"
#include "Export_Function.h"
#include "ColliderManager.h"


CRussianHat::CRussianHat(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName,_uint uiIdx)
	: CDynamicObject(pGraphicDev,wstrName,uiIdx)
{
	
}

CRussianHat::CRussianHat(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx, TRANSFORM_INFO tInfo)
	:CDynamicObject(pGraphicDev,wstrName,uiIdx,tInfo)
{
}

CRussianHat::~CRussianHat()
{
}

HRESULT CRussianHat::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	m_fCurHp = m_fMaxHp = 100.f;
	m_fAttackRange = 4.f;

	switch ((LOADMODE)LOAD_MODE)
	{
	case LOAD_NOMAL:
		m_pNaviCom->Set_Index(38);// Base Init Idx 38 

		Load_Text(L"../../Resource/Data/NavMash/BaseCompleteNav.txt");
		break;
	case LOAD_NOMAL2:
		m_pTransformCom->Set_Pos(-85.f, 1.3f, 0.01f);
		m_pNaviCom->Set_Index(0);// Base Init Idx 38 

		Load_Text(L"../../Resource/Data/NavMash/Temp5.txt");
		break;
	case LOAD_NOMAL3:
		break;

	case LOAD_PLAYER:
		break;
	case LOAD_MONSTER:
		break;
	case LOAD_BATTLE:
		m_pNaviCom->Set_Index(38);// Base Init Idx 38 
		Load_Text(L"../../Resource/Data/NavMash/BaseCompleteNav.txt");
		break;
	case LOAD_END:
		break;
	default:
		break;
	}
	m_eCurState = RUSSIAN_START_IDLE;
	
	
	Set_TransformData();
	
	m_pMeshCom->Set_AnimationSet(38);//Idle38 




	return S_OK;
}

HRESULT CRussianHat::LateReady_GameObject()
{
	m_pTargetTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Transform", Engine::ID_DYNAMIC));
	//m_pNaviCom = dynamic_cast<Engine::CNaviMesh*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Navi", Engine::ID_STATIC));
	//m_pNaviCom->Set_Index(38);// Base Init Idx 38 

	return S_OK;
}

_int CRussianHat::Update_GameObject(const _float & fTimeDelta)
{
	StateMachine();
	m_fDistance=Get_TargetDist();
	Pattern(fTimeDelta);
	
	





	Engine::CGameObject::Update_GameObject(fTimeDelta);
	m_pMeshCom->Play_Animation(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return S_OK;
}

void CRussianHat::Render_GameObject(void)
{

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_uint	iPassMax = 0;

	SetUp_ConstantTable(pEffect);

	pEffect->Begin(&iPassMax, 0);

	pEffect->BeginPass(0);

	m_pMeshCom->Render_Meshes(pEffect);

	pEffect->EndPass();

	pEffect->End();

	Safe_Release(pEffect);

	m_pColliderGroupCom->Render_Collider();



}

HRESULT CRussianHat::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(RESOURCE_STAGE, L"RussianHat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);// º¯°æ

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

	pComponent = m_pCalculatorCom = Engine::CCalculator::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);

	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pColliderGroupCom = Engine::CColliderGroup::Create(m_pGraphicDev, m_ObjName, m_pTransformCom, m_pMeshCom);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_DYNAMIC].emplace(L"Com_ColliderGroup", pComponent);

	pComponent = m_pNaviCom = dynamic_cast<Engine::CNaviMesh*>(Engine::CNaviMesh::Create(m_pGraphicDev));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Navi", pComponent);


	CColliderManager::GetInstance()->Get_ObjCollider(m_pColliderGroupCom, m_ObjName);

	return S_OK;
}

void CRussianHat::Set_TransformData()
{
	if (m_bIsRespawn)
	{
		m_pTransformCom->m_vScale = m_tTransformInfo.vScale;
		m_pTransformCom->m_vAngle = m_tTransformInfo.vRotation;
		m_pTransformCom->m_vInfo[Engine::INFO_POS] = m_tTransformInfo.vPosition;
	}
	else
	{
		m_pTransformCom->Set_Scale(DEFAULT_MESH_SCALE);
		m_pTransformCom->Set_Pos(INIT_VEC3);
	}
}

void CRussianHat::StateMachine()
{
	if (m_ePreState != m_eCurState)
	{
		m_ePreState = m_eCurState;

		switch (m_eCurState)
		{
		case RUSSIAN_START_IDLE:
			m_pMeshCom->Set_AnimationSet(39);
			if (m_pMeshCom->Get_TrackPosition() >= m_pMeshCom->Get_Period()*0.9f)
				m_eCurState = RUSSIAN_BATTLE_IDLE;
			break;
		case RUSSIAN_BATTLE_START:
			m_pMeshCom->Set_AnimationSet(40);
			break;
		case RUSSIAN_BATTLE_IDLE:
		{

			m_fAttackRange = 4.f;
			m_pMeshCom->Set_AnimationSet(38);
		}
			break;
		case RUSSIAN_IDLE_TURN://34~37
		{
			
		}
			break;
		case RUSSIAN_WALK://29~32
			m_pMeshCom->Set_AnimationSet(31);
			break;
		case RUSSIAN_RUN: //
			m_pMeshCom->Set_AnimationSet(33);
			break;
		case RUSSIAN_HURT://26~27
			break;
		case RUSSIAN_APPERANCE_N:
			m_pMeshCom->Set_AnimationSet(40);
			break;
		case RUSSIAN_DOWN:
			m_pMeshCom->Set_AnimationSet(25);
			break;
		case RUSSIAN_DODGE:
			m_pMeshCom->Set_AnimationSet(28);
			break;
		case RUSSIAN_ATTACK_READY:
		{
			m_fAttackRange = 4.f;
			m_pMeshCom->Set_AnimationSet(24);
		}
			break;
		case RUSSIAN_ATTACK1:
		{
			m_fAttackRange = 10.f;
			m_pMeshCom->Set_AnimationSet(23);
		}
			break;
		case RUSSIAN_ATTACK2:
			m_pMeshCom->Set_AnimationSet(22);
			break;
		case RUSSIAN_ATTACK3:
			m_pMeshCom->Set_AnimationSet(21);
			break;
		case RUSSIAN_ATTACK_SLIDE_S:
			m_pMeshCom->Set_AnimationSet(20);
			break;
		case RUSSIAN_ATTACK_SLIDE_L:
			m_pMeshCom->Set_AnimationSet(19);
			break;
		case RUSSIAN_ATTACK_SLIDE_E:
			m_pMeshCom->Set_AnimationSet(18);
			break;
		case RUSSIAN_ATTACK_HORN1:
			m_pMeshCom->Set_AnimationSet(16);
			break;
		case RUSSIAN_ATTACK_HORN2:
			m_pMeshCom->Set_AnimationSet(15);
			break;
		case RUSSIAN_ATTACK_JUMP:
			m_pMeshCom->Set_AnimationSet(14);
			break;
		case RUSSIAN_DEFORMATION:
			m_pMeshCom->Set_AnimationSet(13);
			break;
		case RUSSIAN_KETSUGI_AIMSHOOT:
			m_pMeshCom->Set_AnimationSet(17);
			break;
		case RUSSIAN_FIST_ATTACKBOOST1_S:
			m_pMeshCom->Set_AnimationSet(12);
			break;
		case RUSSIAN_FIST_ATTACKBOOST1_L:
			m_pMeshCom->Set_AnimationSet(11);
			break;
		case RUSSIAN_FIST_ATTACKBOOST1_A_E:
			m_pMeshCom->Set_AnimationSet(10);
			break;
		case RUSSIAN_FIST_ATTACKBOOST1_B_E:
			m_pMeshCom->Set_AnimationSet(9);
			break;
		case RUSSIAN_FIST_ATTACKNORMAL1:
			m_pMeshCom->Set_AnimationSet(7);
			break;
		case RUSSIAN_FIST_ATTACKJUMP_S:
			m_pMeshCom->Set_AnimationSet(6);
			break;
		case RUSSIAN_FIST_ATTACKJUMP_L:
			m_pMeshCom->Set_AnimationSet(5);
			break;
		case RUSSIAN_FIST_ATTACKJUMP_E:
			m_pMeshCom->Set_AnimationSet(4);
			break;
		case RUSSIAN_FIST_BOOSTUP_S:
			m_pMeshCom->Set_AnimationSet(3);
			break;
		case RUSSIAN_FIST_KEYSUGI_ALLRANGE_S:
			m_pMeshCom->Set_AnimationSet(2);
			break;
		case RUSSIAN_FIST_KEYSUGI_ALLRANGE_L:
			m_pMeshCom->Set_AnimationSet(1);
			break;
		case RUSSIAN_FIST_KEYSUGI_ALLRANGE_E:
			m_pMeshCom->Set_AnimationSet(0);
			break;
		case RUSSIAN_END:
			break;
		default:
			break;
		}

	}

}

void CRussianHat::Pattern(_float fTimeDelta)
{
	if (m_fCurHp / m_fMaxHp >= 0.6f)
		Phase1(fTimeDelta);
	else
	{

	}

}

_float CRussianHat::Get_TargetDist()
{
	return D3DXVec3Length(&(Get_TargetPos()-Get_Pos()));
}

void CRussianHat::Chaing_Target(_float fTimeDelta)
{
	if (m_pNaviCom != nullptr)
	{
		_vec3	vPos, vDir, vDiagonalDir, vOutPos, vLook;
		_float	fDgree;
		vLook = Get_Look();
		vDir = Get_TargetPos() - Get_Pos();
		m_fDistance = D3DXVec3Length(&vDir);
		vDir.y = 0.f;

		D3DXVec3Normalize(&vDir, &vDir);

		fDgree = Get_Angle(vDir, vLook);
		if ((fDgree <= 180.0f &&fDgree >= 5.0f) || (fDgree <= -5.0f&&fDgree >= -180.f))
		{
			if (fDgree >= 5.0f)
				m_pTransformCom->Rotation(Engine::ROT_Y, m_fSpeed* fTimeDelta);
			if (fDgree <= -5.0f)
				m_pTransformCom->Rotation(Engine::ROT_Y, -m_fSpeed* fTimeDelta);
		}

		m_pNaviCom->Move_OnNaviMesh(&Get_Pos(), &(vDir * m_fSpeed* fTimeDelta), &vOutPos);
		m_pTransformCom->Set_Pos(vOutPos.x, vOutPos.y, vOutPos.z);
	}

}

void CRussianHat::Phase1(_float fTimeDelta)
{
	if (m_fDistance >= m_fAttackRange)
	{
		m_eCurState = RUSSIAN_WALK;
		Chaing_Target(fTimeDelta);

	}
	else
	{
		if (m_fDistance <= m_fAttackRange)
		{
			switch (m_eCurState)
			{
			case RUSSIAN_BATTLE_IDLE:
				TshieldAttack_Ready(fTimeDelta);
				break;
			case RUSSIAN_DODGE:
				break;
			case RUSSIAN_ATTACK_READY:
				TshieldAttack_Ready(fTimeDelta);
				break;
			case RUSSIAN_ATTACK1:
				TshieldAttack(fTimeDelta);
				break;
			case RUSSIAN_ATTACK2:
				break;
			case RUSSIAN_ATTACK3:
				break;
			case RUSSIAN_ATTACK_SLIDE_S:
				break;
			case RUSSIAN_ATTACK_SLIDE_L:
				break;
			case RUSSIAN_ATTACK_SLIDE_E:
				break;
			case RUSSIAN_ATTACK_HORN1:
				break;
			case RUSSIAN_ATTACK_HORN2:
				break;
			case RUSSIAN_DEFORMATION:
				break;
			case RUSSIAN_KETSUGI_AIMSHOOT:
				break;
			default:
				Idle(fTimeDelta);

				break;
			}
			//m_uiPattern++;

		}
	}

}

void CRussianHat::TshieldAttack_Ready(_float fTimeDelta)
{
	if (m_eCurState == RUSSIAN_BATTLE_IDLE)
	{
		if (Get_AniRatio() >= 0.4f)
			m_eCurState = RUSSIAN_ATTACK_READY;
	}
	else if (m_eCurState == RUSSIAN_ATTACK_READY)
	{
		if (Get_AniRatio() >= 0.58f)
			m_eCurState = RUSSIAN_ATTACK1;

	}
}

void CRussianHat::TshieldAttack(_float fTimeDelta)
{
	if (m_eCurState == RUSSIAN_ATTACK1)
	{
		if (Get_AniRatio() >= 0.9f)
			m_eCurState = RUSSIAN_BATTLE_IDLE;
		else
		{
			MoveAni(fTimeDelta, 0.0f, 0.10f, 30.0f, Get_Look());
		}
	}
}

void CRussianHat::Idle(_float fTimeDelta)
{
	if(m_eCurState==RUSSIAN_WALK)
		m_eCurState = RUSSIAN_BATTLE_IDLE;
	else if(m_pMeshCom->Is_AnimationSetEnd())
		m_eCurState = RUSSIAN_BATTLE_IDLE;

}


CRussianHat * CRussianHat::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName,_uint uiIdx)
{
	CRussianHat*	pInstance = new CRussianHat(pGraphicDev, wstrName, uiIdx);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}
CRussianHat * CRussianHat::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx, TRANSFORM_INFO tInfo)
{
	CRussianHat*	pInstance = new CRussianHat(pGraphicDev, wstrName, uiIdx, tInfo);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;

}
void CRussianHat::Free(void)
{
	Engine::CGameObject::Free();
}
