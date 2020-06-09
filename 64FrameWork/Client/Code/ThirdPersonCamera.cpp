#include "stdafx.h"
#include "ThirdPersonCamera.h"
#include "Export_Function.h"

CThirdPersonCamera::CThirdPersonCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CCamera(pGraphicDev)
{

}

CThirdPersonCamera::~CThirdPersonCamera(void)
{

}

void CThirdPersonCamera::SetUp_Target(Engine::CTransform* pTargetInfo)
{
	m_pTargetInfo = pTargetInfo;
}

HRESULT CThirdPersonCamera::Ready_GameObject(const _vec3* pEye,
	const _vec3* pAt,
	const _vec3* pUp,
	const _float& fFovY,
	const _float& fAspect,
	const _float& fNear,
	const _float& fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFovY = fFovY;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	FAILED_CHECK_RETURN(Engine::CCamera::Ready_GameObject(), E_FAIL);


	return S_OK;
}

_int CThirdPersonCamera::Update_GameObject(const _float& fTimeDelta)
{
	if (m_pTargetInfo == nullptr)
		m_pTargetInfo = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Transform", Engine::ID_DYNAMIC));

	Target_Renewal(fTimeDelta);
	Mouse_Move(fTimeDelta);
	Key_Input(fTimeDelta);
	_int iExit = Engine::CCamera::Update_GameObject(fTimeDelta);



	return iExit;
}

void CThirdPersonCamera::Key_Input(const _float& fTimeDelta)
{

}

void CThirdPersonCamera::Target_Renewal(const _float& fTimeDelta)
{
	if (nullptr == m_pParentBoneMatrix)
	{
		Engine::CDynamicMesh*	pPlayerMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Mesh", Engine::ID_STATIC));
		if (pPlayerMeshCom == nullptr)
			return;

		const Engine::D3DXFRAME_DERIVED* pBone = pPlayerMeshCom->Get_FrameByName("Head");
		if (pBone == nullptr)
			return;

		m_pParentBoneMatrix = &pBone->CombinedTransformationMatrix;

		Engine::CTransform*	pPlayerTransCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Transform", Engine::ID_DYNAMIC));

		m_pParentWorldMatrix = m_pTargetInfo->Get_WorldMatrixPointer();
	}

		m_vOldEye = m_pTargetInfo->m_vInfo[Engine::INFO_LOOK] * -1.f;
	m_matWorld = (*m_pParentBoneMatrix * *m_pParentWorldMatrix);

	_vec3 vHeadPos;
	memcpy(&vHeadPos, &m_matWorld.m[3][0], sizeof(_vec3));
	m_vEye = m_vOldEye;
	D3DXVec3Normalize(&m_vEye, &m_vEye);

	m_vEye *= m_fDistance;

	_vec3		vRight;
	memcpy(&vRight, &m_pTargetInfo->m_matWorld.m[0][0], sizeof(_vec3));

	_matrix		matRotAxis;
	D3DXMatrixRotationAxis(&matRotAxis, &vRight, m_fLerpVerticalAngle);
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRotAxis);

	m_vEye += vHeadPos;
	m_vAt = vHeadPos;

	_vec3	vUp = _vec3(0.f, 1.f, 0.f);
	_vec3	vCamPos = m_vEye - m_vAt;

	D3DXMatrixRotationAxis(&matRotAxis, &vUp, D3DXToRadian(m_fHorizonAngle));
	D3DXVec3TransformNormal(&vCamPos, &vCamPos, &matRotAxis);

	m_vEye = m_vAt + vCamPos;

}

void CThirdPersonCamera::Mouse_Move(const _float & fTimeDelta)
{
	_long	dwMouseMove = 0;
	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_Y))
	{
		m_fOldVerticalAngle = m_fVerticalAngle;
		m_fVerticalAngle += D3DXToRadian(dwMouseMove*10.f*fTimeDelta);

		if (D3DXToDegree(m_fVerticalAngle) < 5.f)
			m_fVerticalAngle = D3DXToRadian(5.f);
		if (D3DXToDegree(m_fVerticalAngle) > 80.f)
			m_fVerticalAngle = D3DXToRadian(80.f);

	}

	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_X))
	{
		m_fOldHorizonAngle = m_fHorizonAngle;
		m_fHorizonAngle += D3DXToRadian(dwMouseMove*300.f)*fTimeDelta;
	
	
	}
	if (Engine::Get_DIMouseMove(Engine::DIMS_Z) > 0)
	{
		if (m_fDistance >= 2.0f)
			m_fDistance -= 0.5f;
	}
	if (Engine::Get_DIMouseMove(Engine::DIMS_Z) < 0)
	{
		if (m_fDistance <= 4.5f)
			m_fDistance += 0.5f;
	}
	Lerp_Cam(fTimeDelta);

}

void CThirdPersonCamera::Lerp_Cam(_float fTimeDelta)
{
	if (D3DXToDegree(fabsf(m_fHorizonAngle - m_fOldHorizonAngle)) >= 1.0f)
	{
		if (m_fTime[0] <= 1.0f)
			m_fTime[0] += fTimeDelta;

		m_fLerpHorizonAngle = Lerp(m_fOldHorizonAngle, m_fHorizonAngle, m_fTime[0]);
		if (m_fTime[0] >= 1.0f)
		{
			m_fTime[0] = 0.f;
			m_fOldHorizonAngle = m_fHorizonAngle;
		}
	}
	if (D3DXToDegree(fabsf(m_fVerticalAngle - m_fOldVerticalAngle)) >= 1.0f)
	{
		if (m_fTime[1] <= 1.0f)
			m_fTime[1] += fTimeDelta;
		m_fLerpVerticalAngle = (Lerp<_float>(m_fOldVerticalAngle, m_fVerticalAngle, m_fTime[1]));
		int a = 3;
		if (m_fTime[1] >= 1.0f)
		{
			m_fTime[1] = 0.f;
			m_fOldVerticalAngle = m_fVerticalAngle;
		}
	}

}

CThirdPersonCamera* CThirdPersonCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev,
	Engine::CTransform* pTargetInfo,
	const _vec3* pEye,
	const _vec3* pAt,
	const _vec3* pUp,
	const _float& fFovY,
	const _float& fAspect,
	const _float& fNear,
	const _float& fFar)
{
	CThirdPersonCamera*	pInstance = new CThirdPersonCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pEye, pAt, pUp, fFovY, fAspect, fNear, fFar)))
		Engine::Safe_Release(pInstance);

	pInstance->SetUp_Target(pTargetInfo);

	return pInstance;
}

void CThirdPersonCamera::Free(void)
{
	Engine::CCamera::Free();
}

