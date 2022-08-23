#pragma once
#include "manager/Enemy/EnemyManager.h"

//============================================================
// �A�v���P�[�V�����N���X
//	APP.�` �łǂ�����ł��A�N�Z�X�\
//============================================================
class Application
{
// �����o
public:

	// �A�v���P�[�V�������s
	void Execute();

	// �A�v���P�[�V�����I��
	void End() { m_endFlag = true; }


	//=====================================================
	// �O���[�o���f�[�^
	//=====================================================

	// �Q�[���E�B���h�E�N���X
	KdWindow		m_window;

	// FPS����
	int				m_fps = 0;					// ���݂�FPS�l
	int				m_maxFps = 60;				// �ő�FPS


private:

	// �A�v���P�[�V����������
	bool Init(int w, int h);

	// �A�v���P�[�V�������
	void Release();

	// �Q�[���I���t���O true�ŏI������
	bool		m_endFlag = false;


//=====================================================
// �V���O���g���p�^�[��
//=====================================================
private:
	// 
	Application() {}

public:
	static Application &GetInstance(){
		static Application Instance;
		return Instance;
	}
};

// �ȒP�ɃA�N�Z�X�ł���悤�ɂ��邽�߂̃}�N��
#define APP Application::GetInstance()
