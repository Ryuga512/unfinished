//=============================================================================
//
// �G�l�~�[���� [enemy.h]
// Author : GP11A132 12 �F�J����
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ENEMY_MAX		(1)		// �G�l�~�[��Max�l��
#define BOSS_HP_MAX     (1000.0f)
#define OTHER_DEFAULT               (0)
#define OTHER_FRONT                 (1)
#define OTHER_SIDE                  (2)
#define OTHER_ROLLING               (3)


//*****************************************************************************
// �\���̒�`
//*****************************************************************************

struct ENEMY
{
	bool			use;			// true:�g���Ă���  false:���g�p
	float			w, h;			// ���ƍ���
	D3DXVECTOR3		pos;			// �|���S���̍��W
	D3DXVECTOR3		rot;			// �|���S���̉�]��
    D3DXVECTOR3     p_pos;
	int				countAnim;		// �A�j���[�V�����J�E���g
	int				patternAnim;	// �A�j���[�V�����p�^�[���i���o�[
	int				texNo;			// �e�N�X�`���ԍ�
	bool			left;			// true:���ֈړ�
	bool			up;				// true:��ֈړ�
    float           hp;             // �̗�
    bool            boss;           // �{�X���ǂ���
    int             atk_delay;      // �U������
    float           r;              // ���b�h
    float           g;              // �O���[��
    float           b;              // �u���[
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

ENEMY *GetEnemy_p(void);


