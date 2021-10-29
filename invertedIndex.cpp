#include "editSearch.h"

void editSearch::IniIndex()
{
	int d = M;
	
	for (int i = 0; i < d; i++)
	{
		vector<vector<int>> invertedList;
		for (int j = 0; j < dicsize; j++)
		{
			vector<int> recordIds;
			invertedList.push_back(recordIds);
		}
		invertedIndex.push_back(invertedList);
	}

	for (size_t i = 0; i < ssRecord.size(); i++)
	{
		ssCount.push_back(0);
	}
}

void editSearch::buildInvertedIndex()
{
	IniIndex();
	for (size_t i = 0; i < ssEmbedRecord.size(); i++)
	{
		if ((int)ssEmbedRecord[i].sequence.size() == M)
		{
			for (size_t j = 0; j < ssEmbedRecord[i].sequence.size(); j++)
			{
				int povit = ssEmbedRecord[i].sequence[j];
				//if sequence starts from 1£¬index starts from [povit-1]
				invertedIndex.at(j).at(povit-1).push_back(ssEmbedRecord[i].rid);
				//if sequence starts from 0£¬index starts from [povit]
				//invertedIndex.at(j).at(povit).push_back(ssEmbedRecord[i].rid);
				//invertedIndex.at(j).at(ii).push_back(i);
			}			
		}
	}
}

void editSearch::searchInvertedIndex(embedRecord r)
{
	int length = (int)r.sequence.size();
	for (int k = 0; k < length; k++)
	{
		int povit = r.sequence[k];
		//if sequence starts from 1£¬index starts from [povit-1]
		vector<int> recordIds = invertedIndex.at(k).at(povit - 1);
		//if sequence starts from 0£¬index starts from [povit]
		//vector<int> recordIds = invertedIndex.at(k).at(povit);
		for (size_t i = 0; i < recordIds.size(); i++)
		{
			try {
				if (ssRecord[recordIds[i]].len <= length *(1 - ep) && ssRecord[recordIds[i]].len >= length *(1 + ep))
					ssCount.at(recordIds[i])++;
			}
			catch (exception ex)
			{
				cerr << recordIds[i];
				throw ex;

			}
		}
	}
	for (size_t i = 0; i < ssCount.size(); i++)
	{
		if (ssCount[i] > M - alpha)
			ssCandidates.push_back(make_pair((int)i, ssCount[i]));
	}
}


void editSearch::buildLearnedIndex()
{
	IniIndex();
	const int epsilon = 32;
	for (size_t i = 0; i < ssEmbedRecord.size(); i++)
	{
		if ((int)ssEmbedRecord[i].sequence.size() == M)
		{
			for (size_t j = 0; j < ssEmbedRecord[i].sequence.size(); j++)
			{
				int povit = ssEmbedRecord[i].sequence[j];
				//if sequence starts from 1£¬index starts from [povit-1]
				invertedIndex.at(j).at(povit - 1).push_back(ssEmbedRecord[i].rid);
				//if sequence starts from 0£¬index starts from [povit]
				//invertedIndex.at(j).at(povit).push_back(ssEmbedRecord[i].rid);
				//invertedIndex.at(j).at(ii).push_back(i);
			}
		}
	}
	
	for (size_t i = 0; i < invertedIndex.size(); i++)
	{
		for (size_t j = 0; j < invertedIndex[i].size(); j++)
		{
			std::sort(invertedIndex[i][j].begin(), invertedIndex[i][j].end());
			pgm::PGMIndex<int, epsilon> index(invertedIndex[i][j]);
			invertedIndexL[i][j] = index;
		}
	}
}

void editSearch::searchLearnedIndex(embedRecord r)
{
	int length = (int)r.sequence.size();
	for (int k = 0; k < length; k++)
	{
		int povit = r.sequence[k];

		//if sequence starts from 1£¬index starts from [povit-1]
		//if sequence starts from 0£¬index starts from [povit]
		auto range = invertedIndexL[k][povit - 1].search(r.rid);
		auto pos = range.pos;

		int posrid = invertedIndex[k][povit - 1][pos];

		for (size_t i = pos; i < invertedIndex[k][povit - 1].size(); i++)
		{
			int posrid = invertedIndex[k][povit - 1][i];
			if(ssRecord[posrid].len <= length *(1 - ep) && ssRecord[posrid].len >= length *(1 + ep))
				ssCount.at(posrid)++;
		}
		for (size_t i = pos; i > 0; i--)
		{
			int posrid = invertedIndex[k][povit - 1][i];
			if (ssRecord[posrid].len <= length *(1 - ep) && ssRecord[posrid].len >= length *(1 + ep))
				ssCount.at(posrid)++;
		}
		
	}
	for (size_t i = 0; i < ssCount.size(); i++)
	{
		if (ssCount[i] > M - alpha)
			ssCandidates.push_back(make_pair((int)i, ssCount[i]));
	}
}


