# ГАЙД по автоматическому тестированию задач

В этом гайде описано, как делать автоматическое тестирование ваших задач. Вы можете тестировать как своими тестами (рекомендуется [GTest](https://github.com/google/googletest)), так и в проверяющих системах (сейчас поддерживаются [Codeforces](https://codeforces.com/) и [Timus Online Judge](https://acm.timus.ru/)). Все выбранные названия можно менять на своё усмотрение, но постарайтесь придерживаться предложенных расширений файлов.
## Общий алгоритм
1. Выделите папку, посвящённую вашему алгоритму. Можно выделить её в корневом каталоге, но, если, например, алгоритм входит в некоторое выделенное семейство алгоритмов, можете создать поддиректорию в папке этого семейства. Мы будем называть эту директорию `cool_algorithm`.
2. Напишите алгоритм в header-only-формате: один (реже несколько) `.hpp`-файл (для удобства будем его называть `cool_algorithm/algo.hpp`). Он должен быть самодостаточным — должна быть возможность, за`#include`ив его и не меняя содержимое, сдать задачу. Исключение из самодостаточности одно: старайтесь в файле не писать `#include`-ы, `#define`-ы, alias-ы и остальные вещи, которые не пригодятся в PDF-ке [TRD](https://neerc.ifmo.ru/information/team-reference.html).
3. Сделайте папку `cool_algorithm/testing`. Каждому тесту должна соответствовать своя подпапка в этой папке.
4. Допустим, мы разрабатываем тест в папке `cool_algorithm/testing/test`. Основной принцип — сделать некую команду, которая, будучи запущена на файлах из этой папки, завершится успешно (код возврата 0), если алгоритм корректный и быстрый, и неуспешно (код возврата не 0) в противном случае. Кроме файлов из этой папки, команда может пользоваться заголовочными файлами (как `cool_algorithm/algo.hpp`, так и другими при надобности) и файлами в корневой папке `/utils`. Команда может быть не одна — в таком случае алгоритм должен быть рабочим тогда и только тогда, когда все эти команды завершаются успешно. Назовём эти команды `command_1`, ..., `command_c`.
5. Создайте YAML-файл в папке `.github/workflows`, который будет отвечать за рабочий поток, тестирующий ваш алгоритм. Рабочий поток — это комплекс мероприятий, который будет проводиться при каждом изменении коммита в ветке master. Список всех рабочих потоков есть во вкладке [Actions](https://github.com/ivanovmp/team-reference-testing-public/actions). Типичный workflow-файл выглядит так:
  ```
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
.
.
.
    - name: <название последнего шага в свободной форме, но не содержащее пробелов>
      run: command_c
  ```
  Такой шаблон создаст рабочий поток, состоящий из одного теста. В один рабочий поток можно и объединить несколько тестов, для этого `jobs` должен состоять не из одного задания, а из нескольких:
  ```
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
.
.
.
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
.
.
.
    - name: step_c_2
      run: command_2_c_2
.
.
.
  ```
Далее опишем тестирование поподробнее.

## Как тестировать Google-тестами?
1. В папке `cool_algorithm/testing/test` нужны файлы `CMakeLists.txt` и `CMakeLists.txt.in`. Проще всего их скопировать, например, [отсюда](https://github.com/ivanovmp/team-reference-testing-public/tree/main/FFT/testing/test). Они отвечают за объявление проекта с главным файлом `cool_algorithm/testing/test/main.cpp`, а также за автоматическое скачивание последней версии репозитория [GTest](https://github.com/google/googletest).
2. Создайте файл `cool_algorithm/testing/test/main.cpp`. Сделайте в нём нужные `#include`-ы и alias-ы, затем напишите `#include "cool_algorithm/algo.hpp"` и, наконец, и напишите в нём один или несколько тестов, проверяющих работу вашего алгоритма. Простейший пример можно найти [тут](https://github.com/ivanovmp/team-reference-testing-public/blob/main/simplex/testing/gtest/main.cpp).
3. Команды в YAML-файле рабочего потока должны быть следующими:
  ```
      run: cd cool_algorithm/testing/test/ && cmake .
    - name: gtest-make
      run: cd cool_algorithm/testing/test && make
    - name: gtest
      run: cd cool_algorithm/testing/gtest && ./название_исполняемого_файла
```
  Название исполняемого файла — это название CMake-цели, которое вы можете найти внизу в файле `CMakeLists.txt`. Например, в [этом примере](https://github.com/ivanovmp/team-reference-testing-public/tree/main/FFT/testing/test/CMakeLists.txt) цель называется `TestTeamReferenceFFT`. Вы можете, конечно, её переименовать.

Подробнее о том, какие возможности предоставляет фреймворк [Google Test](https://github.com/google/googletest), смотрите в его документации и в примерах в нашем репозитории.
