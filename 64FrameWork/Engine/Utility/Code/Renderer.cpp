//#include "Renderer.h"
//#include "GraphicDev.h"

#include "Export_Utility.h"

USING(Engine)
IMPLEMENT_SINGLETON(CRenderer)

Engine::CRenderer::CRenderer(void)
{

}

Engine::CRenderer::~CRenderer(void)
{
	Free();
}

void CRenderer::Set_Perspective(const _matrix & matPerspective)
{
	m_matPerspective = matPerspective;
}

void Engine::CRenderer::Add_RenderGroup(RENDERID eGroup, CGameObject* pGameObject)
{
	if (RENDER_END <= eGroup)
		return;
	m_RenderGroup[eGroup].push_back(pGameObject);
	pGameObject->AddRef();
}

void Engine::CRenderer::Render_GameObject(void)
{
	// 원근 투영 적용
	RenderState_Projection(PROJ_PERSPECTIVE);

	for (_uint i = 0; i < RENDER_END; ++i)
	{
		// 직교 투영 적용
		if (RENDER_UI == i)
			RenderState_Projection(PROJ_ORTHOGRAPH);

		for (auto& iter : m_RenderGroup[i])
		{
			iter->Render_GameObject();
			Safe_Release(iter);
		}
		m_RenderGroup[i].clear();
	}
	RenderState_Projection(PROJ_PERSPECTIVE);
}

void CRenderer::Render_GameObject(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	Render_Priority(pGraphicDev);

	Render_Defferd(pGraphicDev);

	Render_Alpha(pGraphicDev);
	Render_UI(pGraphicDev);

	Engine::Render_DebugBuffer(L"MRT_Defferd");

	Clear_RenderGroup();
}

void Engine::CRenderer::Clear_RenderGroup(void)
{
	for (_uint i = 0; i < RENDER_END; ++i)
	{
		for_each(m_RenderGroup[i].begin(), m_RenderGroup[i].end(), CDeleteObj());
		m_RenderGroup[i].clear();
	}
}

void CRenderer::RenderState_Projection(PROJECTIONTYPE eProjType)
{
	//switch (eProjType)
	//{
	//case PROJ_PERSPECTIVE:
	//	CGraphicDev::GetInstance()->Get_Device()->SetTransform(D3DTS_PROJECTION, &m_matPerspective);
	//	break;
	//case PROJ_ORTHOGRAPH:
	//	RESOLUTION tResolution;
	//	CGraphicDev::GetInstance()->Get_Resolution(tResolution);
	//	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_Device();

	//	_matrix matWorld, matView, matProj;

	//	D3DXMatrixIdentity(&matWorld);
	//	matProj = matView = matWorld;

	//	D3DXMatrixOrthoLH(&matProj, tResolution.fWidth, tResolution.fHeigth, 0.f, 1.f);

	//	pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	//	pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	//	pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
	//	break;
	//}

}


void CRenderer::Render_Priority(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (auto& iter : m_RenderGroup[RENDER_PRIORITY])
		iter->Render_GameObject();

	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void CRenderer::Render_NonAlpha(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	for (auto& iter : m_RenderGroup[RENDER_NONALPHA])
		iter->Render_GameObject();
}

_bool	Compare_Z(CGameObject* pDest, CGameObject* pSour)
{
	return pDest->Get_ViewZ() > pSour->Get_ViewZ();
}

void CRenderer::Render_Alpha(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_RenderGroup[RENDER_ALPHA].sort(Compare_Z);

	for (auto& iter : m_RenderGroup[RENDER_ALPHA])
		iter->Render_GameObject();

	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void CRenderer::Render_UI(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	for (auto& iter : m_RenderGroup[RENDER_UI])
		iter->Render_GameObject();

	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void Engine::CRenderer::Render_Defferd(LPDIRECT3DDEVICE9& pGraphicDev)
{
	Engine::Begin_MRT(L"MRT_Defferd");

	Render_NonAlpha(pGraphicDev);

	Engine::End_MRT(L"MRT_Defferd");
}

void Engine::CRenderer::Free(void)
{
	Clear_RenderGroup();
}

