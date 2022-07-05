//=============================================================================
//
// ���C������ [main.h]
// Author : GP11A132 12 �F�J����
//
//=============================================================================
#pragma once

#pragma warning(push)
#pragma warning(disable:4005)

#define _CRT_SECURE_NO_WARNINGS			// scanf ��warning�h�~
#include <stdio.h>

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <d3dx9.h>

#define DIRECTINPUT_VERSION 0x0800		// �x���Ώ�
#include "dinput.h"
#include "mmsystem.h"

#pragma warning(pop)


//*****************************************************************************
// ���C�u�����̃����N
//*****************************************************************************
#pragma comment (lib, "d3d11.lib")		
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "d3dx11.lib")	
#pragma comment (lib, "d3dx9.lib")	
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dxerr.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SCREEN_WIDTH	(960)			// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(540)			// �E�C���h�E�̍���
#define SCREEN_CENTER_X	(SCREEN_WIDTH / 2)	// �E�C���h�E�̒��S�w���W
#define SCREEN_CENTER_Y	(SCREEN_HEIGHT / 2)	// �E�C���h�E�̒��S�x���W
#define WALL_LEFT       (-640)
#define WALL_RIGHT      (640)
#define WALL_UP         (640)
#define WALL_DOWN       (-640)
#define GROUND          (0.0f)          

// ��ԗp�̃f�[�^�\���̂��`
struct INTERPOLATION_DATA
{
    D3DXVECTOR3 pos;		// ���_���W(�e�̍��W�����_�Ƃ������΍��W)
    D3DXVECTOR3 rot;		// ��](�e�̉�]�ɑ΂��Ď�������])
    D3DXVECTOR3 scl;		// �g��k��(�e�̊g�k�ɑ΂��Ď������g�k)
    float		frame;		// ���s�t���[���� ( dt = 1.0f/frame )
};
typedef struct ENEMY;
typedef struct BULLET;
typedef enum
{
    MODE_TITLE = 0,							// �^�C�g�����
    MODE_GAME,								// �Q�[�����
    MODE_RESULT,							// ���U���g���
    MODE_MAX
} MODE;

struct BBOX
{
	D3DXVECTOR3 Max;
	D3DXVECTOR3 Min;
	D3DXVECTOR3 AxisX;		//��
	D3DXVECTOR3 AxisY;
	D3DXVECTOR3 AxisZ;
	FLOAT LengthX;
	FLOAT LengthY;
	FLOAT LengthZ;
	BBOX()
	{
		ZeroMemory(this, sizeof(BBOX));
		AxisX = D3DXVECTOR3(1, 0, 0);
		AxisY = D3DXVECTOR3(0, 1, 0);
		AxisZ = D3DXVECTOR3(0, 0, 1);
	}
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
long GetMousePosX(void);
long GetMousePosY(void);
char* GetDebugStr(void);
void HitCheck(D3DXVECTOR3 light_position);
bool HitCheckBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float size1, float size2);
bool HitCheckBB(D3DXVECTOR3 mpos, float mhw, float mhh, D3DXVECTOR3 ypos, float yhw, float yhh);
bool HitCheckBB3D(D3DXVECTOR3 mpos, float mx, float my, float mz, D3DXVECTOR3 ypos, float yx, float yy, float yz);
bool HitCheckOBB(BULLET* model_bullet, ENEMY* model_enemy);
bool CompareLength(BBOX* box_a, BBOX* box_b, D3DXVECTOR3* separate, D3DXVECTOR3* distance);
void SetMode(MODE mode);
MODE GetMode(void);