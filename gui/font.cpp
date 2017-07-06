#include "main.h"

static void PrintString(float x, float y, char *text)
{
	void (*_PrintString)(float x, float y, char *text);
    *(void **) (&_PrintString) = (void*)(g_libGTASA+0x5353B4+1);
    (*_PrintString)(x, y, text);
}

static void SetScale(float scale)
{
	void (*_SetScale)(float scale);
    *(void **) (&_SetScale) = (void*)(g_libGTASA+0x533694+1);
    (*_SetScale)(scale);
}

static void SetSlant(float slant)
{
	void (*_SetSlant)(float slant);
    *(void **) (&_SetSlant) = (void*)(g_libGTASA+0x5336DC+1);
    (*_SetSlant)(slant);
}

static void SetColor(uint32_t color)
{
	void (*_SetColor)(uint32_t color);
    *(void **) (&_SetColor) = (void*)(g_libGTASA+0x5336F4+1);
    (*_SetColor)(color);
}

static void SetFontStyle(eFontStyle style)
{
	void (*_SetFontStyle)(eFontStyle style);
    *(void **) (&_SetFontStyle) = (void*)(g_libGTASA+0x533748+1);
    (*_SetFontStyle)(style);
}

static void SetWrapx(float value)
{
	void (*_SetWrapx)(float value);
    *(void **) (&_SetWrapx) = (void*)(g_libGTASA+0x53381C+1);
    (*_SetWrapx)(value);
}

static void SetCentreSize(float value)
{
	void (*_SetCentreSize)(float value);
    *(void **) (&_SetCentreSize) = (void*)(g_libGTASA+0x533834+1);
    (*_SetCentreSize)(value);
}

static void SetRightJustify(int value)
{
	void (*_SetRightJustify)(int value);
    *(void **) (&_SetRightJustify) = (void*)(g_libGTASA+0x53384C+1);
    (*_SetRightJustify)(value);
}

static void SetAlphaFade(float alpha)
{
	void (*_SetAlphaFade)(float alpha);
    *(void **) (&_SetAlphaFade) = (void*)(g_libGTASA+0x533864+1);
    (*_SetAlphaFade)(alpha);
}

static void SetDropColor(uint32_t color)
{
	void (*_SetDropColor)(uint32_t color);
    *(void **) (&_SetDropColor) = (void*)(g_libGTASA+0x53387C+1);
    (*_SetDropColor)(color);
}

static void SetDropShadowPosition(int value)
{
	void (*_SetDropShadowPosition)(int value);
    *(void **) (&_SetDropShadowPosition) = (void*)(g_libGTASA+0x5338DC+1);
    (*_SetDropShadowPosition)(value);
}

static void SetProportional(bool on)
{
	void (*_SetProportional)(bool on);
    *(void **) (&_SetProportional) = (void*)(g_libGTASA+0x533970+1);
    (*_SetProportional)(on);
}

static void SetBackground(bool enable, bool includeWrap)
{
	void (*_SetBackground)(bool enable, bool includeWrap);
    *(void **) (&_SetBackground) = (void*)(g_libGTASA+0x533988+1);
    (*_SetBackground)(enable, includeWrap);
}

static void SetBackgroundColor(uint32_t color)
{
	void (*_SetBackgroundColor)(uint32_t color);
    *(void **) (&_SetBackgroundColor) = (void*)(g_libGTASA+0x5339A4+1);
    (*_SetBackgroundColor)(color);
}

static int GetStringWidth(char *string, bool unk1, bool unk2)
{
	int (*_GetStringWidth)(char *string, bool unk1, bool unk2);
    *(void **) (&_GetStringWidth) = (void*)(g_libGTASA+0x534BAC+1);
    return (*_GetStringWidth)(string, unk1, unk2);
}

static int GetNumberLines(bool print, float x, float y, char *text)
{
	int (*_GetNumberLines)(bool print, float x, float y, char *text);
    *(void **) (&_GetNumberLines) = (void*)(g_libGTASA+0x5352C4+1);
    return (*_GetNumberLines)(print, x, y, text);
}

static void GetTextRect(void *rect, float x, float y, char *text)
{
	void (*_GetTextRect)(void *rect, float x, float y, char *text);
    *(void **) (&_GetTextRect) = (void*)(g_libGTASA+0x5352C4+1);
    (*_GetTextRect)(rect, x, y, text);
}