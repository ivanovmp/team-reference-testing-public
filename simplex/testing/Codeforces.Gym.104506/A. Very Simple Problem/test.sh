mkdir ./temp
cat <(echo -n "// ") <(date) "./simplex/testing/Codeforces.Gym.104506/A. Very Simple Problem/pre.cpp" ./simplex/simplex.hpp "./simplex/testing/Codeforces.Gym.104506/A. Very Simple Problem/post.cpp" > ./temp/code.cpp
python3 ./utils/online_judges_benchmark.py ./temp/code.cpp codeforces A -c 104506 -s gym -t OK
