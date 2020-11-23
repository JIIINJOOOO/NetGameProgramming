#include "stdafx.h"
#include "SceneMgr.h"
#include "Stage.h"

IMPLEMENT_SINGLETON(CSceneMgr)

CSceneMgr::CSceneMgr()
	: m_pScene(nullptr), m_eCurScene(END), m_ePreScene(END)
{
}


CSceneMgr::~CSceneMgr()
{
	Release();
}

HRESULT CSceneMgr::SceneChange(SCENEID eID)
{
	m_eCurScene = eID;

	if (m_ePreScene != m_eCurScene)
	{
		SafeDelete(m_pScene);

		switch (m_eCurScene)
		{
		case STAGE:
			m_pScene = new CStage;
			break;
		}

		m_ePreScene = m_eCurScene;
		return m_pScene->Initialize();
	}

	return S_OK;
}

void CSceneMgr::Update()
{
	m_pScene->Update();
}

void CSceneMgr::LateUpdate()
{
	m_pScene->LateUpdate();
}

void CSceneMgr::Render()
{
	m_pScene->Render();
}

void CSceneMgr::Release()
{
	SafeDelete(m_pScene);
}
