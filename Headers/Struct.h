#pragma once

//typedef struct tagInfo
//{
//	D3DXVECTOR3 vPos;
//	D3DXVECTOR3 vDir;
//	D3DXVECTOR3 vLook;
//	D3DXMATRIX  matWorld;
//}INFO;

typedef struct tagTexture
{
	// LPDIRECT3DTEXTURE9: �̹��� ������ ������ Com��ü
	LPDIRECT3DTEXTURE9	pTexture;

	// �̹��� ������ �����ϱ� ���� ����ü.
	D3DXIMAGE_INFO		tImgInfo;

	TCHAR	szImagePath[MAX_STR]; // �̹��� �ּ�
	int iCount; // ��Ƽ �ؽ����� ����
}TEXINFO;

typedef struct tagInfo
{
	D3DXVECTOR3	vPos;	// Ÿ���� ����.
	D3DXVECTOR3	vSize;	// Ÿ���� ũ��.

	D3DXVECTOR3 vDir;			// ���⺤�� -> �߰�
	D3DXVECTOR3 vLook;			// ���غ��� -> �߰�

	D3DXMATRIX	matWorld;		// ������� -> �߰�

	/////////////////////////// Ÿ�� ����
	BYTE byOption;		// Ÿ���� �ɼ�. -> 1�̸� ���������� Ÿ��,0�̸� �������� �ִ� Ÿ��
	BYTE byDrawID;		// ������ �ɼ�.
	bool bIsRender;		// �׷����� ���� -> ���߿� ������

	int iIndex = 0;	// ���� Ÿ���� �ε��� ����.
	int iParentIdx = 0;	// �θ� Ÿ���� �ε��� ����.

	/////////////////////////// Scale�� -> �߰�
	float fScaleX = 1.f;
	float fScaleY = 1.f;
	// MAPID�� -> �߰�
	MAPID	eMapID; // ���̵� ���� �ε�?

	
}INFO;

typedef struct tagUnitData
{
//#ifndef _AFX
//	wstring strName;
//#else
//	CString strName;
//#endif
//
//	int		iAtt;
//	int		iDef;
//	BYTE	byJobIndex;
//	BYTE	byItem;
	D3DXVECTOR3 vPos; // �̹��� ����
	D3DXVECTOR3 vDir;
	D3DXVECTOR3 vLook;
	D3DXMATRIX  matWorld;


	D3DXVECTOR3 vColBoxPos;		// �浹�ڽ� ����.
	D3DXVECTOR3 vColBoxSize;	// �浹�ڽ� ������
	RECT		rcColRect;		// �浹 RECT

	// �÷��̾�& �� ���Ⱚ
	WEAPONID	eWeaponID; // ���� ���� ������ �ִ���


}UNIT_DATA; // �׳� Info�� ��ġ�� �浹 �ڽ��� ���� �Ⱦ��� ���� �𸣰���

typedef struct tagTexturePath
{
	wstring wstrObjKey = L"";
	wstring wstrStateKey = L"";
	wstring wstrPath = L"";
	int iCount = 0;
}IMGPATH;

typedef struct tagFrame
{
	tagFrame()
		: fFrame(0.f), fMax(0.f)
	{}

	tagFrame(float frame, float max)
		: fFrame(frame), fMax(max)
	{}

	float fFrame; // ���� �ִϸ��̼� ��� ����.
	float fMax;	// �ִ� ��� ����.

}FRAME;

#pragma pack(push,1)
typedef struct PlayerInfo
{
	int PosX;
	int PosY;
	int playerID;
	int weaponID;
	int CurBulletNum;
	int MaxBulletNum;
	int HP;
	int money;
	float angle;
};
#pragma pack(pop)

#pragma pack(push,1)
typedef struct KeyInput 
{
	bool key_W_Press;
	bool key_A_Press;
	bool key_S_Press;
	bool key_D_Press;
	bool key_R_Press;
	bool key_E_Press;
	bool Mouse_R_Press;
	bool Mouse_L_Press;
	int playerID;
	float mouseX;
	float mouseY;
};
#pragma pack(pop)

#pragma pack(push,1)
typedef struct PlayerNumCheck 
{
	int enterPlayerNum;
	int playerID;
};
#pragma pack(pop)

#pragma pack(push,1)
typedef struct CollisionObj
{
	int objID;
	long left;
	long top;
	long right;
	long bottom;
	float PosX;
	float PosY;
}COLOBJ;
#pragma pack(pop)
