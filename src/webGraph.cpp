#include "webGraph.h"


webGraph::webGraph(string impressionsFile, string edgesFile, string keyWordsFile, string viewsFile)
{
	// initialize vectors
	websiteToIndex(impressionsFile, viewsFile);
	vector<Edge> edges = siteToIndexEdges(edgesFile);

	size = websites.size();

	keyWords.resize(size);
	keywordsSets(keyWordsFile);


	reverseAdjList.resize(size);
	this->websites = websites;
	this->websites_map = websites_map;
	atThis.resize(size);
	fromThis.resize(size);
	views.resize(size);


	// create reversed adj list
	for (auto& edge : edges) {

		reverseAdjList[edge.dest].push_back(edge.src);
		atThis[edge.dest]++;
		fromThis[edge.src]++;

	}

	ranks.resize(size);
	fill(ranks.begin(), ranks.end(), (1.0 / size));
	ranksI.resize(size);

	// do the initial page rank
	pageRank();

}

void webGraph::websiteToIndex(string fileName, string viewsFile)
{
	// read the existing websites and the number of views and impressions for each
	ifstream websites_file;
	websites_file.open(fileName);

	ifstream views_file;
	views_file.open(viewsFile);

	int currIndex = 0;

	if (websites_file.is_open() && views_file.is_open())
	{
		string site;
		string impression;

		string siteV;
		string view;

		getline(websites_file, site, ',');
		getline(websites_file, impression);

		getline(views_file, siteV, ',');
		getline(views_file, view);

		while (!websites_file.eof())
		{
			if (!websites_map.count(site))
			{
				//cout << "the current site is " << site << endl;
				websites_map[site] = currIndex;
				websites.push_back(site);
				impressions.push_back(stoi(impression));
				views.push_back(stoi(view));
				currIndex++;
			}
			else continue;

			getline(websites_file, site, ',');
			getline(websites_file, impression);

			getline(views_file, siteV, ',');
			getline(views_file, view);
		}

	}
	else cout << "error openning the file!!" << endl;



	websites_file.close();
	views_file.close();
}

vector<Edge> webGraph::siteToIndexEdges(string fileName)
{
	// create the edges vector from the files in the right format then return it

	vector<Edge> edges;

	ifstream websites_file;
	websites_file.open(fileName);

	if (websites_file.is_open())
	{
		string src, dest;

		getline(websites_file, src, ',');
		getline(websites_file, dest);

		while (!websites_file.eof())
		{
			edges.push_back({ websites_map[src], websites_map[dest] });

			getline(websites_file, src, ',');
			getline(websites_file, dest);

		}


	}
	else cout << "error openning the file!!" << endl;

	websites_file.close();

	return edges;
}

void webGraph::keywordsSets(string fileName)
{
	ifstream keyWordsF;
	keyWordsF.open(fileName);

	if (keyWordsF.is_open())
	{
		// for each website
		for (int i = 0; i < size; i++)
		{
			string site;
			string words;

			getline(keyWordsF, site, ',');
			getline(keyWordsF, words);

			string temp = "";
			string tempUp = "";

			// for each character in the keywords string
			for (int j = 0; j < words.size() - 1; j++)
			{
				// if it is not a new word add the character
				if (words[j] != ',') temp += words[j];
				else
				{
					// if it is a comma add the previous word
					keyWords[websites_map[site]].insert(temp);
					tempUp = temp;
					// add this to handle the case insensitive searching (more elaboration in the documentation file)
					transform(tempUp.begin(), tempUp.end(), tempUp.begin(), ::toupper);
					keyWords[websites_map[site]].insert("?case?" + tempUp + "?insensitive?");
					temp = "";
				}
			}
			// same for the last word
			temp += words[words.size() - 1];
			keyWords[websites_map[site]].insert(temp);
			tempUp = temp;
			transform(tempUp.begin(), tempUp.end(), tempUp.begin(), ::toupper);
			keyWords[websites_map[site]].insert("?case?" + tempUp + "?insensitive?");
			temp = "";
		}
	}
	else cout << "error openning the file!!" << endl;

	keyWordsF.close();
}

void webGraph::pageRank()
{
	// rank pages in omega(n) and O(n^2)
	// we do 100 iterations as it will have less and less margin of error
	for (int it = 1; it < 100; it++)
	{
		vector<double> tempRanks(size, 0);
		// for every webpage
		for (int i = 0; i < size; i++)
		{
			// for every webpage pointing at it
			for (int j = 0; j < atThis[i]; j++)
			{
				int currInd = reverseAdjList[i][j];
				tempRanks[i] += (ranks[currInd] / fromThis[currInd]);

			}
		}
		ranks = tempRanks;
	}

}


