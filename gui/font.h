#pragma once

enum eFontStyle : uint8_t
{
	FONT_GOTHIC,
	FONT_SUBTITLES,
	FONT_MENU,
	FONT_PRICEDOWN
};

class CFont
{
public:
	
	static void PrintString(float x, float y, char *text);
	static void SetScale(float scale);
	static void SetSlant(float slant);
	static void SetColor(uint32_t color);
	static void SetFontStyle(eFontStyle style);
	static void SetWrapx(float value);
	static void SetCentreSize(float value);
	static void SetJustify(int value);
	static void SetEdge(int value);
	static void SetRightJustify(int value);
	static void SetAlphaFade(float alpha);
	static void SetDropColor(uint32_t color);
	static void SetDropShadowPosition(int value);
	static void SetProportional(bool on);
	static void SetOrientation(uint32_t  value);
	static void SetBackground(bool enable, bool includeWrap);
	static void SetBackgroundColor(uint32_t color);
	static int GetStringWidth(char *string, bool unk1, bool unk2);
	static int GetNumberLines(bool print, float x, float y, char *text);
	static void GetTextRect(void *rect, float x, float y, char *text);
	static void AsciiToGxtChar(char* ascii, char *gxt);
};