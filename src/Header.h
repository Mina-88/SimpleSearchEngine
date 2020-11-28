#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <map>
#include <unordered_set>

using namespace std;

struct Edge
{
	int src, dest;
};

struct keywordType
{
	string keyword;
	int type;
	bool quotations;
};


#endif
