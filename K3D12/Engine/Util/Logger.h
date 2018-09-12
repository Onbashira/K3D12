#pragma once
#include <string>
#include <fstream>
#include <sstream>
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
		virtual void		Log(LogLevel level, std::string format) = 0;
		virtual void		LogW(LogLevel level, std::wstring format) = 0;
		virtual void		SetFilter(LogLevel level) = 0;
		virtual LogLevel	GetFilter() = 0;
		virtual ~ILogger() {};
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
		~SystemLogger();
	};

#define LOG (logLevel , str) SystemLogger::GetInstance().Log(logLevel,"%s","%d","%s",__FILE__,__LINE__,str.c_str())
#define LOGW(logLevel , str) SystemLogger::GetInstance().LogW(logLevel"%ls","%ld","%ls",__FILE__,__LINE__,str.c_str())

}