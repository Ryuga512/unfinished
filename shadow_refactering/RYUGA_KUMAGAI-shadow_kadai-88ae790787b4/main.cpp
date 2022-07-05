//=============================================================================
//
// ���C������ [main.cpp]
// Author : GP11A132 12 �F�J����
//
//=============================================================================
#pragma once
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "player.h"
#include "enemy.h"
#include "meshfield.h"
#include "meshwall.h"
#include "shadow.h"
#include "tree.h"
#include "bullet.h"
#include "score.h"
#include "sound.h"
#include "particle.h"
#include "game.h"
#include "title.h"
#include "fade.h"
#include "StencilShadow.h"
#include "result.h"
#include "shadow.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CLASS_NAME		"AppClass"				// �E�C���h�E�̃N���X��
#define WINDOW_NAME		"�e�\��"	            // �E�C���h�E�̃L���v�V������
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//*****************************************************************************
// �O���[�o���ϐ�:
//*****************************************************************************
long g_MouseX = 0;
long g_MouseY = 0;

MODE g_Mode = MODE_TITLE;

#ifdef _DEBUG
int		g_CountFPS;							// FPS�J�E���^
char	g_DebugStr[2048] = WINDOW_NAME;		// �f�o�b�O�����\���p

#endif

//=============================================================================
// ���C���֐�
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	UNREFERENCED_PARAMETER(lpCmdLine);		// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j

	// ���Ԍv���p
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	WNDCLASSEX	wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND		hWnd;
	MSG			msg;
	
	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �E�B���h�E�̍쐬
	hWnd = CreateWindow(CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,																		// �E�B���h�E�̍����W
		CW_USEDEFAULT,																		// �E�B���h�E�̏���W
		SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2,									// �E�B���h�E����
		SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),	// �E�B���h�E�c��
		NULL,
		NULL,
		hInstance,
		NULL);

	// ����������(�E�B���h�E���쐬���Ă���s��)
	if(FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	// �t���[���J�E���g������
	timeBeginPeriod(1);	// ����\��ݒ�
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// �V�X�e���������~���b�P�ʂŎ擾
	dwCurrentTime = dwFrameCount = 0;

	// �E�C���h�E�̕\��(�����������̌�ɌĂ΂Ȃ��Ƒʖ�)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	// ���b�Z�[�W���[�v
	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��Ƒ��o
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)	// 1�b���ƂɎ��s
			{
#ifdef _DEBUG
				g_CountFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime;				// FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;							// �J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))	// 1/60�b���ƂɎ��s
			{
				dwExecLastTime = dwCurrentTime;	// ��������������ۑ�

#ifdef _DEBUG	// �f�o�b�O�ł̎�����FPS��\������
				wsprintf(g_DebugStr, WINDOW_NAME);
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " FPS:%d", g_CountFPS);
#endif

				Update();			// �X�V����
				Draw();				// �`�揈��

#ifdef _DEBUG	// �f�o�b�O�ł̎������\������
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " MX:%d MY:%d", GetMousePosX(), GetMousePosY());
				SetWindowText(hWnd, g_DebugStr);
#endif

				dwFrameCount++;
			}
		}
	}

	timeEndPeriod(1);				// ����\��߂�

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// �I������
	Uninit();

	return (int)msg.wParam;
}

//=============================================================================
// �v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	case WM_MOUSEMOVE:
		g_MouseX = LOWORD(lParam);
		g_MouseY = HIWORD(lParam);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
    InitSound(hWnd);

	InitRenderer(hInstance, hWnd, bWindow);
    InitStencilShadow();

	InitCamera();

	// ���͏����̏�����
	InitInput(hInstance, hWnd);

    InitFade();

    // �ŏ��̃��[�h���Z�b�g
    SetMode(g_Mode);	// ������SetMode�̂܂܂ŁI

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Uninit(void)
{
    // �I���̃��[�h���Z�b�g
    SetMode(MODE_MAX);

    UninitFade();

	//���͂̏I������
	UninitInput();

	UninitRenderer();

    UninitSound();
}

