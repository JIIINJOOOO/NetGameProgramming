#include "stdafx.h"
#include "KeyMgr.h"

IMPLEMENT_SINGLETON(CKeyMgr)

CKeyMgr::CKeyMgr()
	: m_dwKey(0), m_dwKeyPressed(0), m_dwKeyDown(0)
{
}


CKeyMgr::~CKeyMgr()
{
}

// 매 프레임마다 호출하여 눌린 키를 조사한다.
void CKeyMgr::KeyCheck()
{
	m_dwKey = 0;	// 0000

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_dwKey |= KEY_LBUTTON;	// 0000 |= 0001
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		m_dwKey |= KEY_RBUTTON; // 0001 |= 0010
	if (GetAsyncKeyState('W') & 0x8000)
		m_dwKey |= KEY_W;
	if (GetAsyncKeyState('S') & 0x8000)
		m_dwKey |= KEY_S;
	if (GetAsyncKeyState('A') & 0x8000)
		m_dwKey |= KEY_A;
	if (GetAsyncKeyState('D') & 0x8000)
		m_dwKey |= KEY_D; // 0001 |= 0010
	if (GetAsyncKeyState('R') & 0x8000)
		m_dwKey |= KEY_R;

	// m_dwKey = 0011
}

bool CKeyMgr::KeyUp(DWORD dwKey)
{
	// dwKey(현재 검사하는 키)와 m_dwKey(현재 눌린 키) 중에 중복되는 비트열이 있는지 검사.
	if (m_dwKey & dwKey)
	{
		m_dwKeyPressed |= dwKey;
		return false;
	}
	// m_dwKeyPressed(이전에 눌렸던 키)와 dwKey(현재 검사하는 키) 중에 
	// 중복되는 비트열이 있는지 검사. 있다면 누른적 있다는 것.
	else if (m_dwKeyPressed & dwKey)
	{
		m_dwKeyPressed ^= dwKey;
		return true;
	}

	return false;
}

bool CKeyMgr::KeyDown(DWORD dwKey)
{
	// 현재 눌렸지만 과거에 누른적 없는 경우 True
	if ((m_dwKey & dwKey) && !(m_dwKeyDown & dwKey))
	{
		m_dwKeyDown |= dwKey;
		return true;
	}
	// 현재 눌리지 않았지만 과거에 누른적 있는 경우 false
	else if (!(m_dwKey & dwKey) && (m_dwKeyDown & dwKey))
	{
		m_dwKeyDown ^= dwKey;
		return false;
	}

	return false;
}

// 두 키가 순서대로 눌린 경우 동작.
bool CKeyMgr::KeyCombine(DWORD dwFirstKey, DWORD dwSecondKey)
{
	if (KeyDown(dwSecondKey) && (m_dwKey & dwFirstKey))
		return true;

	return false;
}

bool CKeyMgr::KeyPressing(DWORD dwKey)
{
	if (m_dwKey & dwKey)
		return true;

	return false;
}
