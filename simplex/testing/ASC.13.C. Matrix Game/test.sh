mkdir ./temp
cat <(echo -n "// ") <(date) "./simplex/testing/ASC.13.C. Matrix Game/pre.cpp" ./simplex/simplex.hpp "./simplex/testing/ASC.13.C. Matrix Game/post.cpp" > ./temp/code.cpp
python3 ./utils/online_judges_benchmark.py ./temp/code.cpp codeforces C -c 100216 -s gym -t OK
