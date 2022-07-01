//=============================================================================
//
// ���f������ [player.cpp]
// Author : GP11A132 12 �F�J����
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "player.h"
#include "shadow.h"
#include "renderer.h"
#include "bullet.h"
#include "StencilShadow.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_PLAYER		"data/MODEL/miku_01.obj"		// �ǂݍ��ރ��f����
#define	MODEL_PLAYER_PARTS	"data/MODEL/miku_02.obj"		// �ǂݍ��ރ��f����

#define	VALUE_MOVE			(2.0f)							// �ړ���
#define VALUE_MOVE_ZOOM     (1.0f)                          // �Y�[�����̈ړ���
#define	VALUE_ROTATE		(D3DX_PI * 0.02f)				// ��]��

#define PLAYER_SHADOW_SIZE	(0.4f)							// �e�̑傫��

#define PLAYER_PARTS_MAX	(2)								// �v���C���[�̃p�[�c�̐�

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static PLAYER				g_Player;	// �v���C���[
static PLAYER				g_Parts[PLAYER_PARTS_MAX];		// �v���C���[�̃p�[�c�p
static DX11_MODEL           g_Shadow;
float roty = D3DX_PI;

// �v���C���[�̊K�w�A�j���[�V�����f�[�^
//static INTERPOLATION_DATA move_tbl[] = {	// pos, rot, scl, frame
//    { D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),         D3DXVECTOR3(1.0f, 1.0f, 1.0f), 120 },
//    { D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI / 2, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 240 },
//    { D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI / 2, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 120 },
//    { D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),	     D3DXVECTOR3(1.0f, 1.0f, 1.0f), 120 },
//
//};

static INTERPOLATION_DATA move_tbl[] = {	// pos, rot, scl, frame
    { D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),         D3DXVECTOR3(1.0f, 1.0f, 1.0f), 120 },
    { D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXVECTOR3(D3DX_PI / 2, 0.0f, 0.0f),  D3DXVECTOR3(1.0f, 1.0f, 1.0f), 120 },
    { D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),         D3DXVECTOR3(1.0f, 1.0f, 1.0f), 120 },

};

//run_tbl

//�E�E�E

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	LoadModel(MODEL_PLAYER, &g_Player.model);
    InitStencilShadow(MODEL_PLAYER, &g_Shadow);
	g_Player.pos = D3DXVECTOR3(0.0f, 7.0f, -50.0f);
	g_Player.rot = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);
	g_Player.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
    g_Player.axis_x = D3DXVECTOR3(1, 0, 0);
    g_Player.axis_y = D3DXVECTOR3(0, 1, 0);
    g_Player.axis_y = D3DXVECTOR3(0, 0, 1);
	g_Player.spd = 0.0f;		// �ړ��X�s�[�h�N���A

	D3DXVECTOR3 pos = g_Player.pos;
	pos.y = 0.0f;
	g_Player.shadowIdx = CreateShadow(pos, PLAYER_SHADOW_SIZE, PLAYER_SHADOW_SIZE);

    g_Player.use = true;

    g_Player.radius = 3.5;

