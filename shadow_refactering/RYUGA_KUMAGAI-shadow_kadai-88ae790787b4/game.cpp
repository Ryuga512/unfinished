//=============================================================================
//
// �Q�[����ʏ��� [game.cpp]
// Author : 
//
//=============================================================================
#include "game.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"

#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "score.h"
#include "sound.h"
#include "meshfield.h"
#include "meshwall.h"
#include "shadow.h"
#include "tree.h"
#include "particle.h"
#include "StencilShadow.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
float x = 0.0f; //���C�g�̍��W
float y = 0.0f;
float z = 0.0f;
bool flg = true; //�����Ƃ�true
int time = 0;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitGame(void)
{
    time = 0;
    x = 0;
    y = 0;
    z = 0;
    InitStencilShadow();

    InitScore();

    //�e�̏�����
    InitBullet();

    //�e�̏�����
    InitShadow();

    // �v���C���[�̏�����
    InitPlayer();

    // �G�l�~�[�̏�����
    InitEnemy();

    InitTree();

    //InitParticle();

    // �t�B�[���h�̏�����
    InitMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 16, 16, 80.0f, 80.0f);

    // �ǂ̏�����
    InitMeshWall(D3DXVECTOR3(0.0f, 0.0f, WALL_UP), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
        D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
    InitMeshWall(D3DXVECTOR3(WALL_LEFT, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
        D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
    InitMeshWall(D3DXVECTOR3(WALL_RIGHT, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
        D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);
    InitMeshWall(D3DXVECTOR3(0.0f, 0.0f, WALL_DOWN), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
        D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 16, 2, 80.0f, 80.0f);

    // ��(�����p�̔�����)
    InitMeshWall(D3DXVECTOR3(0.0f, 0.0f, 640.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
        D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
    InitMeshWall(D3DXVECTOR3(-640.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.50f, 0.0f),
        D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
    InitMeshWall(D3DXVECTOR3(640.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.50f, 0.0f),
        D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);
    InitMeshWall(D3DXVECTOR3(0.0f, 0.0f, -640.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
        D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.25f), 16, 2, 80.0f, 80.0f);

    //�n��
    //InitMeshWall(D3DXVECTOR3(0.0f, 0.0f, -640.0f), D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f),
     //   D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 16, 16, 80.0f, 80.0f);


    // ���C�g��L����
    SetLightEnable(true);

    // �w�ʃ|���S�����J�����O
    SetCullingMode(CULL_MODE_BACK);

    return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGame(void)
{
    // �G�l�~�[�̏I������
    UninitEnemy();

    UninitScore();

    // �n�ʂ̏I������
    UninitMeshField();

    // �ǂ̏I������
    UninitMeshWall();

    //UninitParticle();

    UninitTree();

    UninitBullet();

    // �v���C���[�̏I������
    UninitPlayer();

    // �e�̏I������
    UninitShadow();

    // �J�����̏I������
    UninitCamera();
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGame(void)
{
    time++;
    if (x >= 80)
    {
        flg = false;
    }
    else if (x <= -80)
    {
        flg = true;
    }

    if (flg)
    {
        x += 0.001f;
        y += 0.001f;
        z += 0.001f;
    }
    else
    {
        x -= 0.001f;
        //y -= 0.001f;
        z -= 0.001f;
    }

    UpdateScore();

    // �G�l�~�[�̍X�V����
    UpdateEnemy();

    // �n�ʏ����̍X�V
    UpdateMeshField();

    // �Ǐ����̍X�V
    UpdateMeshWall();

    UpdateTree();

    UpdateBullet();

    // �e�̍X�V����
    UpdateShadow();

    UpdateRenderer(x, y, z);

    // �v���C���[�̍X�V����
    UpdatePlayer();


    HitCheck();

    if (time / 60 == 30)
    {
        SetMode(MODE_RESULT);
    }
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGame(void)
{
    // �n�ʂ̕`�揈��
    DrawMeshField();

    DrawBullet();


    // �ǂ̕`�揈��
    DrawMeshWall();

    DrawTree();
    DrawShadow();
    // �v���C���[�̕`�揈��
    DrawPlayer();

    // �G�l�~�[�̕`�揈��
    DrawEnemy();

    polygon();

    //DrawParticle();

    //2D�̕`�揈��������i���C�e�B���O�𖳌��j
    SetLightEnable(false);

    //Z��r�Ȃ�
    SetDepthEnable(false);

    //���̊Ԃ�2D�̏���������
    DrawScore();

    //
    SetLightEnable(true);

    SetDepthEnable(true);

}
