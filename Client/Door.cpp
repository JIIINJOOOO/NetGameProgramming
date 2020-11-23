#include "stdafx.h"
#include "Door.h"


CDoor::CDoor()
{
}

CDoor::CDoor(const INFO & tInfo)
	:CObj(tInfo)
{
}


CDoor::~CDoor()
{
	Release();
}

HRESULT CDoor::Initialize()
{
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Door/Door_0%d.png",
		L"Door", TEX_MULTI, L"Door", 4)))
	{
		ERR_MSG(L"Door Texture Load Failed!");
		return E_FAIL;
	}
	return S_OK;
}

void CDoor::LateInit()
{
}

int CDoor::Update()
{
	CObj::LateInit();

	return NO_EVENT;
}

void CDoor::LateUpdate()
{
}

void CDoor::Render()
{
	D3DXMATRIX matTrans;

	D3DXVECTOR3 vScroll = m_tInfo.vPos + CScrollMgr::GetScroll();

	D3DXMatrixIdentity(&m_tInfo.matWorld);
	D3DXMatrixTranslation(&matTrans, vScroll.x, vScroll.y, 0.f);

	m_tInfo.matWorld = matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);

	const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
		L"Door", L"Door", m_tInfo.byDrawID);

	if (nullptr == pTexInfo)
		return;

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture,
		nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CDoor::Release()
{
}
