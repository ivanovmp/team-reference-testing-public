mkdir ./temp
cat <(echo -n "// ") <(date) "./simplex/testing/NERC.2020.Online.O. Optimum Server Location/pre.cpp" ./simplex/simplex.hpp "./dinic/testing/NERC.2020.Online.O. Optimum Server Location/post.cpp" > ./temp/code.cpp
python3 ./utils/online_judges_benchmark.py ./temp/code.cpp codeforces O -c 102896 -s gym -t OK
