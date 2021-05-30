#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <tuple>

class User
{
private:
	bool inDatabase;
	std::string firstName;
	std::string lastName;
	unsigned long long userId;
	int userIdLength = 9;
	std::string fileSaveLocation{ "Text_Files/Users.txt" };
public:
	User(bool registered, std::string first, std::string last, unsigned long long id = 0)
	{
		inDatabase = registered;
		firstName = first;
		lastName = last;
		userId = id;
	}
	unsigned long long addUserToDatabase();
	std::tuple<std::string, unsigned long long> findUserInDatabase();
};