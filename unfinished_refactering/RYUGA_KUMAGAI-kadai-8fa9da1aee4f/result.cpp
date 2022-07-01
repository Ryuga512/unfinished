//=============================================================================
//
// ���U���g��ʏ��� [result.cpp]
// Author : GP11A132 12 �F�J����
//
//=============================================================================
#include "result.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "sprite.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(2)				// �e�N�X�`���̐�

#define TEXTURE_WIDTH_LOGO			(480)			// ���S�T�C�Y
#define TEXTURE_HEIGHT_LOGO			(80)			// 

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer				*g_VertexBuffer = NULL;		// ���_���
static ID3D11ShaderResourceView	*g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char *g_TexturName[] = {
    "data/TEXTURE/fade_black.png",
    "data/TEXTURE/result_logo.png",
};


static bool						g_Use;						// true:�g���Ă���  false:���g�p
static float					g_w, g_h;					// ���ƍ���
static D3DXVECTOR3				g_Pos;						// �|���S���̍��W
static int						g_TexNo;					// �e�N�X�`���ԍ�


//=============================================================================
// ����������
//=============================================================================
HRESULT InitResult(void)
{
    ID3D11Device *pDevice = GetDevice();

    //�e�N�X�`������
    for (int i = 0; i < TEXTURE_MAX; i++)
    {
        g_Texture[i] = NULL;
        D3DX11CreateShaderResourceViewFromFile(GetDevice(),
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


    // �ϐ��̏�����
    g_Use = true;
    g_w = TEXTURE_WIDTH;
    g_h = TEXTURE_HEIGHT;
    g_Pos = D3DXVECTOR3(g_w / 2, g_h / 2, 0.0f);
    g_TexNo = 0;

    // BGM�Đ�
    PlaySound(SOUND_LABEL_BGM_sample002);

    return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitResult(void)
{
    if (g_VertexBuffer)
    {
        g_VertexBuffer->Release();
        g_VertexBuffer = NULL;
    }

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
void UpdateResult(void)
{

    if (GetKeyboardTrigger(DIK_RETURN))
    {// Enter��������A�X�e�[�W��؂�ւ���
        SetFade(FADE_OUT, MODE_TITLE);
    }
    // �Q�[���p�b�h�œ��͏���
    else if (IsButtonTriggered(0, BUTTON_START))
    {
        SetFade(FADE_OUT, MODE_TITLE);
    }
    else if (IsButtonTriggered(0, BUTTON_B))
    {
        SetFade(FADE_OUT, MODE_TITLE);
    }


#ifdef _DEBUG	// �f�o�b�O����\������
    //char *str = GetDebugStr();
    //sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawResult(void)
{
    // ���_�o�b�t�@�ݒ�
    UINT stride = sizeof(VERTEX_3D);
    UINT offset = 0;
    GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

    // �}�g���N�X�ݒ�
    SetWorldViewProjection2D();

    // �v���~�e�B�u�g�|���W�ݒ�
    GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    // �}�e���A���ݒ�
    MATERIAL material;
    ZeroMemory(&material, sizeof(material));
    material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    SetMaterial(material);

    // ���U���g�̔w�i��`��
    {
        // �e�N�X�`���ݒ�
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);
        float ro = 0;
        // �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
        SetSprite(g_VertexBuffer, g_Pos.x, g_Pos.y, g_w, g_h, 0.0f, 0.0f, 1.0f, 1.0f);

        // �|���S���`��
        GetDeviceContext()->Draw(4, 0);
    }

    // ���U���g�̃��S��`��
    {
        // �e�N�X�`���ݒ�
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[1]);
        float ro = 0.0f;
        // �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
        SetSprite(g_VertexBuffer, g_Pos.x, g_Pos.y, TEXTURE_WIDTH_LOGO, TEXTURE_HEIGHT_LOGO, 0.0f, 0.0f, 1.0f, 1.0f);

        // �|���S���`��
        GetDeviceContext()->Draw(4, 0);
    }

}

