#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <comdef.h>
#include "NonCopyable.h"

namespace K3D12 {
	enum class LogLevel {
		Details,
		Info,
		Debug,
		Warning,
		Error
	};

	class  ILogger {
	private:

	public:

	private:

	public:

		virtual ~ILogger() {};

		virtual void		Log(LogLevel level, std::string format) = 0;

		virtual void		LogW(LogLevel level, std::wstring format) = 0;

		virtual void		SetFilter(LogLevel level) = 0;

		virtual LogLevel	GetFilter() = 0;

	};

	class  SystemLogger : public ILogger, private NonCopyable {
	private:

		LogLevel _LogFilter;
		
		std::ofstream _debugFile;

	public:

	private:

		SystemLogger();
		
		void CreateDebugFile();

	public:

		~SystemLogger();

		void		Map(std::string fileName);
		
		void		UnMap();
		
		void		Log(LogLevel level, HRESULT hr);
		
		void		Log(LogLevel level, std::string format);
		
		void		Log(LogLevel level, const char* format, ...);
		
		void		LogW(LogLevel level, std::wstring format);
		
		void		LogW(LogLevel level, const wchar_t* format, ...);
		
		void		SetFilter(LogLevel level);
		
		LogLevel	GetFilter();
		
		void		SetDebugFilePath(std::string filePath);
		
		static SystemLogger& GetInstance();
	};

#define HRESULT_LOG(hr)		SystemLogger::GetInstance().Log(K3D12::LogLevel::Details,"[File: %s, Line: %d] { %s }\n", __FILE__, __LINE__, reinterpret_cast<const char*>(_com_error(hr).ErrorMessage()))
#define	DEBUG_LOG(str)		SystemLogger::GetInstance().Log(K3D12::LogLevel::Debug,		"[File: %s, Line: %d] { %s }\n", __FILE__, __LINE__, str.c_str())
#define INFO_LOG(str)		SystemLogger::GetInstance().Log(K3D12::LogLevel::Info ,		"[File: %s, Line: %d] { %s }\n", __FILE__, __LINE__, str.c_str())
#define DETAILS_LOG(str)	SystemLogger::GetInstance().Log(K3D12::LogLevel::Details ,	"[File: %s, Line: %d] { %s }\n", __FILE__, __LINE__, str.c_str())
#define WARNING_LOG(str)	SystemLogger::GetInstance().Log(K3D12::LogLevel::Warning ,	"[File: %s, Line: %d] { %s }\n", __FILE__, __LINE__, str.c_str())
#define ERROR_LOG(str)		SystemLogger::GetInstance().Log(K3D12::LogLevel::Error ,	"[File: %s, Line: %d] { %s }\n", __FILE__, __LINE__, str.c_str())


#if defined(UNICODE) || defined(_UNICODE)
#else
#endif

}