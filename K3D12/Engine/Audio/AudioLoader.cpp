
//ロード時間測定用
#include <Windows.h>
#include <ks.h>
#include <ksmedia.h>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iomanip>

#include "AudioLoader.h"
#include "../Util/Utility.h"
#include "../Util/Logger.h"
#include "AudioWaveSource.h"
#include "AudioSourceManager.h"
#include "../Util/Math.h"

//このクラスで使うthreadの数
constexpr unsigned int AUDIO_LOADER_THREAD_NUM = 12;
constexpr float AUDIO_8BIT_NORMALIZE_FACTOR = 127.0f;
constexpr float AUDIO_16BIT_NORMALIZE_FACTOR = 32768.0f;
constexpr float AUDIO_24BIT_NORMALIZE_FACTOR = 0xFFFF;


namespace K3D12 {

	enum AUDIO_BITS_PER_SAMPLE {
		AUDIO_BITS_PER_SAMPLE_8, //128を無音とする0～255の値を使用する
		AUDIO_BITS_PER_SAMPLE_16, //0を無音とする-32768～32767の値を使用する
		AUDIO_BITS_PER_SAMPLE_24,
	};

	const DWORD SPEAKER_SETTINGS[] = {
		KSAUDIO_SPEAKER_MONO,
		KSAUDIO_SPEAKER_STEREO,
		KSAUDIO_SPEAKER_STEREO | SPEAKER_LOW_FREQUENCY,
		KSAUDIO_SPEAKER_QUAD,
		0,
		KSAUDIO_SPEAKER_5POINT1,
		0,
		KSAUDIO_SPEAKER_7POINT1_SURROUND
	};

	void LoadStereo(HMMIO mmio, AudioWaveSource & wave, MMCKINFO& dataChunk, unsigned int firstReadIndex);
	void LoadMonaural(HMMIO mmio, AudioWaveSource & wave, MMCKINFO& dataChunk, unsigned int firstReadIndex);

	void LoadStereo8bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO& dataChunk, unsigned int firstReadIndex = 0);
	void LoadStereo16bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO& dataChunk, unsigned int firstReadIndex = 0);
	void LoadStereo24bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO& dataChunk, unsigned int firstReadIndex = 0);
	void LoadStereo32bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO& dataChunk, unsigned int firstReadIndex = 0);
	void LoadMonaural8bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO& dataChunk, unsigned int firstReadIndex = 0);
	void LoadMonaural16bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO& dataChunk, unsigned int firstReadIndex = 0);
	void LoadMonaural24bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO& dataChunk, unsigned int firstReadIndex = 0);
	void LoadMonaural32bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO& dataChunk, unsigned int firstReadIndex = 0);
}


