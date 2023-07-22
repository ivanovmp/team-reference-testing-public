mkdir ./temp
cat date ./dinic/testing/marbles/pre.cpp ./dinic/flow.hpp ./dinic/testing/marbles/post.cpp > ./temp/code.cpp
python3 ./utils/online_judges_benchmark.py ./temp/code.cpp codeforces I -c 103855 -s gym -t OK
