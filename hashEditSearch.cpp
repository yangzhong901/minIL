#include "editSearch.h"



uint64_t A;//random number
uint64_t B;//
vector<uint32_t> tableA;
vector<uint32_t> tableB;

void editSearch::makeRandTable()
{
	srand(1);
	for (int i = 0; i <= M; i++)
	{
		tableA.push_back(rand());
		tableB.push_back(rand());
	}

	cerr << "tableA: " << endl;
	for (size_t i = 0; i < tableA.size(); i++)
	{
		cerr << tableA[i] << ";";
	}
	cerr << endl;
	cerr << "tableB: " << endl;
	for (size_t i = 0; i < tableB.size(); i++)
	{
		cerr << tableB[i] << ";";
	}
	cerr << endl;
}

uint16_t editSearch::hash(uint16_t x,int m)
{
	uint32_t v1 = (tableA[m] * x + tableB[m])%(w);	
	uint16_t v3 = (uint16_t)v1;
	return v3;	
}

int editSearch::minhash(vector<int> seq, int start, int end,int& hash_m, int& pos)
{
	uint16_t min = (uint16_t)0xFFFFFFFF;
	int range = end - start + 1;

	for (int i = 0; i < range; i++)
	{
		uint16_t value = hash((uint16_t)seq[start+i], hash_m);
		if (value < min)
		{
			min = value;
			pos = start + i;
		}
	}
	hash_m++;
	return seq[pos];
}


int editSearch::minhash(vector<int> seq, int& hash_m, int& pos)
{
	uint16_t min = (uint16_t)0xFFFFFFFF;

	for (size_t i = 0; i < seq.size(); i++)
	{
		uint16_t value = hash((uint16_t)seq[i], hash_m);
		if (value < min)
		{
			min = value;
			pos = (int)i;
		}
	}
	hash_m++;
	return seq[pos];
}


void editSearch::RecursiveStep(vector<int> sequence, embedRecord& em)
{
	stack< pair<vector<int>, int>> s;
	int m = 0;
	int hash_m = 0;
	vector<int> seq;
	s.push(make_pair(sequence, m));
	
	while (!s.empty())
	{
		seq.clear();
		seq = s.top().first;
		m = s.top().second;
		s.pop();

		if (seq.size() == NULL || seq.size() == 0)
			continue;
		else
		{
			int pos = 0;
			if (seq.size() - eplen < 0)
			{
				cerr << "seq.size() - eplen < 0" << endl;
				continue;
			}

			int startpos=0, endpos = 0;			
			
			//use a slightly larger eplen in the first round
			if (m == 0)
			{
				startpos = (int)ceilf((seq.size() - 1.0f - eplen) / 2);
				endpos = (int)ceilf((seq.size() - 1.0f + eplen) / 2) - 1;
			}
			else
			{
				startpos = (int)ceilf((seq.size() - 1.0f - eplen) / 2);
				endpos = (int)ceilf((seq.size() - 1.0f + eplen) / 2) - 1;
			}


			if (endpos - startpos < 0)
			{
				cerr <<"endpos - startpos < 0" << endl;
				em.sequence.push_back(-1);
				continue;
			}
			if (endpos - startpos + 1 - (int)seq.size() >0)
			{
				cerr << "endpos - startpos + 1 - seq.size() >0" << endl;
				int value = minhash(seq, 0, (int)seq.size()-1, hash_m, pos);
				em.sequence.push_back(value);				
				continue;
			}

			int value = minhash(seq,startpos,endpos,hash_m,pos);			

			em.sequence.push_back(value);

			if (m < mr)
			{
				vector<int> seqleft, seqright;

				if (pos == 0 || pos == seq.size())
					continue;
				seqleft.assign(seq.begin(), seq.begin() + pos - 1);
				seqright.assign(seq.begin() + pos + 1, seq.end());

				s.push(make_pair(seqright, m + 1));
				s.push(make_pair(seqleft, m + 1));
			}
		}
	}
}

embedRecord editSearch::minhashEmbedding(record input)
{
	embedRecord em;
	em.rid = input.rid;	
	eplen = (int)floorf(alphalen*input.len / (powf(2.0f,mr + 1.0f) - 1.0f));
	RecursiveStep(input.sequence, em);
	em.len = (int)em.sequence.size();
	return em;
}

