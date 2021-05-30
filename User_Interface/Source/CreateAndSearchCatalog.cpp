#include "../Header/CreateAndSearchCatalog.h"




std::tuple<std::vector<std::string>, std::vector<int>> CreateAndSearchCatalog::checkData(bool reload)
{
	{
		std::string fileSaveLocation{ "Text_Files/Catalog.txt" };
		if (reload == true)
		{
			std::fstream catalog;
			std::vector<std::string> books{};
			std::vector<int> bookFrequency{};
			bool done = false;
			unsigned int individualCounter{ 0 };
			unsigned int allBooks{ 0 };
			unsigned int frequency{ 0 };

			catalog.open("Resources/catalog.csv", std::ios::in);
			if (catalog.is_open())
			{
				std::string bookName;
				while (getline(catalog, bookName))
				{
					books.push_back(bookName);  // stores the book name in books vector
				}
				catalog.close();
			}

			size_t lengthVector = books.size();

			while (!done)
			{
				// counts the number of books of the same type and removes all the books which are repeated
				if (books.at(individualCounter) == books.at(allBooks))
				{
					frequency++;
					books.erase(books.begin() + allBooks);
					lengthVector = books.size();
				}
				allBooks++;
				if (allBooks == lengthVector)
				{
					bookFrequency.push_back(frequency);  // stores in the bookFrequency vector
					individualCounter++;
					allBooks = 0;
					frequency = 0;
				}

				if (individualCounter == lengthVector)
				{
					done = true;
					continue;
				}
			}

			std::tuple<std::vector<std::string>, std::vector<int>> booksAndFrequency(books, bookFrequency);  // creates a tuple with bookname, frequency

			// writes to the tuple to a text file 
			std::fstream write; 
			write.open(fileSaveLocation, std::ios::app);
			write.clear();
			if (write.is_open())
			{
				for (size_t i{ 0 }; i < lengthVector; ++i)
				{
					write << books.at(i) << " " << bookFrequency.at(i) << '\n';
				}
			}

			return booksAndFrequency;
			
		}
		else
		{
			// if the text file already exsits then this reads the data from the text file and stores it in books and bookFrequency vectors
			// then creates a tuple to return
			std::string lines;
			std::vector<int> bookFrequency{};
			std::vector<std::string> books{};
			std::fstream catalog;
			catalog.open(fileSaveLocation, std::ios::in);
			if (catalog.is_open())
			{
				while (getline(catalog, lines))
				{
					size_t length = lines.length();
					size_t delimiter = lines.find_last_of(" ");
					int frequency = std::stoi(lines.substr(length - 1, delimiter));
					// std::cout << frequency << '\n';
					std::string bookName = lines.substr(0, delimiter);
					bookFrequency.push_back(frequency);
					books.push_back(bookName);
				}
			}
			std::tuple<std::vector<std::string>, std::vector<int>> booksAndFrequency(books, bookFrequency);
			return booksAndFrequency;
		}
	}
}

void CreateAndSearchCatalog::clearCatalog()
{
	std::fstream clear;
	clear.open("Text_Files/Catalog.txt", std::ios::out | std::ios::trunc);
	clear.close();
}



std::vector<std::string> CreateAndSearchCatalog::searchBook(std::tuple<std::vector<std::string>, std::vector<int>>* catalog)
{
	// searches for the user requested book
	std::vector<std::string> matches{};
	for (std::string book : std::get<0>(*catalog))
	{
		bool match = false;
		int where = book.find(userSearch);
		if (userSearch == book)
		{
			match = true;
			matches.push_back(book);
		}
		else if ((where >= 0) && (!match))
		{
			match = false;
			matches.push_back(book);
		}
	}
	return matches;
}


bool CreateAndSearchCatalog::displaySearchResults(std::tuple<std::vector<std::string>, std::vector<std::string>> *searchResult)
{
	bool empty = true;
	int counter{ 0 };
	for (std::string perfect : std::get<0>(*searchResult))
	{
		if (counter == 0)
		{
			std::cout << "Perfect Match -------------------------------------->" << '\n';
			std::cout << '\n';
		}
		std::cout << counter <<  ". " <<perfect << '\n';
		counter++;
		empty = false;
	}
	
	counter = 0;

	for (std::string partial : std::get<1>(*searchResult))
	{
		if (counter == 0)
		{
			std::cout << "Partial Match ---------------------------------------->" << '\n' << '\n';
		}
		std::cout << counter << ". " << partial << '\n';
		counter++;
		empty = false;
	}

	if (empty)
	{
		std::cout << "No such book in the library" << std::endl;
	}
	return empty;
}


std::string CreateAndSearchCatalog::finalizeBook(std::tuple<std::vector<std::string>, std::vector<std::string>> *searchResult, std::string group, int bookPos)
{
	std::string bookRequired;
	if (group == "Pe" || group == "pe")
	{
		bookRequired = std::get<0>(*searchResult).at(bookPos);
	}
	else if (group == "Pa" || group == "pa")
	{
		bookRequired = std::get<1>(*searchResult).at(bookPos);
	}
	return bookRequired;
}

