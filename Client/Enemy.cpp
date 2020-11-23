#include "stdafx.h"
#include "Enemy.h"


CEnemy::CEnemy()
{
	srand((unsigned)time(NULL));

}


CEnemy::~CEnemy()
{
	Release();
}

HRESULT CEnemy::Initialize()
{
	m_wstrObjKey = L"ENEMY_UNARMED";
	m_wstrStateKey = L"Walk";
	m_eCurState = STANCE::IDLE;

	m_tFrame.fFrame = 0.f;
	m_tFrame.fMax = 8.f;

	m_fSpeed = 50.f;

	return S_OK;
}

void CEnemy::LateInit()
{
	m_fAngle = rand() % 180;
	m_wstrObjKey = GetObjKey();
}

int CEnemy::Update()
{
	CObj::LateInit();
	if (m_bIsDead)
	{
		return DEAD_OBJ;
	}

	D3DXMATRIX matScale, matRotZ, matTrans;
	D3DXVECTOR2 vScroll = m_tInfo.vPos + CScrollMgr::GetScroll();
	//m_fAngle = -180.f * atan2(m_pMouse->GetInfo().vPos.y - m_tInfo.vDir.y - vScroll.y, m_pMouse->GetInfo().vPos.x - m_tInfo.vDir.x - vScroll.x) / D3DX_PI;

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_fAngle));
	D3DXMatrixTranslation(&matTrans, vScroll.x, vScroll.y, 0.f);

	// 스 자 이 공 부 
	m_tInfo.matWorld = matScale * matRotZ * matTrans;
	

	/*플레이어 위치벡터*/
	//D3DXVec3TransformCoord(&m_tInfo.vPos, &m_tInfo.vLook, &m_tInfo.matWorld);
	/*플레이어 방향벡터*/
	D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vLook, &m_tInfo.matWorld);


	return NO_EVENT;
}

void CEnemy::LateUpdate()
{
	/*if (m_eCurState != IDLE)
	{*/
		CObj::MoveFrame();
		m_eCurState = STANCE::IDLE;
	//}
}

void CEnemy::Render()
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

void CEnemy::Release()
{
}

const TCHAR * CEnemy::GetObjKey()
{
	// 랜덤 무기에 따라 ObjKey 다르게 세팅
	switch (m_eWeapon)
	{
	case WEAPONID::NO_WEAPON:
		return L"ENEMY_UNARMED";
		break;
	case WEAPONID::RIFLE:
		return L"ENEMY_RIFLE";
		break;
	case WEAPONID::SMG:
		return L"ENEMY_SMG";
		break;
	case WEAPONID::SHOTGUN:
		return L"ENEMY_SHOTGUN";
		break;
	case WEAPONID::BAT:
		return L"ENEMY_BAT";
		break;
	case WEAPONID::CLUB:
		return L"ENEMY_CLUB";
		break;
	case WEAPONID::KNIFE:
		return L"ENEMY_KNIFE";
		break;
	default:
		break;
	}
	return nullptr;
}
