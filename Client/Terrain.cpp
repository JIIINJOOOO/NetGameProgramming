#include "stdafx.h"
#include "Terrain.h"
#include "Mouse.h"


CTerrain::CTerrain()
{

}

CTerrain::~CTerrain()
{
	Release();
}

HRESULT CTerrain::Initialize()
{
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Floor/Floor_0%d.png", 
		L"Floor", TEX_MULTI, L"Floor", 14)))
	{
		ERR_MSG(L"Tile Texture Load Failed!");
		return E_FAIL;
	}

	if (FAILED(LoadTile()))
		return E_FAIL;
	// 프레임 최적화
	m_vecTexture = CTextureMgr::GetInstance()->GetTexVector(L"Floor", L"Floor");


	return S_OK;
}

void CTerrain::LateInit()
{
	ReadyAdjacency();
	
}

int CTerrain::Update()
{
	CObj::LateInit();	

	float fTime = CTimeMgr::GetInstance()->GetTime();

	
	
	/*if (0 > CMouse::GetMousePos().x)
		CScrollMgr::SetScroll(500.f * fTime, 0.f);
	if (WINCX < CMouse::GetMousePos().x)
		CScrollMgr::SetScroll(-500.f * fTime, 0.f);
	if (0 > CMouse::GetMousePos().y)
		CScrollMgr::SetScroll(0.f, 500.f * fTime);
	if (WINCY < CMouse::GetMousePos().y)
		CScrollMgr::SetScroll(0, -500.f * fTime);*/


	
	/*if (CKeyMgr::GetInstance()->KeyPressing(KEY_A))
		CScrollMgr::SetScroll(100.f * fTime, 0.f);
	if (CKeyMgr::GetInstance()->KeyPressing(KEY_D))
		CScrollMgr::SetScroll(-100.f * fTime, 0.f);
	if (CKeyMgr::GetInstance()->KeyPressing(KEY_W))
		CScrollMgr::SetScroll(0.f, 100.f * fTime);
	if (CKeyMgr::GetInstance()->KeyPressing(KEY_S))
		CScrollMgr::SetScroll(0, -100.f * fTime);*/
	



	return NO_EVENT;
}

void CTerrain::LateUpdate()
{
}

void CTerrain::Render()
{
	D3DXMATRIX matScale, matTrans;

	// 컬링 적용.
	D3DXVECTOR3 vScroll = CScrollMgr::GetScroll();

	int iCullX = -(int)vScroll.x / TILECX;
	int iCullY = -(int)vScroll.y / (TILECY/* / 2*/);

	int iCullEndX = iCullX + WINCX / TILECX;
	int iCullEndY = iCullY + WINCY / (TILECY /*/ 2*/);

	for (int i = iCullY; i < iCullEndY + 3; ++i)
	{
		for (int j = iCullX; j < iCullEndX + 2; ++j)
		{
			int iIndex = j + (TILEX * i);

			if (0 > iIndex || m_vecTile.size() <= (size_t)iIndex)
				continue;

			/*if (!m_vecTile[iIndex]->bIsRender)
				continue;*/

			D3DXMatrixIdentity(&m_tInfo.matWorld);
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			D3DXMatrixTranslation(&matTrans,
				m_vecTile[iIndex]->vPos.x + CScrollMgr::GetScroll().x,
				m_vecTile[iIndex]->vPos.y + CScrollMgr::GetScroll().y,
				m_vecTile[iIndex]->vPos.z);

			m_tInfo.matWorld = matScale * matTrans;

			CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);

			/*const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
				L"Floor", L"Floor", m_vecTile[iIndex]->byDrawID);*/

			// 프레임 최적화
			const TEXINFO* pTexInfo = m_vecTexture[m_vecTile[iIndex]->byDrawID];


			if (nullptr == pTexInfo)
				continue;

			float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
			float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

			if (!m_vecTile[iIndex]->bIsRender)
				continue;
			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
				&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}	
}

void CTerrain::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), SafeDelete<INFO*>);
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
}

