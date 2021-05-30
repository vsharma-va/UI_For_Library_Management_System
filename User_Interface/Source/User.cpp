#include "../Header/User.h"

unsigned long long User::addUserToDatabase()
{
	std::string tempId{ "" };
	if (!inDatabase)
	{
		std::fstream userDatabase;
		for (int i = 0; i < 10; ++i)
		{
			std::random_device rd;
			std::mt19937 e{ rd() };
			std::uniform_int_distribution<int> dist{ 1, userIdLength };
			tempId += std::to_string(dist(e));
		}
		userDatabase.open(fileSaveLocation, std::ios::app);
		if (userDatabase.is_open())
		{
			userId = std::stoull(tempId);
			userDatabase << firstName << " " << userId << '\n';
			std::cout << "User successfully added to the database" << std::endl;
			std::cout << "This is your user id number. Don't not forget this: " << tempId << std::endl;
		}
		userDatabase.close();
	}
	return userId;
}

std::tuple<std::string, unsigned long long> User::findUserInDatabase()
{
	std::fstream userDatabase;
	std::tuple<std::string, unsigned long long> requiredUser;
	bool userFound = false;
	userDatabase.open(fileSaveLocation, std::ios::in);
	if (userDatabase.is_open())
	{
		std::string line;
		while (std::getline(userDatabase, line))
		{
			size_t delimiter = line.find_last_of(" ");
			std::string userName = line.substr(0, delimiter);
			unsigned long long id = std::stoull(line.substr(delimiter, line.length() - 1));
			if ((userName == firstName) && (id == userId))
			{
				requiredUser = std::make_tuple(userName, id);
				userFound = true;
				break;
			}
		}
	}
	if (!userFound)
	{
		requiredUser = std::make_tuple(std::string("false"), 0);
	}
	return requiredUser;
}
