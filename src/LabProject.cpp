#include "header.h"
#include "webGraph.h"

// a function to exit the search engine
void Exit(webGraph graph);

// a function that turns what the user typed into a valid query
vector<keywordType> stringToQuery(string searchText);

// a function to perform a new search
void newSearch(webGraph& graph);

// a function to open a certain webpage
void openWebpage(webGraph& graph, vector<string> results);

// a function that displays the choices when the user is currently viewing a webpage
void threeChoicesViewing(webGraph& graph, vector<string> results);

// a function which displays the choices when the user is currently looking at search results
void threeChoices(webGraph& graph, vector<string> results);

// a function which gives the user the options to either search or exit
void twoChoices(webGraph& graph);




int main()
{
	// first menu for the user to choose from
	cout << "Welcome!" << '\n';

	// constructing the graph
	webGraph graph("numberofImpressionsFile.csv", "webGraphFile.csv", "keyWordFile.csv", "numberofViewsFile.csv");

	// starting the search engine
	twoChoices(graph);

	return 0;
}



void Exit(webGraph graph)
{
	// updating impressions and views files before exiting
	graph.updateImpressions("numberofImpressionsFile.csv");
	graph.updateViews("numberofViewsFile.csv");

	// terminating the program
	cout << "Thanks for using our search engine!" << endl;
	exit(0);
}


vector<keywordType> stringToQuery(string searchText)
{
	vector<keywordType> search;

	int i = 0;
	int type = 0;
	int currWord = -1;
	string temp = "";
	bool flagQ = false;

	// change the words from string form to vector form
	while (i < searchText.size())
	{

		/*
		the idea here is that each word is either considered as an AND side or an OR side while taking care of quotations

		type 1 represents a word that is on the side of an AND
		type 0 represents a word that is on the side of an AND
		
		flagQ represents whether the word must be taken as it is or not ( not splitted and case sensitive )
		
		precedence of operations:
		1 - quotations
		2 - AND
		3 - OR
		*/
		if (searchText[i] == '"' && !flagQ) { flagQ = true; }
		else if (searchText[i] == '"' && flagQ) { search.push_back({ temp, type, flagQ }); temp = ""; flagQ = false; currWord++; i++; }
		else if (currWord != -1 && searchText[i] == 'A' && searchText[i + 1] == 'N' && searchText[i + 2] == 'D')
		{
			type = 1;
			search[currWord].type = type;
			i = i + 3;
		}
		else if (currWord != -1 && searchText[i] == 'O' && searchText[i + 1] == 'R') i = i + 2;
		else if (searchText[i] == ' ' && !flagQ) { search.push_back({ temp, type, flagQ }); currWord++; temp = ""; type = 0; }
		else if (i == searchText.size() - 1) { temp += searchText[i]; search.push_back({ temp, type, flagQ }); currWord++; }
		else
		{
			temp += searchText[i];
		}
		i++;

	}


	return search;
}

void newSearch(webGraph& graph)
{
	string search;

	// take the search string and change it into query
	cout << "Enter the search that you want to perform: ";
	getline(cin, search);
	vector<keywordType> searchQuery = stringToQuery(search);

	// display the results of the query
	vector<string> results = graph.displayResults(searchQuery);

	// if there are no results display no results found
	if (results.size() == 0)
	{
		cout << "no results were found!!" << endl;
		twoChoices(graph);
	}
	else
	{
		// display the results
		cout << "Search results: " << endl;
		for (int i = 0; i < results.size(); i++)
			cout << i + 1 << ". " << results[i] << endl;
		cout << "\n\n";

		threeChoices(graph, results);
	}
}

void openWebpage(webGraph& graph, vector<string> results)
{
	// if the user wants to open a website check if the choosen website is shown in results or not then view the website

	int page;
	cout << "which webpage would you like to open? enter your choice: ";
	cin >> page;

	while (page > results.size())
	{
		cout << "enter a valid choice: " << endl;
		cin >> page;
	}
	graph.viewSite(results[page - 1]);

	threeChoicesViewing(graph, results);
}

void threeChoicesViewing(webGraph& graph, vector<string> results)
{
	int choice;

	cout << "would you like to: " << endl;
	cout << "1. Return to search results" << endl;
	cout << "2. New search" << endl;
	cout << "3. Exit" << endl << endl;

	cout << "type in your choice: ";
	cin >> choice;
	cout << endl;

	if (choice == 1)
	{
		cin.ignore();
		cout << "Search results: " << endl;
		for (int i = 0; i < results.size(); i++)
			cout << i + 1 << ". " << results[i] << endl;
		cout << "\n\n";
		threeChoices(graph, results);
	}
	else if (choice == 2)
	{
		cin.ignore();
		newSearch(graph);
	}
	else Exit(graph);
}

void threeChoices(webGraph& graph, vector<string> results)
{
	int choice;

	cout << "would you like to: " << endl;
	cout << "1. Choose a webpage to open" << endl;
	cout << "2. New search" << endl;
	cout << "3. Exit" << endl << endl;

	cout << "type in your choice: ";
	cin >> choice;
	cout << endl;

	if (choice == 1)
	{
		cin.ignore();
		openWebpage(graph, results);
	}
	else if (choice == 2)
	{
		cin.ignore();
		newSearch(graph);
	}
	else Exit(graph);
}

void twoChoices(webGraph& graph)
{
	int choice;

	cout << "What would you like to do?" << endl;
		cout << "1. Search" << '\n' << "2. Exit" << "\n";
	cout << "Type in your choice: ";
	cin >> choice;

	while (choice != 1 && choice != 2)
	{
		cout << "please enter a right choice (1 or 2)" << endl;
		cout << "1. Search" << '\n' << "2. Exit" << "\n\n";

		cout << "Type in your choice: ";
		cin >> choice;
		cout << endl;
	}
	cin.ignore();

	if (choice == 1)
	{
		newSearch(graph);
	}
	else { Exit(graph); }
}


