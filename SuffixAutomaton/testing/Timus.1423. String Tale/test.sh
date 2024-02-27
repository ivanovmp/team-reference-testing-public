mkdir ./temp
cat "./SuffixAutomaton/testing/Timus.1423. String Tale/pre.cpp" ./utils/typedefs.hpp ./SuffixAutomaton/suffix_automaton_template.hpp "./SuffixAutomaton/testing/Timus.1423. String Tale/post.cpp" > ./temp/code.cpp
python3 ./utils/online_judges_benchmark.py ./temp/code.cpp timus 1423 -t Accepted