//=============================================================================
// �X�V����
//=============================================================================
void Update(void)
{
    // ���͂̍X�V����
	UpdateInput();

	// �J�����X�V
	UpdateCamera();

    // ���[�h�ɂ���ď����𕪂���
    switch (g_Mode)
    {
    case MODE_TITLE:		// �^�C�g����ʂ̍X�V
        UpdateTitle();
        break;

    case MODE_GAME:			// �Q�[����ʂ̍X�V
        UpdateGame();
        break;

    case MODE_RESULT:		// ���U���g��ʂ̍X�V
        UpdateResult();
        break;
    }

    UpdateFade();
}

//=============================================================================
// �`�揈��
//=============================================================================
void Draw(void)
{
	// �o�b�N�o�b�t�@�N���A
	Clear();

	SetCamera();

    // ���[�h�ɂ���ď����𕪂���
    switch (g_Mode)
    {
    case MODE_TITLE:		// �^�C�g����ʂ̕`��
        DrawTitle();
        break;

    case MODE_GAME:			// �Q�[����ʂ̕`��
        DrawGame();
        break;

    case MODE_RESULT:		// ���U���g��ʂ̕`��
        DrawResult();
        break;
    }

    DrawFade();

	// �o�b�N�o�b�t�@�A�t�����g�o�b�t�@����ւ�
	Present();
}


long GetMousePosX(void)
{
	return g_MouseX;
}


long GetMousePosY(void)
{
	return g_MouseY;
}


#ifdef _DEBUG
char* GetDebugStr(void)
{
	return g_DebugStr;
}
#endif

//=============================================================================
// BC�ɂ�铖���蔻�菈��
// �T�C�Y�͔��a
// �߂�l�F�������Ă���true
//=============================================================================
bool HitCheckBC(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float size1, float size2)
{
    float len = (size1 + size2) * (size1 + size2);	// ���a��2�悵����
    D3DXVECTOR3 temp = pos1 - pos2;
    float fLengthSq = D3DXVec3LengthSq(&temp);		// 2�_�Ԃ̋����i2�悵�����j

    if (len >= fLengthSq)
    {
        return true;
    }

    return false;
}

//=============================================================================
// BB�ɂ�铖���蔻�菈��
// ��]�͍l�����Ȃ�
// �߂�l�F�������Ă���true
//=============================================================================
bool HitCheckBB(D3DXVECTOR3 mpos, float mhw, float mhh, D3DXVECTOR3 ypos, float yhw, float yhh)
{
    bool ans = false;

    if ((mpos.x + mhw > ypos.x - yhw) &&
        (mpos.x - mhw < ypos.x + yhw) &&
        (mpos.y + mhh > ypos.y - yhh) &&
        (mpos.y - mhh < ypos.y + yhh))
    {
        ans = true;
    }
    return ans;
}

bool HitCheckBB3D(D3DXVECTOR3 mpos, float mx, float my, float mz, D3DXVECTOR3 ypos, float yx, float yy, float yz)
{
    bool ans = false;

    if ((mpos.x + mx / 2 > ypos.x - yx / 2) &&
        (mpos.x - mx / 2 < ypos.x + yy / 2) &&
        (mpos.z + mz / 2 > ypos.z - yz / 2) &&
        (mpos.z - mz / 2 < ypos.z + yz / 2) &&
        (mpos.y - my / 2 < ypos.y + yy / 2) &&
        (mpos.y + my / 2 > ypos.y - yy / 2))
    {
        ans = true;
    }
    return ans;
}

