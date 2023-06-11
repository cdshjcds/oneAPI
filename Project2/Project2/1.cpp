#include<iostream>
#include<immintrin.h>
#include<cmath>
#include<time.h>
#include<fstream>
#include<omp.h>



#define max_N 10000

using namespace std;

float A[max_N][max_N];
float B[max_N / 8][max_N * 8];

void f1(int m, int n, float a[][max_N]) {
	__m256 t1, t2, c;
	for (int k = 0; k < (m < n ? m : n); k++)
	{
		//选取k列绝对值最大的元素为主元,主元在row行
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
		//k列全为0则不需处理
		if (s == 0)
			continue;

		//交换row行与k行
		if (row != k)
			for (int j = 0; j < n; j++)
				swap(a[k][j], a[row][j]);
		for (int j = k + 1; j < n; j++)
			a[k][j] = a[k][j] / a[k][k];
		a[k][k] = 1;

		int j;
#pragma omp parallel for private(t1,t2,c,j) num_threads(6)
		for (int i = k + 1; i < m; i++)
		{
			c = _mm256_set1_ps(a[i][k]);
			for (j = k + 1; j <= n - 8; j += 8)
			{
				t1 = _mm256_loadu_ps(a[k] + j);
				t2 = _mm256_loadu_ps(a[i] + j);
				t1 = _mm256_mul_ps(t1, c);
				t2 = _mm256_sub_ps(t2, t1);
				_mm256_store_ps(a[i] + j, t2);
			}
		}

		for (int j = n - (n - k - 1) % 8; j < n; j++)
			for (int i = k + 1; i < m; i++)
				a[i][j] = a[i][j] - a[i][k] * a[k][j];
		for (int i = k + 1; i < m; i++)
			a[i][k] = 0;

	}
}

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
	int i, j;
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			in >> A[i][j];
		}
	}
	time_t start, end;

	start = clock();
	f1(m, n, A);
	end = clock();
	cout << "Time useage: " << (float)(end - start) / 1000 << " s" << endl;
	cout << A[m - 1][n-1] << endl;
}