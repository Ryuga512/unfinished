//=============================================================================
//
// �G�l�~�[���� [enemy.cpp]
// Author : GP11A132 12 �F�J����
//
//=============================================================================
#include "enemy.h"
#include "sprite.h"
#include "player.h"
#include <time.h>
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// (200)			// �L�����T�C�Y
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// (200)			// 
#define TEXTURE_MAX					(4)		// �e�N�X�`���̐�

#define TEXTURE_PATTERN_DIVIDE_X	(4)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(2)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			((TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y))	// �A�j���[�V�����p�^�[����
#define ANIM_WAIT					(8)    // �A�j���[�V�����̐؂�ւ��Wait�l

#define ENEMY_SPEED					(2.0f)	// �ړ����x

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void BossMotion(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[] = {	// Tex�t�@�C���̏ꏊ
	"data/TEXTURE/boss/other_default.png",
    "data/TEXTURE/boss/other_front_attack.png",
    "data/TEXTURE/boss/other_side_attack.png",
    "data/TEXTURE/boss/other_rolling_attack.png",
};

ENEMY enemy[ENEMY_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(void)
{
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


	// �G�l�~�[�̏�����
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		enemy[i].use         = true;
		enemy[i].w           = 400;
		enemy[i].h           = 400;
		enemy[i].rot         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		enemy[i].texNo       = OTHER_DEFAULT;

		enemy[i].countAnim   = 0;
		enemy[i].patternAnim = 0;		        // �A�j���̃p�^�[���ԍ�
		enemy[i].left        = true;			// ���ֈړ�������
        enemy[i].atk_delay   = 0;
        enemy[i].r           = 1.0f;
        enemy[i].g           = 1.0f;
        enemy[i].b           = 1.0f;
	}

    for (int i = 1; i < ENEMY_MAX; i++)
    {
        enemy[i].boss = false;
        enemy[i].hp   = 10.0f;
    }
    enemy[0].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5 - 120, 0.0f);
    enemy[0].boss = true;
    enemy[0].hp   = BOSS_HP_MAX;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
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
void UpdateEnemy(void)
{
    PLAYER *player = GetPlayer_p();
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (enemy[i].use == true)
		{		

			// �A�j���[�V���� (Wait�l) 
			enemy[i].countAnim++;
			if ((enemy[i].countAnim % ANIM_WAIT) == 0)
			{
				// �p�^�[���̐؂�ւ�
				enemy[i].patternAnim = (enemy[i].patternAnim + 1) % (ANIM_PATTERN_NUM);

                if (enemy[i].patternAnim == 0 && enemy[i].texNo > OTHER_DEFAULT)
                {
                    enemy[i].texNo = OTHER_DEFAULT;
                }
                else if (enemy[i].patternAnim == 0)
                {
                    BossMotion();
                }
			}

            if (enemy[i].texNo == OTHER_ROLLING)
            {
                if (enemy[i].p_pos.x > enemy[i].pos.x)
                {
                    enemy[i].pos.x += 1;
                }
                else if (enemy[i].p_pos.x < enemy[i].pos.x)
                {
                    enemy[i].pos.x -= 1;
                }

                if (enemy[i].p_pos.y > enemy[i].pos.y)
                {
                    enemy[i].pos.y += 1;
                }
                else if (enemy[i].p_pos.y < enemy[i].pos.y)
                {
                    enemy[i].pos.y -= 1;
                }
            }
		}
	}


#ifdef _DEBUG	// �f�o�b�O����\������
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);
	
#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
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

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (enemy[i].use == true)		                    // �v���C���[���g���Ă���H
		{						// Yes
			// �e�N�X�`���ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[enemy[i].texNo]);

			// �v���C���[�̈ʒu��e�N�X�`���[���W�𔽉f
			float px = enemy[i].pos.x;	                    // �v���C���[�̕\���ʒuX
			float py = enemy[i].pos.y;	                    // �v���C���[�̕\���ʒuY
			float pw = enemy[i].w;		                    // �v���C���[�̕\����
			float ph = enemy[i].h;		                    // �v���C���[�̕\������

			float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	                        // �e�N�X�`���̕�
			float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	                        // �e�N�X�`���̍���
			float tx = tw * enemy[i].patternAnim;	                            // �e�N�X�`���̍���X���W
			float ty = th * (enemy[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X);	// �e�N�X�`���̍���Y���W
            // �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
            SetSpriteColor(0, g_VertexBuffer, px, py, pw, ph,tx, ty, tw, th, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

            // �|���S���`��
			GetDeviceContext()->Draw(4, 0);                 
		}
	}
}


//=============================================================================
// �G�l�~�[�\���̂̃A�h���X���擾
//=============================================================================
ENEMY *GetEnemy_p(void)
{
	return &enemy[0];
}

void BossMotion(void)
{
    PLAYER *player = GetPlayer_p();
    srand((unsigned int)time(NULL));
    int r = rand() % 2;
    for (int i = 0; i < PLAYER_MAX; i++)
    {
        for (int j = 0; j < ENEMY_MAX; j++)
        {
            if (enemy[j].boss)
            {
                if (r == 0)
                {
                    enemy[i].p_pos = player[i].pos;
                    enemy[j].texNo = OTHER_ROLLING;
                }
                //���ʂ̋���300���炢�ɂ���Ƃ�
                else if (player[i].pos.x >= enemy[j].pos.x - enemy[j].w * 0.3f &&
                    player[i].pos.x <= enemy[j].pos.x + enemy[j].w * 0.3f &&
                    player[i].pos.y  - enemy[j].pos.y <= 300              &&
                    player[i].pos.y  - enemy[j].pos.y >= 0)
                {
                    enemy[j].texNo = OTHER_FRONT;
                }
                else if (player[i].pos.y >= enemy[j].pos.y - enemy[j].h * 0.5f &&
                         player[i].pos.y <= enemy[j].pos.y + enemy[j].h * 0.5f &&
                         player[i].pos.x  - enemy[j].pos.x <= 300              &&
                         player[i].pos.x  - enemy[j].pos.x >= 0)
                {
                    enemy[j].texNo = OTHER_SIDE;
                }
                else if (player[i].pos.y >= enemy[j].pos.y - enemy[j].h * 0.5f &&
                         player[i].pos.y <= enemy[j].pos.y + enemy[j].h * 0.5f &&
                         player[i].pos.x - enemy[j].pos.x >= -300 &&
                         player[i].pos.x - enemy[j].pos.x <= 0)
                {
                    enemy[j].texNo = OTHER_SIDE;
                }
                else
                {
                    enemy[j].texNo = OTHER_DEFAULT;
                }
            }
        }
    }
    return;
}
