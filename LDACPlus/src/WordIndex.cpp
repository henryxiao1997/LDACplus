/*
 * Copyright (C) 2014 by Jinghui Xiao (xiaojinghui1978@qq.com)
 *
 * LDACplus is a free software. I implement it for studying LDA 
 * and gibbs sampling. And I hope it could be useful. But there 
 * is no warranty. You can modify it or redistribut it under the 
 * the terms of the GNU General Public License. 
 */

#include "WordIndex.h"
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

WordIndex::WordIndex(void)
{
}


WordIndex::~WordIndex(void)
{
}

bool WordIndex::LoadLexicon (const char* sFileName)
{
	ifstream in (sFileName);
	if (!in)
	{
		cerr << "can not open the file of " << sFileName << endl;
		return false;
	}
	string sWord;
	while (in >> sWord)
		LexVec.push_back(sWord);
	sort(LexVec.begin(),LexVec.end());
	return true;
}

bool WordIndex::IsAvailable (void)
{
	if (LexVec.empty())
		return false;
	else
		return true;
}

int WordIndex::GetWordNum (void)
{
	return (int)(LexVec.size());
}

bool WordIndex::GetWordIndexFrmString(string & sWord, int & iWordIndex)
{
	if (LexVec.empty())
		return false;
	vector<string>::iterator p = lower_bound(LexVec.begin(), LexVec.end(), sWord);
	if ((p!=LexVec.end()) && (*p==sWord))
	{
		iWordIndex = (int)(p - LexVec.begin());
		return true;
	}
	else
		return false;
}

bool WordIndex::GetWordStringFrmIndex(int iWordIndex, string & sWord)
{
	if ((iWordIndex < 0) || (iWordIndex>=(int)LexVec.size()))
		return false;
	sWord = LexVec.at(iWordIndex);
	return true;
}
