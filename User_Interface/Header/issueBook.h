#pragma once
#include <iostream>
#include <tuple>
#include <string>
#include <fstream>
#include <ctime>


class IssueBook
{
private:
	int userIdLength = 9;
	std::string fileSaveLocation{ "Text_Files/Issue_Record.txt" };
public:
	bool finalizeProcess(std::tuple<std::string, unsigned long long> *userIssuing, std::string bookRequired);
	void returnBook(std::tuple<std::string, unsigned long long> *requiredUser);
};

