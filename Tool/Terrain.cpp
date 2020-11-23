#include "stdafx.h"
#include "Terrain.h"
#include "ToolView.h"

CTerrain::CTerrain()
	: m_pMainView(nullptr)
{
}


CTerrain::~CTerrain()
{
	Release();
}

void CTerrain::Initialize()
{
	// ��Ƽ �ؽ��� �ε�
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Floor/Floor_0%d.png", L"Floor",
		TEX_MULTI, L"Floor", 14)))
	{
		AfxMessageBox(L"Terrain Image Insert Failed!!!");
		return;
	}
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Wall/Wall_0%d.png", L"Wall",
		TEX_MULTI, L"Wall", 6)))
	{
		AfxMessageBox(L"Wall Image Insert Failed!!!");
		return;
	}
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Door/Door_0%d.png", L"Door",
		TEX_MULTI, L"Door", 4)))
	{
		AfxMessageBox(L"Door Image Insert Failed!!!");
		return;
	}
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Deco/Deco_0%d.png", L"Deco",
		TEX_MULTI, L"Deco", 22)))
	{
		AfxMessageBox(L"Deco Image Insert Failed!!!");
		return;
	}
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Weapon/Weapon_0%d.png", L"Weapon",
		TEX_MULTI, L"Weapon", 6)))
	{
		AfxMessageBox(L"Weapon Image Insert Failed!!!");
		return;
	}

	// �� ����Ÿ�Ժ� ��Ƽ �ؽ��� �ε�
	/*if (FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Enemy/EnemyEdit/Enemy_01.png", L"Bat",
		TEX_SINGLE)))
	{
		AfxMessageBox(L"Enemy Image Insert Failed!!!");
		return;
	}*/
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Enemy/EnemyEdit/Enemy_0%d.png", L"Bat",
		TEX_MULTI, L"Bat", 2)))
	{
		AfxMessageBox(L"Enemy Image Insert Failed!!!");
		return;
	}

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			INFO* pTile = new INFO;

			float fX = float(TILECX * 0.5f + TILECX * j);
			float fY = float(TILECY * 0.5f + TILECY * i);

			pTile->vPos = D3DXVECTOR3(fX, fY, 0.f);
			pTile->vSize = D3DXVECTOR3((float)TILECX, (float)TILECY, 0.f);
			pTile->byOption = 0;
			pTile->byDrawID = 0;
			pTile->bIsRender = false;
			pTile->iIndex = j + (TILEX * i);
			pTile->iParentIdx = 0;
			pTile->eMapID = MAPID::FLOOR;

			m_vecTile.push_back(pTile);
		}
	}
}

void CTerrain::Update()
{
}

