#pragma once

namespace FFT {
 	class NTT {
 	public:
 		const int n = 20;
 		const int size = (1 << n);
 		const int MOD = 998244353, g = 3;
 		const int ROOT = 565042129;
 		
 		NTT() : revers(size), root(size),
        fftA(size), fftB(size) {
 			root[0] = 1;
 			for (int i = 1; i < size; i++)
 				root[i] = (ll) root[i - 1] * ROOT % MOD;	
 		}

 		void fft(vector <int> &poly, int newN) {
 			revers[0] = 0;
 			for (int i = 1; i < (1 << newN); i++) {
 				if (i % 2 == 0) revers[i] = revers[i / 2] / 2;
 				else revers[i] = revers[i / 2] / 2 + (1 << (newN - 1));
 				if (revers[i] < i) swap(poly[revers[i]], poly[i]);
 			}

 			for (int level = 1; level <= newN; level++)
 			 	for (int block = 0; block < (1 << (newN - level)); block++)
 			 		for (int step = 0; step < (1 << (level - 1)); step++) {
 			 			int num1 = (block << level) + step;
 			 			int num2 = num1 + (1 << (level - 1));
 			 			int valA = poly[num1];
 			 			int valB = (ll) root[step << (n - level)] * poly[num2] % MOD;
 			 			poly[num1] = (valA + valB) % MOD;
 			 			poly[num2] = (valA - valB + MOD) % MOD;	 	
 			 		}
 		}

 		void rev_fft(vector <int> &poly, int step) {
 		 	fft(poly, step);
 		 	reverse(poly.begin() + 1, poly.begin() + (1 << step));
 		 	int inv_size = NT::binpow((1 << step), MOD - 2, MOD);
 		 	for (int i = 0; i < (1 << step); i++) poly[i] = (ll) poly[i] * inv_size % MOD;
 		}

 		vector <int> multiply(const vector <int> &A, const vector <int> &B, int step) {
 		 	fill(fftA.begin(), fftA.begin() + (1 << step), 0);
 		 	fill(fftB.begin(), fftB.begin() + (1 << step), 0);
 		 	for (int i = 0; i < (int) A.size(); i++) fftA[i] = A[i] % MOD;
 		 	for (int i = 0; i < (int) B.size(); i++) fftB[i] = B[i] % MOD;
 		 	
 		 	fft(fftA, step);
 		 	fft(fftB, step);
 		 	for (int i = 0; i < (1 << step); i++) fftA[i] = (ll) fftA[i] * fftB[i] % MOD;
 		 	rev_fft(fftA, step);

 		 	vector <int> result(1 << step);
 		 	for (int i = 0; i < (1 << step); i++) result[i] = fftA[i];
            return result;
 		}
 	private:
 		vector <int> root, revers, fftA, fftB;
 	};
}
