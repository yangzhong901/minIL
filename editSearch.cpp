#include "editSearch.h"



float editSearch::Similarity(int dist, int len1, int len2)
{
	return 1.0f - 1.0f*dist / max(len1, len2);
}

int editSearch::EditDistance(vector<int>& a, vector<int>& b)
{
	int m, n;
	n = (int)a.size();
	m = (int)b.size();
	if (m == 0) return n;
	if (n == 0) return m;

	int row = n + 1;
	int col = m + 1;

	vector<vector<short>> f(row, vector<short>(col));
	
	for (int i = 0; i < row; i++)
		f[i][0] = i;

	for (int i = 0; i < col; i++)
		f[0][i] = i;

	for (int i = 1; i < row; i++)
		for (int j = 1; j < col; j++) {
			if (a[i - 1] == b[j - 1])
			{
				f[i][j] = f[i - 1][j - 1];
			}
			else
				f[i][j] = f[i - 1][j - 1] + 1;
			f[i][j] = (short)min((int)f[i][j], min(f[i - 1][j] + 1, f[i][j - 1] + 1));
		}

	short ed = f[row - 1][col - 1];
	f.swap(vector<vector<short>>());
	return ed;
}

