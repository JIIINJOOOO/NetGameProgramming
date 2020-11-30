#include "stdafx.h"
#include "Player.h"
#include "Mouse.h"
#include "PlayerBullet.h"
#include "Struct.h"
#include <iostream>


extern PlayerNumCheck playercheck;


CPlayer::CPlayer()
{
}


CPlayer::~CPlayer()
{
	Release();
}

HRESULT CPlayer::Initialize()
{
	

	if (m_iPlayerID == 1) 
	{
		m_tInfo.vPos.x = 200.f;
		m_tInfo.vPos.y = 350.f;
	}
	if (m_iPlayerID == 2)
	{
		m_tInfo.vPos.x = 850.f;
		m_tInfo.vPos.y = 350.f;
	}
	//m_tInfo.vSize = { 20.f, 40.f, 0.f };

	m_wstrObjKey = L"PLAYER_UNARMED";
	m_wstrStateKey = L"Walk";
	m_eCurState = STANCE::IDLE;

	m_tFrame.fFrame = 0.f;
	m_tFrame.fMax = 8.f;

	m_fSpeed = 150.f;
	m_fAngle = 0.f;
	//m_bIsSetMaxBul = false;

	//m_pMouse = CObjMgr::GetInstance()->GetMouse();
	// ������ ����ȭ -> �̰� �ϸ� ���⺯�� �ִϸ��̼��� �ȵ�
	//m_vecTexture = CTextureMgr::GetInstance()->GetTexVector(m_wstrObjKey.c_str(), m_wstrStateKey.c_str());
	m_pBulletLst = CObjMgr::GetInstance()->GetPlayerBulletLst();
	m_iHP = 100;
	m_iMoney = 1000;

	return S_OK;
}

void CPlayer::LateInit()
{
	
}

int CPlayer::Update()
{
	CObj::LateInit();
	//PlayerInfo playerinfo;

	KeyCheck();


	D3DXMATRIX matScale, matRotZ, matTrans;
	D3DXVECTOR2 vPos = m_tInfo.vPos /*+ CScrollMgr::GetScroll()*/;
	//m_fAngle = -180.f * atan2(m_pMouse->GetInfo().vPos.y - m_tInfo.vDir.y - vScroll.y, m_pMouse->GetInfo().vPos.x - m_tInfo.vDir.x - vScroll.x) / D3DX_PI;


	

	
	if (m_iPlayerID == 1) 
	{
		if(m_iPlayerID == playercheck.playerID)
			//std::cout << playerInfo_1.PosX << "," << playerInfo_1.PosY << endl;
		// ���� ���� ������ ����
		/*vPos.x = playerinfo.PosX;
		vPos.y = playerinfo.PosY;*/
		// 201129 ������ ���� ������ ����
		vPos.x = playerInfo_1.PosX;
		vPos.y = playerInfo_1.PosY;
		
		m_fAngle = playerInfo_1.angle;

	}

	if (m_iPlayerID == 2)
	{
		if (m_iPlayerID == playercheck.playerID)
			//std::cout << playerInfo_2.PosX << "," << playerInfo_2.PosY << endl;
		// ���� ���� ������ ����
		/*vPos.x = playerinfo.PosX;
		vPos.y = playerinfo.PosY;*/
		// 201129 ������ ���� ������ ����
		vPos.x = playerInfo_2.PosX;
		vPos.y = playerInfo_2.PosY;

		m_fAngle = playerInfo_2.angle;

	}
	
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_fAngle));
	D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, 0.f);


		// �� �� �� �� �� 
		m_tInfo.matWorld = matScale * matRotZ * matTrans;

		/*�÷��̾� ��ġ����*/
		//D3DXVec3TransformCoord(&m_tInfo.vPos, &m_tInfo.vLook, &m_tInfo.matWorld);
		/*�÷��̾� ���⺤��*/
		D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vLook, &m_tInfo.matWorld);


	

	return NO_EVENT;
}

void CPlayer::LateUpdate()
{
	// ���� ������ �÷��̾� �̹��� �ٲ�
	m_wstrObjKey = ChangeWeapon();
	// �ִϸ��̼� ��� �ڵ�
	if (m_eCurState != IDLE)
	{
	/*if (m_tFrame.fFrame > 6)
	{
		m_tFrame.fFrame = 0.f;
	}*/
		CObj::MoveFrame();
		//if(m_tFrame.fFrame > m_tFrame.fMax)
		m_eCurState = STANCE::IDLE;
	}
}

