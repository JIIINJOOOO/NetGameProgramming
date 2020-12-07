#pragma once
// 나중에 선생님 충돌 처리 코드 보고 벡터로 고치자
class CCollisionMgr
{
public:
	CCollisionMgr();
	~CCollisionMgr();
public:
	// 사각형 충돌
	static void CollisionRect(OBJLIST& dstLst, OBJLIST& srcLst);
	// 201208 플레이어-총알 충돌
	static void CollisionRectPlayerBul(OBJLIST& dstLst, OBJLIST& srcLst);
	// 벽 충돌
	static void CollisionRectEx(OBJLIST& dstLst, OBJLIST& srcLst);
	// 원 충돌
	static void CollisionSphere(OBJLIST&dstLst, OBJLIST& srcLst);
	// 무기-플레이어 충돌
	static void CollisionRectWeapon(OBJLIST& dstLst, OBJLIST& srcLst);

public:
	// 원 중심 사이의 거리
	static bool CheckSphere(CObj* pDst, CObj*pSrc);
	// 사각형 겹쳤는지
	static bool CheckRect(CObj* pDst, CObj* pSrc, float* pMoveX, float* pMoveY);


};

