#include "../Header/issueBook.h"


bool IssueBook::finalizeProcess(std::tuple<std::string, unsigned long long> *userRequired, std::string bookRequired)
{
	std::fstream issueRecord;
	bool alreadyIssued = false;
	issueRecord.open(fileSaveLocation, std::ios::in);
	if (issueRecord.is_open())
	{
		std::string line;
		while (getline(issueRecord, line))
		{
			size_t secondDelimiter = line.find_last_of(" ");
			size_t firstDelimiter = line.find_first_of(" ");
			size_t thirdDelimiter = firstDelimiter + userIdLength + 2;
			std::string bookName = line.substr(thirdDelimiter, line.length() - 1);
			std::string userName = line.substr(0, firstDelimiter);
			unsigned long long id = std::stoull(line.substr(firstDelimiter, secondDelimiter));

			if ((userName == std::get<0>(*userRequired)) && id == std::get<1>(*userRequired))
			{
				std::cout << "You have already issued " << bookName << " . Please return the book to issue another." << '\n';
				alreadyIssued = true;
				break;
			}
		}
		issueRecord.close();
		
		if (!alreadyIssued)
		{
			std::fstream issueRecord;
			issueRecord.open(fileSaveLocation, std::ios::app);
			issueRecord << std::get<0>(*userRequired) << " " << std::get<1>(*userRequired) << " " << bookRequired << '\n';
			std::cout << "Successfully issued the book" << std::endl;
			std::cout << std::endl;
			issueRecord.close();	
		}

	}
	return alreadyIssued;
}

void IssueBook::returnBook(std::tuple<std::string, unsigned long long> *requiredUser)
{
	std::fstream issueRecord;
	std::fstream tempFile;
	tempFile.open("Text_Files/Temp.txt", std::ios::out | std::ios::trunc);
	issueRecord.open(fileSaveLocation, std::ios::in);
	if (issueRecord.is_open())
	{
		std::string line;
		while (getline(issueRecord, line))
		{
			size_t secondDelimiter = line.find_last_of(" ");
			size_t firstDelimiter = line.find_first_of(" ");
			size_t thirdDelimiter = firstDelimiter + userIdLength + 2;
			std::string bookName = line.substr(thirdDelimiter, line.length() - 1);
			std::string userName = line.substr(0, firstDelimiter);
			unsigned long long userId = std::stoull(line.substr(firstDelimiter, thirdDelimiter));
			if (userId != std::get<1>(*requiredUser))
			{
				tempFile << userName << " " << std::to_string(userId) << bookName << '\n';
			}
		}
	}
	issueRecord.close();
	tempFile.close();
	auto fileLocation = fileSaveLocation.c_str();
	remove(fileLocation);
	rename("Text_Files/Temp.txt", "Text_Files/Issue_Record.txt");
}