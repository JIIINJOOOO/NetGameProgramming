#include "stdafx.h"
#include "Deco.h"


CDeco::CDeco()
{
}

CDeco::CDeco(const INFO & tInfo)
	:CObj(tInfo)
{
}


CDeco::~CDeco()
{
	Release();
}

HRESULT CDeco::Initialize()
{
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Deco/Deco_0%d.png",
		L"Deco", TEX_MULTI, L"Deco", 22)))
	{
		ERR_MSG(L"Deco Texture Load Failed!");
		return E_FAIL;
	}

	// 프레임 최적화
	m_vecTexture = CTextureMgr::GetInstance()->GetTexVector(L"Deco", L"Deco");

	

	return S_OK;
}

void CDeco::LateInit()
{
}

int CDeco::Update()
{
	CObj::LateInit();

	return NO_EVENT;
}

void CDeco::LateUpdate()
{
}

void CDeco::Render()
{
	//CObj::UpdateCollRect(m_vecTexture[m_tInfo.byDrawID]); // 충돌박스 추가

	D3DXMATRIX matScale,matTrans;

	D3DXVECTOR3 vScroll = m_tInfo.vPos + CScrollMgr::GetScroll();

	D3DXMatrixIdentity(&m_tInfo.matWorld);
	D3DXMatrixScaling(&matScale, m_tInfo.fScaleX, m_tInfo.fScaleY, 0.f);
	D3DXMatrixTranslation(&matTrans, vScroll.x, vScroll.y, 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);

	/*const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
		L"Deco", L"Deco", m_tInfo.byDrawID);*/

	// 프레임 최적화
	const TEXINFO* pTexInfo = m_vecTexture[m_tInfo.byDrawID];

	CObj::UpdateCollRect(pTexInfo); // 충돌박스 추가


	if (nullptr == pTexInfo)
		return;

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture,
		nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CDeco::Release()
{
}
