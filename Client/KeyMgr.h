#pragma once

class CKeyMgr
{
	DECLARE_SINGLETON(CKeyMgr)

private:
	CKeyMgr();
	~CKeyMgr();

public:
	void KeyCheck();
	bool KeyUp(DWORD dwKey);
	bool KeyDown(DWORD dwKey);
	bool KeyCombine(DWORD dwFirstKey, DWORD dwSecondKey);
	bool KeyPressing(DWORD dwKey);

private:
	DWORD m_dwKey;
	DWORD m_dwKeyPressed;
	DWORD m_dwKeyDown;
};

