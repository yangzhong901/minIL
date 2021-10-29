#pragma once
#ifndef EDITSEARCH_H_
#define EDITSEARCH_H_


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <sys/timeb.h>
#include "trie.h"
#include <stack>
#include <bitset>
#include "pgm/pgm_index.hpp"

using namespace std;


struct record
{
	int rid;
	int len;
	vector<int> sequence;

	record() {};
};

struct result
{
	int rid;
	int distance;
	float sim;

	int embdis;
};

struct embedRecord
{
	int rid;
	int len;
	vector<int> sequence;
};

class editSearch
{
public:
	int recordNum;
	int d;


	int mr = 6;//iteration time
	int M = (int)pow(2,mr+1)-1;//length after embedding
	float ep = 0.03f;//threhold factor
	int threshold=0;
	int eplen = 1;
	int alpha = 3;
	int w = 9;
	float alphalen = 0.5f;
	int dicsize = 27;//size of dictionary
	int shiftpos = 0;
	const int epsilon = 32;// epsilon of pgm
public: Trie trie;
public: vector<record> ssRecordOrigin;

public: vector<result> ssResult;

public: vector<record> ssRecord;
public: vector<embedRecord> ssEmbedRecord;
public: vector<pair<int, int>> ssEmbdis;//distance between embeddings <rid,embdis>
public: vector<pair<int,int>> ssCandidates;//candidates <rid,embdis>
public: vector<int> ssCandidates2;//candidates <rid>
public: vector<result> ssMinResult;//results from candidates
public: vector<vector<vector<int>>> invertedIndex;
public: vector<vector<pgm::PGMIndex<int, 32>>> invertedIndexL;
public: vector<int> ssCount;
public:
	editSearch() {
		recordNum = 0;
		d = 2;
	}
	void ReadPara(char* argv[]);
	void ReadData(char* filename);
	void ReadTextData(char * filename);

	float Similarity(int dist, int len1, int len2);
	int EditDistance(vector<int>& a, vector<int>& b);	
	
	//=====================minhashSearch=======================//
	void makeRandTable();
	uint16_t hash(uint16_t x,int i);
	int minhash(vector<int> range, int start, int end, int& hash_m,int& pos);
	int minhash(vector<int> seq, int& hash_m, int & pos);

	void RecursiveStep(vector<int> sequence, embedRecord & em);
	embedRecord minhashEmbedding(record input);
	vector<embedRecord> minhashShiftEmbedding(record input);
	int embedDistance(embedRecord a, embedRecord b);
	void preEmbedding();
	void verifyResults(record r);
	void minhashSearch(record r);
	void minhashSearchResults(record r);
	void accurateResults(record r);
	void otherResults(record r);
	
	//index inverted
	void IniIndex();
	void buildInvertedIndex();
	void searchInvertedIndex(embedRecord r);
	void buildLearnedIndex();
	void searchLearnedIndex(embedRecord r);
	//index trie
	void buildTrie();
	void searchTrie(embedRecord r);
	//
	void statistic1(record r);
};
#endif