import urllib.request
from copy import copy


def decode_sequence(s: str) -> list[int]:
    ans = []
    for i in s.split('\n')[:-2]:
        if len(i) >= 2:
            ans.append(int(i.split(' ')[1]))
    return ans


def sequence_url(num: str) -> str:
    return f'https://oeis.org/{num}/b{num[1:]}.txt'


def get_sequence(num: str) -> list[int]:
    with urllib.request.urlopen(sequence_url(num)) as f:
        return decode_sequence(f.read().decode('utf-8'))


fact = [1]
for i in range(1, 51):
    fact.append(fact[-1] * i)


def choose(n: int, k: int) -> int:
    return fact[n] // (fact[k] * fact[n - k])


def prod(a: list[int], b: list[int], n: int) -> list[int]:
    ans = [0 for i in range(n)]
    for i in range(min(len(a), n)):
        for j in range(min(len(b), n - i)):
            ans[i + j] += choose(i + j, i) * a[i] * b[j]
    return ans


def find_log(v: list[int]) -> list[int]:
    a = copy(v)
    a[0] = 0
    ans = copy(a)
    power = copy(a)
    for i in range(2, len(v)):
        power = prod(power, a, len(v))
        for j in range(1, len(v)):
            assert(power[j] % i == 0)
            ans[j] -= power[j] // i * pow(-1, i)
    return ans

A006125 = get_sequence('A006125')
A001187 = get_sequence('A001187')

A006125_log = find_log(A006125)
print(A006125_log)
print(A001187)
for i in range(len(A001187)):
    if A001187[i] != A006125_log[i]:
        print(f'{i} is bad')