HRESULT CTerrain::LoadTile() // 추후 불러오는 파일 수정해야함
{
	HANDLE hFile = CreateFile(L"../Data/Stage1-MapData5.dat", GENERIC_READ, 0, 0,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		ERR_MSG(L"Tile Load Failed!!");
		return E_FAIL;
	}			

	Release();

	INFO tTile = {};
	DWORD dwByte = 0;

	while (true)
	{
		ReadFile(hFile, &tTile, sizeof(INFO), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		if (tTile.eMapID == MAPID::FLOOR)
		{
			INFO* pTile = new INFO(tTile);
			m_vecTile.push_back(pTile);
		}
	}

	CloseHandle(hFile);
	return S_OK;
}

// 인접관계를 구성하는 함수. (그래프를 구성)
void CTerrain::ReadyAdjacency()
{
	m_vecAdjacency.resize(m_vecTile.size());

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			int iIndex = j + (TILEX * i);

			// 8방향에 대해서 이웃관계를 검사한다.

			// 상
			if (0 <= iIndex - TILEX * 2 && 1 != m_vecTile[iIndex - TILEX * 2]->byOption)
				m_vecAdjacency[iIndex].push_back(m_vecTile[iIndex - TILEX * 2]);

			// 하
			if((int)m_vecTile.size() > iIndex + TILEX * 2 && 1 != m_vecTile[iIndex + TILEX * 2]->byOption)
				m_vecAdjacency[iIndex].push_back(m_vecTile[iIndex + TILEX * 2]);

			// 좌
			if(0 != iIndex % TILEX && 1 != m_vecTile[iIndex - 1]->byOption)
				m_vecAdjacency[iIndex].push_back(m_vecTile[iIndex - 1]);

			// 우
			if(iIndex != TILEX * (i + 1) - 1 && 1 != m_vecTile[iIndex + 1]->byOption)
				m_vecAdjacency[iIndex].push_back(m_vecTile[iIndex + 1]);


			// 좌상
			if (0 != i && (0 != iIndex % (TILEX * 2)))
			{
				if( (0 != i % 2) && 1 != m_vecTile[iIndex - TILEX]->byOption)
					m_vecAdjacency[iIndex].push_back(m_vecTile[iIndex - TILEX]);
				else if((0 == i % 2) && 1 != m_vecTile[iIndex - (TILEX + 1)]->byOption)
					m_vecAdjacency[iIndex].push_back(m_vecTile[iIndex - (TILEX + 1)]);
			}

			// 우상
			if (0 != i && !((TILEX * 2 - 1) == iIndex % (TILEX * 2)))
			{
				if ((0 != i % 2) && 1 != m_vecTile[iIndex - (TILEX - 1)]->byOption)
					m_vecAdjacency[iIndex].push_back(m_vecTile[iIndex - (TILEX - 1)]);
				else if ((0 == i % 2) && 1 != m_vecTile[iIndex - TILEX]->byOption)
					m_vecAdjacency[iIndex].push_back(m_vecTile[iIndex - TILEX]);
			}

			// 좌하
			if ((0 != iIndex % (TILEX * 2)) && i != (TILEY - 1))
			{
				if( (0 != i % 2) && 1 != m_vecTile[iIndex + TILEX]->byOption)
					m_vecAdjacency[iIndex].push_back(m_vecTile[iIndex + TILEX]);
				else if( (0 == i % 2) && 1 != m_vecTile[iIndex + (TILEX - 1)]->byOption)
					m_vecAdjacency[iIndex].push_back(m_vecTile[iIndex + (TILEX - 1)]);
			}


			// 우하
			if (!((TILEX * 2 - 1) == iIndex % (TILEX * 2)) && i != (TILEY - 1))
			{
				if ((0 != i % 2) && 1 != m_vecTile[iIndex + (TILEX + 1)]->byOption)
					m_vecAdjacency[iIndex].push_back(m_vecTile[iIndex + (TILEX + 1)]);
				else if ((0 == i % 2) && 1 != m_vecTile[iIndex + TILEX]->byOption)
					m_vecAdjacency[iIndex].push_back(m_vecTile[iIndex + TILEX]);
			}
		}
	}
}
