#pragma once
#include <unordered_map>
#include <iostream>
#include <queue>
using namespace std;

typedef struct TrieNode {
	
	list<int> recordIDs;   
	bool isEnd = false;     
	int depth=0;
 	unordered_map<int, TrieNode*> children;

}TrieNode; 

class Trie {
private:
	
	TrieNode* root;
	void completeSearch(vector<int> seq, int seqid, TrieNode* p, int lastflag, vector<int>& results);

	void completeSearch(vector<int> seq, TrieNode * p, int lastflag, vector<int>& results);

public:
	Trie() : root(new TrieNode) {};
	int alpha = 2;
	int dicNum = 27;
	void insert(vector<int> seq, int id);	
	vector<int> search(vector<int> seq);

};