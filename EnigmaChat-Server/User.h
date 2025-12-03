#pragma once
#define USER_H
#include <windows.h>

class User {
	char* UserName;
	WORD UserColor;

	void setUserColor(WORD col);
	void setUserName(char* name);


};

