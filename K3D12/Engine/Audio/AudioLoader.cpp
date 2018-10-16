
//ロード時間測定用
#include <chrono>
#include <sstream>
#include <iomanip>

#include "AudioLoader.h"
#include "../Util/Utility.h"
#include "../Util/Logger.h"
#include "AudioNormalWav.h"
#include "AudioSourceManager.h"

//このクラスで使うthreadの数
constexpr unsigned int AUDIO_LOADER_THREAD_NUM = 10;

namespace K3D12 {
	enum AUDIO_BITS_PER_SAMPLE {
		AUDIO_BITS_PER_SAMPLE_8, //128を無音とする0～255の値を使用する
		AUDIO_BITS_PER_SAMPLE_16 , //0を無音とする-32768～32767の値を使用する
		AUDIO_BITS_PER_SAMPLE_24 ,
	};
}

//std::vector<unsigned char> ReadWav8Bit(HMMIO mmio, MMCKINFO& dataChunk);
//std::vector<short> ReadWav16Bit(HMMIO mmio, MMCKINFO& dataChunk);
//std::vector<float> ReadWav24bit(HMMIO mmio, MMCKINFO& dataChunk);


K3D12::AudioLoader::AudioLoader()
{
}

K3D12::AudioLoader::~AudioLoader()
{
}

std::shared_ptr<K3D12::IWaveResource> K3D12::AudioLoader::LoadAudio(std::string audioFilePath)
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

		ss <<"Format = "<< format.wFormatTag;
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
	MMCKINFO dataChunck;
	{
		dataChunck.ckid = mmioFOURCC('d', 'a', 't', 'a');
		ret = mmioDescend(mmio, &dataChunck, &riffChunk, MMIO_FINDCHUNK);
		if (ret != MMSYSERR_NOERROR) {
			ERROR_LOG(std::string("mmioDescendが失敗しました。(DataSerch)"));
			mmioClose(mmio, 0);
			return nullptr;
		}
	}
	std::chrono::system_clock::time_point start, end;
	std::shared_ptr<K3D12::AudioNormalWav> res = std::make_shared<K3D12::AudioNormalWav>();
	res->_wave.resize(dataChunck.cksize);

	//波形データ抽出
	{   
		start = std::chrono::system_clock::now();

		auto size = mmioRead(mmio, (HPSTR)&res->_wave[0], dataChunck.cksize);
		if (size != dataChunck.cksize) {
			ERROR_LOG(std::string("mmioReadが失敗しました。(WaveLoad)"));
			mmioClose(mmio, 0);
			return nullptr;
		}

		end = std::chrono::system_clock::now();
		auto elapsed = std::chrono::duration_cast< std::chrono::milliseconds >(end - start).count();
		std::stringstream ss;
		ss << "Read time [ " << elapsed << " ] ms.";
		INFO_LOG(std::string(ss.str()));
	}

	//ハンドルクローズ
	mmioClose(mmio, 0);

	//実データ構築
	
	res->_resolutionType = K3D12::WAVE_RESOLUTION_TYPE::AUDIO_RESOURCE_TYPE_NORMAL_RESOLUTION;
	res->_waveFormat = format;
	res->_wave.swap(wav);

	AudioSourceManager::GetInstance().SetResource(audioFilePath, res);

	return res;
}
