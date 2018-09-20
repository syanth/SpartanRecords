#include "SendData.h"
#include "stdafx.h"

#pragma comment(lib, "winhttp.lib")

using namespace std;

struct PlayerData {
	std::string name;
	short kills, deaths, assists;
};

void SendData(string data)
{
	LPSTR pszData = const_cast<char *>(data.c_str());
	DWORD dwBytesWritten = 0;

	BOOL  bResults = FALSE;
	HINTERNET hSession = NULL,
		hConnect = NULL,
		hRequest = NULL;

	// Use WinHttpOpen to obtain a session handle.
	hSession = WinHttpOpen(L"A WinHTTP Example Program/1.0",
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS, 0);

	// Specify an HTTP server.
	if (hSession)
		hConnect = WinHttpConnect(hSession, L"syanth.pythonanywhere.com",
			80, 0);

	// Create an HTTP Request handle.
	if (hConnect)
		hRequest = WinHttpOpenRequest(hConnect, L"POST",
			L"/receive-data",
			NULL, WINHTTP_NO_REFERER,
			WINHTTP_DEFAULT_ACCEPT_TYPES,
			0);

	// Send a Request.
	if (hRequest)
		bResults = WinHttpSendRequest(hRequest,
			WINHTTP_NO_ADDITIONAL_HEADERS,
			0, WINHTTP_NO_REQUEST_DATA, 0,
			(DWORD)strlen(pszData), 0);

	// Write data to the server.
	if (bResults)
		bResults = WinHttpWriteData(hRequest, pszData,
		(DWORD)strlen(pszData),
			&dwBytesWritten);

	// End the request.
	if (bResults)
		bResults = WinHttpReceiveResponse(hRequest, NULL);

	//// Report errors.
	//if (!bResults)
	//	printf("Error %d has occurred.\n", GetLastError());

	// Close open handles.
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);
}

void addStream(stringstream &ss, string key, int value) {
	ss << "\"" << key << "\":" << value;
}

string formatJson(struct PlayerData & data) {
	stringstream jstream;
	jstream << "{ ";
	jstream << "\"" << "name" << "\": \"" << data.name << "\"";
	jstream << ", ";
	addStream(jstream, "kills", data.kills);
	jstream << ", ";
	addStream(jstream, "deaths", data.deaths);
	jstream << ", ";
	addStream(jstream, "assists", data.assists);
	jstream << " }";
	return jstream.str();
}

string getPlayerName(HANDLE han, char *address) {
	char temp = 'a';
	string out = "";
	for (int i = 0; i < 24; i += 2) {
		ReadProcessMemory(han, (void*)(address + i), &temp, sizeof(temp), 0);
		if (temp == NULL) break;
		out += temp;
	}
	return out;
}