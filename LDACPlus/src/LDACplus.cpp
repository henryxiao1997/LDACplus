/*
 * Copyright (C) 2014 by Jinghui Xiao (xiaojinghui1978@qq.com)
 *
 * LDACplus is a free software. I implement it for studying LDA 
 * and gibbs sampling. And I hope it could be useful. But there 
 * is no warranty. You can modify it or redistribut it under the 
 * the terms of the GNU General Public License. 
 */

#include "LDACplus.h"
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

GibbsSettings::GibbsSettings(void)
{
	// set the default values
	iTopicNum = 100;
	dAlpha = 50.0 / iTopicNum;
	dBeta = 0.01;
	iIterNum = 200;
}

GibbsSettings::GibbsSettings (GibbsSettings & theSettings)
{
	iTopicNum = theSettings.iTopicNum;
	dAlpha = theSettings.dAlpha;
	dBeta = theSettings.dBeta;
	iIterNum = theSettings.iIterNum;
}

GibbsSettings::~GibbsSettings(void)
{
}

void GibbsSettings::SetGibbsSettings (GibbsSettings & theSettings)
{
	iTopicNum = theSettings.iTopicNum;
	dAlpha = theSettings.dAlpha;
	dBeta = theSettings.dBeta;
	iIterNum = theSettings.iIterNum;
}

LDACplus::LDACplus(void)
{
}


LDACplus::~LDACplus(void)
{
}

bool LDACplus::InitForEstimating(GibbsSettings & theGibbsSettings, 
		const char * sLexFileName, const char * sSampleFileName)
{
	if (!toWordIndex.LoadLexicon (sLexFileName))
		return false;
	if (!theTrainingSamples.LoadSamplesFrmTxt (toWordIndex, sSampleFileName))
		return false;
	theSettings.SetGibbsSettings (theGibbsSettings);

	int iWordNum = toWordIndex.GetWordNum();
	WordTopicMatrix.resize(iWordNum);
	for (int i=0; i<iWordNum; i++)
		WordTopicMatrix[i].resize(theGibbsSettings.iTopicNum, 0);

	int iSampleNum = (int)theTrainingSamples.SampleVec.size();
	DocTopicMatrix.resize (iSampleNum);
	SumWordDocVec.resize (iSampleNum, 0);
	DocTopicProbMatrix.resize (iSampleNum);
	for (int i=0; i<iSampleNum; i++)
	{
		DocTopicMatrix[i].resize (theGibbsSettings.iTopicNum, 0);
		DocTopicProbMatrix[i].resize (theGibbsSettings.iTopicNum, 0.0);
	}

	SumWordTopicVec.resize(theGibbsSettings.iTopicNum, 0);
	TopicWordProbMatrix.resize (theGibbsSettings.iTopicNum);
	for (int i=0; i<theGibbsSettings.iTopicNum; i++)
		TopicWordProbMatrix[i].resize (iWordNum, 0.0);

	// get the random topics to init WordTopicMatrix, DocTopicMatrix, SumWordTopicVec and SumWordDocVec
	srand((unsigned int)time(0));
	for (int iSample=0; iSample<iSampleNum; iSample++)
	{
		int iItemNum = (int)theTrainingSamples.SampleVec[iSample].ItemVec.size();
		for (int iItem=0; iItem<iItemNum; iItem++)
		{
			int iTopic = (int)(((double)rand()/(RAND_MAX+1)) * theSettings.iTopicNum);
			theTrainingSamples.SampleVec[iSample].ItemVec[iItem].iTopic = iTopic;
			WordTopicMatrix[theTrainingSamples.SampleVec[iSample].ItemVec[iItem].iWordIndex][iTopic] ++;
			DocTopicMatrix[iSample][iTopic] ++;
			SumWordTopicVec[iTopic] ++;
		}
		SumWordDocVec[iSample] = iItemNum;
	}
	
	return true;
}

