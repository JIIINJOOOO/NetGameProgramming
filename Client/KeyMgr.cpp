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


extern int retval;
extern WSADATA wsa;
extern SOCKET sock;
extern SOCKADDR_IN serveraddr;
extern void err_display(char* msg);
extern void err_quit(char* msg);
// �� �����Ӹ��� ȣ���Ͽ� ���� Ű�� �����Ѵ�.
void CKeyMgr::KeyCheck()
{
	KeyInput key;

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		key.keycode = 'L';
	}
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		key.keycode = 'R';
	}
	if (GetAsyncKeyState('W') & 0x8000)
	{
		key.keycode = 'W';

	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		key.keycode = 'S';
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		key.keycode = 'A';

	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		key.keycode = 'D';

	}
	if (GetAsyncKeyState('R') & 0x8000)
	{
		key.keycode = 'R';

	}
	if (GetAsyncKeyState('E') & 0x8000)
	{
		key.keycode = 'E';

	}
	retval = send(sock, (char *)&key, sizeof(KeyInput), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return;
	}
}

bool CKeyMgr::KeyUp(DWORD dwKey)
{
	// dwKey(���� �˻��ϴ� Ű)�� m_dwKey(���� ���� Ű) �߿� �ߺ��Ǵ� ��Ʈ���� �ִ��� �˻�.
	if (m_dwKey & dwKey)
	{
		m_dwKeyPressed |= dwKey;
		return false;
	}
	// m_dwKeyPressed(������ ���ȴ� Ű)�� dwKey(���� �˻��ϴ� Ű) �߿� 
	// �ߺ��Ǵ� ��Ʈ���� �ִ��� �˻�. �ִٸ� ������ �ִٴ� ��.
	else if (m_dwKeyPressed & dwKey)
	{
		m_dwKeyPressed ^= dwKey;
		return true;
	}

	return false;
}

bool CKeyMgr::KeyDown(DWORD dwKey)
{
	// ���� �������� ���ſ� ������ ���� ��� True
	if ((m_dwKey & dwKey) && !(m_dwKeyDown & dwKey))
	{
		m_dwKeyDown |= dwKey;
		return true;
	}
	// ���� ������ �ʾ����� ���ſ� ������ �ִ� ��� false
	else if (!(m_dwKey & dwKey) && (m_dwKeyDown & dwKey))
	{
		m_dwKeyDown ^= dwKey;
		return false;
	}

	return false;
}

// �� Ű�� ������� ���� ��� ����.
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
