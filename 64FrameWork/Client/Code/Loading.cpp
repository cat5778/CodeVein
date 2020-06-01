#include "stdafx.h"
#include "Loading.h"
#include <fstream>
#include "Export_Function.h"
#include "DirectoryMgr.h"
#include <iostream>
CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bFinish(false)
{
	ZeroMemory(m_szLoading, sizeof(_tchar) * 256);
	m_pGraphicDev->AddRef();
}

CLoading::~CLoading(void)
{

}

_uint	CALLBACK CLoading::Thread_Main(void* pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;
	_uint iFlag = 0;

	EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->Get_LoadingID())
	{
	case LOADING_STAGE:
		iFlag = pLoading->Loading_ForStage();		
		break;

	case LOADING_BOSS:
		break;
	}
	
	LeaveCriticalSection(pLoading->Get_Crt());
	_endthreadex(0);

	return iFlag;
}

HRESULT CLoading::Ready_Loading(LOADINGID eLoading)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);

	m_eLoading = eLoading;

	return S_OK;
}

_uint CLoading::Loading_ForStage(void)
{
	lstrcpy(m_szLoading, L"Buffer Loading.............................");

	int i = 0;

	// buffer
	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev,
												RESOURCE_STATIC,
												L"Buffer_TerrainTex",
												Engine::BUFFER_TERRAINTEX,
												VTXCNTX,
												VTXCNTZ,
												VTXITV),
												E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Buffer(m_pGraphicDev,
												RESOURCE_STATIC,
												L"Buffer_CubeTex",
												Engine::BUFFER_CUBETEX),
												E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Navi",
		Engine::TYPE_NAVI,
		NULL,
		NULL),
		E_FAIL);

	lstrcpy(m_szLoading, L"Texture Loading.............................");
	
	// 텍스쳐

	//FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
	//											RESOURCE_STAGE,
	//											L"Texture_Terrain",
	//											Engine::TEX_NORMAL,
	//											L"../Bin/Resource/Texture/Terrain/Grass_%d.tga", 2),
	//											E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
												RESOURCE_STAGE,
												L"Texture_Cube",
												Engine::TEX_CUBE,
												L"../../Resource/Texture/SkyBox/burger%d.dds",
												4),
												E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
												RESOURCE_STAGE,
												L"Texture_Player",
												Engine::TEX_NORMAL,
												L"../../Resource/Texture/Player%d.png"),
												E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
	//											RESOURCE_STAGE,
	//											L"Texture_Monster",
	//											Engine::TEX_NORMAL,
	//											L"../../Resource/Texture/Monster%d.png"),
	//											E_FAIL);
	
	lstrcpy(m_szLoading, L"Mesh Loading.............................");


	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Player",
		Engine::TYPE_DYNAMIC,
		L"../../Resource/Mesh/DynamicMesh/PlayerXfile/",
		L"Player.X"),
		E_FAIL);

		Mesh_Loading();

	//FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
	//	RESOURCE_STAGE,
	//	L"Mesh_Sword",
	//	Engine::TYPE_STATIC,
	//	L"../../Resource/Mesh/StaticMesh/Sword/",
	//	L"Sword.X"),
	//	E_FAIL);
	
	lstrcpy(m_szLoading, L"Loading Complete!!!");

	m_bFinish = true;


	return 0;
}

_bool CLoading::Ready_Mesh(MESH_PATH * pPathInfo)
{
	if (Engine::Ready_Meshes(m_pGraphicDev, RESOURCE_STAGE, pPathInfo->wstrObjectType.c_str(), Engine::TYPE_STATIC, pPathInfo->wstrRelative.c_str(), pPathInfo->wstrName.c_str()) < 0)
		return false;

	return 	true;
}

_bool CLoading::Mesh_Loading() //텍스트 읽고와서 메쉬 로딩
{
	TCHAR szFileName[MAX_STR] = L"../../Resource/Data/PathInfo.txt";

	ifstream fin;

	fin.open(szFileName,ios::in);

	if (fin.fail())
		return E_FAIL;
	
	char cTemp[MIN_STR];
	while (!fin.eof())
	{
		
		MESH_PATH* pPathInfo = new MESH_PATH;
		
		fin.getline(cTemp, MIN_STR);
		wchar_t* ppwchar = CharToWChar(cTemp);
		pPathInfo->wstrGroup = ppwchar;
		delete ppwchar;

		fin.getline(cTemp, MIN_STR);
		ppwchar = CharToWChar(cTemp);
		pPathInfo->wstrMap = ppwchar;
		delete ppwchar;

		fin.getline(cTemp, MIN_STR);
		ppwchar = CharToWChar(cTemp);
		pPathInfo->wstrMeshType= ppwchar;
		delete ppwchar;
		
		fin.getline(cTemp, MIN_STR);
		ppwchar = CharToWChar(cTemp);
		pPathInfo->wstrName= ppwchar;
		delete ppwchar;

		fin.getline(cTemp, MIN_STR);
		ppwchar = CharToWChar(cTemp);
		pPathInfo->wstrObjectType= ppwchar;
		delete ppwchar;

		fin.getline(cTemp, MIN_STR);
		ppwchar = CharToWChar(cTemp);
		pPathInfo->wstrRelative= ppwchar;
		delete ppwchar;

		if (pPathInfo->wstrName.empty())
		{
			delete pPathInfo;
			pPathInfo = nullptr;
			continue;
		}
		else
			m_pPathList.push_back(pPathInfo);
	}

	for (auto pPathInfo : m_pPathList)
	{
		if (pPathInfo->wstrMeshType.compare(L"StaticMesh") == 0)
		{
			if (pPathInfo->wstrObjectType.compare(L"SM_Ceiling_a_ba01"))
			{
				int a = 3;
			}
			if (Engine::Ready_Meshes(m_pGraphicDev,	RESOURCE_STAGE,pPathInfo->wstrObjectType.c_str(),Engine::TYPE_STATIC,pPathInfo->wstrRelative.c_str(),pPathInfo->wstrName.c_str()) < 0)
			{
				wstring wstr =L"Loading.cpp ReadyMesh Failed!!!  Object =" + pPathInfo->wstrObjectType;
				DE_COUT(wstr.c_str())
				continue;
			}

		}
		else
			continue;
	}

	


	return false;
}

wchar_t * CLoading::CharToWChar(const char * pstrSrc)
{
	int nLen = strlen(pstrSrc) + 1;
	wchar_t* pwstr = (LPWSTR)malloc(sizeof(wchar_t)* nLen);
	mbstowcs(pwstr, pstrSrc, nLen);
	return pwstr;

}


CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eLoading)
{
	CLoading*	pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eLoading)))
		Safe_Release(pInstance);

	return pInstance;

}

void CLoading::Free(void)
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);

	Engine::Safe_Release(m_pGraphicDev);
}

