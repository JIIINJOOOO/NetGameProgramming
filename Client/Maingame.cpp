#include "stdafx.h"
#include "Maingame.h"


CMaingame::CMaingame()
{
}


CMaingame::~CMaingame()
{
	Release();
}

HRESULT CMaingame::Initialize()
{
	// 커서 온오프
	ShowCursor(false);
	CTimeMgr::GetInstance()->InitTime();
	CSoundMgr::GetInstance()->Initialize();

	if (FAILED(CDevice::GetInstance()->InitDevice()))
	{
		ERR_MSG(L"Device Init Failed");
		return E_FAIL;
	}

	if (FAILED(CSceneMgr::GetInstance()->SceneChange(CSceneMgr::STAGE)))
	{
		ERR_MSG(L"Stage Scene Change Failed");
		return E_FAIL;
	}

	return S_OK;
}

void CMaingame::Update()
{
	// 시간 갱신은 Update 최상단에서 호출.
	CTimeMgr::GetInstance()->UpdateTime();
	CKeyMgr::GetInstance()->KeyCheck();

	CSceneMgr::GetInstance()->Update();
}

void CMaingame::LateUpdate()
{
	CSceneMgr::GetInstance()->LateUpdate();	
	CSoundMgr::GetInstance()->UpdateSound();

}

void CMaingame::Render()
{
	CDevice::GetInstance()->Render_Begin();

	CSceneMgr::GetInstance()->Render();

	CDevice::GetInstance()->Render_End();
}

void CMaingame::Release()
{
	CTextureMgr::GetInstance()->DestroyInstance();
	CDevice::GetInstance()->DestroyInstance();

	CSceneMgr::GetInstance()->DestroyInstance();
	CObjMgr::GetInstance()->DestroyInstance();
	CTimeMgr::GetInstance()->DestroyInstance();
	CKeyMgr::GetInstance()->DestroyInstance();
	CSoundMgr::GetInstance()->DestroyInstance();

}
