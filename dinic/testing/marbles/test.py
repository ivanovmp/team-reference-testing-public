import datetime
import orjson
import subprocess
import sys


source = "// Urgant Team: " + datetime.datetime.now().isoformat() + "\n"
with open("Source_pre.cpp", "r", encoding="utf-8") as f:
  source += f.read()
with open("../../flow.cpp", "r", encoding="utf-8") as f:
  source += f.read()
with open("Source.cpp", "r", encoding="utf-8") as f:
  source += f.read()
with open("main.cpp", "w", encoding="utf-8") as f:
  f.write(source)

result = subprocess.run(["python3", "../../../utils/online_judges_benchmark.py", "main.cpp", "codeforces", "I", "-c", "103855", "-s", "gym"], stdout=subprocess.PIPE)
print(result)
verdict = orjson.loads(result.stdout)
if verdict.get("verdict") != "OK":
  raise Exception(str(verdict))
