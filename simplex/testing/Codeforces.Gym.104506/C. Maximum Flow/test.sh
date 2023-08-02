mkdir ./temp
cat <(echo -n "// ") <(date) "./simplex/testing/Codeforces.Gym.104506/C. Maximum Flow/pre.cpp" ./simplex/simplex.hpp "./simplex/testing/Codeforces.Gym.104506/C. Maximum Flow/post.cpp" > ./temp/code.cpp
python3 ./utils/online_judges_benchmark.py ./temp/code.cpp codeforces C -c 104506 -s gym -t OK