vector<embedRecord> editSearch::minhashShiftEmbedding(record input)
{
	embedRecord em, eml, emr;
	vector<embedRecord> ems;
	record r1, r2;
	r1.rid = r1.rid = input.rid;
	r1.sequence = input.sequence;
	for (int i = 0; i < shiftpos; i++)
	{
		r1.sequence.push_back(dicsize); //add place holders
		r2.sequence.push_back(dicsize);
	}
	for (size_t i = 0; i < input.sequence.size(); i++)
	{
		r2.sequence.push_back(input.sequence[i]);
	}

	RecursiveStep(input.sequence, em);
	RecursiveStep(r1.sequence, eml);
	RecursiveStep(r2.sequence, emr);

	ems.push_back(em);
	ems.push_back(eml);
	ems.push_back(emr);
}

int editSearch::embedDistance(embedRecord a, embedRecord b)
{
	int delta = abs(a.len - b.len);
	int size = min(a.len,b.len);
	int emdis = 0;

	for (int i = 0; i < size; i++)
	{
		if (a.sequence[i] != b.sequence[i])
			emdis++;
	}
	return emdis+ delta;
}

void editSearch::preEmbedding()
{
	for (size_t i = 0; i < ssRecord.size(); i++)
	{
		embedRecord em = minhashEmbedding(ssRecord[i]);
		ssEmbedRecord.push_back(em);
	}
}

void editSearch::minhashSearch(record r)
{
	embedRecord er0 = minhashEmbedding(r);

	searchInvertedIndex(er0);
	vector<int> emdiss(64);
	for (size_t i = 0; i < ssEmbedRecord.size(); i++)
	{
		int lenDis = abs(r.len - ssRecord[i].len);

		if (lenDis > r.len*ep)
			continue;

		int emdis = embedDistance(er0, ssEmbedRecord[i]);
		
		emdiss[emdis]++;
		
		if (emdis <= alpha)
			ssCandidates.push_back(make_pair(ssEmbedRecord[i].rid,emdis));

		ssEmbdis.push_back(make_pair(ssEmbedRecord[i].rid,emdis));
	}

	cerr << "emdiss:" << endl;
	for (size_t i = 0; i < emdiss.size(); i++)
	{
		cerr << emdiss[i] << endl;
	}
}

void editSearch::verifyResults(record r)
{
	for (size_t i = 0; i < ssCandidates2.size(); i++)
	{
		int index = ssCandidates2[i];

		int ed = EditDistance(r.sequence, ssRecord[index].sequence);
		float sim = Similarity(ed, r.len, ssRecord[index].len);

		if (ed <= ep*r.len)
		{
			result rlt;
			rlt.rid = ssRecord[index].rid;
			rlt.distance = ed;
			rlt.sim = sim;
			ssMinResult.push_back(rlt);
		}
	}
}

void editSearch::minhashSearchResults(record r)
{
	for (size_t i = 0; i < ssCandidates.size(); i++)
	{
		int index = ssCandidates[i].first;
		
		int ed = EditDistance(r.sequence, ssRecord[index].sequence);
		float sim = Similarity(ed, r.len, ssRecord[index].len);

		if(ed<= ep*r.len)
		{
			result rlt;
			rlt.rid = ssRecord[index].rid;
			rlt.distance = ed;
			rlt.sim = sim;
			ssMinResult.push_back(rlt);			
		}		
	}
}

void editSearch::accurateResults(record r)
{
	for (size_t i = 0; i < ssRecord.size(); i++)
	{		
		int ed = EditDistance(r.sequence, ssRecord[i].sequence);
		float sim = Similarity(ed, r.len, ssRecord[i].len);
		
		if (ed<=ep*r.len)
		{
			result rlt;
			rlt.rid = ssRecord[i].rid;
			rlt.distance = ed;
			rlt.sim = sim;
			
			ssResult.push_back(rlt);
		}
	}
}

void editSearch::otherResults(record r)
{
	for (size_t i = 0; i < ssEmbdis.size(); i++)
	{
		if (ssEmbdis[i].second > alpha)
		{
			int ind = ssEmbdis[i].first;
			int ed = EditDistance(r.sequence, ssRecord[ind].sequence);
			float sim = Similarity(ed, r.len, ssRecord[ind].len);
			
			if (ed<= ep*r.len)
			{
				result rlt;
				rlt.rid = ssRecord[ind].rid;
				rlt.distance = ed;
				rlt.sim = sim;
				rlt.embdis = ssEmbdis[ind].second;
				ssResult.push_back(rlt);
			}
		}
	}
}