void CTerrain::Render()
{
	// ȭ�� ũ�⿡ ���� ������ ����.
	RECT rcWnd = {};

	// GetWindowRect: ���� �����츦 ���ΰ� �ִ� ��ũ�ѹٳ� �������� ũ����� ���´�.
	//m_pMainView->GetWindowRect(&rcWnd);

	// GetClientRect: ���ڷ� ������ �������� ��Ȯ�� View������ ���´�.
	::GetClientRect(m_pMainView->m_hWnd, &rcWnd);

	float fRatioX = WINCX / float(rcWnd.right - rcWnd.left);
	float fRatioY = WINCY / float(rcWnd.bottom - rcWnd.top);


	// floor ������
	D3DXMATRIX matWorld, matScale, matTrans;

	TCHAR szIndex[MIN_STR] = L"";

	for (size_t i = 0; i < m_vecTile.size(); ++i )
	{
		if (!m_vecTile[i]->bIsRender)
			continue;
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, fRatioX, fRatioY, 0.f);
		D3DXMatrixTranslation(&matTrans, 
			(m_vecTile[i]->vPos.x - m_pMainView->GetScrollPos(0)/*0: x�� ��ũ�� ��*/) * fRatioX,
			(m_vecTile[i]->vPos.y - m_pMainView->GetScrollPos(1)/*1: y�� ��ũ�� ��*/) * fRatioY,
			m_vecTile[i]->vPos.z);

		matWorld = matScale * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

		const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
			L"Floor", L"Floor", m_vecTile[i]->byDrawID);

		if (nullptr == pTexInfo)
			continue;

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		// ��Ʈ ���
		swprintf_s(szIndex, L"%d", i);

		CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite(),
			szIndex, lstrlen(szIndex), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	// Wall ������
	for (size_t i = 0; i < m_vecWall.size(); ++i)
	{
		/*if (!m_vecTile[i]->bIsRender)
		continue;*/
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, fRatioX, fRatioY, 0.f);
		D3DXMatrixTranslation(&matTrans,
			(m_vecWall[i]->vPos.x - m_pMainView->GetScrollPos(0)/*0: x�� ��ũ�� ��*/) * fRatioX,
			(m_vecWall[i]->vPos.y - m_pMainView->GetScrollPos(1)/*1: y�� ��ũ�� ��*/) * fRatioY,
			m_vecWall[i]->vPos.z);

		matWorld = matScale * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

		const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
			L"Wall", L"Wall", m_vecWall[i]->byDrawID);

		if (nullptr == pTexInfo)
			continue;

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	// Door ������
	for (size_t i = 0; i < m_vecDoor.size(); ++i)
	{
		/*if (!m_vecTile[i]->bIsRender)
		continue;*/
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, fRatioX, fRatioY, 0.f);
		D3DXMatrixTranslation(&matTrans,
			(m_vecDoor[i]->vPos.x - m_pMainView->GetScrollPos(0)/*0: x�� ��ũ�� ��*/) * fRatioX,
			(m_vecDoor[i]->vPos.y - m_pMainView->GetScrollPos(1)/*1: y�� ��ũ�� ��*/) * fRatioY,
			m_vecDoor[i]->vPos.z);

		matWorld = matScale * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

		const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
			L"Door", L"Door", m_vecDoor[i]->byDrawID);

		if (nullptr == pTexInfo)
			continue;

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	// Deco ���
	for (size_t i = 0; i < m_vecDeco.size(); ++i)
	{
		/*if (!m_vecTile[i]->bIsRender)
		continue;*/
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, m_vecDeco[i]->fScaleX * fRatioX, m_vecDeco[i]->fScaleY * fRatioY, 0.f);
		D3DXMatrixTranslation(&matTrans,
			(m_vecDeco[i]->vPos.x - m_pMainView->GetScrollPos(0)/*0: x�� ��ũ�� ��*/) * fRatioX,
			(m_vecDeco[i]->vPos.y - m_pMainView->GetScrollPos(1)/*1: y�� ��ũ�� ��*/) * fRatioY,
			m_vecDeco[i]->vPos.z);

		matWorld = matScale * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

		const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
			L"Deco", L"Deco", m_vecDeco[i]->byDrawID);

		if (nullptr == pTexInfo)
			continue;

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	// Weapon ���
	for (size_t i = 0; i < m_vecWeapon.size(); ++i)
	{
		/*if (!m_vecTile[i]->bIsRender)
		continue;*/
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, fRatioX, fRatioY, 0.f);
		D3DXMatrixTranslation(&matTrans,
			(m_vecWeapon[i]->vPos.x - m_pMainView->GetScrollPos(0)/*0: x�� ��ũ�� ��*/) * fRatioX,
			(m_vecWeapon[i]->vPos.y - m_pMainView->GetScrollPos(1)/*1: y�� ��ũ�� ��*/) * fRatioY,
			m_vecWeapon[i]->vPos.z);

		matWorld = matScale * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

		const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
			L"Weapon", L"Weapon", m_vecWeapon[i]->byDrawID);

		if (nullptr == pTexInfo)
			continue;

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	// Enemy ���
	for (size_t i = 0; i < m_vecEnemy.size(); ++i)
	{
		D3DXMatrixScaling(&matScale, fRatioX, fRatioY, 0.f);
		D3DXMatrixTranslation(&matTrans,
			m_vecEnemy[i]->vPos.x - m_pMainView->GetScrollPos(0),
			m_vecEnemy[i]->vPos.y - m_pMainView->GetScrollPos(1),
			0.f);

		matWorld = matScale * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

		const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
			L"Enemy", L"Enemy", 0);

		if (nullptr == pTexInfo)
			continue;

		float fCenterX = pTexInfo->tImgInfo.Width*0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height*0.5f;

		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture,
			nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f),
			nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

}

