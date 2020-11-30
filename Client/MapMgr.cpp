#include "stdafx.h"
#include "MapMgr.h"
#include "Wall.h"
#include "Door.h"
#include "Deco.h"
#include "Weapon.h"
#include <iostream>
using namespace std;

IMPLEMENT_SINGLETON(CMapMgr)


CMapMgr::CMapMgr()
{
}


CMapMgr::~CMapMgr()
{
}

//HRESULT CMapMgr::LoadTile(const TCHAR * szFilePath)
//{
//	HANDLE hFile = CreateFile(szFilePath, GENERIC_READ, 0, 0,
//		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
//
//	if (INVALID_HANDLE_VALUE == hFile)
//	{
//		ERR_MSG(L"Tile Load Failed");
//		return E_FAIL;
//	}
//
//	for_each(m_vecTile.begin(), m_vecTile.end(), SafeDelete<INFO*>);
//	m_vecTile.clear();
//	m_vecTile.shrink_to_fit();
//
//	INFO tTile = {};
//	DWORD dwByte = 0;
//
//	while (true)
//	{
//		ReadFile(hFile, &tTile, sizeof(INFO), &dwByte, nullptr);
//
//		if (0 == dwByte)
//			break;
//
//		if (tTile.eMapID == MAPID::FLOOR)
//		{
//			INFO* pTile = new INFO(tTile);
//			//pTile->Initialize(); // 객체로 만들어야될지 보고 하던가 하자
//			m_vecTile.push_back(pTile);
//		}
//	}
//
//	CloseHandle(hFile);
//
//	/*타일 그래프를 만들어 준다.*/
//	ReadyAdjacency();
//
//	return S_OK;
//}

HRESULT CMapMgr::LoadWall(const TCHAR * szFilePath)
{
	HANDLE hFile = CreateFile(szFilePath, GENERIC_READ, 0, 0,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		ERR_MSG(L"Wall Load Failed");
		return E_FAIL;
	}

	for_each(m_vecWall.begin(), m_vecWall.end(), SafeDelete<CObj*>);
	m_vecWall.clear();
	m_vecWall.shrink_to_fit();

	INFO tInfo = {};
	DWORD dwByte = 0;

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		if (tInfo.eMapID == MAPID::WALL)
		{
			CObj* pWall = new CWall(tInfo);
			pWall->Initialize();
			m_vecWall.push_back(pWall);

			CObjMgr::GetInstance()->AddObject(pWall, OBJID::OBJ_OBSTACLE);
		}
	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CMapMgr::LoadDoor(const TCHAR * szFilePath)
{
	HANDLE hFile = CreateFile(szFilePath, GENERIC_READ, 0, 0,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		ERR_MSG(L"Door Load Failed");
		return E_FAIL;
	}

	for_each(m_vecDoor.begin(), m_vecDoor.end(), SafeDelete<CObj*>);
	m_vecDoor.clear();
	m_vecDoor.shrink_to_fit();

	INFO tInfo = {};
	DWORD dwByte = 0;

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		if (tInfo.eMapID == MAPID::DOOR)
		{
			CObj* pDoor = new CDoor(tInfo);
			pDoor->Initialize();
			m_vecDoor.push_back(pDoor);


			CObjMgr::GetInstance()->AddObject(pDoor, OBJID::OBJ_OBSTACLE);
		}
	}

	CloseHandle(hFile);
	return S_OK;
}

HRESULT CMapMgr::LoadDeco(const TCHAR * szFilePath)
{
	HANDLE hFile = CreateFile(szFilePath, GENERIC_READ, 0, 0,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		ERR_MSG(L"Deco Load Failed");
		return E_FAIL;
	}

	for_each(m_vecDeco.begin(), m_vecDeco.end(), SafeDelete<CObj*>);
	m_vecDeco.clear();
	m_vecDeco.shrink_to_fit();

	INFO tInfo = {};
	DWORD dwByte = 0;

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		if (tInfo.eMapID == MAPID::DECO)
		{
			CObj* pDeco = new CDeco(tInfo);
			pDeco->Initialize();
			m_vecDeco.push_back(pDeco);


			CObjMgr::GetInstance()->AddObject(pDeco, OBJID::OBJ_OBSTACLE);
		}
	}

	CloseHandle(hFile);
	return S_OK;
}

HRESULT CMapMgr::LoadWeapon(const TCHAR * szFilePath)
{
	HANDLE hFile = CreateFile(szFilePath, GENERIC_READ, 0, 0,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		ERR_MSG(L"Weapon Load Failed");
		return E_FAIL;
	}

	for_each(m_vecWeapon.begin(), m_vecWeapon.end(), SafeDelete<CObj*>);
	m_vecWeapon.clear();
	m_vecWeapon.shrink_to_fit();

	INFO tInfo = {};
	DWORD dwByte = 0;

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		if (tInfo.eMapID == MAPID::WEAPON)
		{
			CObj* pWeapon = new CWeapon(tInfo);
			pWeapon->Initialize();
			m_vecWeapon.push_back(pWeapon);


			CObjMgr::GetInstance()->AddObject(pWeapon, OBJID::OBJ_WEAPON);
		}
	}

	CloseHandle(hFile);
	return S_OK;
}

HRESULT CMapMgr::Initialize()
{
	return S_OK;
}

void CMapMgr::Update()
{
}

void CMapMgr::LateUpdate()
{
}

void CMapMgr::Render()
{
	/*D3DXVECTOR3 vScroll = CScrollMgr::GetScroll();


	int iCullX = -(int)vScroll.x / TILECX;
	int iCullY = -(int)vScroll.y / (TILECY / 2);

	int iCullEndX = iCullX + WINCX / TILECX;
	int iCullEndY = iCullY + WINCY / (TILECY / 2);

	for (int i = iCullY; i < iCullEndY; ++i)
	{
		for (int j = iCullX; j < iCullEndX; ++j)
		{
			int iIndex = j + (TILEX * i);

			if (iIndex < 0 || (size_t)iIndex >= m_vecTile.size())
				break;

			m_vecWall[iIndex]->Render();
		}
	}*/
}

void CMapMgr::Release()
{
}

void CMapMgr::ReadyAdjacency()
{
}
