//=============================================================================
//
// �G�l�~�[���� [hp.cpp]
// Author : GP11A132 12 �F�J����
//
//=============================================================================
#include "hp.h"
#include "sprite.h"
#include "game.h"
#include "enemy.h"
#include "player.h"
#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// (200)			// �L�����T�C�Y
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// (200)			// 
#define TEXTURE_MAX					(HP_MAX)		// �e�N�X�`���̐�

#define TEXTURE_PATTERN_DIVIDE_X	(1)		    // �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		    // �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define ANIM_WAIT					(4)		    // �A�j���[�V�����̐؂�ւ��Wait�l



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[] = {	// Tex�t�@�C���̏ꏊ
    "data/TEXTURE/fade_black.png",
    "data/TEXTURE/HP.png",
    "data/TEXTURE/blood.png",
};

HP hp[HP_MAX];
int m;
//=============================================================================
// ����������
//=============================================================================
HRESULT InitHp(void)
{
    m = GetMode();
    ID3D11Device *pDevice = GetDevice();
    //�e�N�X�`������
    for (int i = 0; i < TEXTURE_MAX; i++)
    {
        g_Texture[i] = NULL;
        D3DX11CreateShaderResourceViewFromFile(
            GetDevice(),
            g_TexturName[i],
            NULL,
            NULL,
            &g_Texture[i],
            NULL);
    }


    // ���_�o�b�t�@����
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(VERTEX_3D) * 4;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


    // �I�u�W�F�N�g�̏�����
    for (int i = 0; i < HP_MAX; i++)
    {
        hp[i].use = true;

        hp[i].texNo = i;				// �ǂ݂��񂾉摜�̏��Ԃŕ\������

        hp[i].countAnim = 0;
        hp[i].patternAnim = 0;		    // �A�j���̃p�^�[���ԍ�
    }

    if (m == MODE_TUTORIAL)
    {
        hp[0].use = false;
        hp[1].use = false;
        hp[2].use = true;
    }

    hp[0].w   = SCREEN_WIDTH - 200;
    hp[0].h   = 20;
    hp[0].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 15, 0.0f);
    hp[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


    hp[1].w   = SCREEN_WIDTH - 200;
    hp[1].h   = 10;
    hp[1].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 15, 0.0f);
    hp[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    hp[2].w   = SCREEN_WIDTH;
    hp[2].h   = SCREEN_HEIGHT;
    hp[2].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
    hp[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitHp(void)
{
    // ���_�������
    if (g_VertexBuffer)
    {
        g_VertexBuffer->Release();
        g_VertexBuffer = NULL;
    }

    // �e�N�X�`���[�������
    for (int i = 0; i < TEXTURE_MAX; i++)
    {
        if (g_Texture[i])
        {
            g_Texture[i]->Release();
            g_Texture[i] = NULL;
        }
    }
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateHp(void)
{
    BossHpBar();

#ifdef _DEBUG	// �f�o�b�O����\������
    //char *str = GetDebugStr();
    //sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawHp(void)
{
    // ���_�o�b�t�@�ݒ�
    UINT stride = sizeof(VERTEX_3D);
    UINT offset = 0;
    GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

    // �}�g���N�X�ݒ�
    SetWorldViewProjection2D();

    // �v���~�e�B�u�g�|���W�ݒ�
    GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    // �}�e���A���ݒ�i�ގ����ATex���j
    MATERIAL material;
    ZeroMemory(&material, sizeof(material));
    material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    SetMaterial(material);

    for (int i = 0; i < HP_MAX; i++)
    {
        if (hp[i].use == true)		// �I�u�W�F�N�g���g���Ă���H
        {						// Yes
            // �e�N�X�`���ݒ�
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[hp[i].texNo]);

            // �I�u�W�F�N�g�̈ʒu��e�N�X�`���[���W�𔽉f
            float px = hp[i].pos.x;	// hp�̕\���ʒuX
            float py = hp[i].pos.y;	// hp�̕\���ʒuY
            float pw = hp[i].w;		// hp�̕\����
            float ph = hp[i].h;		// hp�̕\������
            float ro = hp[i].rot.z;

            float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// �e�N�X�`���̕�
            float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// �e�N�X�`���̍���
            float tx = tw * hp[i].patternAnim;	        // �e�N�X�`���̍���X���W
            float ty = 0.0f;	                        // �e�N�X�`���̍���Y���W

            if (i < 2)
            {
                // �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
                SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ro);
            }
            else
            {
                PLAYER *player = GetPlayer_p();
                SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th, D3DXCOLOR(1.0f, 1.0f, 1.0f, (100 - player[0].hp) / 100.0f), ro);
            }
            // �|���S���`��
            GetDeviceContext()->Draw(4, 0);
        }
    }
}


//=============================================================================
// �I�u�W�F�N�g�\���̂̃A�h���X���擾
//=============================================================================
HP *GetHp_p(void)
{
    return &hp[0];
}


void BossHpBar(void)
{
    ENEMY *enemy = GetEnemy_p();

    for (int i = 0; i < ENEMY_MAX; i++)
    {
        if (enemy[i].boss)
        {
            float width  = hp[0].w * enemy[i].hp / BOSS_HP_MAX;
            hp[1].w      = width;
            hp[1].pos.x  = hp[0].pos.x - (BOSS_HP_MAX - enemy[i].hp) * 0.76f * (500.0f / BOSS_HP_MAX);
        }
    }
}