//    // �K�w�A�j���[�V�����̏�����
//    for (int i = 0; i < PLAYER_PARTS_MAX; i++)
//    {
//        g_Parts[i].use = true;
//
//        // �ʒu�E��]�E�X�P�[���̏����ݒ�
//        g_Parts[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//        g_Parts[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//        g_Parts[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
//
//        // �e�q�֌W
//        g_Parts[i].parent = &g_Player;	// �� �����ɐe�̃A�h���X������
////      g_Parts[�r].parent= &g_Player;  // �r��������e�͖{�́i�v���C���[�j 
//
//        // �K�w�A�j���[�V�����p�̃����o�[�ϐ��̏�����
//        g_Parts[i].tbl_adr = move_tbl;
//        g_Parts[i].move_time = 0.0f;
//
//        // �p�[�c�̓ǂݍ���
//        LoadModel(MODEL_PLAYER_PARTS, &g_Parts[i].model);
//    }
//    g_Parts[0].parent = &g_Player;	    // ����0��
//    g_Parts[1].parent = &g_Parts[0];	// ���̏��1��


	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	// ���f���̉������
	UnloadModel(&g_Player.model);
    UnloadStencilShadow(&g_Shadow);
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	CAMERA *cam = GetCamera();

    

	g_Player.spd *= 0.0f;
    if (!IsMouseRightPressed())
    {
        // �ړ�����
        if (GetKeyboardPress(DIK_A) || IsButtonPressed(0, BUTTON_LEFT))
        {
            g_Player.spd = VALUE_MOVE;
            roty = D3DX_PI / 2.0f;
        }

        else if (GetKeyboardPress(DIK_D) || IsButtonPressed(0, BUTTON_RIGHT))
        {
            g_Player.spd = VALUE_MOVE;
            roty = D3DX_PI * 1.5f;
        }
        else if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_UP))
        {
            g_Player.spd = VALUE_MOVE;
            roty = D3DX_PI;
        }
        else if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN))
        {
            g_Player.spd = VALUE_MOVE;
            roty = 0.0f;
        }
        if (GetKeyboardPress(DIK_W) && GetKeyboardPress(DIK_A) || IsButtonPressed(0, BUTTON_UP) && IsButtonPressed(0, BUTTON_LEFT))
        {
            g_Player.spd = VALUE_MOVE;
            roty = D3DX_PI - D3DX_PI * 0.25f;
        }
        if (GetKeyboardPress(DIK_W) && GetKeyboardPress(DIK_D) || IsButtonPressed(0, BUTTON_UP) && IsButtonPressed(0, BUTTON_RIGHT))
        {
            g_Player.spd = VALUE_MOVE;
            roty = D3DX_PI + D3DX_PI * 0.25f;
        }
        if (GetKeyboardPress(DIK_S) && GetKeyboardPress(DIK_A) || IsButtonPressed(0, BUTTON_DOWN) && IsButtonPressed(0, BUTTON_LEFT))
        {
            g_Player.spd = VALUE_MOVE;
            roty = D3DX_PI * 0.25f;
        }
        if (GetKeyboardPress(DIK_S) && GetKeyboardPress(DIK_D) || IsButtonPressed(0, BUTTON_DOWN) && IsButtonPressed(0, BUTTON_RIGHT))
        {
            g_Player.spd = VALUE_MOVE;
            roty = - D3DX_PI * 0.25f;
        }


        g_Player.rot.y = roty + cam->rot.y;
        //�ړ�����
        g_Player.pos.x -= sinf(g_Player.rot.y) * g_Player.spd;
        g_Player.pos.z -= cosf(g_Player.rot.y) * g_Player.spd;

    }
    else
    {
        // �ړ�����
        if (GetKeyboardPress(DIK_A) || IsButtonPressed(0, BUTTON_LEFT))
        {
            g_Player.spd = VALUE_MOVE_ZOOM;
            g_Player.pos -= g_Player.axis_x * g_Player.spd;

        }
        if (GetKeyboardPress(DIK_D) || IsButtonPressed(0, BUTTON_RIGHT))
        {
            g_Player.spd = VALUE_MOVE_ZOOM;
            g_Player.pos += g_Player.axis_x * g_Player.spd;

        }
        if (GetKeyboardPress(DIK_W) || IsButtonPressed(0, BUTTON_UP))
        {
            g_Player.spd = VALUE_MOVE_ZOOM;
            g_Player.pos += g_Player.axis_z * g_Player.spd;

        }
        if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN))
        {
            g_Player.spd = VALUE_MOVE_ZOOM;
            g_Player.pos -= g_Player.axis_z * g_Player.spd;

        }

    }


	if (IsMouseRightPressed() && GetKeyboardTrigger(DIK_SPACE) || IsButtonTriggered(0, BUTTON_R))
	{
        SetBullet(g_Player.pos, D3DXVECTOR3(cam->rot.x, cam->rot.y, cam->rot.z));
	}

    //if (GetKeyboardPress(DIK_R))
    //{
    //    g_Player.pos.z = g_Player.pos.x = 0.0f;
    //    g_Player.spd = 0.0f;
    //}



	// �e���v���C���[�̈ʒu�ɍ��킹��
	//D3DXVECTOR3 pos = g_Player.pos;
	//pos.y = 0.0f;
	//SetPositionShadow(g_Player.shadowIdx, pos);

    // �K�w�A�j���[�V����
    for (int i = 0; i < PLAYER_PARTS_MAX; i++)
    {
        // �g���Ă���Ȃ珈������
        if (g_Parts[i].use == true)
        {
            // �ړ�����
            int		index = (int)g_Parts[i].move_time;
            float	time = g_Parts[i].move_time - index;
            int		size = sizeof(move_tbl) / sizeof(INTERPOLATION_DATA);

            float dt = 1.0f / g_Parts[i].tbl_adr[index].frame;	// 1�t���[���Ői�߂鎞��
            g_Parts[i].move_time += dt;					// �A�j���[�V�����̍��v���Ԃɑ���

            if (index > (size - 2))	// �S�[�����I�[�o�[���Ă�����A�S�[���֖߂�
            {
                g_Parts[i].move_time = 0.0f;
                index = 0;
            }

            // ���W�����߂�	X = StartX + (EndX - StartX) * ���̎���
            D3DXVECTOR3 vec = g_Parts[i].tbl_adr[index + 1].pos - g_Parts[i].tbl_adr[index].pos;
            g_Parts[i].pos = g_Parts[i].tbl_adr[index].pos + vec * time;

            // ��]�����߂�	R = StartX + (EndX - StartX) * ���̎���
            D3DXVECTOR3 rot = g_Parts[i].tbl_adr[index + 1].rot - g_Parts[i].tbl_adr[index].rot;
            g_Parts[i].rot = g_Parts[i].tbl_adr[index].rot + rot * time;

            // scale�����߂� S = StartX + (EndX - StartX) * ���̎���
            D3DXVECTOR3 scl = g_Parts[i].tbl_adr[index + 1].scl - g_Parts[i].tbl_adr[index].scl;
            g_Parts[i].scl = g_Parts[i].tbl_adr[index].scl + scl * time;


        }
    }


}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	// �J�����O����
	SetCullingMode(CULL_MODE_NONE);

    D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Player.mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, g_Player.scl.x, g_Player.scl.y, g_Player.scl.z);
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxScl);

	// ��]�𔽉f
    D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player.rot.y + D3DX_PI, g_Player.rot.x, g_Player.rot.z);

    //���[�J�����W��p����]
    D3DXVec3TransformCoord(&g_Player.axis_x, &D3DXVECTOR3(1, 0, 0), &mtxRot);
    D3DXVec3TransformCoord(&g_Player.axis_y, &D3DXVECTOR3(0, 1, 0), &mtxRot);
    D3DXVec3TransformCoord(&g_Player.axis_z, &D3DXVECTOR3(0, 0, 1), &mtxRot);
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxTranslate);


	// ���[���h�}�g���b�N�X�̐ݒ�
	SetWorldMatrix(&g_Player.mtxWorld);

	// ���f���`��
	DrawModel(&g_Player.model);

    LIGHT *Light = GetLight();

    //�e�`��
    CreateStencilShadow(g_Player.mtxWorld, *Light, &g_Shadow);
    DrawStencilShadow(&g_Shadow, g_Player.mtxWorld);

    // �K�w�A�j���[�V����
    for (int i = 0; i < PLAYER_PARTS_MAX; i++)
    {
        // �g���Ă���Ȃ珈������
        if (g_Parts[i].use == false) continue;

        // ���[���h�}�g���b�N�X�̏�����
        D3DXMatrixIdentity(&g_Parts[i].mtxWorld);

        // �X�P�[���𔽉f
        D3DXMatrixScaling(&mtxScl, g_Parts[i].scl.x, g_Parts[i].scl.y, g_Parts[i].scl.z);
        D3DXMatrixMultiply(&g_Parts[i].mtxWorld, &g_Parts[i].mtxWorld, &mtxScl);

        // ��]�𔽉f
        D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Parts[i].rot.y, g_Parts[i].rot.x, g_Parts[i].rot.z);
        D3DXMatrixMultiply(&g_Parts[i].mtxWorld, &g_Parts[i].mtxWorld, &mtxRot);

        // �ړ��𔽉f
        D3DXMatrixTranslation(&mtxTranslate, g_Parts[i].pos.x, g_Parts[i].pos.y, g_Parts[i].pos.z);
        D3DXMatrixMultiply(&g_Parts[i].mtxWorld, &g_Parts[i].mtxWorld, &mtxTranslate);

        if (g_Parts[i].parent != NULL)	// �q����������e�ƌ�������
        {
            D3DXMatrixMultiply(&g_Parts[i].mtxWorld, &g_Parts[i].mtxWorld, &g_Parts[i].parent->mtxWorld);
        }                                                                              //��g_Player.mtxWorld���w���Ă���                         

        // ���[���h�}�g���b�N�X�̐ݒ�
        SetWorldMatrix(&g_Parts[i].mtxWorld);

        // ���f���`��
        DrawModel(&g_Parts[i].model);

    }

    // �J�����O�ݒ��߂�
    SetCullingMode(CULL_MODE_BACK);

}


//=============================================================================
// �v���C���[�����擾
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_Player;
}

