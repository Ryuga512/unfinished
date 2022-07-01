//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : GP11A132 12 �F�J����
//
//=============================================================================
#include "player.h"
#include "sprite.h"
#include "input.h"
#include "attack.h"
#include "sound.h"
#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// (200)			// �L�����T�C�Y
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// (200)			// 
#define TEXTURE_MAX					(21)    // �e�N�X�`���̐�

#define TEXTURE_PATTERN_DIVIDE_X	(8)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define ANIM_WAIT					(8)		// �A�j���[�V�����̐؂�ւ��Wait�l

#define PLAYER_SPEED				(5.0f)	// �v���C���[�̈ړ����x
#define ATTACK_HEIGHT               (15)
#define ATTACK_WIDTH                (15)

#define STEP                        (100)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[] = {	// Tex�t�@�C���̏ꏊ
    //�`���[�g���A�����̃v���C���[
	"data/TEXTURE/tutorial_player/player_bdefault.png", 
    "data/TEXTURE/tutorial_player/player_fdefault.png", 
    "data/TEXTURE/tutorial_player/player_ldefault.png",
    "data/TEXTURE/tutorial_player/player_rdefault.png",
    "data/TEXTURE/tutorial_player/player_bwalk.png",
    "data/TEXTURE/tutorial_player/player_fwalk.png",
    "data/TEXTURE/tutorial_player/player_lwalk.png",
    "data/TEXTURE/tutorial_player/player_rwalk.png",
    "data/TEXTURE/tutorial_player/attack_side.png",

    //�Q�[�����̃v���C���[
    "data/TEXTURE/game_player/player_bdefault.png",
    "data/TEXTURE/game_player/player_fdefault.png",
    "data/TEXTURE/game_player/player_ldefault.png",
    "data/TEXTURE/game_player/player_rdefault.png",
    "data/TEXTURE/game_player/player_bwalk.png",
    "data/TEXTURE/game_player/player_fwalk.png",
    "data/TEXTURE/game_player/player_lwalk.png",
    "data/TEXTURE/game_player/player_rwalk.png",
    //�U��
    "data/TEXTURE/game_player/player_bdefault.png",
    "data/TEXTURE/game_player/player_fdefault.png",
    "data/TEXTURE/game_player/player_ldefault.png",
    "data/TEXTURE/game_player/player_rdefault.png",

};

