#pragma once

#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

class Message
{
private:

public:
	Message() {}
	static vector<string> split(const string& s, char delimiter);
};

