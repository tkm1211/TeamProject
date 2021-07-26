#pragma once

#define WIN32_WINNT 0x0510

#include <Windows.h>
#include <xaudio2.h>
#include <mmsystem.h>
#include <thread>
#include <mutex>
#include <vector>
#include <cassert>
#include <vector>
#include <list>

#include "WAVFileReader.h"

#pragma comment(lib, "xaudio2.lib")

class CXAudio2
{
public:
	//std::vector<SourceVoice> sourceVoice;
	std::vector<IXAudio2SourceVoice *> lpSourceVoice;
	std::unique_ptr<uint8_t[]> waveFile;
	wchar_t fName[256];
	WAVData waveData;
	XAUDIO2_BUFFER buffer = { 0 };
	bool playing;

	enum Type
	{
		normal,
		loop,
	};

public:
	CXAudio2(const wchar_t* fileName, int type);
	CXAudio2() {}
	~CXAudio2()
	{
		//lpSourceVoice.clear();
	}
	void Create(const wchar_t* fileName, int type);
	void Relese()
	{
		for (auto& it : lpSourceVoice)
		{
			it->DestroyVoice();
		}
		lpSourceVoice.clear();
	}

public:
	bool PlayWAV();				// �Đ�
	bool PauseWAV();			// �ꎞ��~
	bool StopWAV();				// ��~
	bool Update();				// �X�V
	bool Copy();				// ����
	bool Volume(float volume);	// �{�����[������
	bool Pitch(float pitch);	// �s�b�`����
};

class CXAudio2Origin
{
public:
	IXAudio2 *lpXAudio = nullptr;
	IXAudio2MasteringVoice *lpMasterVoice = nullptr;

private:
	CXAudio2Origin();

public:
	~CXAudio2Origin() {}

	void Relese()
	{
		if (lpMasterVoice) lpMasterVoice->DestroyVoice();

		if (lpXAudio)
		{
			lpXAudio->Release();
			lpXAudio = nullptr;
		}

		CoUninitialize();
	}

	static CXAudio2Origin* GetInstance()
	{
		static CXAudio2Origin instance;
		return &instance;
	}
};

#define pCXAudio2Origin  (CXAudio2Origin::GetInstance())