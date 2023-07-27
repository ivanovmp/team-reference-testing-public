mkdir ./temp
cat "./suffix_array/testing/Timus.1590. Bacon’s Cipher/pre.cpp" ./utils/typedefs.hpp ./suffix_array/suffix_array.hpp "./suffix_array/testing/Timus.1590. Bacon’s Cipher/post.cpp" > ./temp/code.cpp
python3 ./utils/online_judges_benchmark.py ./temp/code.cpp timus 1590 -t Accepted
