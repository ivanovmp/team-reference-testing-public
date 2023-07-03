#include <bits/stdc++.h>
#define pb push_back
#define mp make_pair
#define x first
#define y second

using namespace std;

typedef long long ll;

namespace Convolution
{
 	class Convolution
 	{
 		using Int = int;

 	public:
 		Convolution(int _n) : n(_n), size(1 << _n)
 		{
 		 	helpA.resize(size), helpB.resize(size);
 		}

 		void or_transform(vector <Int> &A)
     	{
     		for (int i = 0; i < n; i++)
     			for (int mask = 0; mask < size; mask++)
     				if ((mask >> i) & 1)
     					A[mask] += A[mask ^ (1 << i)];
     		return;	
     	}

     	void rev_or_transform(vector <Int> &A)
     	{
     		for (int i = 0; i < n; i++)
     			for (int mask = 0; mask < size; mask++)
     				if ((mask >> i) & 1)
     					A[mask] -= A[mask ^ (1 << i)];
     		return;	
     	}

     	void and_transform(vector <Int> &A)
     	{
     		for (int i = 0; i < n; i++)
     			for (int mask = 0; mask < size; mask++)
     				if ((mask >> i) & 1)
     					A[mask ^ (1 << i)] += A[mask];
     		return;	
     	}

     	void rev_and_transform(vector <Int> &A)
     	{
     		for (int i = 0; i < n; i++)
     			for (int mask = 0; mask < size; mask++)
     				if ((mask >> i) & 1)
     					A[mask ^ (1 << i)] -= A[mask];
     		return;	
     	}

     	void xor_transform(vector <Int> &A)
     	{
     	 	for (int i = 0; i < n; i++)
     	 		for (int mask = 0; mask < size; mask++)
     	 			if ((mask >> i) & 1)
     	 			{
     	 			 	int u = (mask ^ (1 << i)), v = mask;
     	 			 	int lastu = A[u], lastv = A[v];
     	 			 	A[u] = lastu + lastv, A[v] = lastu - lastv;
     	 			}
     	 	return;
     	}
     	
     	void rev_xor_transform(vector <Int> &A)
     	{
     	 	xor_transform(A);
     	 	for (int i = 0; i < size; i++) A[i] >>= n;
     	 	return;
     	}

        vector <Int> or_convolution(const vector <Int> &A, const vector <Int> &B)
     	{
     	 	for (int i = 0; i < size; i++) helpA[i] = A[i];
     	 	for (int i = 0; i < size; i++) helpB[i] = B[i];

     	 	or_transform(helpA);
     	 	or_transform(helpB);
     	 	for (int i = 0; i < size; i++) helpA[i] = helpA[i] * helpB[i];
     	 	rev_or_transform(helpA);
     	 	return helpA;
     	}

     	vector <Int> and_convolution(const vector <Int> &A, const vector <Int> &B)
     	{
     	 	for (int i = 0; i < size; i++) helpA[i] = A[i];
     	 	for (int i = 0; i < size; i++) helpB[i] = B[i];

     	 	and_transform(helpA);
     	 	and_transform(helpB);
     	 	for (int i = 0; i < size; i++) helpA[i] = helpA[i] * helpB[i];
     	 	rev_and_transform(helpA);
     	 	return helpA;
     	}

     	vector <Int> xor_convolution(const vector <Int> &A, const vector <Int> &B)
     	{
     	 	for (int i = 0; i < size; i++) helpA[i] = A[i];
     	 	for (int i = 0; i < size; i++) helpB[i] = B[i];

     	 	xor_transform(helpA);
     	 	xor_transform(helpB);
     	 	for (int i = 0; i < size; i++) helpA[i] = helpA[i] * helpB[i];
     	 	rev_xor_transform(helpA);
     	 	return helpA;
     	}

    private:
    	vector <Int> helpA, helpB;
    	int n, size;
 	};
}