mkdir ./temp
cat "./palindromes/testing/Timus.1297 - Palindromes/pre.cpp" ./utils/typedefs.hpp ./palindromes/manacher.cpp "./palindromes/testing/Timus.1297 - Palindromes/post.cpp" > ./temp/code.cpp
python3 ./utils/online_judges_benchmark.py ./temp/code.cpp timus 1297 -t Accepted
