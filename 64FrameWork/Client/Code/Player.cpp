#include "stdafx.h"
#include "Player.h"
#include <fstream>

#include "Export_Function.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CPlayer::~CPlayer(void)
{

}

HRESULT CPlayer::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	Load_Text(L"../../Resource/Data/NavMash/BaseNav10.txt");
	m_pNaviCom->Set_Index(38);// Base Init Idx 38 
	m_pMeshCom->Set_AnimationSet(m_iAnim);

	m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);

	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
	//SetUp_OnTerrain();
	Key_Input(fTimeDelta);
	_matrix mat = m_pTransformCom->m_matWorld;

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pMeshCom->Play_Animation(fTimeDelta);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return 0;
}

void CPlayer::Render_GameObject(void)
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);
	//m_pMeshCom->Render_Meshes();
	m_pNaviCom->Render_NaviMesh();

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


}

HRESULT CPlayer::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(RESOURCE_STAGE, L"Mesh_Player"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	pComponent = m_pNaviCom = dynamic_cast<Engine::CNaviMesh*>(Engine::Clone(RESOURCE_STAGE, L"Mesh_Navi"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Navi", pComponent);


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


	//// collider
	//pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev,
	//	m_pMeshCom->Get_VtxPos(),
	//	m_pMeshCom->Get_NumVtx(),
	//	m_pMeshCom->Get_Stride());
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	return S_OK;
}

void CPlayer::Key_Input(const _float& fTimeDelta)
{
	m_pTransformCom->Get_Info(Engine::INFO_LOOK, &m_vDir);

	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		m_iAnim++;
		_vec3	vPos, vDir;
		m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
		m_pTransformCom->Get_Info(Engine::INFO_LOOK, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		//vPos += vDir*fTimeDelta;
		//m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
		_vec3 vOutPos;
		m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir * 3.f * fTimeDelta), &vOutPos);
		//m_pTransformCom->Set_Pos(&m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir * 3.f * fTimeDelta)));
		m_pTransformCom->Set_Pos(vOutPos.x, vOutPos.y, vOutPos.z);
		m_pMeshCom->Set_AnimationSet(m_iAnim);

		/*D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pTransformCom->Move_Pos(&(m_vDir * m_fSpeed * fTimeDelta));*/

		//m_pMeshCom->Set_AnimationSet(54);

	}
	

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pTransformCom->Move_Pos(&(m_vDir * -m_fSpeed * fTimeDelta));
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_pTransformCom->Rotation(Engine::ROT_Y, D3DXToRadian(90.f * fTimeDelta));

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_pTransformCom->Rotation(Engine::ROT_Y, D3DXToRadian(-90.f * fTimeDelta));

	if (Engine::Get_DIMouseState(Engine::DIM_LB) & 0x80)
	{
		//_vec3		vPickPos = PickUp_OnTerrain();

		//m_pTransformCom->Move_TargetPos(&vPickPos, m_fSpeed, fTimeDelta);
	}

	if (Engine::Get_DIKeyState(DIK_RETURN) & 0x80)
	{
		m_pMeshCom->Set_AnimationSet(0);
	}

	if (true == m_pMeshCom->Is_AnimationSetEnd())
		m_pMeshCom->Set_AnimationSet(0);

	_matrix mat = m_pTransformCom->m_matWorld;

}

HRESULT CPlayer::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
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

HRESULT CPlayer::Load_Text(const TCHAR * pFilePath)
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
		//pNaviData->vPosition1.x = atof(cTemp);

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
		uidx++;
	}
	fin.close();
	return S_OK;
}

wchar_t * CPlayer::CharToWChar(const char * pstrSrc)
{
	int nLen = strlen(pstrSrc) + 1;
	wchar_t* pwstr = (LPWSTR)malloc(sizeof(wchar_t)* nLen);
	mbstowcs(pwstr, pstrSrc, nLen);
	return pwstr;
}



CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer*	pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CPlayer::Free(void)
{


	Engine::CGameObject::Free();
}
//
//void CPlayer::SetUp_OnTerrain(void)
//{
//	_vec3		vPos;
//	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
//
//	Engine::CTerrainTex*		pTerrainBufferCom = dynamic_cast<Engine::CTerrainTex*>
//		(Engine::Get_Component(L"GameLogic", L"Terrain", L"Com_Buffer", Engine::ID_STATIC));
//	NULL_CHECK(pTerrainBufferCom);
//
//	_float	fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos,
//		pTerrainBufferCom->Get_VtxPos(),
//		pTerrainBufferCom->Get_VtxCntX(),
//		pTerrainBufferCom->Get_VtxCntZ());
//
//	m_pTransformCom->Set_Pos(vPos.x, fHeight, vPos.z);
//}
//
//_vec3 CPlayer::PickUp_OnTerrain(void)
//{
//	Engine::CTerrainTex*	pTerrainBufferCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(L"GameLogic", L"Terrain", L"Com_Buffer", Engine::ID_STATIC));
//	NULL_CHECK_RETURN(pTerrainBufferCom, _vec3(0.f, 0.f, 0.f));
//
//	Engine::CTransform*	pTerrainTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"Terrain", L"Com_Transform", Engine::ID_DYNAMIC));
//	NULL_CHECK_RETURN(pTerrainTransformCom, _vec3(0.f, 0.f, 0.f));
//
//	return m_pCalculatorCom->Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransformCom);
//}
