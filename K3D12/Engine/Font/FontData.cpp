#include <Shlwapi.h>
#include <string>
#include "FontData.h"
#include "../Util/Utility.h"


K3D12::FontData::FontData() :
	_fontParamater(), _fontName("NONE"), _fontPath("NONE")
{

}

K3D12::FontData::~FontData()
{
	Discard();
}


LOGFONT K3D12::FontData::GetPramater()const
{
	return _fontParamater;
}

HFONT K3D12::FontData::GetFontHandle()const
{
	return _font;
}

void K3D12::FontData::SetFontHandle(const HFONT & font)
{
	_font = font;
}

std::string K3D12::FontData::GetFontName()const
{
	return _fontName;
}

std::string K3D12::FontData::GetFontPath()const
{
	return _fontPath;
}

void K3D12::FontData::Create(LOGFONT paramater, std::string fontName, std::string fontPath)
{
	_fontParamater = paramater;
	_fontName = fontName;
	_fontPath = fontPath;

	DESIGNVECTOR design ={};

	AddFontResourceEx(
		Util::StringToWString(_fontPath).c_str(), //ttf�t�@�C���ւ̃p�X
		FR_PRIVATE,
		&design
	);
	_font = CreateFontIndirect(&_fontParamater);
}

void K3D12::FontData::Discard()
{

	DESIGNVECTOR design = {};
	RemoveFontResourceEx(
		Util::StringToWString(_fontPath).c_str(), //ttf�t�@�C���ւ̃p�X
		FR_PRIVATE,
		&design
	);
	_font = nullptr;

}






