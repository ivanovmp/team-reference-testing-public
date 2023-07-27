mkdir ./temp
cat "./SuffixAutomaton/testing/Timus.1590. Bacon’s Cipher/pre.cpp" ./utils/typedefs.hpp ./SuffixAutomaton/suffix_automaton.hpp "./SuffixAutomaton/testing/Timus.1590. Bacon’s Cipher/post.cpp" > ./temp/code.cpp
python3 ./utils/online_judges_benchmark.py ./temp/code.cpp timus 1590 -t Accepted
