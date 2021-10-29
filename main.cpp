#include "editSearch.h"

//test trie
int main(int argc, char* argv[])
{
	timeb timeStart, timeEnd;
	editSearch es;

	es.ReadPara(argv);
	es.ReadData(argv[1]);
	cerr << "Document: " << argv[1] << endl;
	cerr << "Record Numbers: " << es.ssRecordOrigin.size() << endl;
	record q = es.ssRecordOrigin[0];
	es.ssRecord.swap(es.ssRecordOrigin);	

	for (size_t i = 0; i < es.ssRecord.size(); i++)
	{
		es.ssRecord[i].rid = (int)i;
	}
	
	cerr << "=== BEGIN (TIMER STARTED) ===" << endl;
	cerr << "makeRandTable" << endl;	

	es.M = (int)pow(2, es.mr + 1) - 1;	
	es.trie.alpha = es.alpha;
	es.trie.dicNum = es.dicsize;
	es.makeRandTable();

	ftime(&timeStart);
	cerr << "Embedding" << endl;	
	es.preEmbedding();
	cerr << "Indexing" << endl;
	es.buildTrie();
	ftime(&timeEnd);	
	cerr << "Total BuildIndex Time: " << setiosflags(ios::fixed) << setprecision(3) << double(timeEnd.time - timeStart.time) + double(timeEnd.millitm - timeStart.millitm) / 1e3 << endl;

	for (size_t i = 0; i < 100; i++)
	{
		int rnum = rand() % (es.ssRecord.size());
		record q = es.ssRecord[rnum];
		cerr << "Searching" << endl;
		ftime(&timeStart);
		embedRecord eq = es.minhashEmbedding(q);
		es.searchTrie(eq);
		ftime(&timeEnd);
		cerr << "Total searching Time: " << setiosflags(ios::fixed) << setprecision(3) << double(timeEnd.time - timeStart.time) + double(timeEnd.millitm - timeStart.millitm) / 1e3 << endl;

		ftime(&timeStart);
		es.verifyResults(q);
		ftime(&timeEnd);
		cerr << "Verification Time: " << setiosflags(ios::fixed) << setprecision(3) << double(timeEnd.time - timeStart.time) + double(timeEnd.millitm - timeStart.millitm) / 1e3 << endl;
		
		//test shift optimization
		//es.shiftpos = (int)(2 * (es.ep * q.len) / 3);
		//vector<embedRecord> eqs = es.minhashShiftEmbedding(q);
		//unordered_map<int,bool> cands2;
		//for (size_t i = 0; i < eqs.size(); i++)
		//{
		//	es.searchTrie(eqs[i]);
		//	for (size_t j = 0; j < ssCandidates2.size(); j++)
		//	{
		//		if (cands2[can[j]] == NULL)			
		//			cands2[can[j]] = true;			
		//	}		
		//}	

		//cerr << "recordNum" <<es.recordNum << endl;
		//cerr <<  setiosflags(ios::fixed) << setprecision(3) << cands1.size() << endl;
		//cerr <<  setiosflags(ios::fixed) << setprecision(3) << cands2.size() << endl;
	}
	system("pause");
	return 0;
}

//test invertedindex
int main(int argc, char* argv[])
{
	timeb timeStart, timeEnd;
	editSearch es;

	es.ReadPara(argv);
	es.ReadData(argv[1]);
	cerr << "Document: " << argv[1] << endl;
	cerr << "Record Numbers: " << es.ssRecordOrigin.size() << endl;
	
	es.ssRecord.swap(es.ssRecordOrigin);

	for (size_t i = 0; i < es.ssRecord.size(); i++)
	{
		es.ssRecord[i].rid = (int)i;
	}

	cerr << "=== BEGIN (TIMER STARTED) ===" << endl;
	cerr << "makeRandTable" << endl;

	es.M = (int)pow(2, es.mr + 1) - 1;	
	es.trie.alpha = es.alpha;
	es.trie.dicNum = es.dicsize;
	es.makeRandTable();

	ftime(&timeStart);
	cerr << "Embedding" << endl;
	es.preEmbedding();
	cerr << "Indexing" << endl;	
	es.buildInvertedIndex();
	ftime(&timeEnd);
	cerr << "Total BuildIndex Time: " << setiosflags(ios::fixed) << setprecision(3) << double(timeEnd.time - timeStart.time) + double(timeEnd.millitm - timeStart.millitm) / 1e3 << endl;
	
	for (size_t i = 0; i < 100; i++)
	{
		cerr << "Searching" << endl;
		ftime(&timeStart);
		int rnum = rand() % (es.ssRecord.size());
		record q = es.ssRecord[rnum];
		embedRecord eq = es.minhashEmbedding(q);
		es.searchInvertedIndex(eq);		
		ftime(&timeEnd);
		cerr << "Total searching Time: " << setiosflags(ios::fixed) << setprecision(3) << double(timeEnd.time - timeStart.time) + double(timeEnd.millitm - timeStart.millitm) / 1e3 << endl;
		
		ftime(&timeStart);
		es.minhashSearchResults(q);
		ftime(&timeEnd);
		cerr << "Verification Time: " << setiosflags(ios::fixed) << setprecision(3) << double(timeEnd.time - timeStart.time) + double(timeEnd.millitm - timeStart.millitm) / 1e3 << endl;
	}
	
	system("pause");
	return 0;
}

