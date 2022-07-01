//=============================================================================
//
// �v���C���[���� [player.h]
// Author : GP11A132 12 �F�J����
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PLAYER_MAX		(1)		// �v���C���[��Max�l��

#define UP                          (0)
#define DOWN                        (1)
#define RIGHT                       (2)
#define LEFT                        (3)
#define ATACK_DELAY                 (64)    // �G�t�F�N�g�̃A�j������10*3(���[�v��)*2(�ꖇ�`�悷��̂ɕK�v�Ȏ���)


//*****************************************************************************
// �\���̒�`
//*****************************************************************************

struct PLAYER
{
	bool			use;			// true:�g���Ă���  false:���g�p
	float			w, h;			// ���ƍ���
	D3DXVECTOR3		pos;			// �|���S���̍��W
	D3DXVECTOR3		rot;			// �|���S���̉�]��
    D3DXVECTOR3     pre_pos;        // �O�̍��W
	int				countAnim;		// �A�j���[�V�����J�E���g
	int				patternAnim;	// �A�j���[�V�����p�^�[���i���o�[
	int				texNo;			// �e�N�X�`���ԍ�
    int             dir;            // �����Ă����
    bool            hit[4];         // �Փ˔���
    bool            in_attack;      // �U������
    int             attack;         // �U����
    float           hp;             // �̗�
    float           atk_h;
    float           atk_w;
    int             atk_delay;
    float           pre_tx;
    int             pre_texNo;
};



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER *GetPlayer_p(void);