PLAYER player[PLAYER_MAX];		// �v���C���[�z��
int mode;
//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
    mode = GetMode();

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


	// �v���C���[�̏�����
	for (int i = 0; i < PLAYER_MAX; i++)
	{
		player[i].use         = true;
		player[i].w           = 100;
		player[i].h           = 200;
		player[i].pos         = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT * 0.5 + 200, 0.0f);
		player[i].rot         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        if (mode == MODE_GAME)
        {
            player[i].texNo = 9;
            player[i].dir   = UP;
        }
        else if(mode == MODE_TUTORIAL)
        {
            player[i].texNo = 0;        
            player[i].dir   = DOWN;
        }
		player[i].countAnim   = 0;
		player[i].patternAnim = 0;		
        player[i].attack      = 100;
        player[i].hp          = 100;
        player[i].pre_pos     = D3DXVECTOR3(-100, 0, 0);
        player[i].pre_tx      = 0;
        player[i].pre_texNo   = 0;
        for (int j = 0; j < 4; j++)
        {
            player[i].hit[j]  = false;
        }
        player[i].atk_w       = 15;
        player[i].atk_h       = 15;
        player[i].atk_delay   = ATACK_DELAY;
        player[i].in_attack   = false;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
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
void UpdatePlayer(void)
{
	for (int i = 0; i < PLAYER_MAX; i++)
	{
        if (player[i].hp <= 0)
        {
            player[i].use = false;
        }
                // �v���C���[���g���Ă���H
		if (player[i].use == true)		    
		{						           
		        // �A�j���[�V���� (Wait�l) 
			player[i].countAnim++;
			if ((player[i].countAnim % ANIM_WAIT) == 0)
			{
                if (player[i].texNo == 8 && player[i].patternAnim == 7)
                {
                    player[i].hp -= 25;
                }

                // �p�^�[���̐؂�ւ�
                player[i].patternAnim = (player[i].patternAnim + 1) % ANIM_PATTERN_NUM;

                //�U����Ԃ̎��A64�t���[��(main�ɋL��)�҂��ăf�t�H���g��Ԃɂ���
                if (mode           == MODE_GAME&&
                   ((player[i].dir == UP       && player[i].texNo == 17) ||
                    (player[i].dir == DOWN     && player[i].texNo == 18) ||
                    (player[i].dir == LEFT     && player[i].texNo == 19) ||
                    (player[i].dir == RIGHT    && player[i].texNo == 20))&&
                     player[i].patternAnim == 0)
                {
                    switch (player[i].dir)
                    {
                    case UP:
                        player[i].texNo = 9;
                        break;
                    case DOWN:
                        player[i].texNo = 10;
                        break;
                    case LEFT:
                        player[i].texNo = 11;
                        break;
                    case RIGHT:
                        player[i].texNo = 12;
                        break;
                    }
                }
			}

            CollisionCheckPlayer(i);

            //�U���̎��ȊO�ړ��Ȃǂł���
            if (((player[i].dir    == UP    && player[i].texNo != 17) ||
                 (player[i].dir    == DOWN  && player[i].texNo != 18) ||
                 (player[i].dir    == LEFT  && player[i].texNo != 19) ||
                 (player[i].dir    == RIGHT && player[i].texNo != 20))&&
                  player[i].use    == true  && player[i].texNo != 8)
            {
                // �L�[���͂ňړ� 
                if (GetKeyboardPress(DIK_DOWN))
                {
                    player[i].dir   = DOWN;
                    if (!player[i].hit[DOWN])
                    {
                        player[i].pos.y += PLAYER_SPEED;
                        if (mode == MODE_GAME)
                        {
                            player[i].texNo = 14;
                        }
                        else
                        {
                            player[i].texNo = 5;
                        }
                    }
                }
                else if (GetKeyboardPress(DIK_UP))
                {
                    player[i].dir = UP;
                    if (!player[i].hit[UP])
                    {
                        player[i].pos.y -= PLAYER_SPEED;
                        if (mode == MODE_GAME)
                        {
                            player[i].texNo = 13;
                        }
                        else
                        {
                            player[i].texNo = 4;
                        }
                    }

                }
                else if (GetKeyboardPress(DIK_RIGHT))
                {
                    player[i].dir = RIGHT;
                    if (!player[i].hit[RIGHT])
                    {
                        player[i].pos.x += PLAYER_SPEED;

                        if (mode == MODE_GAME)
                        {
                            player[i].texNo = 16;
                        }
                        else
                        {
                            player[i].texNo = 7;
                        }
                    }
                }
                else if (GetKeyboardPress(DIK_LEFT))
                {
                    player[i].dir = LEFT;
                    if (!player[i].hit[LEFT])
                    {
                        player[i].pos.x -= PLAYER_SPEED;

                        if (mode == MODE_GAME)
                        {
                            player[i].texNo = 15;
                        }
                        else
                        {
                            player[i].texNo = 6;
                        }
                    }
                }
                else if (player[i].dir == DOWN)
                {
                    if (mode == MODE_GAME)
                    {
                        player[i].texNo = 10;
                    }
                    else
                    {
                        player[i].texNo = 1;
                    }
                }
                else if (player[i].dir == UP)
                {
                    if (mode == MODE_GAME)
                    {
                        player[i].texNo = 9;
                    }
                    else
                    {
                        player[i].texNo = 0;
                    }
                }
                else if (player[i].dir == RIGHT)
                {
                    if (mode == MODE_GAME)
                    {
                        player[i].texNo = 12;
                    }
                    else
                    {
                        player[i].texNo = 3;
                    }
                }
                else if (player[i].dir == LEFT)
                {
                    if (mode == MODE_GAME)
                    {
                        player[i].texNo = 11;
                    }
                    else
                    {
                        player[i].texNo = 2;
                    }
                }

                if (mode == MODE_GAME && GetKeyboardTrigger(DIK_SPACE))
                {
                    player[i].patternAnim = 0;

                    switch (player[i].dir)
                    {
                    case UP:
                        player[i].texNo = 17;
                        break;
                    case DOWN:
                        player[i].texNo = 18;
                        break;
                    case LEFT:
                        player[i].texNo = 19;
                        break;
                    case RIGHT:
                        player[i].texNo = 20;
                        break;
                    }
                }
                else if (mode == MODE_TUTORIAL && GetKeyboardTrigger(DIK_SPACE) &&
                    player[i].pos.x == 860 && player[i].pos.y >= 200 && player[i].pos.y <= 280)
                {
                    player[i].patternAnim = 0;
                    player[i].texNo = 8;
                }

                // �Q�[���p�b�h�ňړ�����
                if (IsButtonPressed(0, BUTTON_DOWN))
                {
                    player[i].pos.y += PLAYER_SPEED;
                }
                else if (IsButtonPressed(0, BUTTON_UP))
                {
                    player[i].pos.y -= PLAYER_SPEED;
                }

                if (IsButtonPressed(0, BUTTON_RIGHT))
                {
                    player[i].pos.x += PLAYER_SPEED;
                }
                else if (IsButtonPressed(0, BUTTON_LEFT))
                {
                    player[i].pos.x -= PLAYER_SPEED;
                }
            }

            //���ł������ł���ʒu
            if (mode == MODE_GAME && GetKeyboardTrigger(DIK_Z))
            {
                if (player[i].pre_pos.x == -100)
                {
                    player[i].pre_pos = player[i].pos;
                }
                else
                {
                    player[i].pos = player[i].pre_pos;
                    player[i].pre_pos = D3DXVECTOR3(-100, 0, 0);
                }

            }
		}

	}


#ifdef _DEBUG	// �f�o�b�O����\������
	char *str = GetDebugStr();
	sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", player[0].pos.x, player[0].pos.y);
	
#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
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

	for (int i = 0; i < PLAYER_MAX; i++)
	{
        if (player[i].use == true)		// �v���C���[���g���Ă���H
        {
            // �e�N�X�`���ݒ�
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[player[i].texNo]);
        }
        else
        {
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[3]);
        }
			// �v���C���[�̈ʒu��e�N�X�`���[���W�𔽉f
			float px = player[i].pos.x;					// �v���C���[�̕\���ʒuX
			float py = player[i].pos.y;					// �v���C���[�̕\���ʒuY
			float pw = player[i].w;						// �v���C���[�̕\����
			float ph = player[i].h;						// �v���C���[�̕\������

			float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// �e�N�X�`���̕�
			float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// �e�N�X�`���̍���
			float tx = tw * player[i].patternAnim;		// �e�N�X�`���̍���X���W
			float ty = 0.0f;							// �e�N�X�`���̍���Y���W
            float ro = 0.0f;
			// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
            SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
                                    D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ro);

			// �|���S���`��
			GetDeviceContext()->Draw(4, 0);

            if (player[i].pre_pos.x != -100)
            {
                GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[player[i].pre_texNo]);
                SetSpriteColorRotation(g_VertexBuffer, player[i].pre_pos.x, player[i].pre_pos.y, pw, ph, player[i].pre_tx, ty, tw, th,
                    D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), ro);
                GetDeviceContext()->Draw(4, 0);
            }
            else
            {
                player[i].pre_tx    = tx;
                player[i].pre_texNo = player[i].texNo;
            }
	}
}

//=============================================================================
// �v���C���[�̍��W���擾
//=============================================================================
PLAYER *GetPlayer_p(void)
{
	return &player[0];
}


