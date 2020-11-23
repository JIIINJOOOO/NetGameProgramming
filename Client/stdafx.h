// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <io.h>
#include <ctime>
#include <random>

#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <string>



using namespace std;

// d3d9: ��ġ(GPU)�� �����ϱ� ���� �ڷ����̳� �Լ��� �����Ѵ�.
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

// d3dx9: ���̷�Ʈ���� ����� ����, ��� �ڷ����̳� �����Լ����� �����Ѵ�.
#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

// �޸� ����
//#include <vld.h>

// FMOD
#include "fmod.h"
#pragma comment(lib, "fmodex_vc.lib")


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include "Extern.h"
#include "Define.h"
#include "Constant.h"
#include "Typedef.h"
#include "Enum.h"
#include "Struct.h"
#include "Function.h"
#include "Functor.h"

// managers
#include "TimeMgr.h"
#include "Device.h"
#include "TextureMgr.h"
#include "AbstractFactoy.h"
#include "KeyMgr.h"
#include "MapMgr.h"
#include "ObjMgr.h"
#include "SceneMgr.h"
#include "ScrollMgr.h"
#include "AstarMgr.h"
#include "SoundMgr.h"

