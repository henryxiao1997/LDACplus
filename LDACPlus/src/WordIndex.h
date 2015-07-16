/*
 * Copyright (C) 2014 by Jinghui Xiao (xiaojinghui1978@qq.com)
 *
 * LDACplus is a free software. I implement it for studying LDA 
 * and gibbs sampling. And I hope it could be useful. But there 
 * is no warranty. You can modify it or redistribut it under the 
 * the terms of the GNU General Public License. 
 */

#pragma once

#include <string>
#include <vector>

using namespace std;

/*
 * The word index class 
 * Turn word string to word index or vice verse.
 */

class WordIndex
{
public:
	WordIndex(void);
	~WordIndex(void);

	// Load the lexicon from text file.
	// The text format: each line contains a word string
	bool LoadLexicon (const char* sFileName);
	// Judege whether the class is available (LexVec is not empty)
	bool IsAvailable (void);
	// Get the total word numer
	int GetWordNum (void);
	// Get the word index from word string
	// return false if sWord is not in LexVec
	bool GetWordIndexFrmString(string & sWord, int & iWordIndex);
	// Get the word string from word index
	// return false if iWordIndex is out of the range of LexVec
	bool GetWordStringFrmIndex(int iWordIndex, string & sWord);

private:
	vector<string> LexVec;
};

