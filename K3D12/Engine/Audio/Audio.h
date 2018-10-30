#pragma once

#include <xaudio2.h>
#include <memory>
#include <array>
#include <deque>
#include "AudioCallBack.h"

namespace K3D12 {

	class SubMixVoice;
	class AudioWaveSource;
	class NormalAudioWav;
	class AudioCallBack;


	class Audio
	{
		friend class AudioManager;

	private:
		//サウンドごとのデータ
		XAUDIO2_BUFFER _audioBuffer;

		IXAudio2SourceVoice* _sourceVoice;
		
		std::weak_ptr<AudioWaveSource> _rawData;

		AudioCallBack _callBack;

		XAUDIO2_VOICE_STATE _voiceState;

		WAVEFORMATEXTENSIBLE _format;

		//生データのどの位置から一秒間のサンプリングを行っているかを知らせる要素番号
		unsigned int _seekPoint;

		//曲の最大ながさ。ループポイントを設定するとこの長さが変わる。デフォルトで結尾部分のポイント
		unsigned int _audioLength;

		//ループするかしないか。
		bool _isLoop;

		bool _isDiscarded;

	public:

	private:

	protected:

		void BulkSubmit();

		void StreamSubmit();

	public:
		
		Audio();

		virtual ~Audio();

		virtual void Play();

		virtual void Stop();

		void LoopEnable();

		void LoopDisable();

		void SetLoopPoint(unsigned int loopPoint = 0);

		XAUDIO2_VOICE_STATE GetState();

		virtual void Pause(bool pause);

		virtual bool CheckPlaying();

		virtual void Discard();

		//再生キューにデータを追加
		void SubmitBuffer();

	};
}