std::shared_ptr<K3D12::AudioWaveSource> K3D12::AudioLoader::LoadAudio(std::string audioFilePath)
{
	if (AudioSourceManager::GetInstance().IsLoaded(audioFilePath)) {
		return AudioSourceManager::GetInstance().GetResource(audioFilePath).lock();
	}

	//現状は16bit音源のみ対応

	HMMIO mmio{};

	MMIOINFO mmioInfo{};

	auto str = Util::StringToWString(audioFilePath);
	mmio = mmioOpen(&str[0], &mmioInfo, MMIO_READ);

	if (!mmio) {
		ERROR_LOG(std::string("mmioからオーディオファイルのロードに失敗しました。"));
	}

	MMRESULT ret{};

	//チャンクロード
	MMCKINFO riffChunk;
	{
		riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		ret = mmioDescend(mmio, &riffChunk, NULL, MMIO_FINDRIFF);

		if (ret != MMSYSERR_NOERROR) {
			ERROR_LOG(std::string("mmioDescendが失敗しました。(FIND_RIFF)"));
			mmioClose(mmio, 0);
			return nullptr;
		}
	}
	//チャンクロード
	MMCKINFO chunk;

	{
		chunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
		ret = mmioDescend(mmio, &chunk, NULL, MMIO_FINDCHUNK);

		if (ret != MMSYSERR_NOERROR) {
			ERROR_LOG(std::string("mmioDescendが失敗しました。(FIND_CHUNK)"));
			mmioClose(mmio, 0);
			return nullptr;
		}
	}

	//フォーマット抽出
	WAVEFORMATEX format{};
	WAVEFORMATEXTENSIBLE exFormat{};

	{
		DWORD size = mmioRead(mmio, (HPSTR)&format, chunk.cksize);
		if (size != chunk.cksize) {
			ERROR_LOG(std::string("mmioReadが失敗しました。(FormatRead)"));
			mmioClose(mmio, 0);
			return nullptr;

		}
		std::stringstream ss;

		ss << "Format = " << format.wFormatTag;
		DEBUG_LOG(std::string(ss.str()));
		ss.str("");
		ss << "Channel = " << format.nChannels;
		DEBUG_LOG(std::string(ss.str()));
		ss.str("");
		ss << "Sampling = " << format.nSamplesPerSec;
		DEBUG_LOG(std::string(ss.str()));
		ss.str("");
		ss << "BitPerSample = " << format.wBitsPerSample;
		DEBUG_LOG(std::string(ss.str()));
		ss.str("");
		ss << "BytePerSec = " << format.nAvgBytesPerSec;
		DEBUG_LOG(std::string(ss.str()));
		ss.str("");

	}
	//階層を一つ上に
	mmioAscend(mmio, &chunk, 0);

	//データチャンクの検索
	MMCKINFO dataChunk;
	{
		dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
		ret = mmioDescend(mmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);
		if (ret != MMSYSERR_NOERROR) {
			ERROR_LOG(std::string("mmioDescendが失敗しました。(DataSerch)"));
			mmioClose(mmio, 0);
			return nullptr;
		}
	}

	std::shared_ptr<K3D12::AudioWaveSource> res = std::make_shared<K3D12::AudioWaveSource>();
	//フォーマットセット
	{
		AudioSourceManager::GetInstance().SetResource(audioFilePath, res);

		res->_format.Format = format;

		res->_format.Format.nSamplesPerSec; //1 秒あたりのサンプル数 (Hz)
		res->_format.Format.nChannels; // ウェーブフォーム オーディオ データのチャンネル数
		res->_format.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE; //サブフォーマットで定義
		res->_format.Format.wBitsPerSample = static_cast<unsigned short>(K3D12::Util::ConvertByteToBit<float>()); //1サンプル点当たりのビット数
		res->_format.Format.nBlockAlign = sizeof(float) * format.nChannels; //1サンプル点のデータが途中で分割されないための最小アライン数
		res->_format.Format.nAvgBytesPerSec = format.nSamplesPerSec * res->_format.Format.nBlockAlign; //１秒当たりのデータ転送量
		res->_format.Format.cbSize = sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX);

		//ExtendedFormatの設定
		res->_format.Samples.wValidBitsPerSample = res->_format.Format.wBitsPerSample;
		res->_format.dwChannelMask = SPEAKER_SETTINGS[format.nChannels - 1];	//スピーカー配置
		res->_format.SubFormat = KSDATAFORMAT_SUBTYPE_IEEE_FLOAT;

		res->GetWave().resize(dataChunk.cksize / (res->GetWaveFormat().nChannels * Util::ConvertBitToByte(16)));
	}

	//子の波形データ抽出部分をマルチスレッド化したい
	{
		//波形データ抽出
		{

			unsigned int loadSize = 0;
			unsigned int waveIndex = 0;
			unsigned int cycleNum = 0;
			unsigned int cycleMax = res->GetWave().size() / 2;
			Stereo16Bit waveInfo = {};

			std::chrono::system_clock::time_point start, end;

			start = std::chrono::system_clock::now();

			do {
				
				res->_loadedSize += mmioRead(mmio, (HPSTR)&waveInfo, sizeof(Stereo16Bit));
				if (res->_loadedSize != sizeof(Stereo16Bit)*(++cycleNum)) {
					ERROR_LOG(std::string("mmioReadが失敗しました。(WaveLoad)"));
					mmioClose(mmio, 0);
					return nullptr;
				}
				//波形　正規化vおよび代入
				{			
					res->GetWave()[waveIndex] = (static_cast<float>(waveInfo.left) / AUDIO_16BIT_NORMALIZE_FACTOR);
					res->GetWave()[waveIndex + 1] = (static_cast<float>(waveInfo.right) / AUDIO_16BIT_NORMALIZE_FACTOR);
				}
				//インクリメント
				{
					waveIndex += static_cast<unsigned int>(res->GetWaveFormat().nChannels);
				}
			
			} while (cycleMax > cycleNum);

			end = std::chrono::system_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
			std::stringstream ss;
			ss << "Read time [ " << elapsed << " ] ms.";
			INFO_LOG(std::string(ss.str()));

		}
	}

	//ハンドルクローズ
	mmioClose(mmio, 0);

	////実データ構築

	return res;
}

