#pragma once

namespace FFT {
    template<class db>
    struct Complex {
        db real, image;

        Complex(db _real = 0, db _image = 0) : real(_real), image(_image) {}

        Complex operator+(const Complex &other) { return Complex(real + other.real, image + other.image); }

        Complex operator-(const Complex &other) { return Complex(real - other.real, image - other.image); }

        Complex operator*(const Complex &other) {
            return Complex(real * other.real - image * other.image, real * other.image + image * other.real);
        }

        Complex operator/(db x) { return Complex(real / x, image / x); }
    };

    template<class db>
    class FFT {
        //double vs long double!!!
    public:
        const db PI = 4 * atan2(1, 1);

        FFT(int _n) : size(1 << _n), n(_n) {
            revers.resize(size), root.resize(size);
            fftA.resize(size), fftB.resize(size);
            for (int i = 0; i < size / 2; i++) {
                root[i] = Complex<db>(cos(2 * PI * i / size), sin(2 * PI * i / size));
                root[i + size / 2] = Complex<db>(-root[i].real, -root[i].image);
            }
        }

        void fft(vector<Complex<db>> &poly, int newN) {
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
                        Complex<db> valA = poly[num1];
                        Complex<db> valB = root[step << (n - level)] * poly[num2];
                        poly[num1] = valA + valB;
                        poly[num2] = valA - valB;
                    }
        }

        void rev_fft(vector<Complex<db>> &poly, int step) {
            fft(poly, step);
            reverse(poly.begin() + 1, poly.begin() + (1 << step));
            for (int i = 0; i < (1 << step); i++) poly[i] = poly[i] / (1 << step);
        }

        vector<db> prod(const vector<db> &A, const vector<db> &B, int step) {
            fill(fftA.begin(), fftA.begin() + (1 << step), 0);
            fill(fftB.begin(), fftB.begin() + (1 << step), 0);
            for (int i = 0; i < (int) A.size(); i++) fftA[i] = A[i];
            for (int i = 0; i < (int) B.size(); i++) fftB[i] = B[i];

            fft(fftA, step);
            fft(fftB, step);
            for (int i = 0; i < (1 << step); i++) fftA[i] = fftA[i] * fftB[i];
            rev_fft(fftA, step);

            vector<db> result(1 << step);
            for (int i = 0; i < (1 << step); i++) result[i] = fftA[i].real;
            return result;
        }

    private:
        int size, n;
        vector<Complex<db>> root;
        vector<int> revers;

        vector<Complex<db>> fftA, fftB;
    };
}
