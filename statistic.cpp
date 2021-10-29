#include "editSearch.h"

using namespace std;
void editSearch::statistic1(record r)
{
	embedRecord embr = minhashEmbedding(r);


	vector<int> ednumbers;
	for (size_t i = 0; i < 11; i++)
	{
		ednumbers.push_back(0);
	}
	for (size_t i = 0; i < ssRecord.size(); i++)
	{
		if(i%10000==0)
			cerr << i << endl;

		int ylen = ssRecord[i].len;
		if (ylen > 2 * r.len)
		{
			ednumbers[10]++;
			continue;
		}

		int ed = EditDistance(r.sequence, ssRecord[i].sequence);
		if (ed<=0.1f*r.len)
			ednumbers[0]++;
		else if(ed<=0.2f*r.len)
			ednumbers[1]++;
		else if (ed <= 0.3f*r.len)
			ednumbers[2]++;
		else if (ed <= 0.4f*r.len)
			ednumbers[3]++;
		else if (ed <= 0.5f*r.len)
			ednumbers[4]++;
		else if (ed <= 0.6f*r.len)
			ednumbers[5]++;
		else if (ed <= 0.7f*r.len)
			ednumbers[6]++;
		else if (ed <= 0.8f*r.len)
			ednumbers[7]++;
		else if (ed <= 0.9f*r.len)
			ednumbers[8]++;
		else if (ed <= 1.0f*r.len)
			ednumbers[9]++;
		else
			ednumbers[10]++;
	}

	cerr << "ed distribution" << endl;
	for (size_t i = 0; i < ednumbers.size(); i++)
	{
		cerr << ednumbers[i] << endl;
	}
	
}