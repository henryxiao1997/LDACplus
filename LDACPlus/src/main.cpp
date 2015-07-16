#include "LDACplus.h"

#include <iostream>

using namespace std;

int main (void)
{
	cout << "Hello world for LDA c++" << endl;

	GibbsSettings theSettings;
	theSettings.iTopicNum = 100;
	theSettings.dAlpha = 50.0 / theSettings.iTopicNum;
	theSettings.dBeta = 0.1;
	theSettings.iIterNum = 20;

	LDACplus theLDA;
	theLDA.InitForEstimating (theSettings, "trainsamples\\trndocs_lex.txt", "trainsamples\\trndocs.dat");
	theLDA.EstimatingLDAByGibbsSampling();
	theLDA.SaveLDAModel ("model\\TopicWordProb.txt", "model\\DocTopicProb.txt");

	cout << "press any key to Bye" << endl;
	getchar();

	return 0;
}