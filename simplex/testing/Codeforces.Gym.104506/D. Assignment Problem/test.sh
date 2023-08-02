mkdir ./temp
cat <(echo -n "// ") <(date) "./simplex/testing/Codeforces.Gym.104506/D. Assignment Problem/pre.cpp" ./simplex/simplex.hpp "./simplex/testing/Codeforces.Gym.104506/D. Assignment Problem/post.cpp" > ./temp/code.cpp
python3 ./utils/online_judges_benchmark.py ./temp/code.cpp codeforces D -c 104506 -s gym -t OK
