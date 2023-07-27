mkdir ./temp
cat "./burrows_wheeler/testing/Timus.1322. Spy/pre.cpp" ./utils/typedefs.hpp ./burrows_wheeler/burrows_wheeler.hpp "./burrows_wheeler/testing/Timus.1322. Spy/post.cpp" > ./temp/code.cpp
python3 ./utils/online_judges_benchmark.py ./temp/code.cpp timus 1322 -t Accepted
