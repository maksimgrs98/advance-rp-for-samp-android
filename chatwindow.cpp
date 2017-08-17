#include "main.h"

extern CGame *pGame;

CChatWindow::CChatWindow()
{
	for(int x =0; x < MAX_MESSAGES; x++)
		memset(&m_ChatWindowEntries[x], 0, sizeof(CHAT_WINDOW_ENTRY));

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
	int len = 0;

	for(int i = 4; i >= 0; i--)
	{
		switch(m_ChatWindowEntries[i].eType)
		{
			case CHAT_TYPE_CHAT:
				len = CFont::GxtCharStrlen(m_ChatWindowEntries[i].szNick);

				if (len) {
					RenderText(m_ChatWindowEntries[i].szNick, x, y, m_ChatWindowEntries[i].uNickColor);
					x += 10 + CFont::GetStringWidth(m_ChatWindowEntries[i].szNick, 0, 0);
				}

				RenderText(m_ChatWindowEntries[i].szMessage, x, y, m_ChatWindowEntries[i].uTextColor);
				x = 220.0f;

				break;
			case CHAT_TYPE_INFO:
			case CHAT_TYPE_DEBUG:
				RenderText(m_ChatWindowEntries[i].szMessage, x, y, m_ChatWindowEntries[i].uTextColor);
				break;
		}
		y += 20.0f;
	}
}

void CChatWindow::RenderText(uint16_t *sz, float x, float y, uint32_t uColor)
{
	CFont::SetOrientation(1);
	CFont::SetProportional(1);
	CFont::SetFontStyle(FONT_SUBTITLES);
	//uColor = RGBA_TO_ABGR(uColor);
	CFont::SetColor((uint8_t*)&uColor);
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

void CChatWindow::AddClientMessage(uint32_t uColor, char* szStr)
{
	uColor = (uColor >> 8) | 0xFF000000; // convert to ARGB

	FilterInvalidChars(szStr);
	AddToChatWindowBuffer(CHAT_TYPE_INFO,szStr,0,uColor,0);
}

void CChatWindow::AddChatMessage(char *szNick, uint32_t uNickColor, char *szMessage)
{
	FilterInvalidChars(szMessage);

	if(strlen(szMessage) > MAX_MESSAGE_LENGTH) return;

	AddToChatWindowBuffer(CHAT_TYPE_CHAT,szMessage,szNick,m_uChatTextColor,uNickColor);
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


inline void CChatWindow::PushBack()
{
	for (int x = MAX_MESSAGES-1; x; x--)
		memcpy(&m_ChatWindowEntries[x],&m_ChatWindowEntries[x-1],sizeof(CHAT_WINDOW_ENTRY));
}

void CChatWindow::AddToChatWindowBuffer(eChatMessageType eType, 
										char *szString, 
										char *szNick, 
										uint32_t 
										uTextColor, 
										uint32_t uNickColor)
{
	PushBack();

	m_ChatWindowEntries[0].eType = eType;
	m_ChatWindowEntries[0].uTextColor = uTextColor;
	m_ChatWindowEntries[0].uNickColor = uNickColor;

	if(szNick)
	{
		int len = strlen(szNick);
		char temp[len+2];
		strcpy(temp, szNick);
		strcat(temp, ":");
		CFont::AsciiToGxtChar(temp, m_ChatWindowEntries[0].szNick);
	}
	else
		m_ChatWindowEntries[0].szNick[0] = 0;

	if(m_ChatWindowEntries[0].eType == CHAT_TYPE_CHAT && strlen(szString) > MAX_LINE_LENGTH)
	{
		int iBestLineLength = MAX_LINE_LENGTH;
		// see if we can locate a space.
		while(szString[iBestLineLength] != ' ' && iBestLineLength)
			iBestLineLength--;

		if((MAX_LINE_LENGTH - iBestLineLength) > 12) {
			// we should just take the whole line
			char temp[MAX_LINE_LENGTH+1];
			strncpy(temp,szString,MAX_LINE_LENGTH);
			temp[MAX_LINE_LENGTH] = '\0';
			CFont::AsciiToGxtChar(temp, m_ChatWindowEntries[0].szMessage);

			PushBack();

			m_ChatWindowEntries[0].eType = eType;
			m_ChatWindowEntries[0].uTextColor = uTextColor;
			m_ChatWindowEntries[0].uNickColor = uNickColor;
			m_ChatWindowEntries[0].szNick[0] = 0;

			CFont::AsciiToGxtChar(szString+MAX_LINE_LENGTH, m_ChatWindowEntries[0].szMessage);
		}
		else {
			// grab upto the found space.
			char temp[iBestLineLength+1];
			strncpy(temp,szString,iBestLineLength);
			temp[iBestLineLength] = '\0';
			CFont::AsciiToGxtChar(temp, m_ChatWindowEntries[0].szMessage);

			PushBack();

			m_ChatWindowEntries[0].eType = eType;
			m_ChatWindowEntries[0].uTextColor = uTextColor;
			m_ChatWindowEntries[0].uNickColor = uNickColor;
			m_ChatWindowEntries[0].szNick[0] = 0;

			CFont::AsciiToGxtChar(szString+(iBestLineLength+1), m_ChatWindowEntries[0].szMessage);
		}
	}
	else
		CFont::AsciiToGxtChar(szString, m_ChatWindowEntries[0].szMessage);
}
