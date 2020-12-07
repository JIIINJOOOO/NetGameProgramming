#pragma once
// ���߿� ������ �浹 ó�� �ڵ� ���� ���ͷ� ��ġ��
class CCollisionMgr
{
public:
	CCollisionMgr();
	~CCollisionMgr();
public:
	// �簢�� �浹
	static void CollisionRect(OBJLIST& dstLst, OBJLIST& srcLst);
	// 201208 �÷��̾�-�Ѿ� �浹
	static void CollisionRectPlayerBul(OBJLIST& dstLst, OBJLIST& srcLst);
	// �� �浹
	static void CollisionRectEx(OBJLIST& dstLst, OBJLIST& srcLst);
	// �� �浹
	static void CollisionSphere(OBJLIST&dstLst, OBJLIST& srcLst);
	// ����-�÷��̾� �浹
	static void CollisionRectWeapon(OBJLIST& dstLst, OBJLIST& srcLst);

public:
	// �� �߽� ������ �Ÿ�
	static bool CheckSphere(CObj* pDst, CObj*pSrc);
	// �簢�� ���ƴ���
	static bool CheckRect(CObj* pDst, CObj* pSrc, float* pMoveX, float* pMoveY);


};

