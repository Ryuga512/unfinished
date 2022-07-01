//=============================================================================
//
// �G�l�~�[���� [effect.cpp]
// Author : GP11A132 12 �F�J����
//
//=============================================================================
#include "enemy.h"
#include "sprite.h"
#include "effect.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// (200)			// �L�����T�C�Y
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// (200)			// 
#define TEXTURE_MAX					(EFFECT_MAX)		// �e�N�X�`���̐�

#define TEXTURE_PATTERN_DIVIDE_X	(5)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(2)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define ANIM_WAIT					(2)		// �A�j���[�V�����̐؂�ւ��Wait�l

#define effect_SPEED					(2.0f)	// �ړ����x


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[] = {	// Tex�t�@�C���̏ꏊ
    "data/TEXTURE/effect/jaki.png",		
};

EFFECT effect[EFFECT_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEffect(void)
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
    for (int i = 0; i < EFFECT_MAX; i++)
    {
        effect[i].use         = true;
        effect[i].w           = 200;
        effect[i].h           = 200;
        effect[i].pos         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        effect[i].rot         = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        effect[i].texNo       = i;			  // �ǂ݂��񂾉摜�̏��Ԃŕ\������
        effect[i].countAnim   = 0;
        effect[i].patternAnim = 0;		      // �A�j���̃p�^�[���ԍ�
        effect[i].alpha       = 0.0f;
    }

    return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEffect(void)
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
void UpdateEffect(D3DXVECTOR3 PosArg, int i, bool ans)
{
        effect[i].use = ans;
        if (effect[i].use == true)
        {						// Yes
                // �A�j���[�V���� (Wait�l) 
            effect[i].countAnim++;
            if ((effect[i].countAnim % ANIM_WAIT) == 0)
            {
                // �p�^�[���̐؂�ւ�
                effect[i].patternAnim = (effect[i].patternAnim + 1) % ANIM_PATTERN_NUM;
            }
            effect[i].pos   = PosArg;
            if (effect[i].countAnim == 64)
            {
                effect[i].alpha = 0.0f;
                effect[i].countAnim = 0;
                effect[i].patternAnim = 0;
            }
            else
            {
                effect[i].alpha = 1.0f;
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
void DrawEffect(void)
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

    for (int i = 0; i < EFFECT_MAX; i++)
    {
        if (effect[i].use == true)		// �v���C���[���g���Ă���H
        {						// Yes
            // �e�N�X�`���ݒ�
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[effect[i].texNo]);

            // �v���C���[�̈ʒu��e�N�X�`���[���W�𔽉f
            float px = effect[i].pos.x;    	// �v���C���[�̕\���ʒuX
            float py = effect[i].pos.y;    	// �v���C���[�̕\���ʒuY
            float pw = effect[i].w;		// �v���C���[�̕\����
            float ph = effect[i].h;		// �v���C���[�̕\������

            float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// �e�N�X�`���̕�
            float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// �e�N�X�`���̍���
            float tx = tw * effect[i].patternAnim;	// �e�N�X�`���̍���X���W
            float ty = th * (effect[i].patternAnim / TEXTURE_PATTERN_DIVIDE_X);	// �e�N�X�`���̍���Y���W
            float ro = 0.0f;
            // �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
            SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th,
                D3DXCOLOR(1.0f, 1.0f, 1.0f, effect[i].alpha), ro);


            // �|���S���`��
            GetDeviceContext()->Draw(4, 0);
        }
    }
}


//=============================================================================
// �G�l�~�[�\���̂̃A�h���X���擾
//=============================================================================
EFFECT *GetEffect_p(void)
{
    return &effect[0];
}


