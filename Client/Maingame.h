#pragma once
class CMaingame
{
public:
	CMaingame();
	~CMaingame();

public:
	HRESULT Initialize();
	void Update();
	void LateUpdate();
	void Render();
	void Release();
};