vector<int> webGraph::searchWeb(vector<keywordType> query)
{
	vector<int> websitesIndices;

	// for each website
	for (int i = 0; i < size; i++)
	{
		bool andFlag = false;
		// for each word in the query
		for (int j = 0; j < query.size(); j++)
		{
			// create an upper case version of that word to check in the case-insensitive case
			string q1 = query[j].keyword;
			transform(q1.begin(), q1.end(), q1.begin(), ::toupper);

			// if it is an AND
			if (query[j].type == 1 && andFlag == false) andFlag = true;
			else if (andFlag)
			{

				string q2 = query[j - 1].keyword;
				transform(q1.begin(), q1.end(), q1.begin(), ::toupper);
				transform(q2.begin(), q2.end(), q2.begin(), ::toupper);
				// search as both case insensitive AND
				if (keyWords[i].count("?case?" + q1 + "?insensitive?") && keyWords[i].count("?case?" + q2 + "?insensitive?") && !query[j].quotations && !query[j-1].quotations)
				{
					websitesIndices.push_back(i);
					break;
				}
				else if (keyWords[i].count(query[j].keyword) && keyWords[i].count(query[j - 1].keyword) && query[j].quotations && query[j - 1].quotations)
				{
					// both case-sensitive, AND
					websitesIndices.push_back(i);
					break;
				}
				else if 
					(
					(query[j].quotations && keyWords[i].count(query[j].keyword) && keyWords[i].count("?case?" + q2 + "?insensitive?"))
					||
					(query[j-1].quotations && keyWords[i].count("?case?" + q1 + "?insensitive?") && keyWords[i].count(query[j - 1].keyword))
					)
				{
					// one is case sensitive while the other is not
					websitesIndices.push_back(i);
					break;
				}
			}
			else if (query[j].quotations && !query[j].type)
			{
				// if there are quotations and it is an or so check while case sensitive
				if (keyWords[i].count(query[j].keyword))
				{
					websitesIndices.push_back(i);
					break;
				}
			}
			else
			{
				// if it is a normal word (OR) with no quotations
				if (keyWords[i].count("?case?" + q1 + "?insensitive?"))
				{
					websitesIndices.push_back(i);
					break;
				}
			}
		}
	}

	// return the indices of the websites that satisfy the conditions
	return websitesIndices;
}

void webGraph::calcScore()
{
	// calculate the current page score for all pages
	for (int i = 0; i < size; i++)
	{
		double pImpressions = 0.1 * impressions[i];
		double ctr;
		if (impressions[i] == 0) ctr = 0;
		else ctr = views[i] / impressions[i];
		ranksI[i] = 0.4 * ranks[i] + ((1 - ((pImpressions) / (1 + pImpressions))) * ranks[i] + ((pImpressions) / (1 + pImpressions)) * ctr) * 0.6;
	}
}

void webGraph::updateImpressions(string name)
{

	// update impressions in the file
	ofstream out;
	out.open(name);

	for (int i = 0; i < websites.size(); i++)
	{
		out << websites[i] << ',' << impressions[i] << '\n';
	}
	
	out.close();
}

void webGraph::updateViews(string name)
{
	// update impressions in the file
	ofstream out;
	out.open(name);

	for (int i = 0; i < websites.size(); i++)
	{
		out << websites[i] << ',' << views[i] << '\n';
	}

	out.close();
}

vector<string> webGraph::displayResults(vector<keywordType> query)
{
	// get the indices that satisfy the query then calc the current page scores
	// for the websites that satisfy the query sort them according to the score

	vector<int> indices = searchWeb(query);

	calcScore();

	vector<string> results;

	while (indices.size() > 0)
	{

		int maxInd = indices[0];
		double maxScore = ranksI[indices[0]];
		int toRemove = 0;

		for (int j = 1; j < indices.size(); j++)
		{
			if (ranksI[indices[j]] > maxScore)
			{
				maxScore = ranksI[indices[j]];
				maxInd = indices[j];
				toRemove = j;
			}
		}
		indices.erase(indices.begin() + toRemove);
		results.push_back(websites[maxInd]);
		impressions[maxInd]++;
	}

	// return the names of the websites sorteds according to the score
	return results;

	

}

void webGraph::viewSite(string siteName)
{
	// view the website and update its views
	cout << "you are now viewing " << siteName << endl;
	views[websites_map[siteName]]++;
}