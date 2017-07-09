#pragma once

#define MAX_MESSAGE_LENGTH 		128
#define MAX_LINE_LENGTH			MAX_MESSAGE_LENGTH/2
#define MAX_MESSAGES			50

enum eChatMessageType
{
	CHAT_TYPE_NONE = 0,
	CHAT_TYPE_CHAT,
	CHAT_TYPE_INFO,
	CHAT_TYPE_DEBUG
};

typedef struct _CHAT_WINDOW_ENTRY
{
	eChatMessageType eType;
	char szMessage[MAX_MESSAGE_LENGTH+1];
	char szNick[MAX_PLAYER_NAME+1];
	uint32_t uTextColor;
	uint32_t uNickColor;
} CHAT_WINDOW_ENTRY;

class CChatWindow
{
public:
	CChatWindow::CChatWindow();
	CChatWindow::~CChatWindow();

	void FilterInvalidChars(char* szString);
	void AddToChatWindowBuffer(eChatMessageType eType, char* szString, char* szNick, uint32_t uTextColor, uint32_t uNickColor);

	void Draw();
	void RenderText(char *sz, float x, float y, uint32_t uColor);

	void AddChatMessage(char *szNick, uint32_t dwNickColor, char *szMessage);
	void AddInfoMessage(char *szFormat, ...);
	void AddDebugMessage(char *szFormat, ...);
	void AddClientMessage(uint32_t dwColor, char* szStr);

private:
	int m_iCount;
	CHAT_WINDOW_ENTRY m_ChatWindowEntries[MAX_MESSAGES];

	uint32_t m_uChatTextColor;
	uint32_t m_uChatInfoColor;
	uint32_t m_uChatDebugColor;
};