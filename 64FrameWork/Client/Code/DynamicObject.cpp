#include "stdafx.h"
#include "DynamicObject.h"
#include "Export_Function.h"
#include "ColliderManager.h"
#include <fstream>

CDynamicObject::CDynamicObject(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName,_uint uiIdx)
	: Engine::CGameObject(pGraphicDev)
{
	m_uiIdx = uiIdx;
	m_ObjName = wstrName;
}

CDynamicObject::CDynamicObject(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx, TRANSFORM_INFO tInfo)
	: Engine::CGameObject(pGraphicDev)
{
	m_uiIdx = uiIdx;
	m_ObjName = wstrName;
	m_tTransformInfo = tInfo;
	m_bIsRespawn = true;
}

CDynamicObject::~CDynamicObject()
{
}

HRESULT CDynamicObject::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	Set_TransformData();

	m_pMeshCom->Set_AnimationSet(0);

	return S_OK;
}

_int CDynamicObject::Update_GameObject(const _float & fTimeDelta)
{
	m_pMeshCom->Set_AnimationSet(m_uiAni);
	Engine::CGameObject::Update_GameObject(fTimeDelta);
	m_pMeshCom->Play_Animation(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return S_OK;
}

void CDynamicObject::Render_GameObject(void)
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);
	//m_pMeshCom->Render_Meshes();
	//_matrix		matWorld;
	//m_pTransformCom->Get_WorldMatrix(&matWorld);
	//m_pColliderGroupCom->Render_Collider();

	_matrix mat = m_pTransformCom->m_matWorld;

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


HRESULT CDynamicObject::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(RESOURCE_STAGE, m_ObjName.c_str()));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);// 변경

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



	pComponent = m_pColliderGroupCom = Engine::CColliderGroup::Create(m_pGraphicDev,m_ObjName,m_pTransformCom,m_pMeshCom);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_DYNAMIC].emplace(L"Com_ColliderGroup", pComponent);
	

	CColliderManager::GetInstance()->Get_ObjCollider(m_pColliderGroupCom, m_ObjName);

	// collider
	//pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev,
	//	m_pMeshCom->Get_VtxPos(),
	//	m_pMeshCom->Get_NumVtx(),
	//	m_pMeshCom->Get_Stride());
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	return S_OK;
}

HRESULT CDynamicObject::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	const D3DLIGHT9*		pLight = Engine::Get_LightInfo(0);
	NULL_CHECK_RETURN(pLight, S_OK);

	pEffect->SetVector("g_vLightDir", &_vec4(pLight->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLight->Diffuse);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLight->Specular);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLight->Ambient);

	D3DMATERIAL9		tMtrlInfo;
	ZeroMemory(&tMtrlInfo, sizeof(D3DMATERIAL9));

	tMtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	tMtrlInfo.Power = 100.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&tMtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&tMtrlInfo.Specular);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&tMtrlInfo.Ambient);

	_vec4		vCamPos;
	D3DXMatrixInverse(&matView, NULL, &matView);
	memcpy(&vCamPos, &matView.m[3][0], sizeof(_vec4));

	pEffect->SetVector("g_vCamPos", &vCamPos);
	pEffect->SetFloat("g_fPower", tMtrlInfo.Power);

	return S_OK;
}

void CDynamicObject::StateMachine()
{
}

_vec3 CDynamicObject::Get_TargetPos()
{
	return *m_pTargetTransformCom->Get_Info(Engine::INFO_POS);
}

_vec3 CDynamicObject::Get_Pos()
{
	return	*m_pTransformCom->Get_Info(Engine::INFO_POS);
}

_vec3 CDynamicObject::Get_Look()
{
	return *m_pTransformCom->Get_Info(Engine::INFO_LOOK);
}

_vec3 CDynamicObject::Get_Right()
{
	return *m_pTransformCom->Get_Info(Engine::INFO_RIGHT);
}


float CDynamicObject::Get_Angle(const D3DXVECTOR3 & a, const D3DXVECTOR3 & b)
{
	float fRadian = acosf(D3DXVec3Dot(&a, &b) / (D3DXVec3Length(&a) * D3DXVec3Length(&b)));

	fRadian = D3DXToDegree(fRadian);
	float fDgree = (a.x * b.z - a.z * b.x > 0.0f) ? fRadian : -fRadian;

	return fDgree;
}

