#include "stdafx.h"
#include "Obj.h"


CObj::CObj()
	: m_bIsInit(false),m_bIsDead(false),m_fAngle(0.f),m_fSpeed(0.f),m_fRange(0.f),m_iDmg(0),m_iHP(0),m_iPlayerID(0)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tCollRect, sizeof(RECT));
	ZeroMemory(&m_tFrame, sizeof(FRAME));
	ZeroMemory(&m_eWeapon, sizeof(WEAPONID));

	D3DXMatrixIdentity(&m_tInfo.matWorld);

	m_tInfo.vDir = { 1.f,0.f,0.f };
	m_tInfo.vLook = { 1.f, 0.f, 0.f };
}

CObj::CObj(const INFO & tInfo)
	:m_tInfo(tInfo)
{
}


CObj::~CObj()
{
}

void CObj::SetPlayerInfo(PlayerInfo pInfo)
{
	if (pInfo.playerID == 1) 
	{
		playerInfo_1 = pInfo;
	}
	if (pInfo.playerID == 2) 
	{
		playerInfo_2 = pInfo;
	}
}

void CObj::LateInit()
{
	if (!m_bIsInit)
	{
		this->LateInit();
		m_bIsInit = true;
	}
}

void CObj::UpdateCollRect(const TEXINFO * pTexInfo)
{
	m_tCollRect.left = LONG(m_tInfo.vPos.x - pTexInfo->tImgInfo.Width * 0.5f);
	m_tCollRect.top = LONG(m_tInfo.vPos.y - pTexInfo->tImgInfo.Height * 0.5f);
	m_tCollRect.right = LONG(m_tInfo.vPos.x + pTexInfo->tImgInfo.Width * 0.5f);
	m_tCollRect.bottom = LONG(m_tInfo.vPos.y + pTexInfo->tImgInfo.Height * 0.5f);
}

void CObj::MoveFrame()
{
	// 1초당 fMax길이 만큼 재생될 것!
	m_tFrame.fFrame += m_tFrame.fMax * CTimeMgr::GetInstance()->GetTime();

	if (m_tFrame.fFrame > m_tFrame.fMax)
		m_tFrame.fFrame = 0.f;
}