std::shared_ptr<K3D12::AudioWaveSource> K3D12::AudioLoader::LoadAudioEx(std::string audioFilePath)
{
	if (AudioSourceManager::GetInstance().IsLoaded(audioFilePath)) {
		return AudioSourceManager::GetInstance().GetResource(audioFilePath).lock();
	}

	//現状は16bit音源のみ対応

	HMMIO mmio{};

	MMIOINFO mmioInfo{};

	auto str = Util::StringToWString(audioFilePath);
	mmio = mmioOpen(&str[0], &mmioInfo, MMIO_READ);

	if (!mmio) {
		ERROR_LOG(std::string("mmioからオーディオファイルのロードに失敗しました。"));
	}

	MMRESULT ret{};

	//チャンクロード
	MMCKINFO riffChunk;
	{
		riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		ret = mmioDescend(mmio, &riffChunk, NULL, MMIO_FINDRIFF);

		if (ret != MMSYSERR_NOERROR) {
			ERROR_LOG(std::string("mmioDescendが失敗しました。(FIND_RIFF)"));
			mmioClose(mmio, 0);
			return nullptr;
		}
	}

	//チャンクロード
	MMCKINFO chunk;

	{
		chunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
		ret = mmioDescend(mmio, &chunk, NULL, MMIO_FINDCHUNK);

		if (ret != MMSYSERR_NOERROR) {
			ERROR_LOG(std::string("mmioDescendが失敗しました。(FIND_CHUNK)"));
			mmioClose(mmio, 0);
			return nullptr;
		}
	}

	//フォーマット抽出
	WAVEFORMATEX format{};
	WAVEFORMATEXTENSIBLE exFormat{};

	{
		DWORD size = mmioRead(mmio, (HPSTR)&format, chunk.cksize);
		if (size != chunk.cksize) {
			ERROR_LOG(std::string("mmioReadが失敗しました。(FormatRead)"));
			mmioClose(mmio, 0);
			return nullptr;

		}
		std::stringstream ss;

		ss << "Format = " << format.wFormatTag;
		DEBUG_LOG(std::string(ss.str()));
		ss.str("");
		ss << "Channel = " << format.nChannels;
		DEBUG_LOG(std::string(ss.str()));
		ss.str("");
		ss << "Sampling = " << format.nSamplesPerSec;
		DEBUG_LOG(std::string(ss.str()));
		ss.str("");
		ss << "BitPerSample = " << format.wBitsPerSample;
		DEBUG_LOG(std::string(ss.str()));
		ss.str("");
		ss << "BytePerSec = " << format.nAvgBytesPerSec;
		DEBUG_LOG(std::string(ss.str()));
		ss.str("");

	}
	//階層を一つ上に
	mmioAscend(mmio, &chunk, 0);

	//データチャンクの検索
	MMCKINFO dataChunk;
	{
		dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
		ret = mmioDescend(mmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);
		if (ret != MMSYSERR_NOERROR) {
			ERROR_LOG(std::string("mmioDescendが失敗しました。(DataSerch)"));
			mmioClose(mmio, 0);
			return nullptr;
		}
	}

	std::shared_ptr<K3D12::AudioWaveSource> res = std::make_shared<K3D12::AudioWaveSource>();
	//フォーマットセット
	{
		AudioSourceManager::GetInstance().SetResource(audioFilePath, res);

		res->_format.Format = format;

		//基本フォーマットの設定
		res->_format.Format.nSamplesPerSec; //1 秒あたりのサンプル数 (Hz)
		res->_format.Format.nChannels; // ウェーブフォーム オーディオ データのチャンネル数
		res->_format.Format.wFormatTag = WAVE_FORMAT_EXTENSIBLE; //サブフォーマットで定義
		res->_format.Format.wBitsPerSample = static_cast<unsigned short>(K3D12::Util::ConvertByteToBit<float>()); //1サンプル点当たりのビット数
		res->_format.Format.nBlockAlign = sizeof(float) * format.nChannels; //1サンプル点のデータが途中で分割されないための最小アライン数
		res->_format.Format.nAvgBytesPerSec = format.nSamplesPerSec * res->_format.Format.nBlockAlign; //１秒当たりのデータ転送量
		res->_format.Format.cbSize = sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX);

		//ExtendedFormatの設定
		res->_format.Samples.wValidBitsPerSample = res->_format.Format.wBitsPerSample;
		res->_format.dwChannelMask = SPEAKER_SETTINGS[format.nChannels - 1];	//スピーカー配置
		res->_format.SubFormat = KSDATAFORMAT_SUBTYPE_IEEE_FLOAT;
	}

	//波形データ抽出
	{
		switch (format.nChannels)
		{
		case 1:
			this->_loadThreadPool.enqueue([&]() {
				auto chunk = dataChunk;
				LoadMonaural(mmio, *res, chunk, 0);
			}
			);
			break;
		case 2:
			this->_loadThreadPool.enqueue([&]() {
				auto chunk = dataChunk;
				LoadStereo(mmio, *res, chunk, 0);
			}
			);
			break;
		default:
			break;
		}
	}

	//復帰
	return res;
}

