#include "editSearch.h"
#include "trie.h"

void editSearch::buildTrie()
{
	for (size_t i = 0; i < ssEmbedRecord.size(); i++)
	{
		try
		{
			trie.insert(ssEmbedRecord[i].sequence, (int)i);
		}
		catch (bad_alloc)
		{
			cerr << "alloc memory failed" << endl;
			cerr << "Inserted Number: "<<i << endl;
			break;
		}
	}
	
}

void editSearch::searchTrie(embedRecord r)
{	
	ssCandidates2=trie.search(r.sequence);
}