int LDACplus::SamplingForOneItem (int iSampleIndex, int iWordIndex, int iTopic)
{
	WordTopicMatrix[iWordIndex][iTopic] -= 1;
	SumWordTopicVec[iTopic] -= 1;
	DocTopicMatrix[iSampleIndex][iTopic] -= 1;
	SumWordDocVec[iSampleIndex] -= 1;

	int iWordNum = toWordIndex.GetWordNum();
	double dVBeta = iWordNum * theSettings.dBeta;
	double dKAlpha = theSettings.iTopicNum * theSettings.dAlpha;
	vector<double> TmpVec (theSettings.iTopicNum, 0.0);
	for (int i=0; i<theSettings.iTopicNum; i++)
	{
		double dTopicWordProb = 
			(WordTopicMatrix[iWordIndex][i]+theSettings.dBeta)/(SumWordTopicVec[i]+dVBeta);
		double dDocTopicProb = 
			(DocTopicMatrix[iSampleIndex][i]+theSettings.dAlpha)/(SumWordDocVec[iSampleIndex]+dKAlpha);
		TmpVec[i] = dTopicWordProb * dDocTopicProb;
	}
	for (int i=1; i<theSettings.iTopicNum; i++)
		TmpVec[i] += TmpVec[i-1];
	double u = ((double)rand() / (RAND_MAX+1)) * TmpVec[theSettings.iTopicNum-1];
	int iEstTopic = 0;
	for (; iEstTopic<theSettings.iTopicNum; iEstTopic++)
	{
		if (TmpVec[iEstTopic] > u)
			break;
	}

	WordTopicMatrix[iWordIndex][iEstTopic] += 1;
	SumWordTopicVec[iEstTopic] += 1;
	DocTopicMatrix[iSampleIndex][iEstTopic] += 1;
	SumWordDocVec[iSampleIndex] += 1;

	return iEstTopic;
}

void LDACplus::ComputeTopicWordProbMatrix (void)
{
	int iTopicNum = theSettings.iTopicNum;
	int iWordNum = toWordIndex.GetWordNum();
	double dVBeta = iWordNum * theSettings.dBeta;
	for (int iTopic=0; iTopic<iTopicNum; iTopic++)
	{
		for (int iWord=0; iWord<iWordNum; iWord++)
		{
			TopicWordProbMatrix[iTopic][iWord] = 
				(WordTopicMatrix[iWord][iTopic]+theSettings.dBeta)/(SumWordTopicVec[iTopic]+dVBeta);
		}
	}
}

void LDACplus::ComputeDocTopicProbMatrix (void)
{
	int iSampleNum = (int)theTrainingSamples.SampleVec.size();
	int iTopicNum = theSettings.iTopicNum;
	double dKAlpha = theSettings.iTopicNum * theSettings.dAlpha;
	for (int iSample=0; iSample<iSampleNum; iSample++)
	{
		for (int iTopic=0; iTopic<iTopicNum; iTopic++)
		{
			DocTopicProbMatrix[iSample][iTopic] = 
				(DocTopicMatrix[iSample][iTopic]+theSettings.dAlpha)/(SumWordDocVec[iSample]+dKAlpha);
		}
	}
}

void LDACplus::EstimatingLDAByGibbsSampling (void)
{
	for (int iIter=0; iIter<theSettings.iIterNum; iIter++)
	{
		// show something on screen
		cout << "In the " << iIter << " iteration" << endl;
		// gibbs sampling
		int iSampleNum = (int)theTrainingSamples.SampleVec.size();
		for (int iSample=0; iSample<iSampleNum; iSample++)
		{
			int iItemNum = (int)theTrainingSamples.SampleVec[iSample].ItemVec.size();
			for (int iItem=0; iItem<iItemNum; iItem++)
			{
				int iWordIndex = theTrainingSamples.SampleVec[iSample].ItemVec[iItem].iWordIndex;
				int iTopic = theTrainingSamples.SampleVec[iSample].ItemVec[iItem].iTopic;
				int iEstTopic = SamplingForOneItem(iSample, iWordIndex, iTopic);
				theTrainingSamples.SampleVec[iSample].ItemVec[iItem].iTopic = iEstTopic;
			}
		}
	}

	// compute p(word|topic) and p(topic|doc)
	ComputeTopicWordProbMatrix ();
	ComputeDocTopicProbMatrix ();
}

bool LDACplus::SaveLDAModel (const char * sFileTopicWord, const char * sFileDocTopic)
{
	ofstream outTopicWord (sFileTopicWord);
	ofstream outDocTopic (sFileDocTopic);
	if (!outTopicWord || !outDocTopic)
	{
		cerr << "can not open the file" << endl;
		return false;
	}
	int iTopicNum = theSettings.iTopicNum;
	int iWordNum = toWordIndex.GetWordNum();
	for (int iTopic=0; iTopic<iTopicNum; iTopic++)
	{
		for (int iWord=0; iWord<iWordNum; iWord++)
		{
			outTopicWord << setprecision(5) << TopicWordProbMatrix[iTopic][iWord] << " ";
		}
		outTopicWord << endl;
	}
	int iSampleNum = (int)theTrainingSamples.SampleVec.size();
	for (int iSample=0; iSample<iSampleNum; iSample++)
	{
		for (int iTopic=0; iTopic<iTopicNum; iTopic++)
		{
			outDocTopic << setprecision(5) << DocTopicProbMatrix[iSample][iTopic] << " ";
		}
		outDocTopic << endl;
	}
	return true;
}
