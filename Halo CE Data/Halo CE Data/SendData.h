#pragma once
#ifndef SENDDATA_H
#define SENDDATA_H

#include "stdafx.h"


void SendData(std::string data);
void addStream(std::stringstream &ss, std::string key, int value);
std::string formatJson(struct PlayerData & data);
std::string getPlayerName(HANDLE han, char *address);

struct PlayerData {
	std::string name;
	short kills, deaths, assists;
};

#endif