void K3D12::LoadStereo(HMMIO mmio, AudioWaveSource & wave, MMCKINFO & dataChunk, unsigned int firstReadIndex)
{
	switch (wave.GetWaveFormat().wBitsPerSample)
	{
	case 8:
		wave.GetWave().resize(dataChunk.cksize / (wave.GetWaveFormat().nChannels * Util::ConvertBitToByte(8)));
		LoadStereo8bitSound(mmio, wave, dataChunk, firstReadIndex);
		break;
	case 16:
		wave.GetWave().resize(dataChunk.cksize / (wave.GetWaveFormat().nChannels * Util::ConvertBitToByte(16)));
		LoadStereo8bitSound(mmio, wave, dataChunk, firstReadIndex);
		break;
	case 24:
		wave.GetWave().resize(dataChunk.cksize / (wave.GetWaveFormat().nChannels * Util::ConvertBitToByte(24)));
		DEBUG_LOG(std::string("未実装"));
		assert(0);
		LoadStereo8bitSound(mmio, wave, dataChunk, firstReadIndex);
		break;
	case 32:
		wave.GetWave().resize(dataChunk.cksize / (wave.GetWaveFormat().nChannels * Util::ConvertBitToByte(32)));
		DEBUG_LOG(std::string("未実装"));
		assert(0);
		LoadStereo8bitSound(mmio, wave, dataChunk, firstReadIndex);
		break;
	default:
		assert(0);
		break;
	}
}

void K3D12::LoadMonaural(HMMIO mmio, AudioWaveSource & wave, MMCKINFO & dataChunk, unsigned int firstReadIndex)
{
	switch (wave.GetWaveFormat().wBitsPerSample)
	{
	case 8:
		wave.GetWave().resize(dataChunk.cksize / (wave.GetWaveFormat().nChannels * Util::ConvertBitToByte(8)));
		LoadMonaural8bitSound(mmio, wave, dataChunk, firstReadIndex);
		break;
	case 16:
		wave.GetWave().resize(dataChunk.cksize / (wave.GetWaveFormat().nChannels * Util::ConvertBitToByte(16)));
		LoadMonaural16bitSound(mmio, wave, dataChunk, firstReadIndex);
		break;
	case 24:
		wave.GetWave().resize(dataChunk.cksize / (wave.GetWaveFormat().nChannels * Util::ConvertBitToByte(24)));
		DEBUG_LOG(std::string("未実装"));
		assert(0);
		LoadMonaural24bitSound(mmio, wave, dataChunk, firstReadIndex);
		break;
	case 32:
		wave.GetWave().resize(dataChunk.cksize / (wave.GetWaveFormat().nChannels * Util::ConvertBitToByte(32)));
		DEBUG_LOG(std::string("未実装"));
		assert(0);
		LoadMonaural32bitSound(mmio, wave, dataChunk, firstReadIndex);
		break;
	default:
		assert(0);
		break;
	}
}

void K3D12::LoadStereo8bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO & dataChunk, unsigned int firstReadIndex)
{

	//波形データ抽出
	{
		std::chrono::system_clock::time_point start, end;

		Stereo8Bit waveInfo = {}; //読み込んだ波形の情報

		unsigned int waveIndex = firstReadIndex;

		unsigned int size = 0;

		start = std::chrono::system_clock::now();

		while (size <= dataChunk.cksize) {
			//本当はチャンネル数で変えるべき。しかし、今のところゲームに２ｃｈ以上の対応はさせないので現段階での実装は控えておく
			size += mmioRead(mmio, (HPSTR)&waveInfo, sizeof(Stereo8Bit));

			if (size != sizeof(Stereo8Bit)) {
				ERROR_LOG(std::string("mmioReadが失敗しました。(WaveLoad)"));
				mmioClose(mmio, 0);
				return;
			}

			//デバッグログ
			{
				end = std::chrono::system_clock::now();
				auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
				std::stringstream ss;
				ss << "Read time [ " << elapsed << " ] ms.";
				INFO_LOG(std::string(ss.str()));
			}

			//波形　正規化vおよび代入
			{
				wave.GetWave()[waveIndex] = (static_cast<float>(waveInfo.left) / AUDIO_8BIT_NORMALIZE_FACTOR) - 1.0f;
				wave.GetWave()[waveIndex + 1] = (static_cast<float>(waveInfo.right) / AUDIO_8BIT_NORMALIZE_FACTOR) - 1.0f;

			}
			//インクリメント
			{
				waveIndex += static_cast<unsigned int>(wave.GetWaveFormat().nChannels);
			}
		}
	}

	//ハンドルクローズ
	mmioClose(mmio, 0);
}