void CTerrain::MiniRender()
{
	// floor ���
	D3DXMATRIX matWorld, matScale, matTrans;

	TCHAR szIndex[MIN_STR] = L"";

	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		if (!m_vecTile[i]->bIsRender)
			continue;
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f); // ���� ���� ��
		D3DXMatrixTranslation(&matTrans,
			m_vecTile[i]->vPos.x * 0.5f,
			m_vecTile[i]->vPos.y * 0.5f,
			m_vecTile[i]->vPos.z);

		matWorld = matScale * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

		const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
			L"Floor", L"Floor", m_vecTile[i]->byDrawID);

		if (nullptr == pTexInfo)
			continue;

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));		
	}
	// Wall ���
	for (size_t i = 0; i < m_vecWall.size(); ++i)
	{
		/*if (!m_vecTile[i]->bIsRender)
		continue;*/
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f); // ���� ���� ��
		D3DXMatrixTranslation(&matTrans,
			m_vecWall[i]->vPos.x * 0.5f,
			m_vecWall[i]->vPos.y * 0.5f,
			m_vecWall[i]->vPos.z);

		matWorld = matScale * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

		const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
			L"Wall", L"Wall", m_vecWall[i]->byDrawID);

		if (nullptr == pTexInfo)
			continue;

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	// Door ���
	for (size_t i = 0; i < m_vecDoor.size(); ++i)
	{
		/*if (!m_vecTile[i]->bIsRender)
		continue;*/
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f); // ���� ���� ��
		D3DXMatrixTranslation(&matTrans,
			m_vecDoor[i]->vPos.x * 0.5f,
			m_vecDoor[i]->vPos.y * 0.5f,
			m_vecDoor[i]->vPos.z);

		matWorld = matScale * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

		const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
			L"Door", L"Door", m_vecDoor[i]->byDrawID);

		if (nullptr == pTexInfo)
			continue;

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	// Deco ���
	for (size_t i = 0; i < m_vecDeco.size(); ++i)
	{
		/*if (!m_vecTile[i]->bIsRender)
		continue;*/
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f); // ���� ���� ��
		D3DXMatrixTranslation(&matTrans,
			m_vecDeco[i]->vPos.x * 0.5f,
			m_vecDeco[i]->vPos.y * 0.5f,
			m_vecDeco[i]->vPos.z);

		matWorld = matScale * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

		const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
			L"Deco", L"Deco", m_vecDeco[i]->byDrawID);

		if (nullptr == pTexInfo)
			continue;

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	// Weapon ���
	for (size_t i = 0; i < m_vecWeapon.size(); ++i)
	{
		/*if (!m_vecTile[i]->bIsRender)
		continue;*/
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f); // ���� ���� ��
		D3DXMatrixTranslation(&matTrans,
			m_vecWeapon[i]->vPos.x * 0.5f,
			m_vecWeapon[i]->vPos.y * 0.5f,
			m_vecWeapon[i]->vPos.z);

		matWorld = matScale * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

		const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
			L"Weapon", L"Weapon", m_vecWeapon[i]->byDrawID);

		if (nullptr == pTexInfo)
			continue;

		float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
		float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
			&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CTerrain::Release()
{
	/*for_each(m_vecTile.begin(), m_vecTile.end(), SafeDelete<TILE*>);
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();*/
	ReleaseTile();
	ReleaseWall();
	ReleaseDoor();
	ReleaseDeco();
	ReleaseWeapon();
}

