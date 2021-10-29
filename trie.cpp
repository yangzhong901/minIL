#include "trie.h"


void Trie::insert(vector<int> seq, int id) 
{	
	TrieNode* p = root;
	for (size_t i = 0; i < seq.size(); ++i) 
	{
		int povit = seq[i] - 1;
		if (p->children[povit] == nullptr)
		{
			p->children[povit] = new TrieNode;
			p->children[povit]->depth = p->depth + 1;
		}
		p = p->children[povit];
	}
	p->isEnd = true;
	p->recordIDs.push_back(id);
}

// recursion method
void Trie::completeSearch(vector<int> seq, int seqid, TrieNode* p, int lastflag, vector<int>& results)
{
	if (p->isEnd)
	{
		for (auto iter = p->recordIDs.begin(); iter != p->recordIDs.end(); ++iter)
		{
			results.push_back(*iter);
		}
		return;
	}


	for (int i = 0; i < dicNum; i++)
	{
		if (p->children[i] == NULL)
			continue;
		else if(i!=seq[seqid]-1)
		{
			int flag = lastflag + 1;
			if (flag <= alpha)
				completeSearch(seq, seqid + 1, p->children[i], flag, results);
			else continue;
		}
		else if (i == seq[seqid]-1)
		{
			completeSearch(seq, seqid + 1, p->children[i], lastflag, results);
		}
	}
	
}

// non-recursive method
void Trie::completeSearch(vector<int> seq, TrieNode* p, int lastflag, vector<int>& results)
{
	if (p == NULL)
		return;

	queue<pair<TrieNode*, int>> nodeQ;
	nodeQ.push(make_pair(p, lastflag));
	while (!nodeQ.empty())
	{
		pair<TrieNode*, int> temp = nodeQ.front();
		nodeQ.pop();

		if (temp.first->isEnd)
		{
			for (auto iter = temp.first->recordIDs.begin(); iter != temp.first->recordIDs.end(); ++iter)
			{
				results.push_back(*iter);
			}
		}
		else
		{
			int temppovit = seq[temp.first->depth] - 1;
			for (int i = 0; i < dicNum; i++)
			{
				if (temp.first->children[i] == NULL)
					continue;
				else if (i != temppovit - 1)
				{
					int flag = temp.second + 1;
					if (flag <= alpha)
						nodeQ.push(make_pair(temp.first->children[temppovit], flag));
					else continue;
				}
				else if (i == temppovit - 1)
				{
					nodeQ.push(make_pair(temp.first->children[temppovit], temp.second));
				}
			}
		}
	}

}

vector<int> Trie::search(vector<int> seq) {
	auto p = root;
	vector<int> results;
	//completeSearch(seq, 0, p, 0, results);
	completeSearch(seq, p, 0, results);
	return results;
}