void CDynamicObject::MoveAni(_float fTimeDelta, _float fMinRatio, _float fMaxRatio, _float fSpeed, _vec3 vDir)
{
	_vec3	vPos, vOutPos,vTempDir;
	vTempDir = vDir;
	vTempDir.y = 0.f;
	D3DXVec3Normalize(&vTempDir, &vTempDir);
	_float fCurRatio = (float)(m_pMeshCom->Get_TrackPosition() / m_pMeshCom->Get_Period());
	if (fCurRatio >= fMinRatio && fCurRatio <= fMaxRatio)
	{
		vPos = Get_Pos();
		m_pNaviCom->Move_OnNaviMesh(&Get_Pos(), &(vTempDir * fSpeed* fTimeDelta), &vOutPos);
		m_pTransformCom->Set_Pos(vOutPos.x, vOutPos.y, vOutPos.z);
	}
}

_float CDynamicObject::Get_AniRatio()
{
	return m_pMeshCom->Get_TrackPosition() / m_pMeshCom->Get_Period();
}

HRESULT CDynamicObject::Load_Text(const TCHAR * pFilePath)
{
	ifstream fin;
	fin.open(pFilePath);
	if (fin.fail())
		return E_FAIL;
	wstring wstrTemp;
	char cTemp[MIN_STR];
	_uint uidx = 0;
	while (!fin.eof())
	{
		fin.getline(cTemp, MIN_STR);
		wchar_t* ppwchar = CharToWChar(cTemp);
		wstrTemp = ppwchar;
		delete ppwchar;

		if (wstrTemp.compare(L"") == 0)
			break;

		Engine::NAVI_DATA* pNaviData = new Engine::NAVI_DATA;

		pNaviData->vPosition1.x = _wtof(wstrTemp.c_str());
		fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		pNaviData->vPosition1.y = atof(cTemp);
		fin.getline(cTemp, MIN_STR);
		pNaviData->vPosition1.z = atof(cTemp);

		fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		pNaviData->vPosition2.x = atof(cTemp);
		fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		pNaviData->vPosition2.y = atof(cTemp);
		fin.getline(cTemp, MIN_STR);
		pNaviData->vPosition2.z = atof(cTemp);

		fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		pNaviData->vPosition3.x = atof(cTemp);
		fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		pNaviData->vPosition3.y = atof(cTemp);
		fin.getline(cTemp, MIN_STR);
		pNaviData->vPosition3.z = atof(cTemp);

		pNaviData->uiIdx = uidx;

		m_pNaviCom->Add_Cell(pNaviData);

		delete pNaviData;
		uidx++;
	}
	fin.close();
	return S_OK;
}

wchar_t * CDynamicObject::CharToWChar(const char * pstrSrc)
{
	int nLen = strlen(pstrSrc) + 1;
	wchar_t* pwstr = (LPWSTR)malloc(sizeof(wchar_t)* nLen);
	mbstowcs(pwstr, pstrSrc, nLen);
	return pwstr;
}

void CDynamicObject::Set_TransformData()
{
	if (m_bIsRespawn)
	{
		m_pTransformCom->m_vScale=m_tTransformInfo.vScale;
		m_pTransformCom->m_vAngle= m_tTransformInfo.vRotation;
		m_pTransformCom->m_vInfo[Engine::INFO_POS]= m_tTransformInfo.vPosition;
	}
	else
	{
		m_pTransformCom->Set_Scale(DEFAULT_MESH_SCALE);
		m_pTransformCom->Set_Pos(INIT_VEC3);
	}

}

CDynamicObject * CDynamicObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName,_uint uiIdx)
{
	CDynamicObject*	pInstance = new CDynamicObject(pGraphicDev, wstrName, uiIdx);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}
CDynamicObject * CDynamicObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx, TRANSFORM_INFO tInfo)
{
	CDynamicObject*	pInstance = new CDynamicObject(pGraphicDev, wstrName, uiIdx, tInfo);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;

}
void CDynamicObject::Free(void)
{
	Engine::CGameObject::Free();
}