void CPlayer::Render()
{
	const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(
		m_wstrObjKey.c_str(), m_wstrStateKey.c_str(), (int)m_tFrame.fFrame);
	// ������ ����ȭ
	//const TEXINFO* pTexInfo = m_vecTexture[(int)m_tFrame.fFrame];

	//m_tCollRect = { LONG(m_tInfo.vPos.x - pTexInfo->tImgInfo.Width*0.5f),  LONG(m_tInfo.vPos.y - pTexInfo->tImgInfo.Height*0.5f), LONG(m_tInfo.vPos.x + pTexInfo->tImgInfo.Width*0.5f), LONG(m_tInfo.vPos.y + pTexInfo->tImgInfo.Height*0.5f) };

	NULL_CHECK(pTexInfo);

	// �浹�ڽ� ���� �߰�
	CObj::UpdateCollRect(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	// 201129 �ؽ�Ʈ ���
	RECT rect;
	SetRect(&rect, 0, 0, 800, 600); // ��ġ
	wchar_t strState[256];
	/*if (m_iCurBulletNum < 0)
	   m_iUIBulletNum = 0;*/
	wsprintfW(strState, L"[HP]: %d\n[AMMO]: %d\n", m_iHP, m_iCurBulletNum);

	CDevice::GetInstance()->GetFont()->DrawText(NULL, strState, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}

void CPlayer::Release()
{
}


const TCHAR * CPlayer::ChangeWeapon()
{
	//m_iCurBulletNum = m_iMaxBulletNum;
	// m_wstrObjKey�� �ٲ㼭 ���� �ٲ� �ִϸ��̼� ��� �ǰ�
	switch (m_eWeapon)
	{
	case WEAPONID::NO_WEAPON:
		return L"PLAYER_UNARMED";
		break;
	case WEAPONID::RIFLE:
		/*if (!m_bIsSetMaxBul)
		{
			m_iMaxBulletNum = 5;
			m_iCurBulletNum = m_iMaxBulletNum;
			m_bIsSetMaxBul = true;
		}*/
		return L"PLAYER_RIFLE";
		break;
	case WEAPONID::SMG:
		//if (!m_bIsSetMaxBul)
		//{
		//	m_iMaxBulletNum = 3; // 201123 ���߷� �ٲܰ� �����ؼ�
		//	m_iCurBulletNum = m_iMaxBulletNum;
		//	m_bIsSetMaxBul = true;
		//}
		return L"PLAYER_SMG";
		break; 
	case WEAPONID::SHOTGUN:
		/*if (!m_bIsSetMaxBul)
		{
			m_iMaxBulletNum = 2;
			m_iCurBulletNum = m_iMaxBulletNum;
			m_bIsSetMaxBul = true;
		}*/
		return L"PLAYER_SHOTGUN";
		break;
	case WEAPONID::BAT:
		return L"PLAYER_BAT";
		break;
	case WEAPONID::CLUB:
		return L"PLAYER_CLUB";
		break;
	case WEAPONID::KNIFE:
		return L"PLAYER_KNIFE";
		CSoundMgr::GetInstance()->PlaySound(L"sndDrawKnife.wav", CSoundMgr::EFFECT);

		break;
	default:
		break;
	}
	return nullptr;
}

void CPlayer::KeyCheck() 
{
	float fTime = CTimeMgr::GetInstance()->GetTime();
	INFO tInfo_sg = m_tInfo;
	

	if (CKeyMgr::GetInstance()->KeyDown(KEY_LBUTTON)) // ���� �� �Ѿ� ����
	{		
		/*--m_iCurBulletNum;
		if (m_iCurBulletNum >= 0) */// 201123
		--m_iCurBulletNum;
		if (m_iCurBulletNum >= 0) 
		{
			switch (m_eWeapon)
			{
			case NO_WEAPON:
				m_eCurState = STANCE::ATTACK;
				m_wstrStateKey = L"Attack";
				m_tFrame.fMax = 7.f;
				CSoundMgr::GetInstance()->PlaySound(L"sndPunch.wav", CSoundMgr::EFFECT);
				m_ePreState = m_eCurState;
				break;
			case RIFLE:
				m_eCurState = STANCE::ATTACK;
				m_wstrStateKey = L"Attack";
				m_tFrame.fMax = 2.f;
				m_pBulletLst->push_back(CAbstractFactory<CPlayerBullet>::CreateObj(m_tInfo.vPos, m_tInfo.vDir, RIFLE));
				CSoundMgr::GetInstance()->PlaySound(L"sndM16.wav", CSoundMgr::EFFECT);
				m_ePreState = m_eCurState;
				break;
			case SMG:
				m_eCurState = STANCE::ATTACK;
				m_wstrStateKey = L"Attack";
				m_tFrame.fMax = 2.f;
				m_pBulletLst->push_back(CAbstractFactory<CPlayerBullet>::CreateObj(m_tInfo.vPos, m_tInfo.vDir, SMG));
				CSoundMgr::GetInstance()->PlaySound(L"sndUzi.wav", CSoundMgr::EFFECT);
				m_ePreState = m_eCurState;
				break;
			case SHOTGUN: // ���� ������ �����°� �߰�
				m_eCurState = STANCE::ATTACK;
				m_wstrStateKey = L"Attack";
				m_tFrame.fMax = 12.f;
				for (int j = -20; j < 20; j += 10)
				{
					D3DXMATRIX matRotZ;
					D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_fAngle + j));
					//m_tInfo.matWorld = matRotZ;
					tInfo_sg.matWorld = matRotZ;

					/*ȸ�� �� ��ġ ���� ��ġ����, ���⺤�� ����.*/
					/*D3DXVec3TransformCoord(&m_tInfo.vPos, &m_tInfo.vLook, &m_tInfo.matWorld);
					D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vLook, &m_tInfo.matWorld);*/
					D3DXVec3TransformCoord(&tInfo_sg.vPos, &tInfo_sg.vLook, &tInfo_sg.matWorld);
					D3DXVec3TransformNormal(&tInfo_sg.vDir, &tInfo_sg.vLook, &tInfo_sg.matWorld);

					//m_pBulletLst->push_back(CAbstractFactory<CPlayerBullet>::CreateObj(m_tInfo.vPos, m_tInfo.vDir, SHOTGUN));
					m_pBulletLst->push_back(CAbstractFactory<CPlayerBullet>::CreateObj(m_tInfo.vPos, tInfo_sg.vDir, SHOTGUN));
				}
				CSoundMgr::GetInstance()->PlaySound(L"sndShotgun.wav", CSoundMgr::EFFECT);
				m_ePreState = m_eCurState;
				break;
			case CLUB:
				m_eCurState = STANCE::ATTACK;
				m_wstrStateKey = L"Attack";
				m_tFrame.fMax = 9.f;

				CSoundMgr::GetInstance()->PlaySound(L"sndSwing2.wav", CSoundMgr::EFFECT);
				m_ePreState = m_eCurState;
				break;
			case BAT:
				m_eCurState = STANCE::ATTACK;
				m_wstrStateKey = L"Attack";
				m_tFrame.fMax = 9.f;

				CSoundMgr::GetInstance()->PlaySound(L"sndSwing1.wav", CSoundMgr::EFFECT);
				m_ePreState = m_eCurState;
				break;
			case KNIFE:
				m_eCurState = STANCE::ATTACK;
				m_wstrStateKey = L"Attack";
				m_tFrame.fMax = 9.f;

				CSoundMgr::GetInstance()->PlaySound(L"sndSwing1.wav", CSoundMgr::EFFECT);
				m_ePreState = m_eCurState;
				break;
			case WEAPON_END:
				break;
			default:
				break;
			}
		}
		
		
	}

	if (CKeyMgr::GetInstance()->KeyPressing(KEY_RBUTTON))
	{
		m_eCurState = STANCE::IDLE;
		m_wstrStateKey = L"Walk";
		m_tFrame.fMax = 8.f;
	}
	if (CKeyMgr::GetInstance()->KeyPressing(KEY_W))
	{
		m_eCurState = STANCE::WALK;
		m_ePreState = m_eCurState;
		//m_tInfo.vPos += m_tInfo.vDir * m_fSpeed; // ���콺 ���󰡰� �ϴ� �ڵ�
		//m_tInfo.vPos.y -= m_fSpeed * fTime;
	}
	if (CKeyMgr::GetInstance()->KeyPressing(KEY_S))
	{
		m_eCurState = STANCE::WALK;
		m_ePreState = m_eCurState;
		//m_tInfo.vPos += m_tInfo.vDir * -m_fSpeed; // ���콺 ���󰡰� �ϴ� �ڵ�
		//m_tInfo.vPos.y += m_fSpeed  * fTime;
	}
	if (CKeyMgr::GetInstance()->KeyPressing(KEY_A))
	{
		m_eCurState = STANCE::WALK;
		m_ePreState = m_eCurState;
		m_fAngle += 5.f;
		//m_tInfo.vPos.x -= m_fSpeed * fTime;
	}
	if (CKeyMgr::GetInstance()->KeyPressing(KEY_D))
	{
		m_eCurState = STANCE::WALK;
		m_ePreState = m_eCurState;
		m_fAngle -= 5.f;
		//m_tInfo.vPos.x += m_fSpeed * fTime;
	}

	//201123 ����Ű �߰�
	if (CKeyMgr::GetInstance()->KeyDown(KEY_R))
	{
		m_iCurBulletNum = m_iMaxBulletNum;
	}
	// ���� ����Ű �߰�
	if (CKeyMgr::GetInstance()->KeyDown(KEY_E))
	{
		// ����� �浹���϶� Ű�� �Էµƴٸ�
		if (m_bIsOverlap)
		{
			m_bIsPressedE = true;
		}
	}
}

