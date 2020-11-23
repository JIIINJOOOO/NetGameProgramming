#include "stdafx.h"
#include "Stage.h"
#include "Terrain.h"
#include "Player.h"
#include "Mouse.h"
#include "Enemy.h"


CStage::CStage()
{
}


CStage::~CStage()
{
	Release();
}

HRESULT CStage::Initialize()
{
	if (FAILED(CTextureMgr::GetInstance()->ReadImgPath(L"../Data/ImgPath.txt")))
		return E_FAIL;

	CObj* pTerrain = nullptr;
	if (FAILED(CAbstractFactory<CTerrain>::CreateObj(pTerrain)))
		return E_FAIL;

	// 벽 로드 코드가 프레임 저하 원인! -> 1000개 넘게 불러와진다
	// -> 프레임 최적화 코드로 해결은 했는데 1000개 불러와지는거 좀 이상하다
	// -> 나중에 해결해보자! (아마 툴쪽 배열에서 문제있을거 같다. 데코 배열이랑 월 배열이랑 비교해보자)
	if (FAILED(CMapMgr::GetInstance()->LoadWall(L"../Data/Stage1-MapData5.dat")))
	{
		ERR_MSG(L"Stage1 Wall Load Fail");
		return E_FAIL;
	}

	if (FAILED(CMapMgr::GetInstance()->LoadDoor(L"../Data/Stage1-MapData5.dat")))
	{
		ERR_MSG(L"Stage1 Door Load Fail");
		return E_FAIL;
	}


	if (FAILED(CMapMgr::GetInstance()->LoadDeco(L"../Data/Stage1-MapData5.dat")))
	{
		ERR_MSG(L"Stage1 Deco Load Fail");
		return E_FAIL;
	}

	if (FAILED(CMapMgr::GetInstance()->LoadWeapon(L"../Data/Stage1-MapData5.dat")))
	{
		ERR_MSG(L"Stage1 Weapon Load Fail");
		return E_FAIL;
	}

	CObj* pMouse = nullptr;
	if (FAILED(CAbstractFactory<CMouse>::CreateObj(pMouse)))
		return E_FAIL;

	CObjMgr::GetInstance()->AddObject(pMouse, OBJ_MOUSE);


	CObj* pPlayer = nullptr;
	if (FAILED(CAbstractFactory<CPlayer>::CreateObj(pPlayer)))
		return E_FAIL;	

	/*CObj* pEnemy1 = nullptr;
	if (FAILED(CAbstractFactory<CEnemy>::CreateObj(pEnemy1, D3DXVECTOR3({ 500.f, 600.f, 0.f }), WEAPONID::RIFLE)));
		return E_FAIL;*/

	CObjMgr::GetInstance()->AddObject(pTerrain, OBJ_TERRAIN);
	CObjMgr::GetInstance()->AddObject(pPlayer, OBJ_PLAYER);
	
	// 적 추가
	// 중앙 복도
	/*CObjMgr::GetInstance()->AddObject(CAbstractFactory<CEnemy>::CreateObj(D3DXVECTOR3({ 750.f,200.f,0.f }), WEAPONID::KNIFE), OBJ_ENEMY);
	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CEnemy>::CreateObj(D3DXVECTOR3({ 800.f,500.f,0.f }), WEAPONID::NO_WEAPON), OBJ_ENEMY);
	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CEnemy>::CreateObj(D3DXVECTOR3({ 900.f,400.f,0.f }), WEAPONID::BAT), OBJ_ENEMY);*/

	// 빨간방
	/*CObjMgr::GetInstance()->AddObject(CAbstractFactory<CEnemy>::CreateObj(D3DXVECTOR3({ 500.f,500.f,0.f }),WEAPONID::RIFLE), OBJ_ENEMY);
	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CEnemy>::CreateObj(D3DXVECTOR3({ 400.f,700.f,0.f }), WEAPONID::RIFLE), OBJ_ENEMY);
	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CEnemy>::CreateObj(D3DXVECTOR3({ 200.f,600.f,0.f }), WEAPONID::RIFLE), OBJ_ENEMY);*/

	// 화장실
	/*CObjMgr::GetInstance()->AddObject(CAbstractFactory<CEnemy>::CreateObj(D3DXVECTOR3({ 300.f,200.f,0.f }), WEAPONID::SMG), OBJ_ENEMY);
	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CEnemy>::CreateObj(D3DXVECTOR3({ 250.f,300.f,0.f }), WEAPONID::SMG), OBJ_ENEMY);*/

	// 갈색방
	/*CObjMgr::GetInstance()->AddObject(CAbstractFactory<CEnemy>::CreateObj(D3DXVECTOR3({ 450.f,200.f,0.f }), WEAPONID::SMG), OBJ_ENEMY);
	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CEnemy>::CreateObj(D3DXVECTOR3({ 600.f,300.f,0.f }), WEAPONID::CLUB), OBJ_ENEMY);*/

	// 보라색방
	/*CObjMgr::GetInstance()->AddObject(CAbstractFactory<CEnemy>::CreateObj(D3DXVECTOR3({ 1120.f,200.f,0.f }), WEAPONID::SHOTGUN), OBJ_ENEMY);
	CObjMgr::GetInstance()->AddObject(CAbstractFactory<CEnemy>::CreateObj(D3DXVECTOR3({ 1050.f,100.f,0.f }), WEAPONID::RIFLE), OBJ_ENEMY);*/

	// 카페트방
	//CObjMgr::GetInstance()->AddObject(CAbstractFactory<CEnemy>::CreateObj(D3DXVECTOR3({ 1100.f,600.f,0.f }), WEAPONID::KNIFE), OBJ_ENEMY);

	CSoundMgr::GetInstance()->PlayBGM(L"InnerAnimal.mp3");


	return S_OK;
}

void CStage::Update()
{
	CObjMgr::GetInstance()->Update();
}

void CStage::LateUpdate()
{
	CObjMgr::GetInstance()->LateUpdate();
}

void CStage::Render()
{
	//CMapMgr::GetInstance()->Render(); -> 맵매니저에서 렌더 안해줘도 오브젝트 매니저에서 렌더 해줌. 맵매니저 있어야 할지? 수정 필요
	CObjMgr::GetInstance()->Render();
}

void CStage::Release()
{
}
