#pragma once

#include <xaudio2.h>
#include <memory>
namespace K3D12 {
	class IAudioSource
	{
		friend class AudioManager;
	private:
		
		//�T�E���h���Ƃ̃f�[�^
		std::unique_ptr<IXAudio2SourceVoice> _sourceVoice;
				
	public:

	private:

	public:

		IAudioSource();

		virtual ~IAudioSource();

		virtual void Create(unsigned int byteInSize, unsigned int elementSize, void * pSrc);

		virtual void Discard();



	};
}