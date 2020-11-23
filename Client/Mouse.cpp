#include "stdafx.h"
#include "Mouse.h"


CMouse::CMouse()
{
}


CMouse::~CMouse()
{
}

D3DXVECTOR3 CMouse::GetMousePos()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	return D3DXVECTOR3((float)pt.x, (float)pt.y, 0.f);
}

HRESULT CMouse::Initialize()
{
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Stage/Mouse/Mouse_0%d.png",
		L"Mouse", TEX_MULTI, L"Mouse", 13)))
	{
		ERR_MSG(L"Mouse Texture Load Failed!");
		return E_FAIL;
	}

	//m_wstrObjKey = L"PLAYER";
	//m_wstrStateKey = L"UnarmedWalk";

	m_tFrame.fFrame = 0.f;
	m_tFrame.fMax = 13.f;
	return S_OK;
}

void CMouse::LateInit()
{
}

int CMouse::Update()
{
	CObj::LateInit();

	if (m_tInfo.vPos != GetMousePos())
		m_tInfo.vPos = GetMousePos() - CScrollMgr::GetScroll();

	D3DXMATRIX matTrans, matScale;
	D3DXVECTOR2 vScroll = m_tInfo.vPos + CScrollMgr::GetScroll();
	//m_fAngle = -180.f * atan2(m_pMouse->GetInfo().vPos.y - m_tInfo.vPos.y - vScroll.y, m_pMouse->GetInfo().vPos.x - m_tInfo.vPos.x - vScroll.x) / D3DX_PI;

	D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);
	//D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_fAngle));
	D3DXMatrixTranslation(&matTrans, vScroll.x, vScroll.y, 0.f);

	m_tInfo.matWorld = matScale * matTrans/* * matRotZ*/;

	return NO_EVENT;
}

void CMouse::LateUpdate()
{
	// 애니메이션 재생 코드
	CObj::MoveFrame();
}

void CMouse::Render()
{
	const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
		L"Mouse", L"Mouse", (int)m_tFrame.fFrame);

	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CMouse::Release()
{
}