void CTerrain::ReleaseTile()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), SafeDelete<INFO*>);
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
}

void CTerrain::ReleaseWall()
{
	for_each(m_vecWall.begin(), m_vecWall.end(), SafeDelete<INFO*>);
	m_vecWall.clear();
	m_vecWall.shrink_to_fit();
}

void CTerrain::ReleaseDoor()
{
	for_each(m_vecDoor.begin(), m_vecDoor.end(), SafeDelete<INFO*>);
	m_vecDoor.clear();
	m_vecDoor.shrink_to_fit();
}

void CTerrain::ReleaseDeco()
{
	for_each(m_vecDeco.begin(), m_vecDeco.end(), SafeDelete<INFO*>);
	m_vecDeco.clear();
	m_vecDeco.shrink_to_fit();
}

void CTerrain::ReleaseWeapon()
{
	for_each(m_vecWeapon.begin(), m_vecWeapon.end(), SafeDelete<INFO*>);
	m_vecWeapon.clear();
	m_vecWeapon.shrink_to_fit();
}

void CTerrain::TileChange(const D3DXVECTOR3 & vPos, const int & iDrawID, int iOption)
{
	int iIndex = GetTileIndex(vPos);

	if (-1 == iIndex)
		return;

	m_vecTile[iIndex]->byDrawID = iDrawID;
	m_vecTile[iIndex]->byOption = iOption;
	m_vecTile[iIndex]->bIsRender = true;
}

void CTerrain::AddWall(D3DXVECTOR3 & vPos, const int & iDrawID, const float & fScaleX, const float & fScaleY, int iOption /*�߰�*/)
{
	INFO* pTile = new INFO;
	if (iDrawID % 2 == 0)
	{
		vPos.y -= TILECY / 2;
	}
	else
	{
		vPos.x -= TILECX / 2;
	}

	pTile->vPos = vPos;
	pTile->byDrawID = iDrawID;
	pTile->byOption = iOption; //�߰�
	pTile->fScaleX = fScaleX;
	pTile->fScaleY = fScaleY;
	pTile->eMapID = MAPID::WALL;

	m_vecWall.push_back(pTile);
}

void CTerrain::AddDoor(D3DXVECTOR3 & vPos, const int & iDrawID, const float & fScaleX, const float & fScaleY, int iOption /*�߰�*/)
{
	INFO* pTile = new INFO;

	if (iDrawID % 2 == 0)
	{
		vPos.y -= TILECY / 2;
	}
	else
	{
		vPos.x -= TILECX / 2;
	}

	pTile->vPos = vPos;
	pTile->byDrawID = iDrawID;
	pTile->byOption = iOption; //�߰�
	pTile->fScaleX = fScaleX;
	pTile->fScaleY = fScaleY;
	pTile->eMapID = MAPID::DOOR;

	m_vecDoor.push_back(pTile);
}

void CTerrain::AddDeco(const D3DXVECTOR3 & vPos, const int & iDrawID, const float & fScaleX, const float & fScaleY, int iOption /*�߰�*/)
{
	INFO* pTile = new INFO;

	pTile->vPos = vPos;
	pTile->byDrawID = iDrawID;
	pTile->byOption = iOption; //�߰�
	pTile->fScaleX = fScaleX;
	pTile->fScaleY = fScaleY;
	pTile->eMapID = MAPID::DECO;

	m_vecDeco.push_back(pTile);
}

void CTerrain::AddWeapon(const D3DXVECTOR3 & vPos, const int & iDrawID, const float & fScaleX, const float & fScaleY, int iOption /*�߰�*/)
{
	INFO* pTile = new INFO;

	pTile->vPos = vPos;
	pTile->byDrawID = iDrawID;
	pTile->byOption = iOption; //�߰�
	pTile->fScaleX = fScaleX;
	pTile->fScaleY = fScaleY;
	pTile->eMapID = MAPID::WEAPON;

	m_vecWeapon.push_back(pTile);
}

