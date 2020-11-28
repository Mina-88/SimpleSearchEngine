#ifndef WEBGRAPH_H
#define WEBGRAPH_H
#include "header.h"

class webGraph
{
private:
	// the adjacency list of the webgraph reversed to calculate page rank as 
	// the original adj list is not used in any other function so it was ommitted 
	// note: if we want to get the original we can transpose the current reversed one.
	vector<vector<int>> reverseAdjList;

	// the number of websites in our database
	int size;

	// the initial ranks of the pages according to PageRank algorithm
	vector <double> ranks;

	// the scores of each page after calculating score with regard to initial page rank and CTR
	vector <double> ranksI;

	// number of views of each page
	vector <int> views;

	// number of websites pointing at each page
	vector<int> atThis;

	//number of websites this page is pointing at
	vector<int> fromThis;

	// the names of the websites in the database
	vector<string> websites;

	// number of impressions for each website
	vector<int> impressions;

	// a hash map that maps the name of website to its index in the other arrays
	map<string, int> websites_map;

	// keywords present in each website according to their index
	vector<unordered_set<string>> keyWords;


public:

	// the constructor creates the websites vector, views, impressions using helper functions and other helping vectors like atThis or fromThis
	// it then construct the reversed adjacency list, then call the initial page ranking algorithm
	webGraph(string impressionsFile, string edgesFile, string keyWordsFile, string viewsFile);

	// the function that takes the websites and then map them to certain indices in the vectors while adding their views and impressions
	void websiteToIndex(string fileName, string viewsFile);
	
	// a function that takes edges from a file and put them into a vector of the right format
	vector<Edge> siteToIndexEdges(string fileName);

	// a function that takes the keyWords of each website and map them to that website accordingly
	void keywordsSets(string fileName);
	
	// the initial page ranking algorithm
	void pageRank();
	
	// a function that searches the database according to the given search query
	// returns the indices of the websites satisfying the query
	vector<int> searchWeb(vector<keywordType> query);

	// a function to calculate the current page score of each page
	void calcScore();

	// a function that updates the impressions in its original file
	void updateImpressions(string name);

	// a function that updates the views in its original file
	void updateViews(string name);
	
	// a function that returns the results of a query as names of websites 
	vector<string> displayResults(vector<keywordType> query);
	
	// a function to display a certain website and update its current views number
	void viewSite(string siteName);
};

#endif