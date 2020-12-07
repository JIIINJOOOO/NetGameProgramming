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
	m_iHP = 0;
	m_iMoney = 0;
	m_bIsUIRender = false;
	for (int i = 0; i < 10; ++i)
	{
		p1_isBulletInit[i] = false;
		p2_isBulletInit[i] = false;
	}

	return S_OK;
}

void CPlayer::LateInit()
{
	
}

int CPlayer::Update()
{
	CObj::LateInit();
	//PlayerInfo playerinfo;
	if (m_iPlayerID == 1)
	{
		m_iHP = playerInfo_1.HP;
		m_iMoney = playerInfo_1.money;
		m_bIsDead = playerInfo_1.IsDead;
	}
	else if (m_iPlayerID == 2)
	{
		m_iHP = playerInfo_2.HP;
		m_iMoney = playerInfo_2.money;
		m_bIsDead = playerInfo_2.IsDead;
	}
	if (m_bIsDead)
		return DEAD_OBJ;

	KeyCheck();


	D3DXMATRIX matScale, matRotZ, matTrans;
	D3DXVECTOR2 vPos = m_tInfo.vPos /*+ CScrollMgr::GetScroll()*/;
	//m_fAngle = -180.f * atan2(m_pMouse->GetInfo().vPos.y - m_tInfo.vDir.y - vScroll.y, m_pMouse->GetInfo().vPos.x - m_tInfo.vDir.x - vScroll.x) / D3DX_PI;
	D3DXVECTOR2 vDir = m_tInfo.vDir;

	

	
	if (m_iPlayerID == 1) 
	{
		/*if(m_iPlayerID == playercheck.playerID)
			std::cout << playerInfo_1.PosX << "," << playerInfo_1.PosY << endl;*/
		// ���� ���� ������ ����
		/*vPos.x = playerinfo.PosX;
		vPos.y = playerinfo.PosY;*/
		// 201129 ������ ���� ������ ����
		vPos.x = playerInfo_1.PosX;
		vPos.y = playerInfo_1.PosY;
		
		m_fAngle = playerInfo_1.angle;

		if (playerInfo_1.weaponID == 1)
		{
			m_eWeapon = WEAPONID::RIFLE;
		}
		if (playerInfo_1.weaponID == 2)
		{
			m_eWeapon = WEAPONID::SMG;
		}
		if (playerInfo_1.weaponID == 3) {
			m_eWeapon = WEAPONID::SHOTGUN;
		}
		m_iMaxBulletNum = playerInfo_1.MaxBulletNum;
		m_iCurBulletNum = playerInfo_1.CurBulletNum;
		vDir.x = playerInfo_1.DirX;
		vDir.y = playerInfo_1.DirY;

		for (int i = 0; i < 10; ++i) 
		{
			if (!playerInfo_1.bullets[i].IsDead) 
			{
				if (!p1_isBulletInit[i]) 
				{
					if (playerInfo_1.weaponID == 1) 
					{
						m_pBulletLst->push_back(CAbstractFactory<CPlayerBullet>::CreateObj(D3DXVECTOR3(vPos.x, vPos.y, 0)/*m_tInfo.vPos*/, D3DXVECTOR3(vDir.x, vDir.y, 0),m_fAngle ,RIFLE));
						CSoundMgr::GetInstance()->PlaySound(L"sndM16.wav", CSoundMgr::EFFECT);

					}
					if (playerInfo_1.weaponID == 2) 
					{
						m_pBulletLst->push_back(CAbstractFactory<CPlayerBullet>::CreateObj(D3DXVECTOR3(vPos.x, vPos.y, 0)/*m_tInfo.vPos*/, D3DXVECTOR3(vDir.x, vDir.y, 0), m_fAngle, SMG));
					}
					if (playerInfo_1.weaponID == 3) 
					{
						m_pBulletLst->push_back(CAbstractFactory<CPlayerBullet>::CreateObj(D3DXVECTOR3(vPos.x, vPos.y, 0), D3DXVECTOR3(vDir.x, vDir.y, 0), m_fAngle, SHOTGUN));

					}

					p1_isBulletInit[i] = true;

				}
				else
				{
					//set pos�� ���� �ǰ� �ȵ����ɵ�?
				}
			}
			else
			{
				p1_isBulletInit[i] = false;
			}

			
		}


	}

	if (m_iPlayerID == 2)
	{
		/*if (m_iPlayerID == playercheck.playerID)
			std::cout << playerInfo_2.PosX << "," << playerInfo_2.PosY << endl;*/
		// ���� ���� ������ ����
		/*vPos.x = playerinfo.PosX;
		vPos.y = playerinfo.PosY;*/
		// 201129 ������ ���� ������ ����
		vPos.x = playerInfo_2.PosX;
		vPos.y = playerInfo_2.PosY;

		m_fAngle = playerInfo_2.angle;

		//cout << playerInfo_2.weaponID << endl;
		if (playerInfo_2.weaponID == 1)
		{
			m_eWeapon = WEAPONID::RIFLE;
		}
		if (playerInfo_2.weaponID == 2)
		{
			m_eWeapon = WEAPONID::SMG;
		}
		if (playerInfo_2.weaponID == 3) {
			m_eWeapon = WEAPONID::SHOTGUN;
		}
		m_iMaxBulletNum = playerInfo_2.MaxBulletNum;
		m_iCurBulletNum = playerInfo_2.CurBulletNum;
		vDir.x = playerInfo_2.DirX;
		vDir.y = playerInfo_2.DirY;

		for (int i = 0; i < 10; ++i) 
		{

			if (!playerInfo_2.bullets[i].IsDead)
			{
				if (!p2_isBulletInit[i])
				{
					if (playerInfo_2.weaponID == 1)
					{
						m_pBulletLst->push_back(CAbstractFactory<CPlayerBullet>::CreateObj(D3DXVECTOR3(playerInfo_2.PosX, playerInfo_2.PosY, 0)/*m_tInfo.vPos*/, D3DXVECTOR3(vDir.x, vDir.y, 0), m_fAngle, RIFLE));
						CSoundMgr::GetInstance()->PlaySound(L"sndM16.wav", CSoundMgr::EFFECT);

					}
					if (playerInfo_2.weaponID == 2) 
					{
						m_pBulletLst->push_back(CAbstractFactory<CPlayerBullet>::CreateObj(D3DXVECTOR3(playerInfo_2.PosX, playerInfo_2.PosY, 0)/*m_tInfo.vPos*/, D3DXVECTOR3(vDir.x, vDir.y, 0), m_fAngle, SMG));

					}
					if (playerInfo_2.weaponID == 3)
					{
						m_pBulletLst->push_back(CAbstractFactory<CPlayerBullet>::CreateObj(D3DXVECTOR3(vPos.x, vPos.y, 0), D3DXVECTOR3(vDir.x, vDir.y, 0), m_fAngle, SHOTGUN));

					}
					p2_isBulletInit[i] = true;
				}
				else
				{
					//set pos�� ���� �ǰ� �ȵ����ɵ�?
				}
			}
			else
			{
				p2_isBulletInit[i] = false;
			}
		}

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
	if (playercheck.playerID == 1)
	{
		/*if (m_iCurBulletNum < 0)
		   m_iUIBulletNum = 0;*/
		wsprintfW(strState, L"[HP]: %d\n[AMMO]: %d\n [MONEY]: %d\n", playerInfo_1.HP/*m_iHP*/, playerInfo_1.CurBulletNum/*m_iCurBulletNum*/, playerInfo_1.money);
	}
	else if (playercheck.playerID == 2)
	{
		wsprintfW(strState, L"[HP]: %d\n[AMMO]: %d\n [MONEY]: %d\n", playerInfo_2.HP, playerInfo_2.CurBulletNum, playerInfo_2.money/*m_iHP, m_iCurBulletNum, m_iMoney*/);
	}
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
				//if(m_iPlayerID == 1)
				//	m_pBulletLst->push_back(CAbstractFactory<CPlayerBullet>::CreateObj(D3DXVECTOR3(playerInfo_1.PosX, playerInfo_1.PosY, 0)/*m_tInfo.vPos*/, D3DXVECTOR3(playerInfo_1.DirX, playerInfo_1.DirY, 0), RIFLE));
				//CSoundMgr::GetInstance()->PlaySound(L"sndM16.wav", CSoundMgr::EFFECT);
				m_ePreState = m_eCurState;
				break;
			case SMG:
				m_eCurState = STANCE::ATTACK;
				m_wstrStateKey = L"Attack";
				m_tFrame.fMax = 2.f;
				//m_pBulletLst->push_back(CAbstractFactory<CPlayerBullet>::CreateObj(m_tInfo.vPos, D3DXVECTOR3(playerInfo_1.DirX, playerInfo_1.DirY, 0), SMG));
				CSoundMgr::GetInstance()->PlaySound(L"sndUzi.wav", CSoundMgr::EFFECT);
				m_ePreState = m_eCurState;
				break;
			case SHOTGUN: // ���� ������ �����°� �߰�
				m_eCurState = STANCE::ATTACK;
				m_wstrStateKey = L"Attack";
				m_tFrame.fMax = 12.f;
				//for (int j = -20; j < 20; j += 10)
				//{
				//	D3DXMATRIX matRotZ;
				//	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-m_fAngle + j));
				//	//m_tInfo.matWorld = matRotZ;
				//	tInfo_sg.matWorld = matRotZ;

				//	/*ȸ�� �� ��ġ ���� ��ġ����, ���⺤�� ����.*/
				//	/*D3DXVec3TransformCoord(&m_tInfo.vPos, &m_tInfo.vLook, &m_tInfo.matWorld);
				//	D3DXVec3TransformNormal(&m_tInfo.vDir, &m_tInfo.vLook, &m_tInfo.matWorld);*/
				//	D3DXVec3TransformCoord(&tInfo_sg.vPos, &tInfo_sg.vLook, &tInfo_sg.matWorld);
				//	D3DXVec3TransformNormal(&tInfo_sg.vDir, &tInfo_sg.vLook, &tInfo_sg.matWorld);

				//	//m_pBulletLst->push_back(CAbstractFactory<CPlayerBullet>::CreateObj(m_tInfo.vPos, m_tInfo.vDir, SHOTGUN));
				//	m_pBulletLst->push_back(CAbstractFactory<CPlayerBullet>::CreateObj(m_tInfo.vPos, D3DXVECTOR3(playerInfo_1.DirX, playerInfo_1.DirY, 0), SHOTGUN));
				//}
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
		//m_fAngle += 5.f;
		//m_tInfo.vPos.x -= m_fSpeed * fTime;
	}
	if (CKeyMgr::GetInstance()->KeyPressing(KEY_D))
	{
		m_eCurState = STANCE::WALK;
		m_ePreState = m_eCurState;
		//m_fAngle -= 5.f;
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
		if (m_iPlayerID == 1)
		{
			if (playerInfo_1.weaponID == 1)
			{
				m_eWeapon = WEAPONID::RIFLE;
			}
			else if (playerInfo_1.weaponID == 2)
			{
				m_eWeapon = WEAPONID::SMG;
			}
			else if (playerInfo_1.weaponID == 3)
			{
				m_eWeapon = WEAPONID::SHOTGUN;
			}
		}
		else if (m_iPlayerID == 2)
		{
			if (playerInfo_2.weaponID == 1)
			{
				m_eWeapon = WEAPONID::RIFLE;
			}
			else if (playerInfo_2.weaponID == 2)
			{
				m_eWeapon = WEAPONID::SMG;
			}
			else if (playerInfo_2.weaponID == 3)
			{
				m_eWeapon = WEAPONID::SHOTGUN;
			}
		}
	}
}

