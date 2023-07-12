mkdir ./temp
cat "./basic_strings/testing/Timus.1423. String Tale/pre.cpp" ./utils/typedefs.hpp ./basic_strings/z_function.hpp "./basic_strings/testing/Timus.1423. String Tale/post.cpp" > ./temp/code.cpp
python3 ./utils/online_judges_benchmark.py ./temp/code.cpp timus 1423 -t Accepted