void K3D12::LoadStereo16bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO & dataChunk, unsigned int firstReadIndex)
{
	//波形データ抽出
	{
		std::chrono::system_clock::time_point start, end;

		Stereo16Bit waveInfo{}; //読み込んだ波形の情報

		unsigned int waveIndex = 0;

		unsigned int size = 0;

		start = std::chrono::system_clock::now();

		while (size <= dataChunk.cksize) {
			//本当はチャンネル数で変えるべき。しかし、今のところゲームに２ｃｈ以上の対応はさせないので現段階での実装は控えておく
			size += mmioRead(mmio, (HPSTR)&waveInfo, sizeof(Stereo16Bit));

			if (size != sizeof(Stereo16Bit)) {
				ERROR_LOG(std::string("mmioReadが失敗しました。(WaveLoad)"));
				mmioClose(mmio, 0);
				return;
			}

			//デバッグログ
			{
				end = std::chrono::system_clock::now();
				auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
				std::stringstream ss;
				ss << "Read time [ " << elapsed << " ] ms.";
				INFO_LOG(std::string(ss.str()));
			}

			//波形　正規化vおよび代入
			{
				wave.GetWave()[waveIndex] = (static_cast<float>(waveInfo.left) / AUDIO_16BIT_NORMALIZE_FACTOR);
				wave.GetWave()[waveIndex + 1] = (static_cast<float>(waveInfo.right) / AUDIO_8BIT_NORMALIZE_FACTOR);

			}
			//インクリメント
			{
				waveIndex += static_cast<unsigned int>(wave.GetWaveFormat().nChannels);
			}
		}

	}
}

void K3D12::LoadStereo24bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO & dataChunk, unsigned int firstReadIndex)
{
	std::chrono::system_clock::time_point start, end;

	unsigned short waveInfo = 0; //読み込んだ波形の情報

	unsigned int waveIndex = 0;

	unsigned int size = 0;

	start = std::chrono::system_clock::now();

	size = mmioRead(mmio, (HPSTR)&waveInfo, sizeof(unsigned short));

	if (size != sizeof(unsigned short)) {
		ERROR_LOG(std::string("mmioReadが失敗しました。(WaveLoad)"));
		mmioClose(mmio, 0);
		return;
	}

	//デバッグログ
	{
		end = std::chrono::system_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		std::stringstream ss;
		ss << "Read time [ " << elapsed << " ] ms.";
		INFO_LOG(std::string(ss.str()));
	}

	//波形　正規化vおよび代入
	{
		wave.GetWave()[waveIndex] = ::Clamp<float>((static_cast<float>(waveInfo) / AUDIO_16BIT_NORMALIZE_FACTOR), -1.0f, 1.0f);
	}
}

void K3D12::LoadStereo32bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO & dataChunk, unsigned int firstReadIndex)
{
	std::chrono::system_clock::time_point start, end;

	unsigned short waveInfo = 0; //読み込んだ波形の情報

	unsigned int waveIndex = 0;

	unsigned int size = 0;

	start = std::chrono::system_clock::now();

	size = mmioRead(mmio, (HPSTR)&waveInfo, sizeof(unsigned short));

	if (size != sizeof(unsigned short)) {
		ERROR_LOG(std::string("mmioReadが失敗しました。(WaveLoad)"));
		mmioClose(mmio, 0);
		return;
	}

	//デバッグログ
	{
		end = std::chrono::system_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		std::stringstream ss;
		ss << "Read time [ " << elapsed << " ] ms.";
		INFO_LOG(std::string(ss.str()));
	}

	//波形　正規化vおよび代入
	{
		wave.GetWave()[waveIndex] = ::Clamp<float>((static_cast<float>(waveInfo) / AUDIO_16BIT_NORMALIZE_FACTOR), -1.0f, 1.0f);
	}
}

void K3D12::LoadMonaural8bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO & dataChunk, unsigned int firstReadIndex)
{
}

void K3D12::LoadMonaural16bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO & dataChunk, unsigned int firstReadIndex)
{
}

void K3D12::LoadMonaural24bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO & dataChunk, unsigned int firstReadIndex)
{
}

void K3D12::LoadMonaural32bitSound(HMMIO mmio, AudioWaveSource & wave, MMCKINFO & dataChunk, unsigned int firstReadIndex)
{
}
