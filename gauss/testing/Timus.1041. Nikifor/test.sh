mkdir ./temp
cat "./gauss/testing/Timus.1041. Nikifor/pre.cpp" ./gauss/gauss.hpp "./gauss/testing/Timus.1041. Nikifor/post.cpp" > ./temp/code.cpp
python3 ./utils/online_judges_benchmark.py ./temp/code.cpp timus 1041 -t Accepted
