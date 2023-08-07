# ГАЙД по автоматическому тестированию задач

В этом гайде описано, как делать автоматическое тестирование ваших задач. Вы можете тестировать как своими тестами (рекомендуется [GTest](https://github.com/google/googletest)), так и в проверяющих системах (сейчас поддерживаются [Codeforces](https://codeforces.com/) и [Timus Online Judge](https://acm.timus.ru/)). Все выбранные названия можно менять на своё усмотрение, но постарайтесь придерживаться предложенных расширений файлов.
## Общий алгоритм
1. Выделите папку, посвящённую вашему алгоритму. Можно выделить её в корневом каталоге, но, если, например, алгоритм входит в некоторое выделенное семейство алгоритмов, можете создать поддиректорию в папке этого семейства. Мы будем называть эту директорию `cool_algorithm`.
2. Напишите алгоритм в header-only-формате: один (реже несколько) `.hpp`-файл (для удобства будем его называть `cool_algorithm/algo.hpp`). Он должен быть самодостаточным — должна быть возможность, за`#include`ив его и не меняя содержимое, сдать задачу. Исключение из самодостаточности одно: старайтесь в файле не писать `#include`-ы, `#define`-ы, alias-ы и остальные вещи, которые не пригодятся в PDF-ке [TRD](https://neerc.ifmo.ru/information/team-reference.html).
3. Сделайте папку `cool_algorithm/testing`. Каждому тесту должна соответствовать своя подпапка в этой папке.
4. Допустим, мы разрабатываем тест в папке `cool_algorithm/testing/test`. Основной принцип — сделать некую команду, которая, будучи запущена на файлах из этой папки, завершится успешно (код возврата 0), если алгоритм корректный и быстрый, и неуспешно (код возврата не 0) в противном случае. Кроме файлов из этой папки, команда может пользоваться заголовочными файлами (как `cool_algorithm/algo.hpp`, так и другими при надобности) и файлами в корневой папке `/utils`. Команда может быть не одна — в таком случае алгоритм должен быть рабочим тогда и только тогда, когда все эти команды завершаются успешно. Назовём эти команды `command_1`, ..., `command_c`.
5. Создайте YAML-файл в папке `.github/workflows`, который будет отвечать за рабочий поток, тестирующий ваш алгоритм. Рабочий поток — это комплекс мероприятий, который будет проводиться при каждом изменении коммита в ветке master. Список всех рабочих потоков есть во вкладке [Actions](https://github.com/ivanovmp/team-reference-testing-public/actions). Типичный workflow-файл выглядит так:
  ```yaml
name: <название рабочего потока в свободной форме>

on:
  push:
    branches: [ "main" ]
  pull_request:
    branches: [ "main" ]

jobs:
  <название теста в свободной форме, но не содержащее пробелов>:

    runs-on: ubuntu-latest

    steps:
    - uses: actions/checkout@v3
    - name: <название первого шага в свободной форме, но не содержащее пробелов>
      run: command_1
    - name: <название второго шага в свободной форме, но не содержащее пробелов>
      run: command_2
#
#
#
    - name: <название последнего шага в свободной форме, но не содержащее пробелов>
      run: command_c
  ```
  Такой шаблон создаст рабочий поток, состоящий из одного теста. В один рабочий поток можно и объединить несколько тестов, для этого `jobs` должен состоять не из одного задания, а из нескольких:
  ```yaml
name: Cool Algorithm CI

on:
  push:
    branches: [ "main" ]
  pull_request:
    branches: [ "main" ]

jobs:
  test_1:

    runs-on: ubuntu-latest

    steps:
    - uses: actions/checkout@v3
    - name: step_1
      run: command_1_1
    - name: step_2
      run: command_1_2
#
#
#
    - name: step_c_1
      run: command_1_c_1

  test_2:

    runs-on: ubuntu-latest

    steps:
    - uses: actions/checkout@v3
    - name: step_1
      run: command_2_1
    - name: step_2
      run: command_2_2
#
#
#
    - name: step_c_2
      run: command_2_c_2
#
#
#
  ```
Далее опишем тестирование поподробнее.

## Как тестировать Google-тестами?
1. В папке `cool_algorithm/testing/test` нужны файлы `CMakeLists.txt` и `CMakeLists.txt.in`. Проще всего их скопировать, например, [отсюда](https://github.com/ivanovmp/team-reference-testing-public/tree/main/FFT/testing/test). Они отвечают за объявление проекта с главным файлом `cool_algorithm/testing/test/main.cpp`, а также за автоматическое скачивание последней версии репозитория [GTest](https://github.com/google/googletest).
2. Создайте файл `cool_algorithm/testing/test/main.cpp`. Сделайте в нём нужные `#include`-ы и alias-ы, затем напишите `#include "cool_algorithm/algo.hpp"` и, наконец, и напишите в нём один или несколько тестов, проверяющих работу вашего алгоритма. Простейший пример можно найти [тут](https://github.com/ivanovmp/team-reference-testing-public/blob/main/simplex/testing/gtest/main.cpp).
3. Команды в YAML-файле рабочего потока должны быть следующими:
  ```yaml
    - name: gtest-cmake
      run: cd cool_algorithm/testing/test/ && cmake .
    - name: gtest-make
      run: cd cool_algorithm/testing/test && make
    - name: gtest
      run: cd cool_algorithm/testing/gtest && ./название_исполняемого_файла
```
  Последняя из этих команд упадёт (с ненулевым кодом возврата), если хотя бы один из тестов будет не пройден. Название исполняемого файла — это название CMake-цели, которое вы можете найти внизу в файле `CMakeLists.txt`. Например, в [этом примере](https://github.com/ivanovmp/team-reference-testing-public/tree/main/FFT/testing/test/CMakeLists.txt) цель называется `TestTeamReferenceFFT`. Вы можете, конечно, её переименовать.

Вы можете делать очень долгие тесты. Поскольку наш репозиторий открытый, все рабочие потоки будут запускаться на машинах GitHub бесплатно. Но учитывайте, что задача, которая займёт более шести часов, будет принудительно остановлена.

Подробнее о том, какие возможности предоставляет фреймворк [Google Test](https://github.com/google/googletest), смотрите в его документации и в примерах в нашем репозитории.

## Что такое `online_judges_benchmark.py`?
Файл [/utils/online_judges_benchmark.py](https://github.com/ivanovmp/team-reference-testing-public/blob/ivanovmp-patch-1-1/utils/online_judges_benchmark.py) — это питоновский скрипт, предназначенный для засылки кода в проверяющие системы и получения вердикта. У него есть три зависимости: `selenium`, `orjson` и `webdriver-manager`. Их очень легко установить: `pip install orjson selenium webdriver-manager`.

Скрипт имеет три обязательных аргумента: `filename` (путь к файлу с кодом посылки), `judge_name` и `problem_name`, их следует указать в первую очередь. Также есть четыре необязательных аргумента:
* `-c contest_name`: если задача принадлежит какому-то контесту, то можно указать название этого контеста;
* `-s problemset_name`: если контест принадлежит какому-то проблемсету, то можно указать название этого проблемсета;
* `-l language`: если базовый C++ вас не устроит для этой посылки, то можете указать другой язык;
* `-t verdict`: если вы ожидаете, что посылка получит некоторый вердикт, укажите его в этом аргументе. Если проверяющая система выдаст другой вердикт, то скрипт кинет исключение (и упадёт с ненулевым кодом возврата).

## Как тестировать посылками в проверяющие системы?

1. Создайте в папке `cool_algorithm/testing/test` файлы `pre.cpp` и `post.cpp`. В `pre.cpp` должен быть код, который должен идти до `cool_algorithm/algo.hpp`, а в `post.cpp` — после. Суммарно эти три файла должны образовывать единый `cpp`-файл, который должен компилироваться — он и будет вашей посылкой по задаче.
2. Создайте файл `cool_algorithm/testing/test/test.sh`. Он будет выполнять две вещи: составление файла с посылкой и засылка этого файла.
3. Добавьте в `cool_algorithm/testing/test/test.sh` строчку `cat "cool_algorithm/testing/test/pre.cpp" "cool_algorithm/algo.hpp" "cool_algorithm/testing/test/post.cpp" > ./temp/code.cpp`. После её вызова в Bash все три указанных файла объединятся в один файл `./temp/code.cpp`.
4. Добавьте в `cool_algorithm/testing/test/test.sh` строчку `python3 ./utils/online_judges_benchmark.py ./temp/code.cpp ...`, где вместо многоточия указаны параметры, такие как название проверяющей системы и имя задачи.
5. Работа в YAML-файле рабочего потока будет состоять из следующих двух шагов:
```yaml
  oj:
    runs-on: ubuntu-latest
    steps:
    - uses: actions/checkout@v3
    - name: install
      run: pip install orjson selenium webdriver-manager
    - name: test
      env:
        <информация о паролях>
      run: bash "cool_algorithm/testing/test/test.sh"
```

### Как тестировать посылками в Timus?
У [Timus](https://acm.timus.ru/) всё просто: `judge_name` — `timus`, `problem_name` — номер задачи. У всех задач на Timus проблемсет называется `1`, но он такой и проставляется по умолчанию. Положительный вердикт называется `Accepted`.

К сожалению, стандартный C++ в Тимусе — Visual C++. Если он вас не устраивает, укажите `-l 68` — и будет использован компилятор GNU.

Таким образом, типичная строчка для тестирования решений выглядит так:
```bash
python3 ./utils/online_judges_benchmark.py ./temp/code.cpp timus <номер задачи> -t Accepted
```
Если вас не устраивает Visual C++, то так:
```bash
python3 ./utils/online_judges_benchmark.py ./temp/code.cpp timus <номер задачи> -l 68 -t Accepted
```

В YAML-файле надо будет сообщить GitHub, чтобы он дал скрипту логин и пароль от нашего аккаунта Timus. Таким образом, описание задачи может выглядеть так:
```yaml
  oj:
    runs-on: ubuntu-latest
    steps:
    - uses: actions/checkout@v3
    - name: install
      run: pip install orjson selenium webdriver-manager
    - name: test
      env:
        TIMUS_LOGIN: ${{ secrets.TIMUS_LOGIN }}
        TIMUS_PASSWORD: ${{ secrets.TIMUS_PASSWORD }}
      run: bash "cool_algorithm/testing/test/test.sh"
```
Вы можете найти пример скрипта [тут](https://github.com/ivanovmp/team-reference-testing-public/blob/main/SuffixAutomaton/testing/Timus.1423.%20String%20Tale/test.sh), а пример YAML [тут](https://github.com/ivanovmp/team-reference-testing-public/blob/ivanovmp-patch-1-1/.github/workflows/SufAuto%20Timus.1423.yml).

### Как тестировать посылками в Codeforces?
У Codeforces всё слегка посложнее. `judge_name` — `codeforces`. `problem_name` — буква задачи. Проблемсет зависит от того, откуда взята задача: если из контеста, то `contest`, если из тренировки — `gym`. В качестве `contest_name` следует указать номер контеста или номер тренировки, соответственно. Язык по умолчанию GNU C++17, менять пока нельзя. Положительный вердикт называется `OK`. Таким образом, посылать код в тренировку 123456 в задачу A3 надо так:
```bash
python3 ./utils/online_judges_benchmark.py ./temp/code.cpp codeforces A3 -c 123456 -s gym -t OK
```

Есть ещё несколько нюансов. Во-первых, Codeforces не разрешает отправлять два раза один и тот же код по задаче. Так что с некоторого момента все CI начнут падать. Сейчас эту проблему предлагается решать в лоб: дописывать в начало файла с кодом комментарий с текущей датой и временем:
```bash
cat <(echo -n "// ") <(date) "cool_algorithm/testing/test/pre.cpp" "cool_algorithm/algo.hpp" "cool_algorithm/testing/test/post.cpp" > ./temp/code.cpp
```
Позже, может, получится придумать способ, как отправлять решение в проверяющую систему лишь в случае, если поменялся файл, важный для этого решения, но пока только так.

У Codeforces нет API для отправки решений, поэтому бот имитирует реального человека: по-человечески открывает браузер, логинится, переходит на страницу задачи, отправляет решение и смотрит на страничке "статус" номер своей посылки, у которой потом узнаёт вердикт через API. Самое сложное тут — открыть браузер. Github Actions притворяется, что на инвокерах Ubuntu предустановлены Google Chrome и Mozilla Firefox. Это наглая ложь, ничего там не установлено. Текущее решение — когда машина запускается, удалить предустановленный Firefox и установить вместо него нормальный. За это отвечает скрипт [prepare-firefox.sh](https://github.com/ivanovmp/team-reference-testing-public/blob/main/utils/firefox/prepare-firefox.sh). Его надо будет вызвать в YAML-файле.

Наконец, сабмиттер на Codeforces тоже нуждается в том, чтобы ему сообщили логин и пароль. Таким образом, описание задачи выглядит так:
```yaml
  codeforces:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      - name: install
        run: sh ./utils/firefox/prepare-firefox.sh
      - name: test-cf
        env:
          CODEFORCES_LOGIN: ${{ secrets.CODEFORCES_LOGIN }}
          CODEFORCES_PASSWORD: ${{ secrets.CODEFORCES_PASSWORD }}
      run: bash "cool_algorithm/testing/test/test.sh"
```
Если задача из приватной тренировки, то публичное API не даст посмотреть вердикт посылки. Тогда надо дополнительно сообщить сабмитерру ключ API:
```yaml
  codeforces:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      - name: install
        run: sh ./utils/firefox/prepare-firefox.sh
      - name: test-cf
        env:
          CODEFORCES_LOGIN: ${{ secrets.CODEFORCES_LOGIN }}
          CODEFORCES_PASSWORD: ${{ secrets.CODEFORCES_PASSWORD }}
          CODEFORCES_API_KEY: ${{ secrets.CODEFORCES_API_KEY }}
          CODEFORCES_API_SECRET: ${{ secrets.CODEFORCES_API_SECRET }}
      run: bash "cool_algorithm/testing/test/test.sh"
```
