#include "editSearch.h"

using namespace std;
bool recordlenComp(const record &a, const record &b)
{
	return a.len < b.len;
}

void editSearch::ReadPara(char* argv[])
{	
	mr = atoi(argv[2]);	
	alpha = atoi(argv[3]);
	ep = atof(argv[4]);
	alphalen = atof(argv[5]);	
	dicsize = w = atoi(argv[6]);
}

void editSearch::ReadData(char* filename)
{
	int rid = 0;
	int rlen;
	int totalLen = 0;
	int inputElement;
	record inputRecord;
	FILE *fp;
	errno_t err;
	err = fopen_s(&fp, filename, "rb");
	if (err != 0)
	{
		cerr << "Open file error" << endl;
		return;
	}
	/* read binary data */
	while (fread(&rlen, sizeof(int), 1, fp) == 1)
	{
		totalLen += rlen;
		inputRecord.rid = rid, inputRecord.len = rlen, inputRecord.sequence.clear();
		for (int i = 0; i < rlen; i++) {
			fread(&inputElement, sizeof(int), 1, fp);			
			inputRecord.sequence.push_back(inputElement);
		}
		ssRecordOrigin.push_back(inputRecord);
		++recordNum;
		++rid;
	}
	fclose(fp);

	sort(ssRecordOrigin.begin(), ssRecordOrigin.end(), recordlenComp);
	/* output statistics */
	cerr << "# Records: " << recordNum << endl;
	cerr << "# Average Record Size: " << double(totalLen) / recordNum << endl;
	cerr << "# Minmum Record Size: " << ssRecordOrigin[0].len << endl;
	cerr << "# Maximum Record Size: " << ssRecordOrigin[recordNum - 1].len << endl;
	
}

void editSearch::ReadTextData(char* filename)
{
	string ncl, ncls, nclss;
	nclss.reserve(30000);
	int val = 0;	
	long long cnt = 0;
	int len = 0;		
	double t1, t2;
	int totalLen = 0;
	ifstream infile(filename);	

	record inputRecord;
	int rid = 0;
	t1 = clock();

	if (infile.is_open())
	{		
		while (getline(infile, ncls))
		{
			len = (int)ncls.size();
			inputRecord.rid = rid, inputRecord.len = len, inputRecord.sequence.clear();
			totalLen += len;
			for (size_t i = 0; i < ncls.size(); i++)
			{
				ncl = ncls.at(i);
				if (ncl == "A")
				{
					val = 1;
					inputRecord.sequence.push_back(val);
				}
				else if (ncl == "G")
				{
					val = 2;
					inputRecord.sequence.push_back(val);
				}
				else if (ncl == "C")
				{
					val = 3;
					inputRecord.sequence.push_back(val);
				}
				else if (ncl == "T")
				{
					val = 4;
					inputRecord.sequence.push_back(val);
				}
				else
				{
					val = 5;
				}
			}
			ssRecordOrigin.push_back(inputRecord);
			++recordNum;
			rid++;
		}
	}
	else
		cout << "Cannot open the DNA file!";

	t2 = clock();
	cout << "Total Execution Time : " << ((t2 - t1) / CLOCKS_PER_SEC) << endl;
	cout << "Total number of DNA : " << cnt << endl;
	infile.close();

	sort(ssRecordOrigin.begin(), ssRecordOrigin.end(), recordlenComp);
	/* output statistics */
	cerr << "# Records: " << recordNum << endl;
	cerr << "# Average Record Size: " << double(totalLen) / recordNum << endl;
	cerr << "# Maximum Record Size: " << ssRecordOrigin[recordNum - 1].len << endl;
}

