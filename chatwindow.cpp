#include "main.h"

extern CGame *pGame;

CChatWindow::CChatWindow()
{
	for(int x =0; x < MAX_MESSAGES; x++)
		memset(&m_ChatWindowEntries[x], 0, sizeof(CHAT_WINDOW_ENTRY));

	m_iCount = 0;

	m_uChatTextColor = 0xFFFFFFFF;
	m_uChatInfoColor = 0xFFFFFFFF;
	m_uChatDebugColor = 0xFFFF0000;
}

CChatWindow::~CChatWindow()
{

}

void CChatWindow::Draw()
{
	float x = 220.0f;
	float y = 25.0f;
	int i = 0;

	for(int z = 5; z > 0; z--)
	{
		i = m_iCount-z;
		if(i < 0) continue;

		switch(m_ChatWindowEntries[i].eType)
		{
			case CHAT_TYPE_DEBUG:
				RenderText(m_ChatWindowEntries[i].szMessage, x, y, m_ChatWindowEntries[i].uTextColor);
				break;
		}
		y += 20.0f;
	}
}

void CChatWindow::RenderText(char *sz, float x, float y, uint32_t uColor)
{
	CFont::SetOrientation(1);
	CFont::SetProportional(1);
	CFont::SetFontStyle(FONT_SUBTITLES);
	CFont::SetColor(&uColor);
	CFont::SetScale(1.22f);

	CFont::PrintString(x, y, sz);
}

void CChatWindow::AddDebugMessage(char *szFormat, ...)
{
	char tmp_buf[512];
	memset(tmp_buf, 0, 512);

	va_list args;
	va_start(args, szFormat);
	vsprintf(tmp_buf, szFormat, args);
	va_end(args);

	FilterInvalidChars(tmp_buf);
	AddToChatWindowBuffer(CHAT_TYPE_DEBUG, tmp_buf, 0, m_uChatDebugColor, 0);
}

void CChatWindow::FilterInvalidChars(char* szString)
{
	while(*szString) 
	{
		if(*szString > 0 && *szString < ' ') 
			*szString = ' ';

		szString++;
	}
}

void CChatWindow::AddToChatWindowBuffer(eChatMessageType eType, 
										char *szString, 
										char *szNick, 
										uint32_t 
										uTextColor, 
										uint32_t uNickColor)
{
	m_ChatWindowEntries[m_iCount].eType = eType;
	m_ChatWindowEntries[m_iCount].uTextColor = uTextColor;
	m_ChatWindowEntries[m_iCount].uNickColor = uNickColor;

	if(szNick)
	{
		strcpy(m_ChatWindowEntries[m_iCount].szNick, szNick);
		strcat(m_ChatWindowEntries[m_iCount].szNick, ":");
	}
	else
		m_ChatWindowEntries[m_iCount].szNick[0] = '\0';

	CFont::AsciiToGxtChar(szString, m_ChatWindowEntries[m_iCount].szMessage);
	//strncpy(m_ChatWindowEntries[m_iCount].szMessage, szString, MAX_MESSAGE_LENGTH);
	//m_ChatWindowEntries[m_iCount].szMessage[MAX_MESSAGE_LENGTH] = '\0';

	m_iCount++;
}