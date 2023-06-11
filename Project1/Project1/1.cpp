#include<iostream>
#include<immintrin.h>
#include<cmath>
#include<time.h>
#include<fstream>

#define max_N 10000

using namespace std;

void normal(int m, int n, float a[][max_N]) {
	for (int k = 0; k < (m < n ? m : n); k++)
	{

		float s = abs(a[k][k]);
		int row = k;
		for (int i = k + 1; i < m; i++)
		{
			float t = abs(a[i][k]);
			if (s < t)
			{
				s = t;
				row = i;
			}
		}
		if (s == 0)
			continue;
		if (row != k)
			for (int j = 0; j < n; j++)
				swap(a[k][j], a[row][j]);
		for (int j = k + 1; j < n; j++)
			a[k][j] = a[k][j] / a[k][k];
		a[k][k] = 1;
		for (int i = k + 1; i < m; i++)
			for (int j = k + 1; j < n; j++)
				a[i][j] = a[i][j] - a[i][k] * a[k][j];
		for (int i = k + 1; i < m; i++)
			a[i][k] = 0;
	}
}

float A[max_N][max_N];

int main()
{
	ifstream in("data.txt");
	if (!in.is_open()) {
		cerr << "Failed to open file!" << endl;
		return -1;
	}
	int m, n;
	in >> m;
	in >> n;
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			in >> A[i][j];
		}
	}
	time_t start, end;
	start = clock();
	normal(m, n, A);
	end = clock();
	cout << "Time useage: " << (float)(end - start) / 1000 << " s" << endl;
	cout << A[m - 1][n - 1] << endl;
	getchar();
}
