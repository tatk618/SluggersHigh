#pragma once
//===============================================
//
// �v���R���p�C���ς݃w�b�_�[
//  �����ɏ��������̂͏���̂݉�͂���邽�߁A�R���p�C�����Ԃ������ɂȂ�B
//  �S�Ă�cpp����C���N���[�h�����K�v������B
//
//===============================================


//===============================================
//
// ��{
//
//===============================================
#pragma comment(lib,"winmm.lib")

#define NOMINMAX
#include <windows.h>
#include <stdio.h>

#include <wrl/client.h>

//===============================================
//
// STL
//
//===============================================
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <array>
#include <vector>
#include <stack>
#include <list>
#include <iterator>
#include <queue>
#include <algorithm>
#include <memory>
#include <random>
#include <fstream>
#include <iostream>
#include <sstream>
#include <functional>
#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include <filesystem>

//===============================================
//
// Direct3D11
//
//===============================================
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")

#include <d3dcommon.h>
#include <dxgi.h>
#include <d3d11.h>

#include <DirectXMath.h>
#include <DirectXCollision.h>

// DirectX Tool Kit
#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "DirectXTKAudioWin8.lib")
#include <SimpleMath.h>
#include <Audio.h>

// DirectX Tex
#pragma comment(lib, "DirectXTex.lib")
#include <DirectXTex.h>

//===============================================
//
// imgui
//
//===============================================




//===============================================
// �����R�[�h�ϊ�
//===============================================
#include "strconv.h"

//===============================================
//
// ����System
//
//===============================================
#include "System/KdSystem.h"


