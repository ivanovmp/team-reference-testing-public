mkdir ./temp
cat "./simplex/testing/Timus.1417. Space Poker 2/pre.cpp" ./simplex/simplex.hpp "./simplex/testing/Timus.1417. Space Poker 2/post.cpp" > ./temp/code.cpp
python3 ./utils/online_judges_benchmark.py ./temp/code.cpp timus 1417 -t Accepted