bool HitCheckOBB(BULLET* model_bullet, STENCIL_SHADOW* model_enemy)
{
    model_bullet->BBox.LengthX = model_bullet->fWidth / 2;
    model_bullet->BBox.LengthY = 1.0f;
    model_bullet->BBox.LengthX = model_bullet->fHeight / 2;

    LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
    VOID* pVertices = NULL;
    D3DXVECTOR3 Max, Min;
    UINT stride = sizeof(VERTEX_3D);

    // ���b�V�������_�ʒu�̍ő�ƍŏ�����������
    D3DXComputeBoundingBox((D3DXVECTOR3*)pVertices, model_enemy->model.VertexNum,
        stride, &Min, &Max);

    //���x�N�g�� ���̒����i���̏ꍇ�{�b�N�X�̊e���a�j������������
    model_enemy->BBox.LengthX = (Max.x - Min.x) / 2;
    model_enemy->BBox.LengthY = 0.1f;
    model_enemy->BBox.LengthX = (Max.z - Min.z) / 2;
         
    D3DXVECTOR3 distance = model_enemy->pos - model_bullet->pos;

    D3DXVECTOR3 separate;

    //���ꂼ��̃��[�J����ꎲ�x�N�g���ɁA���ꂼ��̉�]�𔽉f������
    model_bullet->BBox.AxisX = D3DXVECTOR3(1, 0, 0);
    model_bullet->BBox.AxisY = D3DXVECTOR3(0, 1, 0);
    model_bullet->BBox.AxisZ = D3DXVECTOR3(0, 0, 1);

    model_enemy->BBox.AxisX = D3DXVECTOR3(1, 0, 0);
    model_enemy->BBox.AxisY = D3DXVECTOR3(0, 1, 0);
    model_enemy->BBox.AxisZ = D3DXVECTOR3(0, 0, 1);

    D3DXVec3TransformCoord(&model_bullet->BBox.AxisX,&model_bullet->BBox.AxisX, &model_bullet->mtxWorld);                                             
    D3DXVec3TransformCoord(&model_bullet->BBox.AxisY,&model_bullet->BBox.AxisY, &model_bullet->mtxWorld);
    D3DXVec3TransformCoord(&model_bullet->BBox.AxisZ,&model_bullet->BBox.AxisZ, &model_bullet->mtxWorld);

    D3DXVec3TransformCoord(&model_enemy->BBox.AxisX, &model_enemy->BBox.AxisX, &model_enemy->mtxWorld);
    D3DXVec3TransformCoord(&model_enemy->BBox.AxisY, &model_enemy->BBox.AxisY, &model_enemy->mtxWorld);
    D3DXVec3TransformCoord(&model_enemy->BBox.AxisZ, &model_enemy->BBox.AxisZ, &model_enemy->mtxWorld);

    	//�{�b�N�XA�̃��[�J����ꎲ����ɂ����A�h�e�h�̎Z�o�i3�p�^�[���j
	{		
		//X���𕪗����Ƃ����ꍇ
		if(!CompareLength(&model_bullet->BBox,&model_enemy->BBox,&model_bullet->BBox.AxisX,&distance)) return false;
		//Y���𕪗����Ƃ����ꍇ
		if(!CompareLength(&model_bullet->BBox,&model_enemy->BBox,&model_bullet->BBox.AxisY,&distance)) return false;
		//Z���𕪗����Ƃ����ꍇ
		if(!CompareLength(&model_bullet->BBox,&model_enemy->BBox,&model_bullet->BBox.AxisZ,&distance)) return false;
	}
    {
        //X���𕪗����Ƃ����ꍇ
        if (!CompareLength(&model_bullet->BBox, &model_enemy->BBox, &model_enemy->BBox.AxisX, &distance)) return false;
        //Y���𕪗����Ƃ����ꍇ
        if (!CompareLength(&model_bullet->BBox, &model_enemy->BBox, &model_enemy->BBox.AxisY, &distance)) return false;
        //Z���𕪗����Ƃ����ꍇ
        if (!CompareLength(&model_bullet->BBox, &model_enemy->BBox, &model_enemy->BBox.AxisZ, &distance)) return false;
    }

    //���݂��̊�ꎲ���m�̊O�σx�N�g������ɂ����A�h�e�h�̎Z�o�i9�p�^�[���j
    {
        //�{�b�N�XA��X��
        {
            //�Ɓ@�{�b�N�XB��X���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
            D3DXVec3Cross(&separate, &model_bullet->BBox.AxisX, &model_enemy->BBox.AxisX);
            if (!CompareLength(&model_bullet->BBox, &model_enemy->BBox, &separate, &distance)) return false;
            //�Ɓ@�{�b�N�XB��Y���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
            D3DXVec3Cross(&separate, &model_bullet->BBox.AxisX, &model_enemy->BBox.AxisY);
            if (!CompareLength(&model_bullet->BBox, &model_enemy->BBox, &separate, &distance)) return false;
            //�Ɓ@�{�b�N�XB��Z���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
            D3DXVec3Cross(&separate, &model_bullet->BBox.AxisX, &model_enemy->BBox.AxisZ);
            if (!CompareLength(&model_bullet->BBox, &model_enemy->BBox, &separate, &distance)) return false;
        }
        //�{�b�N�XA��Y��
        {
            //�Ɓ@�{�b�N�XB��X���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
            D3DXVec3Cross(&separate, &model_bullet->BBox.AxisY, &model_enemy->BBox.AxisX);
            if (!CompareLength(&model_bullet->BBox, &model_enemy->BBox, &separate, &distance)) return false;
            //�Ɓ@�{�b�N�XB��Y���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
            D3DXVec3Cross(&separate, &model_bullet->BBox.AxisY, &model_enemy->BBox.AxisY);
            if (!CompareLength(&model_bullet->BBox, &model_enemy->BBox, &separate, &distance)) return false;
            //�Ɓ@�{�b�N�XB��Z���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
            D3DXVec3Cross(&separate, &model_bullet->BBox.AxisY, &model_enemy->BBox.AxisZ);
            if (!CompareLength(&model_bullet->BBox, &model_enemy->BBox, &separate, &distance)) return false;
        }
        //�{�b�N�XA��Z��
        {
            //�Ɓ@�{�b�N�XB��X���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
            D3DXVec3Cross(&separate, &model_bullet->BBox.AxisZ, &model_enemy->BBox.AxisX);
            if (!CompareLength(&model_bullet->BBox, &model_enemy->BBox, &separate, &distance)) return false;
            //�Ɓ@�{�b�N�XB��Y���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
            D3DXVec3Cross(&separate, &model_bullet->BBox.AxisZ, &model_enemy->BBox.AxisY);
            if (!CompareLength(&model_bullet->BBox, &model_enemy->BBox, &separate, &distance)) return false;
            //�Ɓ@�{�b�N�XB��Z���Ƃ̊O�σx�N�g���𕪗����Ƃ����ꍇ
            D3DXVec3Cross(&separate, &model_bullet->BBox.AxisZ, &model_enemy->BBox.AxisZ);
            if (!CompareLength(&model_bullet->BBox, &model_enemy->BBox, &separate, &distance)) return false;
        }
    }

    return true;

}
bool CompareLength(BBOX* box_a, BBOX* box_b, D3DXVECTOR3* separate, D3DXVECTOR3* distance)
{
    float f_distance = fabsf(D3DXVec3Dot(distance, separate));
    float f_shadow_a = 0;
    float f_shadow_b = 0;

    f_shadow_a = fabsf(D3DXVec3Dot(&box_a->AxisX, separate) * box_a->LengthX) +
                 fabsf(D3DXVec3Dot(&box_a->AxisY, separate) * box_a->LengthY) +
                 fabsf(D3DXVec3Dot(&box_a->AxisZ, separate) * box_a->LengthZ);

    f_shadow_b = fabsf(D3DXVec3Dot(&box_b->AxisX, separate) * box_b->LengthX) +
                 fabsf(D3DXVec3Dot(&box_b->AxisY, separate) * box_b->LengthY) +
                 fabsf(D3DXVec3Dot(&box_b->AxisZ, separate) * box_b->LengthZ);

    if (f_distance > f_shadow_a + f_shadow_b)
    {
        return false;
    }
    return true;
}
//=============================================================================
// �����蔻�菈��
//=============================================================================
void HitCheck(D3DXVECTOR3 light_position)
{
    PLAYER *player = GetPlayer();
    ENEMY  *enemy = GetEnemy();
    BULLET *bullet = GetBullet();

    // �v���C���[�ƓG�Ƃ̓����蔻��
    //for (int i = 0; i < MAX_PLAYER; i++)
    {
        if (player->use == true)// ����ł�����s��Ȃ�
        {

            for (int j = 0; j < MAX_ENEMY; j++)
            {
                if (enemy[j].use == false) continue;	// ����ł�����s��Ȃ�

                //�o�E���f�B���O�T�[�N��(BC)�ł̓����蔻����s��
                bool ans = HitCheckBC(player->pos, enemy[j].pos, player->radius, enemy[j].radius);
                if (ans == true)
                {
                    //���̓G�l�~�[�������Ă���
                    enemy[j].use = false;
                    ReleaseShadow(enemy[j].shadowIdx);
                    if (enemy[j].stop)
                    {
                        PlaySound(SOUND_LABEL_SE_lockon000);
                        AddScore(10);
                    }
                    else
                    {
                        PlaySound(SOUND_LABEL_SE_defend000);
                    }
                }

            }
        }
    }

    for (int i = 0; i < MAX_BULLET; i++)
    {
        if (!bullet[i].bUse)continue;
        for (int j = 0; j < MAX_ENEMY; j++)
        {
            if (!enemy[j].use) continue;

            if (HitCheckBC(bullet[i].pos, enemy[j].pos, bullet[i].radius, enemy[j].radius))
            {
                enemy[j].use = false;
                ReleaseShadow(enemy[j].shadowIdx);
                AddScore(1);
            }
        }
    }

    for (int i = 0; i < MAX_BULLET; i++)
    {
        if (!bullet[i].bUse)continue;
        for (int j = 0; j < MAX_ENEMY; j++)
        {
            if (!enemy[j].use || enemy[j].stop) continue;

            //if (HitCheckBB3D(bullet[i].pos, bullet[i].fWidth/3, 0.5f, bullet[i].fWidth/3, shadow[enemy[j].shadowIdx].pos - D3DXVECTOR3(light_position.x, 0.0f, light_position.z) * 50, shadow[enemy[j].shadowIdx].scl.x + light_position.x * 100, 0.2f, shadow[enemy[j].shadowIdx].scl.z + light_position.z * 100)) //�œK����             
            if(HitCheckOBB(&bullet[i], &g_shadow[enemy[j].shadowIdx]))
            {
                if (g_shadow[enemy[j].shadowIdx].bUse)
                {
                    PlaySound(SOUND_LABEL_SE_defend001);
                    enemy[j].stop = true;

                    ReleaseShadow(enemy[j].shadowIdx);
                }
            }
        }
    }
}



//=============================================================================
// ���[�h�̐ݒ�
//=============================================================================
void SetMode(MODE mode)
{
    // ���[�h��ς���O�ɑS������������������Ⴄ
    // �^�C�g����ʂ̏I������
    UninitTitle();

    // �Q�[����ʂ̏I������
    if (mode == MODE_GAME)
    {
        UninitGame();
    }

    // ���U���g��ʂ̏I������
    UninitResult();


    g_Mode = mode;

    switch (g_Mode)
    {
    case MODE_TITLE:
        // �^�C�g����ʂ̏�����
        InitTitle();
        break;

    case MODE_GAME:
        // �Q�[����ʂ̏�����
        InitGame();
        break;

    case MODE_RESULT:
        // ���U���g��ʂ̏�����
        InitResult();
        break;

        // �Q�[���I�����̏���
    case MODE_MAX:
        break;
    }
}

//=============================================================================
// ���[�h�̎擾
//=============================================================================
MODE GetMode(void)
{
    return g_Mode;
}
