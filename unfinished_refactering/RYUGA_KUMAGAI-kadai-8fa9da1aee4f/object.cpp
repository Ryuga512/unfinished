//=============================================================================
//
// �G�l�~�[���� [object.cpp]
// Author : GP11A132 12 �F�J����
//
//=============================================================================
#include "object.h"
#include "sprite.h"
#include "game.h"
#include "main.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// (200)			// �L�����T�C�Y
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// (200)			// 
#define TEXTURE_MAX					(OBJECT_MAX)		// �e�N�X�`���̐�

#define TEXTURE_PATTERN_DIVIDE_X	(1)		// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define TEXTURE_PATTERN_DIVIDE_Y	(1)		// �A�j���p�^�[���̃e�N�X�`�����������iY)
#define ANIM_PATTERN_NUM			(TEXTURE_PATTERN_DIVIDE_X*TEXTURE_PATTERN_DIVIDE_Y)	// �A�j���[�V�����p�^�[����
#define ANIM_WAIT					(4)		// �A�j���[�V�����̐؂�ւ��Wait�l

#define object_SPEED			    (2.0f)	// �ړ����x


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[] = {	// Tex�t�@�C���̏ꏊ
    "data/TEXTURE/wall/saike.jpeg",
    "data/TEXTURE/wall/saike.jpeg",
    "data/TEXTURE/wall/saike.jpeg",
    "data/TEXTURE/wall/saike.jpeg",

    "data/TEXTURE/wall/kirtain.png",
    "data/TEXTURE/wall/kirtain.png",
    "data/TEXTURE/wall/kirtain.png",
    "data/TEXTURE/wall/kirtain.png",

    "data/TEXTURE/yubi.png",

};

OBJECT object[OBJECT_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitObject(void)
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


    // �I�u�W�F�N�g�̏�����
    for (int i = 0; i < OBJECT_MAX; i++)
    {
        object[i].use = true;
        object[i].texNo = i;				// �ǂ݂��񂾉摜�̏��Ԃŕ\������

        object[i].countAnim = 0;
        object[i].patternAnim = 0;		    // �A�j���̃p�^�[���ԍ�

    }

    object[0].w   = 100;
    object[0].h   = SCREEN_WIDTH;
    object[0].pos = D3DXVECTOR3(0.0f,SCREEN_HEIGHT * 0.5, 0.0f);
    object[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    object[4].w   = 100;
    object[4].h   = SCREEN_WIDTH;
    object[4].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT * 0.5, 0.0f);
    object[4].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);



    object[1].w   = 100;
    object[1].h   = SCREEN_WIDTH;
    object[1].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT * 0.5, 0.0f);
    object[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    object[5].w   = 100;
    object[5].h   = SCREEN_WIDTH;
    object[5].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT * 0.5, 0.0f);
    object[5].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);



    object[2].w   = SCREEN_WIDTH;
    object[2].h   = 100;
    object[2].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5, SCREEN_HEIGHT, 0.0f);
    object[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    object[6].w   = SCREEN_WIDTH;
    object[6].h   = 100;
    object[6].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5, SCREEN_HEIGHT, 0.0f);
    object[6].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);



    object[3].w   = SCREEN_WIDTH;
    object[3].h   = 100;
    object[3].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5, 0.0f, 0.0f);
    object[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    object[7].w   = SCREEN_WIDTH;
    object[7].h   = 100;
    object[7].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5, 0.0f, 0.0f);
    object[7].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitObject(void)
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
void UpdateObject(void)
{

#ifdef _DEBUG	// �f�o�b�O����\������
    //char *str = GetDebugStr();
    //sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawObject(void)
{
    int n = GetMode();

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

    for (int i = 0; i < OBJECT_MAX; i++)
    {
        if (object[i].use == true)		// �I�u�W�F�N�g���g���Ă���H
        {						// Yes
            // �e�N�X�`���ݒ�
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[object[i].texNo]);

            // �I�u�W�F�N�g�̈ʒu��e�N�X�`���[���W�𔽉f
            float px = object[i].pos.x;	// �I�u�W�F�N�g�̕\���ʒuX
            float py = object[i].pos.y;	// �I�u�W�F�N�g�̕\���ʒuY
            float pw = object[i].w;		// �I�u�W�F�N�g�̕\����
            float ph = object[i].h;		// �I�u�W�F�N�g�̕\������
            float ro = object[i].rot.z;

            float tw = 1.0f / TEXTURE_PATTERN_DIVIDE_X;	// �e�N�X�`���̕�
            float th = 1.0f / TEXTURE_PATTERN_DIVIDE_Y;	// �e�N�X�`���̍���
            float tx = tw * object[i].patternAnim;	    // �e�N�X�`���̍���X���W
            float ty = 0.0f;	                        // �e�N�X�`���̍���Y���W

            // �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
            // �`���[�g���A���ƃQ�[���{�҂ŕ����Ă���
            if ((n == MODE_TUTORIAL && 4 <= i) ||
                (n == MODE_GAME     && i < 4)   )
            {
                SetSpriteColorRotation(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ro);

                // �|���S���`��
                GetDeviceContext()->Draw(4, 0);
            }
        }
    }

    if (n == MODE_TUTORIAL)
    {
        // �e�N�X�`���ݒ�
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[8]);

        // �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
        SetSprite(g_VertexBuffer, 900, 100, 200, 200, 0, 0, 1, 1);

        // �|���S���`��
        GetDeviceContext()->Draw(4, 0);
    }

}


//=============================================================================
// �I�u�W�F�N�g�\���̂̃A�h���X���擾
//=============================================================================
OBJECT *GetObject_p(void)
{
    return &object[0];
}
