mkdir ./temp
cat "./FFT/testing/Timus.1996. Cipher Message 3/pre_fft.cpp" ./utils/typedefs.hpp ./basic_strings/z_function.hpp ./FFT/FFT.hpp "./FFT/testing/Timus.1996. Cipher Message 3/post_fft.cpp" > ./temp/code_fft.cpp
cat "./FFT/testing/Timus.1996. Cipher Message 3/pre_ntt.cpp" ./utils/typedefs.hpp ./basic_strings/z_function.hpp ./number_theory/NT.hpp ./FFT/NTT.hpp "./FFT/testing/Timus.1996. Cipher Message 3/post_ntt.cpp" > ./temp/code_ntt.cpp
echo "Trying FFT:"
python3 ./utils/online_judges_benchmark.py ./temp/code_fft.cpp timus 1996 -t Accepted
echo "Trying NTT:"
python3 ./utils/online_judges_benchmark.py ./temp/code_ntt.cpp timus 1996 -t Accepted
