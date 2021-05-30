#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <tuple>


class CreateAndSearchCatalog
{
private:
	std::string userSearch;
public:
	CreateAndSearchCatalog(std::string search)
	{
		userSearch = search;  // book user has asked to search
	}
	CreateAndSearchCatalog() = default;
	// if it is your first time set reload to true when running it will create the
	// necessary files for the catalog. Then you can change to false for faster compilation.
	static std::tuple<std::vector<std::string>, std::vector<int>> checkData(bool reload);  
	static void clearCatalog();
	// takes the catalog from checkData by reference/pointer
	std::vector<std::string> searchBook(std::tuple<std::vector<std::string>, std::vector<int>> *catalog);
	// will do what is written inside the main function
	bool displaySearchResults(std::tuple<std::vector<std::string>, std::vector<std::string>> *searchResult);
	std::string finalizeBook(std::tuple<std::vector<std::string>, std::vector<std::string>> *searchResult, std::string group, int bookPos);
};

