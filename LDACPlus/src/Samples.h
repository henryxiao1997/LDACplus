/*
 * Copyright (C) 2014 by Jinghui Xiao (xiaojinghui1978@qq.com)
 *
 * LDACplus is a free software. I implement it for studying LDA 
 * and gibbs sampling. And I hope it could be useful. But there 
 * is no warranty. You can modify it or redistribut it under the 
 * the terms of the GNU General Public License. 
 */

#pragma once

#include "WordIndex.h"

using namespace std;

/*
 * The class of word index and its topic
 */

class WordIdAndTopic
{
public:
	WordIdAndTopic(void);
	WordIdAndTopic(int iWordIndex, int iTopic);
	~WordIdAndTopic(void);

public:
	int iWordIndex;
	int iTopic;
};

/*
 * The sample class
 */

class Sample
{
public:
	Sample(void);
	~Sample(void);

	// make a sample by spliting the line and getting words to fill WordIdVec
	// return false if the resulted WordIdVec is empty
	bool LoadSampleFrmLine (WordIndex & toWordIndex, string & sLine);

public:
	// a sample is a document containing a series of words
	vector<WordIdAndTopic> ItemVec;
};

/*
 * The class of the samples for training
 */

class Samples
{
public:
	Samples(void);
	~Samples(void);

	// load the sample from text file
	bool LoadSamplesFrmTxt (WordIndex & toWordIndex, const char * sFileName);
	// set the random topic on samples, for gibbs sampling
	void SetRandomTopicOnSamples (int iTopicNum);

public:
	vector<Sample> SampleVec;
};

