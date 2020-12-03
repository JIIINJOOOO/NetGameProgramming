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
	// LPDIRECT3DTEXTURE9: 이미지 한장을 제어할 Com객체
	LPDIRECT3DTEXTURE9	pTexture;

	// 이미지 정보를 보관하기 위한 구조체.
	D3DXIMAGE_INFO		tImgInfo;

	TCHAR	szImagePath[MAX_STR]; // 이미지 주소
	int iCount; // 멀티 텍스쳐의 개수
}TEXINFO;

typedef struct tagInfo
{
	D3DXVECTOR3	vPos;	// 타일의 중점.
	D3DXVECTOR3	vSize;	// 타일의 크기.

	D3DXVECTOR3 vDir;			// 방향벡터 -> 추가
	D3DXVECTOR3 vLook;			// 기준벡터 -> 추가

	D3DXMATRIX	matWorld;		// 월드행렬 -> 추가

	/////////////////////////// 타일 정보
	BYTE byOption;		// 타일의 옵션. -> 1이면 못지나가는 타일,0이면 지나갈수 있는 타일
	BYTE byDrawID;		// 렌더링 옵션.
	bool bIsRender;		// 그려질지 말지 -> 나중에 지우자

	int iIndex = 0;	// 현재 타일의 인덱스 정보.
	int iParentIdx = 0;	// 부모 타일의 인덱스 정보.

	/////////////////////////// Scale값 -> 추가
	float fScaleX = 1.f;
	float fScaleY = 1.f;
	// MAPID값 -> 추가
	MAPID	eMapID; // 아이디에 따라 로드?

	
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
	D3DXVECTOR3 vPos; // 이미지 중점
	D3DXVECTOR3 vDir;
	D3DXVECTOR3 vLook;
	D3DXMATRIX  matWorld;


	D3DXVECTOR3 vColBoxPos;		// 충돌박스 중점.
	D3DXVECTOR3 vColBoxSize;	// 충돌박스 사이즈
	RECT		rcColRect;		// 충돌 RECT

	// 플레이어& 적 무기값
	WEAPONID	eWeaponID; // 무슨 무기 가지고 있는지


}UNIT_DATA; // 그냥 Info로 퉁치고 충돌 박스는 쓸지 안쓸지 아직 모르겠음

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

	float fFrame; // 현재 애니메이션 재생 구간.
	float fMax;	// 최대 재생 길이.

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
