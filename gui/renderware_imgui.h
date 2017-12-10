#pragma once

namespace ImGuiPlus
{
	bool          	ProcessInlineHexColor(const char* start, const char* end, ImVec4& color);
	void          	TextWithColors(const char* fmt, ...);
	void			cp1251_to_utf8(char *out, const char *in);
}

void ImGui_RenderWare_Init();
void ImGui_RenderWare_Shutdown();
void ImGui_RenderWare_NewFrame();
void ImGui_RenderWare_DrawLists(ImDrawData *draw_data);
void ImGui_RenderWare_CreateDeviceObjects();
void ImGui_RenderWare_InvalidateDeviceObjects();