#pragma once

class CChatWindow
{
public:
	CChatWindow::CChatWindow();
	CChatWindow::~CChatWindow();

	void AddChatMessage(char *szNick, uint32_t dwNickColor, char *szMessage);
	void AddInfoMessage(char *szFormat, ...);
	void AddDebugMessage(char *szFormat, ...);
	void AddClientMessage(uint32_t dwColor, char* szStr);
};