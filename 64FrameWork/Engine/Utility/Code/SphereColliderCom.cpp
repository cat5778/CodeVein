#include "SphereColliderCom.h"
#include "Transform.h"
USING(Engine)

CSphereColliderCom::CSphereColliderCom(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjTag, wstring wstrBoneTag,Engine::CTransform * pObjTransform, const Engine::D3DXFRAME_DERIVED * pBone)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
	m_wstrObjTag = wstrObjTag;
	m_wstrBoneTag = wstrBoneTag;
	m_pParentWorldMatrix = pObjTransform->Get_WorldMatrixPointer();

	m_pParentBoneMatrix = &pBone->CombinedTransformationMatrix;

}

CSphereColliderCom::~CSphereColliderCom(void)
{
}


void CSphereColliderCom::Render_Collider()
{
	//m_matWorld = *pColliderMatrix;
#ifdef _DEBUG

	if (m_pSphereMesh != nullptr)
	{
		m_pGraphicDev->BeginScene();

		_matrix matOldWorld;
		m_pGraphicDev->GetTransform(D3DTS_WORLD, &matOldWorld);
		//m_pTransformCom->Get_WorldMatrix(&matOldWorld);

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);


		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME); //Test
		D3DMATERIAL9 mtrl;
		ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
		if (m_bIsColl)
		{
			mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
			mtrl.Diffuse.g = mtrl.Ambient.g = 0.0f;
			mtrl.Diffuse.b = mtrl.Ambient.b = 0.0f;
			mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;

		}
		else
		{
			mtrl.Diffuse.r = mtrl.Ambient.r = 0.0f;
			mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
			mtrl.Diffuse.b = mtrl.Ambient.b = 0.0f;
			mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;

		}
		m_pGraphicDev->SetMaterial(&mtrl);
		m_pSphereMesh->DrawSubset(0);

		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matOldWorld);

		m_pGraphicDev->EndScene();
	}
#endif
}

HRESULT CSphereColliderCom::Ready_Component(void)
{
	Ready_SphereMesh();
	return S_OK;
}

_int CSphereColliderCom::Update_Component(const _float & fTimeDelta)
{
	if (nullptr != m_pParentBoneMatrix && !m_wstrObjTag.empty())
	{
		_matrix matScale, matTrans , matView;
		D3DXMatrixIdentity(&m_matWorld);
		D3DXMatrixScaling(&matScale, m_fRadius, m_fRadius, m_fRadius);
		D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, m_vPos.z);
		m_matWorld = matScale  * matTrans;
		m_matWorld *= (*m_pParentBoneMatrix * *m_pParentWorldMatrix);

		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
		matView = m_matWorld* *m_pParentWorldMatrix;

		Debug_IsColl();
		return 0;
	}



	return 0;

}

void CSphereColliderCom::Ready_SphereMesh()
{
	LPD3DXMESH	pSphereMesh = nullptr;
	Engine::VTXCOL* pVertices = nullptr;

	D3DXCreateSphere(m_pGraphicDev, 1.f, 10, 10, &pSphereMesh, nullptr);

	LPDIRECT3DVERTEXBUFFER9 pSphereBuffer;

	pSphereMesh->CloneMeshFVF(0, Engine::FVF_COL, m_pGraphicDev, &m_pSphereMesh);

	m_pSphereMesh->GetVertexBuffer(&pSphereBuffer);
	_uint uiNumVtx = m_pSphereMesh->GetNumVertices();

	pSphereBuffer->Lock(0, 0, (void**)&pVertices, 0);

	for (_uint i = 0; i < uiNumVtx; i++)
		pVertices[i].dwColor = D3DCOLOR_RGBA(0, 255, 0, 255);

	pSphereBuffer->Unlock();
	pSphereBuffer->Release();

	pSphereMesh->Release();

}

void CSphereColliderCom::Debug_IsColl()
{
	if (m_bIsColl)
	{
		Engine::VTXCOL* pVertices = nullptr;
		LPDIRECT3DVERTEXBUFFER9 pSphereBuffer;

		m_pSphereMesh->GetVertexBuffer(&pSphereBuffer);
		_uint uiNumVtx = m_pSphereMesh->GetNumVertices();

		pSphereBuffer->Lock(0, 0, (void**)&pVertices, 0);

		for (_uint i = 0; i < uiNumVtx; i++)
			pVertices[i].dwColor = D3DCOLOR_RGBA(255, 0, 0, 255);
		pSphereBuffer->Unlock();
		pSphereBuffer->Release();
	}
	else
	{
		Engine::VTXCOL* pVertices = nullptr;
		LPDIRECT3DVERTEXBUFFER9 pSphereBuffer;

		m_pSphereMesh->GetVertexBuffer(&pSphereBuffer);
		_uint uiNumVtx = m_pSphereMesh->GetNumVertices();

		pSphereBuffer->Lock(0, 0, (void**)&pVertices, 0);

		for (_uint i = 0; i < uiNumVtx; i++)
			pVertices[i].dwColor = D3DCOLOR_RGBA(0,255, 0, 255);

		pSphereBuffer->Unlock();
		pSphereBuffer->Release();

	}
}

HRESULT CSphereColliderCom::Set_DMParentMatrix(const Engine::D3DXFRAME_DERIVED * pBone)
{
	if(m_pParentBoneMatrix == nullptr)
		m_pParentBoneMatrix = &pBone->CombinedTransformationMatrix;

	
	return E_NOTIMPL;
}

CSphereColliderCom * CSphereColliderCom::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjTag, wstring wstrBoneTag, Engine::CTransform* pObjTransform, const Engine::D3DXFRAME_DERIVED * pBone)
{
	CSphereColliderCom*	pInstance = new CSphereColliderCom(pGraphicDev, wstrObjTag, wstrBoneTag, pObjTransform, pBone);

	if (FAILED(pInstance->Ready_Component()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CSphereColliderCom::Free(void)
{
	Safe_Release(m_pSphereMesh);
	Safe_Release(m_pGraphicDev);

}
