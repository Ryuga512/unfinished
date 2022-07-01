//=============================================================================
//
// �J�������� [camera.cpp]
// Author : GP11A132 12 �F�J����
//
//=============================================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "player.h"
#include "StencilShadow.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	POS_X_CAM		(0.0f)			// �J�����̏����ʒu(X���W)
#define	POS_Y_CAM		(50.0f)			// �J�����̏����ʒu(Y���W)
#define	POS_Z_CAM		(-140.0f)		// �J�����̏����ʒu(Z���W)

#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							// �r���[���ʂ̎���p
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��	
#define	VIEW_NEAR_Z		(10.0f)											// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z		(10000.0f)										// �r���[���ʂ�FarZ�l

#define	VALUE_MOVE_CAMERA	(2.0f)										// �J�����̈ړ���
#define	VALUE_ROTATE_CAMERA	(D3DX_PI * 0.01f)							// �J�����̉�]��
#define ZOOM_CAMERA         (30.0f)                                     // �Y�[�����̃J����
#define TPS_CAMERA          (100.0f)
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static CAMERA			g_Camera;		// �J�����f�[�^
//�J�����͎p���s��ɕۑ��B���̃t���[���̃J�����̕ω��͂��̎��̃J�����̎p���s�񂩂�X�^�[�g
static D3DXMATRIX PoseMat(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
float Yaw = 0, Pitch = 0, Roll = 0;
//�t���[�����Ƃɑ������ށB�d�v�Ȃ̂́h�P���������h�������ނ���
D3DXMATRIX DeltaMat(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);//�p���̔����ω���

// �J�����̒����_�̐��`�ړ��p�̈ړ����W�e�[�u��
static D3DXVECTOR3	g_MoveTbl_at[] = {
    D3DXVECTOR3(0.0f, 7.0f, 0.0f),
    D3DXVECTOR3(-200.0f, 7.0f, 0.0f),
    D3DXVECTOR3(-200.0f, 7.0f, 200.0f),
};


// �J�����̒����_�̐��`�ړ��p�̈ړ��X�s�[�h�e�[�u��
static float	g_MoveSpd_at[] = {
    0.01f,
    0.1f,
    0.005f,
};

// �J�����̎��_�̐��`�ړ��p�̈ړ����W�e�[�u��
static D3DXVECTOR3	g_MoveTbl_pos[] = {
    D3DXVECTOR3(0.0f, 50.0f, 0.0f),
    D3DXVECTOR3(-200.0f, 50.0f, 50.0f),
    D3DXVECTOR3(-200.0f, 50.0f, 100.0f),
};


// �J�����̎��_�̐��`�ړ��p�̈ړ��X�s�[�h�e�[�u��
static float	g_MoveSpd_pos[] = {
    0.01f,
    0.1f,
    0.005f,
};


//=============================================================================
// ����������
//=============================================================================
void InitCamera(void)
{
    //�J�����̎��_
	g_Camera.pos = D3DXVECTOR3(POS_X_CAM, POS_Y_CAM, POS_Z_CAM);

    //�J�����̒����_
	g_Camera.at = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    //�����
	g_Camera.up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

    //��]�p�x
	g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//// ���_�ƒ����_�̋������v�Z
	//float vx, vz;
	//vx = g_Camera.pos.x - g_Camera.at.x;
	//vz = g_Camera.pos.z - g_Camera.at.z;
	g_Camera.len = TPS_CAMERA;

    // �C�x���g�J�����̃t���O��������
    g_Camera.event_camera = false;

    // �J�����̒����_�p�̎��Ԃ��N���A
    g_Camera.time_at = 0.0f;

    //�J�����̎��_�p�̎��Ԃ��N���A
    g_Camera.time_pos = 0.0f;
}


//=============================================================================
// �J�����̏I������
//=============================================================================
void UninitCamera(void)
{

}


//=============================================================================
// �J�����̍X�V����
//=============================================================================
void UpdateCamera(void)
{
    PLAYER *player = GetPlayer();


    ////Yaw����
    //if (GetKeyState(VK_LEFT) & 0x80)
    //{
    //    D3DXMatrixRotationAxis(&DeltaMat, &D3DXVECTOR3(PoseMat._21, PoseMat._22, PoseMat._23), -0.01);
    //}
    //else if (GetKeyState(VK_RIGHT) & 0x80)
    //{
    //    D3DXMatrixRotationAxis(&DeltaMat, &D3DXVECTOR3(PoseMat._21, PoseMat._22, PoseMat._23), 0.01);
    //}
    ////Pitch����
    //else if (GetKeyState(VK_UP) & 0x80)
    //{
    //    D3DXMatrixRotationAxis(&DeltaMat, &D3DXVECTOR3(PoseMat._11, PoseMat._12, PoseMat._13), -0.01);
    //}
    //else if (GetKeyState(VK_DOWN) & 0x80)
    //{
    //    D3DXMatrixRotationAxis(&DeltaMat, &D3DXVECTOR3(PoseMat._11, PoseMat._12, PoseMat._13), 0.01);
    //}
    ////Roll����
    //else if (GetKeyState('Z') & 0x80)
    //{
    //    D3DXMatrixRotationAxis(&DeltaMat, &D3DXVECTOR3(PoseMat._31, PoseMat._32, PoseMat._33), -0.01);
    //}
    //else if (GetKeyState('X') & 0x80)
    //{
    //    D3DXMatrixRotationAxis(&DeltaMat, &D3DXVECTOR3(PoseMat._31, PoseMat._32, PoseMat._33), 0.01);
    //}

    if (GetKeyboardPress(DIK_Q) || IsButtonPressed(0, BUTTON_A))
    {// �����_����u���v
        g_Camera.rot.y -= VALUE_ROTATE_CAMERA;
        if (g_Camera.rot.y < -D3DX_PI)
        {
            g_Camera.rot.y += D3DX_PI * 2.0f;
        }

        g_Camera.at.x = g_Camera.pos.x + sinf(g_Camera.rot.y) * g_Camera.len;
        g_Camera.at.z = g_Camera.pos.z + cosf(g_Camera.rot.y) * g_Camera.len;
    }

    if (GetKeyboardPress(DIK_E) || IsButtonPressed(0, BUTTON_C))
    {// �����_����u�E�v
        g_Camera.rot.y += VALUE_ROTATE_CAMERA;
        if (g_Camera.rot.y > D3DX_PI)
        {
            g_Camera.rot.y -= D3DX_PI * 2.0f;
        }

        g_Camera.at.x = g_Camera.pos.x + sinf(g_Camera.rot.y) * g_Camera.len;
        g_Camera.at.z = g_Camera.pos.z + cosf(g_Camera.rot.y) * g_Camera.len;
    }
    player->rot.y = g_Camera.rot.y;

    if (GetKeyboardPress(DIK_T) || IsButtonPressed(0, BUTTON_X))
    {// �����_�ړ��u��v
     // ����x�����g��
        if (g_Camera.rot.x < 1.0f)
        {
            g_Camera.rot.x += VALUE_ROTATE_CAMERA;
        }

    }

    if (GetKeyboardPress(DIK_B) || IsButtonPressed(0, BUTTON_B))
    {// �����_�ړ��u���v
     // ����x�����g��
        if (g_Camera.rot.x  > -1.0f)
        {
            g_Camera.rot.x -= VALUE_ROTATE_CAMERA;

        }
    }


    if (!g_Camera.event_camera)
    {
        //cam->at = g_Player.pos;
        //cam->pos = cam->at + D3DXVECTOR3(0.0f, 50.0f, -140.0f);
        //cam->pos.x = cam->at.x - sinf(cam->rot.y) * cam->len;
        //cam->pos.z = cam->at.z - cosf(cam->rot.y) * cam->len;
    }



    //�E�N���b�N�ŃY�[���A�����Ɩ߂�
    if (IsMouseRightPressed() || IsButtonPressed(0, BUTTON_L))
    {
        g_Camera.at = player->pos;
        g_Camera.at.y = player->pos.y + 10;
        g_Camera.len = ZOOM_CAMERA;        
        player->rot.y = g_Camera.rot.y + D3DX_PI;
        g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
        g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
        g_Camera.pos.y = player->pos.y + 10;
    }
    else
    {
        g_Camera.len = TPS_CAMERA;
        g_Camera.at = player->pos;
        g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
        g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
        g_Camera.pos.y = POS_Y_CAM;
    }

    g_Camera.at.y = g_Camera.pos.y + sinf(g_Camera.rot.x) * g_Camera.len;
    //if (GetKeyboardPress(DIK_M))
    //{// �����
    //    g_Camera.len += VALUE_MOVE_CAMERA;
    //    g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
    //    g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
    //}

    //// �J�����������ɖ߂�
    //if (GetKeyboardPress(DIK_R))
    //{
    //    UninitCamera();
    //    InitCamera();
    //}

    //if (GetKeyboardTrigger(DIK_9))
    //{
    //    g_Camera.event_camera = g_Camera.event_camera ? false : true;
    //}

    //if (g_Camera.event_camera)
    //{
    //    {
    //    // �G�l�~�[�O�Ԃ����e�[�u���ɏ]���č��W�ړ��i���`��ԁj
    //    int nowNo = (int)g_Camera.time_at;			// �������ł���e�[�u���ԍ������o���Ă���
    //    int maxNo = (sizeof(g_MoveTbl_at) / sizeof(D3DXVECTOR3));       // �o�^�e�[�u�����𐔂��Ă���
    //    int nextNo = (nowNo + 1) % maxNo;			// �ړ���e�[�u���̔ԍ������߂Ă���
    //    D3DXVECTOR3	pos = g_MoveTbl_at[nextNo] - g_MoveTbl_at[nowNo];   // XYZ�ړ��ʂ��v�Z���Ă���
    //    float nowTime = g_Camera.time_at - nowNo;	// ���ԕ����ł��鏭�������o���Ă���
    //    pos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���

    //    // �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
    //    g_Camera.at = g_MoveTbl_at[nowNo] + pos;

    //    g_Camera.time_at += g_MoveSpd_at[nowNo];	// ���Ԃ�i�߂Ă���
    //    if ((int)g_Camera.time_at >= maxNo)		    // �o�^�e�[�u���Ō�܂ňړ��������H
    //    {
    //        g_Camera.time_at -= maxNo;			    // �O�ԖڂɃ��Z�b�g�������������������p���ł���
    //    }
    //}

    //    {
    //    // �G�l�~�[�O�Ԃ����e�[�u���ɏ]���č��W�ړ��i���`��ԁj
    //    int nowNo = (int)g_Camera.time_pos;			// �������ł���e�[�u���ԍ������o���Ă���
    //    int maxNo = (sizeof(g_MoveTbl_pos) / sizeof(D3DXVECTOR3));       // �o�^�e�[�u�����𐔂��Ă���
    //    int nextNo = (nowNo + 1) % maxNo;			// �ړ���e�[�u���̔ԍ������߂Ă���
    //    D3DXVECTOR3	pos = g_MoveTbl_pos[nextNo] - g_MoveTbl_pos[nowNo];   // XYZ�ړ��ʂ��v�Z���Ă���
    //    float nowTime = g_Camera.time_pos - nowNo;	// ���ԕ����ł��鏭�������o���Ă���
    //    pos *= nowTime;								// ���݂̈ړ��ʂ��v�Z���Ă���

    //    // �v�Z���ċ��߂��ړ��ʂ����݂̈ړ��e�[�u��XYZ�ɑ����Ă��遁�\�����W�����߂Ă���
    //    g_Camera.pos = g_MoveTbl_pos[nowNo] + pos;

    //    g_Camera.time_pos += g_MoveSpd_pos[nowNo];	// ���Ԃ�i�߂Ă���
    //    if ((int)g_Camera.time_pos >= maxNo)		    // �o�^�e�[�u���Ō�܂ňړ��������H
    //    {
    //        g_Camera.time_pos -= maxNo;			    // �O�ԖڂɃ��Z�b�g�������������������p���ł���
    //    }
    //    }

    //}

}

void DrawCamera(void)
{
    return;
}
//=============================================================================
// �J�����̍X�V
//=============================================================================
void SetCamera(void) 
{
    ////�����ω��𑫂�����
    //PoseMat *= DeltaMat;

    //D3DXVECTOR3 CamDir;//�J��������������
    //CamDir = D3DXVECTOR3(0, 0, 1);//�ŏ���Z�������������Ƃ��납��X�^�[�g
    //D3DXVec3TransformCoord(&CamDir, &CamDir, &PoseMat);

    //// �r���[�g�����X�t�H�[���@���ꂪ�h�J�����h
    //D3DXVECTOR3 LookPos = g_Camera.pos + CamDir;//�����_�@���̓_�ƃJ�����ʒu����J�����̕��������肳���
    //D3DXVec3TransformCoord(&g_Camera.up, &g_Camera.up, &PoseMat);

	// �r���[�}�g���b�N�X�ݒ�
	D3DXMatrixLookAtLH(&g_Camera.mtxView, &g_Camera.pos, &g_Camera.at, &g_Camera.up);

    SetViewMatrix(&g_Camera.mtxView);

	float det;
	D3DXMatrixInverse(&g_Camera.mtxInvView, &det, &g_Camera.mtxView);


	// �v���W�F�N�V�����}�g���b�N�X�ݒ�
	D3DXMatrixPerspectiveFovLH(&g_Camera.mtxProjection, 1.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, VIEW_NEAR_Z, VIEW_FAR_Z);

	SetProjectionMatrix(&g_Camera.mtxProjection);
}


//=============================================================================
// �J�����̎擾
//=============================================================================
CAMERA *GetCamera(void) 
{
	return &g_Camera;
}

