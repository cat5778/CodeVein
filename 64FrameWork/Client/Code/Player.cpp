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
	m_eCurState = OBJ_START;
	m_pMeshCom->Set_AnimationSet(46);

	m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);
	
	
	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);
	_matrix mat = m_pTransformCom->m_matWorld;

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pMeshCom->Play_Animation(fTimeDelta*m_fAnimSpeed);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return 0;
}

void CPlayer::Render_GameObject(void)
{
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

	m_pKeyMgr = CKeyMgr::GetInstance();

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
	m_pKeyMgr->Update();
	m_dwDirectionFlag = 0;
	m_bIsShift = false;

	IdleOption();

	if (m_pKeyMgr->KeyDown(KEY_NUM1))
	{
		m_fRotSpeed++;
		cout <<"회전스피드=" <<m_fRotSpeed << endl;
	}

	if (m_pKeyMgr->KeyDown(KEY_NUM2))
	{
		m_fRotSpeed--;
		cout << "회전스피드=" << m_fRotSpeed << endl;
	}
	if (m_pKeyMgr->KeyDown(KEY_Q))
		m_bIsLockOn ? m_bIsLockOn = false : m_bIsLockOn = true;


	
	if (m_pKeyMgr->KeyPressing(KEY_W))
	{
		m_dwDirectionFlag |= FRONT;
		if (m_eCurState == OBJ_IDLE)
			m_eCurState = OBJ_WALK;
	}
	if (m_pKeyMgr->KeyPressing(KEY_S))
	{
		m_dwDirectionFlag |= BACK;
		if (m_eCurState == OBJ_IDLE)
			m_eCurState = OBJ_WALK;

	}

	if (m_pKeyMgr->KeyPressing(KEY_A))
	{
		m_dwDirectionFlag |= LEFT;
		if (m_eCurState == OBJ_IDLE)
			m_eCurState = OBJ_WALK;

	}
	if (m_pKeyMgr->KeyPressing(KEY_D))
	{
		m_dwDirectionFlag |= RIGHT;
		if (m_eCurState == OBJ_IDLE)
			m_eCurState = OBJ_WALK;

	}
	if (m_pKeyMgr->KeyPressing(KEY_SHIFT))
	{
		if(m_eCurState ==OBJ_WALK)
			m_eCurState = OBJ_RUN;
	}
	if (m_pKeyMgr->KeyDown(KEY_LBUTTON))
	{
		if (m_eCurState == OBJ_ATTACK)
		{
			_double dRatio = m_pMeshCom->Get_TrackPosition() / m_pMeshCom->Get_Period();
			if (dRatio >= 0.25&&dRatio < 0.7)
			{
				m_uiCombo++;
				switch (m_uiCombo)
				{
				case 2:
					m_pMeshCom->Set_AnimationSet(30);
					break;
				case 3:
					m_pMeshCom->Set_AnimationSet(29);
					break;
				case 4:
					m_pMeshCom->Set_AnimationSet(28);
					break;
				case 5:
					m_pMeshCom->Set_AnimationSet(27);
					break;
				case 6:
				{
					m_pMeshCom->Set_AnimationSet(31);
					m_uiCombo = 1;
				}
					break;
				default:
					break;
				}
			}
		}
		else
		{

			if (m_eCurState == OBJ_DODGE)
			{
				_double dRatio = m_pMeshCom->Get_TrackPosition() / m_pMeshCom->Get_Period();
				cout << dRatio << endl;
				if (dRatio >= 0.225&&dRatio < 0.9)
				{
					m_eCurState = OBJ_ATTACK;
					m_pMeshCom->Set_AnimationSet(18);
				}

			}
			else
			{
				m_eCurState = OBJ_ATTACK;
				m_pMeshCom->Set_AnimationSet(31);
				m_uiCombo = 1;
			}
		}

	}
	if (m_pKeyMgr->KeyPressing(KEY_RBUTTON))
	{
		m_pMeshCom->Set_AnimationSet(26);
		if(m_fChargeTime<1.0f)
		m_fChargeTime += fTimeDelta;
	}
	if (m_pKeyMgr->KeyUp(KEY_RBUTTON))
	{
		if (m_fChargeTime >= 0.75f)
		{
			m_eCurState = OBJ_CHARGE_ATTACK;

		}
		else
			m_eCurState = OBJ_STRONG_ATTACK;

	}
	if (m_pKeyMgr->KeyDown(KEY_SPACE))
	{
		if (m_eCurState != OBJ_DODGE)
		{
			m_eCurState = OBJ_DODGE;
			if (m_dwDodge_DirectionFlag == 0)
				m_dwDodge_DirectionFlag = m_dwDirectionFlag;
		}
	}

	

	if (m_eCurState == OBJ_ATTACK || m_eCurState == OBJ_STRONG_ATTACK)
		m_dwDirectionFlag = 0;

	if (Engine::Get_DIKeyState(DIK_RETURN) & 0x80)
	{
		m_pMeshCom->Set_AnimationSet(0);
	}


	if (m_pCameraTransformCom != nullptr)
	{
		Check_Direction(fTimeDelta);
		StateMachine();
	}

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

wchar_t * CPlayer::CharToWChar(const char * pstrSrc)
{
	int nLen = strlen(pstrSrc) + 1;
	wchar_t* pwstr = (LPWSTR)malloc(sizeof(wchar_t)* nLen);
	mbstowcs(pwstr, pstrSrc, nLen);
	return pwstr;
}

void CPlayer::Check_Direction(_float fTimeDelta)
{

	_vec3 vCamLook = *m_pCameraTransformCom->Get_Info(Engine::INFO_LOOK);
	_vec3 vPlayerLook = *m_pTransformCom->Get_Info(Engine::INFO_LOOK);
	_vec3 vPlayerRight = { 1.0f,0.f,0.f };
	vCamLook.y = 0;
	vPlayerLook.y = 0;

	D3DXVec3TransformNormal(&vPlayerRight, &vPlayerRight, &m_pTransformCom->m_matWorld);
	D3DXVec3Normalize(&vCamLook, &vCamLook);
	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);
	D3DXVec3Normalize(&vPlayerRight, &vPlayerRight);

	float fDgree = Get_Angle(vCamLook, vPlayerLook) + 180.f;
	
	_vec3	vPos, vDir, vDiagonalDir,vOutPos;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

	DWORD dwDirectionFlag;

	if (m_eCurState == OBJ_DODGE)
		dwDirectionFlag = m_dwDodge_DirectionFlag;
	else		
		dwDirectionFlag = m_dwDirectionFlag;

	switch (dwDirectionFlag)
	{
	case DIR_F:
	{
		if (fDgree > 5.f&&fDgree < 180.f) //왼쪽으로 외전
			m_pTransformCom->Rotation(Engine::ROT_Y, m_fRotSpeed* fTimeDelta);
		else if (fDgree < 355.f && fDgree >= 180.f)
			m_pTransformCom->Rotation(Engine::ROT_Y, -m_fRotSpeed* fTimeDelta);

		m_pCameraTransformCom->Get_Info(Engine::INFO_LOOK, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		m_pNaviCom->Move_OnNaviMesh(&vPos, &(-vDir * m_fSpeed* fTimeDelta), &vOutPos);
		m_pTransformCom->Set_Pos(vOutPos.x, vOutPos.y, vOutPos.z);

	}
	break;
	case DIR_R:
	{
		if (fDgree >= 90.f&&fDgree <= 265.f)
			m_pTransformCom->Rotation(Engine::ROT_Y, -m_fRotSpeed* fTimeDelta);
		else if ((fDgree >= 275.f && fDgree <= 360.f) || (fDgree >= 0.f&& fDgree < 90.f))
			m_pTransformCom->Rotation(Engine::ROT_Y, +m_fRotSpeed* fTimeDelta);

		m_pCameraTransformCom->Get_Info(Engine::INFO_RIGHT, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir * m_fSpeed * fTimeDelta), &vOutPos);
		m_pTransformCom->Set_Pos(vOutPos.x, vOutPos.y, vOutPos.z);

	}
	break;
	case DIR_FR:
	{
		if ((fDgree <= 310.f && fDgree >= 135.f))
			m_pTransformCom->Rotation(Engine::ROT_Y, -m_fRotSpeed* fTimeDelta);
		else if ((fDgree >= 320.f&&fDgree <= 360.f) || (fDgree >= 0.f&& fDgree < 135.f))
			m_pTransformCom->Rotation(Engine::ROT_Y, +m_fRotSpeed* fTimeDelta);

		m_pCameraTransformCom->Get_Info(Engine::INFO_LOOK, &vDiagonalDir);
		m_pCameraTransformCom->Get_Info(Engine::INFO_RIGHT, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		D3DXVec3Normalize(&vDiagonalDir, &vDiagonalDir);
		vDir -= vDiagonalDir;
		D3DXVec3Normalize(&vDir, &vDir);

		m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir * m_fSpeed * fTimeDelta), &vOutPos);
		m_pTransformCom->Set_Pos(vOutPos.x, vOutPos.y, vOutPos.z);

	}
	break;
	case DIR_B:
		if (fDgree > 185.f&& fDgree < 360.f)
			m_pTransformCom->Rotation(Engine::ROT_Y, +m_fRotSpeed* fTimeDelta);
		else if (fDgree <= 175.f&&fDgree >= 0.f)
			m_pTransformCom->Rotation(Engine::ROT_Y, -m_fRotSpeed* fTimeDelta);

		m_pCameraTransformCom->Get_Info(Engine::INFO_LOOK, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir * m_fSpeed * fTimeDelta), &vOutPos);
		m_pTransformCom->Set_Pos(vOutPos.x, vOutPos.y, vOutPos.z);

		break;
	case DIR_BR:
	{
		if ((fDgree >= 45.f&&fDgree <= 220.f))
			m_pTransformCom->Rotation(Engine::ROT_Y, -m_fRotSpeed* fTimeDelta);
		else if ((fDgree <= 360.f && fDgree >= 230.f) || (fDgree >= 0.f&& fDgree < 45.f))
			m_pTransformCom->Rotation(Engine::ROT_Y, +m_fRotSpeed* fTimeDelta);

		m_pCameraTransformCom->Get_Info(Engine::INFO_LOOK, &vDiagonalDir);
		m_pCameraTransformCom->Get_Info(Engine::INFO_RIGHT, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		D3DXVec3Normalize(&vDiagonalDir, &vDiagonalDir);
		vDir += vDiagonalDir;
		D3DXVec3Normalize(&vDir, &vDir);

		m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir * m_fSpeed * fTimeDelta), &vOutPos);
		m_pTransformCom->Set_Pos(vOutPos.x, vOutPos.y, vOutPos.z);
	}
	break;
	case DIR_L:
	{

		if (fDgree >= 95.f&&fDgree <= 270.f)
			m_pTransformCom->Rotation(Engine::ROT_Y, +m_fRotSpeed* fTimeDelta);
		else if ((fDgree > 270.f&& fDgree < 360.f) || (fDgree >= 0.f&& fDgree < 85.f))
			m_pTransformCom->Rotation(Engine::ROT_Y, -m_fRotSpeed* fTimeDelta);

		m_pCameraTransformCom->Get_Info(Engine::INFO_RIGHT, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		m_pNaviCom->Move_OnNaviMesh(&vPos, &(-vDir * m_fSpeed * fTimeDelta), &vOutPos);
		m_pTransformCom->Set_Pos(vOutPos.x, vOutPos.y, vOutPos.z);
	}
	break;
	case DIR_FL:
	{
		if ((fDgree >= 50.f&&fDgree <= 225.f))
			m_pTransformCom->Rotation(Engine::ROT_Y, +m_fRotSpeed* fTimeDelta);
		else if ((fDgree <= 360.f && fDgree >= 225.f) || (fDgree >= 0.f&& fDgree < 40.f))
			m_pTransformCom->Rotation(Engine::ROT_Y, -m_fRotSpeed* fTimeDelta);
		m_pCameraTransformCom->Get_Info(Engine::INFO_LOOK, &vDiagonalDir);
		m_pCameraTransformCom->Get_Info(Engine::INFO_RIGHT, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		D3DXVec3Normalize(&vDiagonalDir, &vDiagonalDir);
		vDir += vDiagonalDir;
		D3DXVec3Normalize(&vDir, &vDir);

		m_pNaviCom->Move_OnNaviMesh(&vPos, &(-vDir * m_fSpeed * fTimeDelta), &vOutPos);
		m_pTransformCom->Set_Pos(vOutPos.x, vOutPos.y, vOutPos.z);
	}

	break;
	case DIR_BL:
	{
		if ((fDgree <= 315.f && fDgree >= 140.f))
			m_pTransformCom->Rotation(Engine::ROT_Y, +m_fRotSpeed* fTimeDelta);
		else if ((fDgree >= 315.f&&fDgree <= 360.f) || (fDgree >= 0.f&& fDgree < 130.f))
			m_pTransformCom->Rotation(Engine::ROT_Y, -m_fRotSpeed* fTimeDelta);

		m_pCameraTransformCom->Get_Info(Engine::INFO_LOOK, &vDiagonalDir);
		m_pCameraTransformCom->Get_Info(Engine::INFO_RIGHT, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		D3DXVec3Normalize(&vDiagonalDir, &vDiagonalDir);
		vDir -= vDiagonalDir;
		D3DXVec3Normalize(&vDir, &vDir);

		m_pNaviCom->Move_OnNaviMesh(&vPos, &(-vDir * m_fSpeed * fTimeDelta), &vOutPos);
		m_pTransformCom->Set_Pos(vOutPos.x, vOutPos.y, vOutPos.z);
	}
	break;
	default:
		break;
	}

}

void CPlayer::StateMachine()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case OBJ_IDLE:
		{
			m_fAnimSpeed = 1.0f;
			m_fRotSpeed = 6.f;
			m_pMeshCom->Set_AnimationSet(44);
		}
			break;
		case OBJ_WALK:
		{
			m_fAnimSpeed = 1.0f;
			m_fSpeed = 0.6f;
			m_fRotSpeed = 6.f;

			if (m_bIsLockOn)
			{
				switch (m_dwDirectionFlag)
				{
				case DIR_F:
					m_pMeshCom->Set_AnimationSet(42);
					break;
				case DIR_R:
					m_pMeshCom->Set_AnimationSet(40);
					break;
				case DIR_FR:
					m_pMeshCom->Set_AnimationSet(40);
					break;
				case DIR_B:
					m_pMeshCom->Set_AnimationSet(43);
					break;
				case DIR_BR:
					m_pMeshCom->Set_AnimationSet(40);
					break;
				case DIR_L:
					m_pMeshCom->Set_AnimationSet(41);
					break;
				case DIR_FL:
					m_pMeshCom->Set_AnimationSet(41);
					break;
				case DIR_BL:
					m_pMeshCom->Set_AnimationSet(41);
					break;
				default:
					break;
				}
			}
			else
			{
				m_pMeshCom->Set_AnimationSet(42);
			}
		}
			break;
		case OBJ_RUN:
		{	
			m_fAnimSpeed = 1.0f;

			m_fSpeed = 3.5f;
			m_fRotSpeed = 10.f;

			if (m_bIsLockOn)
			{
				switch (m_dwDirectionFlag)
				{
				case DIR_F:
					m_pMeshCom->Set_AnimationSet(37);
					break;
				case DIR_R:
					m_pMeshCom->Set_AnimationSet(33);
					break;
				case DIR_FR:
					m_pMeshCom->Set_AnimationSet(33);
					break;
				case DIR_B:
					m_pMeshCom->Set_AnimationSet(39);
					break;
				case DIR_BR:
					m_pMeshCom->Set_AnimationSet(33);
					break;
				case DIR_L:
					m_pMeshCom->Set_AnimationSet(35);
					break;
				case DIR_FL:
					m_pMeshCom->Set_AnimationSet(35);
					break;
				case DIR_BL:
					m_pMeshCom->Set_AnimationSet(35);
					break;
				default:
					break;
				}
			}
			else
			{
				m_pMeshCom->Set_AnimationSet(37);

			}
		}
			break;
		case OBJ_DODGE:
		{
			m_fAnimSpeed = 2.5f;
			m_fSpeed = 10.f;
			m_fRotSpeed = 10.f;

			if (m_bIsLockOn)
			{
				switch (m_dwDirectionFlag)
				{
				case DIR_F:
					m_pMeshCom->Set_AnimationSet(21);
					break;
				case DIR_R:
					m_pMeshCom->Set_AnimationSet(19);
					break;
				case DIR_FR:
					m_pMeshCom->Set_AnimationSet(19);
					break;
				case DIR_B:
					m_pMeshCom->Set_AnimationSet(22);
					break;
				case DIR_BR:
					m_pMeshCom->Set_AnimationSet(19);
					break;
				case DIR_L:
					m_pMeshCom->Set_AnimationSet(20);
					break;
				case DIR_FL:
					m_pMeshCom->Set_AnimationSet(20);
					break;
				case DIR_BL:
					m_pMeshCom->Set_AnimationSet(20);
					break;
				default:
					break;
				}
			}
			else
			{
				m_pMeshCom->Set_AnimationSet(21);
			}
		}
			break;
		case OBJ_JUMP:
			break;
		case OBJ_FALL:
			break;
		case OBJ_ATTACK:
		{
			m_fAnimSpeed = 1.75f;

		}
			break;
		case OBJ_STRONG_ATTACK:
		{
			m_fAnimSpeed = 2.f;

			m_pMeshCom->Set_AnimationSet(26);
			//m_pMeshCom->Set_AddTrackTime(m_fChargeTime);

			m_fChargeTime = 0.f;

		}
			break;
		case OBJ_CHARGE_ATTACK:
		{
			m_fAnimSpeed = 2.f;
			m_pMeshCom->Set_AnimationSet(25);
			m_pMeshCom->Set_AddTrackTime(m_fChargeTime);
			m_fChargeTime = 0.f;

			//m_pMeshCom->get
		}
			break;
		case OBJ_GUARD:
			m_pMeshCom->Set_AnimationSet(16);
			break;
		case OBJ_HURT:
			break;
		case OBJ_DEAD:
			break;
		case OBJ_START:
			break;
		case OBJ_END:
			break;
		default:
			break;
		}
		m_ePreState = m_eCurState;

	}

}

void CPlayer::IdleOption()
{
	//cout << "Period= " << m_pMeshCom->Get_Period() << endl;
	//cout << "Position= " << m_pMeshCom->Get_TrackPosition() << endl;


	if (m_eCurState == OBJ_DODGE)
	{
		if (m_pMeshCom->Get_TrackPosition() >= 1.0)
			m_dwDodge_DirectionFlag = 0;
		if (m_pMeshCom->Get_TrackPosition() >= 2.25)
			m_eCurState = OBJ_IDLE;
	}
	if (m_eCurState != OBJ_IDLE )
	{
		if (m_pMeshCom->Is_AnimationSetEnd())
		{
			m_eCurState = OBJ_IDLE;
			m_uiCombo = 0;
		}
	}

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
	m_pKeyMgr->DestroyInstance();

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


float CPlayer::Get_Angle(const D3DXVECTOR3& a, const D3DXVECTOR3& b)
{
	float fRadian = acosf(D3DXVec3Dot(&a, &b) / (D3DXVec3Length(&a) * D3DXVec3Length(&b)));

	fRadian = D3DXToDegree(fRadian);
	float fDgree = (a.x * b.z - a.z * b.x > 0.0f) ? fRadian : -fRadian;

	return fDgree;

}
