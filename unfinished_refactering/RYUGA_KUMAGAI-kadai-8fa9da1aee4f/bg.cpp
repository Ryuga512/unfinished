//=============================================================================
//
// �w�i���� [bg.cpp]
// Author : GP11A132 12 �F�J����
//
//=============================================================================
#include "bg.h"
#include "player.h"
#include "sprite.h"
#include "title.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH * 1.3f)	// �L�����T�C�Y
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)			// 
#define TEXTURE_MAX					(4)						// �e�N�X�`���̐�


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
    "data/TEXTURE/yuka.jpg",
    "data/TEXTURE/tenki-.png",

};


static bool						g_Use;						// true:�g���Ă���  false:���g�p
static float					g_w, g_h;					// ���ƍ���
static D3DXVECTOR3				g_Pos;						// �|���S���̍��W
static int						g_TexNo;					// �e�N�X�`���ԍ�
int g_mode;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitBg(void)
{
    g_mode = GetMode();
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


	// BG�̏�����
	g_Use   = true;
	g_w     = TEXTURE_WIDTH;
	g_h     = TEXTURE_HEIGHT;
	g_Pos   = D3DXVECTOR3(TEXTURE_WIDTH/2, TEXTURE_HEIGHT/2, 0.0f);

    if (g_mode == MODE_GAME)
    {
        g_TexNo = 0;
    }
    else
    {
        g_TexNo = 1;
    }
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitBg(void)
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
void UpdateBg(void)
{

#ifdef _DEBUG	// �f�o�b�O����\������
	//char *str = GetDebugStr();
	//sprintf(&str[strlen(str)], " PX:%.2f PY:%.2f", g_Pos.x, g_Pos.y);
	
#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBg(void)
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

	// �e�N�X�`���ݒ�
	GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[g_TexNo]);

	// �X�R�A�̈ʒu��e�N�X�`���[���W�𔽉f
	float px = g_Pos.x;		// �w�i�̕\���ʒuX
	float py = g_Pos.y;		// �w�i�̕\���ʒuY
	float pw = g_w;			// �w�i�̕\����
	float ph = g_h;			// �w�i�̕\������

	float tw = 1.0f;		// �e�N�X�`���̕�
	float th = 1.0f;		// �e�N�X�`���̍���
	float tx = 0.0f;		// �e�N�X�`���̍���X���W
	float ty = 0.0f;		// �e�N�X�`���̍���Y���W
	// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
    SetSprite(g_VertexBuffer, px, py, pw, ph, tx, ty, tw, th);

	// �|���S���`��
	GetDeviceContext()->Draw(4, 0);

    if (g_mode == MODE_TUTORIAL)
    {
        // �e�N�X�`���ݒ�
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[2]);

        // �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
        SetSprite(g_VertexBuffer, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 200, 100, tx, ty, tw, th);

        // �|���S���`��
        GetDeviceContext()->Draw(4, 0);


    }
}






