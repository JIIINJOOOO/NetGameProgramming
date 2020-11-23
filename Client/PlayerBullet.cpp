#include "stdafx.h"
#include "PlayerBullet.h"
#include "Mouse.h"

CPlayerBullet::CPlayerBullet()
{
}


CPlayerBullet::~CPlayerBullet()
{
	Release();
}

HRESULT CPlayerBullet::Initialize()
{
	m_wstrObjKey = L"BULLET";
	m_wstrStateKey = L"Rifle";
	m_tFrame.fFrame = 0.f;
	m_tFrame.fMax = 0.f;
	m_fRange = 200.f;
	return S_OK;
}

void CPlayerBullet::LateInit()
{
	switch (m_eWeapon)
	{
	case WEAPONID::RIFLE:
		m_wstrStateKey = L"Rifle";
		m_fSpeed = 2.f;
		m_iDmg = 10;
		break;
	case WEAPONID::SMG:
		m_wstrStateKey = L"Smg";
		m_fSpeed = 3.f;
		m_fRange = 150.f;
		m_iDmg = 8;
		break;
	case WEAPONID::SHOTGUN:
		m_wstrStateKey = L"Shotgun";
		m_fSpeed = 4.f;
		m_fRange = 100.f;
		m_iDmg = 15;
		break;
	default:
		break;
	}
	D3DXVECTOR2 vScroll = m_tInfo.vPos + CScrollMgr::GetScroll();
	m_fAngle = -180.f * atan2(CMouse::GetMousePos().y - m_tInfo.vDir.y - vScroll.y, CMouse::GetMousePos().x - m_tInfo.vDir.x - vScroll.x) / D3DX_PI;

	m_fPlayerPos = m_tInfo.vPos; // 발사된 위치
}

int CPlayerBullet::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
		return DEAD_OBJ;

	// 201117 총 사거리
	float fX = fabs(m_fPlayerPos.x - m_tInfo.vPos.x);
	float fY = fabs(m_fPlayerPos.y - m_tInfo.vPos.y);
	m_fDistance = sqrt((fX*fX) + (fY*fY));

	float fTime = CTimeMgr::GetInstance()->GetTime();
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
	m_tInfo.vPos += m_tInfo.vDir * m_fSpeed /** fTime*/;
	D3DXMATRIX matScale, matRotZ, matTrans;
	D3DXVECTOR2 vScroll = m_tInfo.vPos + CScrollMgr::GetScroll();
	//m_fAngle = -180.f * atan2(CMouse::GetMousePos().y - m_tInfo.vDir.y - vScroll.y, CMouse::GetMousePos().x - m_tInfo.vDir.x - vScroll.x) / D3DX_PI;

	D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_fAngle));

	D3DXMatrixTranslation(&matTrans, vScroll.x, vScroll.y, 0.f);

	// 스 자 이 공 부 
	m_tInfo.matWorld = matScale * matRotZ* matTrans;


	return NO_EVENT;
}

void CPlayerBullet::LateUpdate()
{
	/*if (m_tInfo.vPos.x > WINCX || m_tInfo.vPos.x < 0 ||
		m_tInfo.vPos.y > WINCY || m_tInfo.vPos.y < 0)
		m_bIsDead = true;*/
	CObj::MoveFrame();

	// 201117 총 사거리
	if (m_fDistance >= m_fRange)
	{
		IsDead();
		//m_bIsDead = true;
	}

}

void CPlayerBullet::Render()
{
	const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
		m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), (int)m_tFrame.fFrame);
	// 프레임 최적화
	//const TEXINFO* pTexInfo = m_vecTexture[(int)m_tFrame.fFrame];

	//m_tCollRect = { LONG(m_tInfo.vPos.x - pTexInfo->tImgInfo.Width*0.5f),  LONG(m_tInfo.vPos.y - pTexInfo->tImgInfo.Height*0.5f), LONG(m_tInfo.vPos.x + pTexInfo->tImgInfo.Width*0.5f), LONG(m_tInfo.vPos.y + pTexInfo->tImgInfo.Height*0.5f) };

	NULL_CHECK(pTexInfo);

	// 충돌박스 생성 추가
	CObj::UpdateCollRect(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void CPlayerBullet::Release()
{
}
