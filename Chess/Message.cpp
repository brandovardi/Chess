#include "Message.h"

vector<string> Message::split(const string& s, char car)
{
	vector<string> splitted = {};
	istringstream startStr(s);
	string element = "";

	while (getline(startStr, element, car))
		splitted.push_back(element);

	return splitted;
}