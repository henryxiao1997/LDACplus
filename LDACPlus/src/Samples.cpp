/*
 * Copyright (C) 2014 by Jinghui Xiao (xiaojinghui1978@qq.com)
 *
 * LDACplus is a free software. I implement it for studying LDA 
 * and gibbs sampling. And I hope it could be useful. But there 
 * is no warranty. You can modify it or redistribut it under the 
 * the terms of the GNU General Public License. 
 */

#include "Samples.h"
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

WordIdAndTopic::WordIdAndTopic(void)
{
	iWordIndex = -1;
	iTopic = -1;
}

WordIdAndTopic::WordIdAndTopic(int iWordIndex, int iTopic)
{
	this->iWordIndex = iWordIndex;
	this->iTopic = iTopic;
}

WordIdAndTopic::~WordIdAndTopic(void)
{
}

Sample::Sample(void)
{
}


Sample::~Sample(void)
{
}

bool Sample::LoadSampleFrmLine (WordIndex & toWordIndex, string & sLine)
{
	if (!toWordIndex.IsAvailable())
		return false;
	stringstream isLine (sLine);
	string sWord;
	while (isLine >> sWord)
	{
		int iWordIndex = -1;
		if (toWordIndex.GetWordIndexFrmString (sWord,iWordIndex))
			ItemVec.push_back(WordIdAndTopic(iWordIndex, -1));
	}
	if (ItemVec.empty())
		return false;
	else
		return true;
}


Samples::Samples(void)
{
}


Samples::~Samples(void)
{
}

bool Samples::LoadSamplesFrmTxt (WordIndex & toWordIndex, const char * sFileName)
{
	ifstream in (sFileName);
	if(!in)
	{
		cerr << "can not open the file of " << sFileName << endl;
		return false;
	}
	SampleVec.clear();
	string sLine;
	while (getline (in, sLine))
	{
		Sample theSample;
		if (theSample.LoadSampleFrmLine (toWordIndex, sLine))
		{
			SampleVec.push_back(theSample);
		}
	}
	return true;
}

void Samples::SetRandomTopicOnSamples (int iTopicNum)
{
	vector<Sample>::iterator pSample = SampleVec.begin();
	while (pSample != SampleVec.end())
	{
		vector<WordIdAndTopic>::iterator p = pSample->ItemVec.begin();
		while (p != pSample->ItemVec.end())
		{
			p->iTopic = (int)(((double)rand()/(RAND_MAX+1)) * iTopicNum);
			p++;
		}
		pSample++;
	}
}