void CTerrain::AddEnemy(const D3DXVECTOR3 & vPos, const UNIT_DATA & tData, const WEAPONID & eID)
{
	UNIT_DATA* pData = new UNIT_DATA(tData);

	pData->vPos = vPos;
	pData->eWeaponID = eID;

	m_vecEnemy.push_back(pData);
}

int CTerrain::GetTileIndex(const D3DXVECTOR3& vPos)
{
	for (size_t i = 0; i < m_vecTile.size(); ++i)
	{
		if (Picking(vPos, i))
			return i;
	}

	return -1;	// �ε��� ã�� ����!
}

const D3DXVECTOR3 & CTerrain::GetTilePos(int iIndex)
{
	// TODO: ���⿡ ��ȯ ������ �����մϴ�.
	return m_vecTile[iIndex]->vPos;
}

bool CTerrain::Picking(const D3DXVECTOR3 & vPos, const int & iIndex)
{
	//// ������ �̿��� ��ŷ

	//// 1. 12�ø� �������� �ð� ������ ������ �������� ���Ѵ�.
	//D3DXVECTOR3 vPoint[4] =
	//{
	//	D3DXVECTOR3(m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + (TILECY * 0.5f), 0.f),
	//	D3DXVECTOR3(m_vecTile[iIndex]->vPos.x + (TILECX * 0.5f), m_vecTile[iIndex]->vPos.y, 0.f),
	//	D3DXVECTOR3(m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - (TILECY * 0.5f), 0.f),
	//	D3DXVECTOR3(m_vecTile[iIndex]->vPos.x - (TILECX * 0.5f), m_vecTile[iIndex]->vPos.y, 0.f)
	//};

	//// 2. �ð� �������� ���� ���͸� ���Ѵ�.
	//D3DXVECTOR3 vDir[4] =
	//{
	//	vPoint[1] - vPoint[0],
	//	vPoint[2] - vPoint[1],
	//	vPoint[3] - vPoint[2],
	//	vPoint[0] - vPoint[3],
	//};

	//// 3. �� ���� ���͸� ���Ѵ�.
	//D3DXVECTOR3 vNormal[4] =
	//{
	//	D3DXVECTOR3(-vDir[0].y, vDir[0].x, 0.f),
	//	D3DXVECTOR3(-vDir[1].y, vDir[1].x, 0.f),
	//	D3DXVECTOR3(-vDir[2].y, vDir[2].x, 0.f),
	//	D3DXVECTOR3(-vDir[3].y, vDir[3].x, 0.f),
	//};

	//// 4. ���� ���� ���͵��� �������ͷ� ����� �ش�.
	//for (int i = 0; i < 4; ++i)
	//	D3DXVec3Normalize(&vNormal[i], &vNormal[i]);

	//// 5. �� �������� ���콺 ���� ���⺤�͸� ���Ѵ�.
	//D3DXVECTOR3 vMouseDir[4] = 
	//{
	//	vPos - vPoint[0],
	//	vPos - vPoint[1],
	//	vPos - vPoint[2],
	//	vPos - vPoint[3]
	//};

	//for (int i = 0; i < 4; ++i)
	//	D3DXVec3Normalize(&vMouseDir[i], &vMouseDir[i]);

	//// 4�鿡 ���ؼ� vMouseDir�� vNormal�� �����Ͽ� ���� ����� �а�(����)�̸� true!
	//for (int i = 0; i < 4; ++i)
	//{
	//	// �ϳ��� ����� ������ false
	//	if (0.f < D3DXVec3Dot(&vMouseDir[i], &vNormal[i]))
	//		return false;
	//}

	//return true;

	int x = int(vPos.x) / TILECX;
	int y = int(vPos.y) / TILECY;

	if (iIndex == x + (TILEX * y))
		return true;
	return false;
}

