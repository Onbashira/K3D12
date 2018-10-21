#pragma once
#include <Windows.h>
#include <string>
#include <iostream>
#include <vector>

namespace K3D12::Util{

	//256�o�C�g�A���C�����g
	inline size_t Alignment256Bytes(size_t size) {
		if (size == 256) {
			return 256;
		}
		size_t ret = (size&(~0xff))+0x0100;
		return ret;
	};

	//128�o�C�g�A���C�����g
	inline size_t Alignment128Bytes(size_t size) {

	};

	//16�o�C�g�A���C�����g
	inline size_t Alignment16Bytes(size_t size) {

	};

	//4�o�C�g�A���C�����g
	inline size_t Alignment4Bytes(size_t size) {

	};

	//2�o�C�g�A���C�����g
	inline size_t Alignmentt2Bytes(size_t size) {

	};

	template<typename T>inline unsigned int ConvertByteToBit()
	{
		return (sizeof(T) * 8);
	}

	inline unsigned int ConvertBitToByte(unsigned int bitCount)
	{
		return (bitCount / 8);
	}


	//�����̕����񂩂�g���q���o
	inline std::string ExtensionExtruction(std::string str)
	{
		size_t dotPos = str.rfind('.');
		size_t cutNum =  str.length() - dotPos;
		std::string ext = str.substr(dotPos + 1, cutNum);
		return ext;
	}

	//std::wstring -> std::string
	inline std::string WStringToString(std::wstring wstr) {
		// wstring �� SJIS
		int iBufferSize = WideCharToMultiByte(CP_OEMCP, 0, wstr.c_str()
			, -1, (char *)NULL, 0, NULL, NULL);

		// �o�b�t�@�̎擾
		char* cpMultiByte = new char[iBufferSize];

		// wstring �� SJIS
		WideCharToMultiByte(CP_OEMCP, 0, wstr.c_str(), -1, cpMultiByte
			, iBufferSize, NULL, NULL);

		// string�̐���
		std::string oRet(cpMultiByte, cpMultiByte + iBufferSize - 1);

		// �o�b�t�@�̔j��
		delete[] cpMultiByte;

		// �ϊ����ʂ�Ԃ�
		return(oRet);
	
	};

	//std::string -> std::wstring
	inline std::wstring StringToWString(const std::string& refSrc)
	{
		std::vector<wchar_t> buffer(MultiByteToWideChar(CP_ACP, 0, refSrc.c_str(), -1, nullptr, 0));
		MultiByteToWideChar(CP_ACP, 0, refSrc.c_str(), -1, &buffer.front(), static_cast<int>(buffer.size()));
		return std::wstring(buffer.begin(), buffer.end());
	}

	//���݃��f���p�X����Texture�p�X�܂ł̑��΃p�X���擾
	inline std::string GetRelativeTexturePath(std::string & modelPath, std::string texturePath)
	{
		size_t pathIndex1 = modelPath.rfind('/');

		std::string textureTempPath = modelPath.substr(0, pathIndex1);
		textureTempPath += '/';
		textureTempPath += texturePath;

		return textureTempPath;
	}

	//���K�V�[��Fopen�EFclose�EFread
	namespace FileStream {

		//�t�@�C�����J��
		inline HRESULT FileOpen(std::string path, std::string mode, FILE** stream) {
			errno_t error = 0;
			error = fopen_s(stream, path.c_str(), mode.c_str());
			if (error != 0) {
				char* errorStr = {};
				strerror_s(errorStr, 256, error);
				if (errorStr != nullptr) {
					printf_s("print error string by strerror  : %s\n", errorStr);
				}
				return E_FAIL;
			}
			return S_OK;
		};

		//�t�@�C�������
		inline void FileClose(FILE* stream) {
			fclose(stream);
		};

		//�t�@�C����ǂݍ���
		template < typename T > inline HRESULT FileRead(void* buffer, size_t elementNum, FILE* stream) {
			size_t refCount = fread_s(buffer, sizeof(&buffer), sizeof(T), elementNum, stream);
			if (refCount < 0)
				return E_FAIL;
			return S_OK;
		};

		//�t�@�C����ǂݍ���
		template < typename T > inline HRESULT FileRead(void* buffer, size_t bufferSize, size_t elementNum, FILE* stream) {
			size_t refCount = fread_s(buffer, bufferSize, sizeof(T), elementNum, stream);
			if (refCount < 0)
				return E_FAIL;
			return S_OK;
		};

		template <class T = void*> size_t FileRead(void* buffer, FILE* stream, size_t size = 0, size_t readSize = 1) {

			size_t inSize = sizeof(T);
			size_t returnValue = -1;
			FILE&  streamRef = *stream;

			if (size == 0) {
				returnValue = std::fread(buffer, sizeof(T), readSize, &streamRef);
			}
			else {
				returnValue = std::fread(buffer, size, readSize, &streamRef);
			}
			return returnValue;
		}
	}

}	


