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
#include "Samples.h"

using namespace std;

/*
 * The GibbsSettings class
 * Containing the parameters of LDA for gibbs sampling
 */

class GibbsSettings
{
public:
	GibbsSettings(void);
	GibbsSettings(GibbsSettings & theSettings);
	~GibbsSettings(void);
	void SetGibbsSettings (GibbsSettings & theSettings);

public:
	// LDA hyperparameters
	double dAlpha, dBeta;
	// the topic number
	int iTopicNum;
	// the iteration number
	int iIterNum;
};

/*
 * The LDACplus class
 * Implement the LDA model with gibbs sampling
 */

class LDACplus
{
public:
	LDACplus(void);
	~LDACplus(void);

	// init for estimating parameters by gibbs sampling
	bool InitForEstimating (GibbsSettings & theGibbsSettings, 
		const char * sLexFileName, const char * sSampleFileName);
	// the training funciton: estimating the parameters of LDA by gibbs sampling
	void EstimatingLDAByGibbsSampling (void);
	// save p(word|topic) and p(topic|doc)
	bool SaveLDAModel (const char * sFileTopicWord, const char * sFileDocTopic);

private:
	// called by EstimatingLDAByGibbsSampling, return the new topic
	int SamplingForOneItem (int iSampleIndex, int iWordIndex, int iTopic);
	// compute the p(word|topic) in TopicWordProbMatrix
	void ComputeTopicWordProbMatrix (void);
	// compute the p(topic|doc) in DocTopicProbMatrix
	void ComputeDocTopicProbMatrix (void);

private:
	WordIndex toWordIndex;
	Samples theTrainingSamples;

	//// the parameters of model
	GibbsSettings theSettings;

	//// the data structures for gibbs sampling
	// WordTopicCount[i][j]: the count that word_i is assigned to topic_j, size of V * K
	vector<vector<int> > WordTopicMatrix; 
	// DocTopicMatrix[i][j]: the count that document_i is assigned to topic_j, size of M * K
	vector<vector<int> > DocTopicMatrix;
	// SumWordTopicVec[j]: the total count that words are assigned to topic_j, size of K
	vector<int> SumWordTopicVec;
	// SumWordDocVec[j]: the total count that words are in document_j, size of M
	vector<int> SumWordDocVec;
	// TopicWordProbMatrix[i][j]: p(word_j|topic_i), size of K * V
	vector<vector<double> > TopicWordProbMatrix;
	// DocTopicProbMatrix[i][j]: p(topic_j|document_i), size of M * K
	vector<vector<double> > DocTopicProbMatrix;
	
};

