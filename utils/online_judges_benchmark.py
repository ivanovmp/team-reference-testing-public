from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.common.by import By
from selenium.webdriver.chrome.service import Service as ChromeService
from selenium.webdriver.chrome import service
from webdriver_manager.chrome import ChromeDriverManager
from webdriver_manager.opera import OperaDriverManager

from webdriver_manager.core.utils import ChromeType
from selenium.webdriver.chrome.options import Options
from selenium.webdriver.chrome.service import Service
from time import sleep
import requests
import orjson
from pathlib import Path
import argparse
import sys
from sys import platform
import os
from dataclasses import dataclass


def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)


def get_chrome_driver() -> webdriver.Chrome:
    chrome_service = Service(ChromeDriverManager(chrome_type=ChromeType.CHROMIUM).install())
    chrome_options = Options()
    options = [
        "--headless",
        "--disable-gpu",
        "--window-size=1920,1200",
        "--ignore-certificate-errors",
        "--disable-extensions",
        "--no-sandbox",
        "--disable-dev-shm-usage"
    ]
    for option in options:
        chrome_options.add_argument(option)
    return webdriver.Chrome(service=chrome_service, options=chrome_options)


def get_opera_driver() -> webdriver.Remote:
    webdriver_service = service.Service(OperaDriverManager().install())
    webdriver_service.start()
    webdriver_options = webdriver.ChromeOptions()
    options = [
        "--headless",
        "--disable-gpu",
        "--window-size=1920,1200",
        "--ignore-certificate-errors",
        "--disable-extensions",
        "--no-sandbox",
        "--disable-dev-shm-usage"
    ]
    for option in options:
        webdriver_options.add_argument(option)
    webdriver_options.add_experimental_option('w3c', True)
    return webdriver.Remote(webdriver_service.service_url, options=webdriver_options)


@dataclass
class SubmissionResult:
    memory: int
    time: int
    verdict: str


class Judge:
    def __init__(self, login_str: str, password: str) -> None:
        self.login_str = login_str
        self.password = password

    def login(self) -> None:
        pass

    def submit(self, problemset_name: str, contest_name: str, problem_name: str, submission_filename: str) -> str:
        pass

    def get_verdict(self, problemset_name: str, contest_name: str, problem_name: str, submission_number: str,
                    wait_time: float = 5) -> SubmissionResult:
        pass

    def finish(self) -> None:
        pass

class Codeforces(Judge):
    def __init__(self, login_str: str, password: str) -> None:
        super().__init__(login_str, password)
        self.driver = get_chrome_driver()

    def login(self) -> None:
        self.driver.get("https://codeforces.com/enter")
        login_input = self.driver.find_element(By.ID, "handleOrEmail")
        password_input = self.driver.find_element(By.ID, "password")
        login_button = self.driver.find_element(By.CLASS_NAME, "submit")

        login_input.send_keys(self.login_str)
        password_input.send_keys(self.password)
        eprint("Typed in login and password.")
        login_button.click()
        eprint("Clicked! Waiting for result...")

        while self.driver.current_url != self.get_link("/"):
            sleep(.05)
        eprint(f"Logged in! Redirected to page {self.driver.current_url}.")

    def get_link(self, link) -> str:
        return "https://codeforces.com" + link

    def get_submit_link_index(self):
        return 1 if sys.platform == "win32" else 2

    def get_problem_link(self, problemset_name: str, contest_name: str, problem_name: str) -> str:
        if problemset_name == "gym":
            return self.get_link(f"/{problemset_name}/{contest_name}/problem/{problem_name}")
        else:
            return self.get_link(f"/{problemset_name}/problem/{contest_name}/{problem_name}")

    def get_submit_link(self, problemset_name: str, contest_name: str) -> str:
        if problemset_name == "gym":
            return self.get_link(f"/{problemset_name}/{contest_name}/submit")
        else:
            return self.get_link(f"/{problemset_name}/submit")

    def get_status_link(self, problemset_name: str, contest_name: str) -> str:
        if problemset_name == "gym":
            return self.get_link(f"/{problemset_name}/{contest_name}/my")
        else:
            return self.get_link(f"/{problemset_name}/status?my=on")

    def submit(self, problemset_name: str, contest_name: str, problem_name: str, submission_filename: str) -> str:
        self.driver.get(self.get_problem_link(problemset_name, contest_name, problem_name))
        menu = self.driver.find_element(By.CLASS_NAME, "second-level-menu-list")
        links = menu.find_elements(By.XPATH, ".//li/a")
        submit_link = links[1]
        eprint(f"Read the statement on page {self.driver.current_url}.")
        submit_link.click()
        eprint(f"Went to submit tab: {self.driver.current_url}.")

        language_field = self.driver.find_element(By.NAME, "programTypeId")
        language_field.click()
        for our_language_field in language_field.find_elements(By.XPATH, ".//option"):
            if our_language_field.get_attribute("value") == "54":
                break
        our_language_field.click()

        choose_file = self.driver.find_element(By.NAME, "sourceFile")
        choose_file.send_keys(str(Path(submission_filename).absolute()))
        submit_button = self.driver.find_element(By.CLASS_NAME, "submit")
        submit_button.click()

        while self.driver.current_url == self.get_submit_link(problemset_name, contest_name):
            sleep(.05)

        eprint(f"Passed to page {self.driver.current_url}")

        if self.driver.current_url != self.get_status_link(problemset_name, contest_name):
            return {}

        rows = self.driver.find_elements(By.CLASS_NAME, "highlighted-row")
        submission_number = rows[0].get_attribute("data-submission-id")
        eprint(f"Submission number is {submission_number}.")
        return submission_number

    def finish(self) -> None:
        self.driver.close()

    def get_verdict(self, problemset_name: str, contest_name: str, problem_name: str, submission_number: str, wait_time: float = 5.) -> SubmissionResult:
        for i in range(20):
            address = self.get_link(f"/api/contest.status?contestId={contest_name}&count=100&handle={self.login_str}")
            eprint(f"Trying to get verdict via {address}.")
            r = requests.get(address)
            response = orjson.loads(r.text)
            if "status" not in response or response["status"] != "OK":
                eprint(f"Status is not OK, try again in {wait_time} second(s)...")
                sleep(wait_time)
                wait_time = max(1., 2 * wait_time)
                continue
            my_submission = None
            for submission in response["result"]:
                if str(submission["id"]) == submission_number:
                    my_submission = submission
                    break
            if my_submission is None:
                eprint(f"Submission {submission_number} not found, halting.")
                return {} # TODO: throw
            verdict = my_submission.get("verdict")
            if verdict is not None and verdict != "TESTING":
                eprint(f"Got verdict {verdict}")
                return my_submission # TODO: convert to SubmissionResult
            eprint(f"Still testing, try again in {wait_time} second(s)...")
            sleep(wait_time)
            wait_time = max(1., 2 * wait_time)


class TimusOnlineJudge(Judge):
    def __init__(self, login_str: str, password: str) -> None:
        super().__init__(login_str, password)

    def submit(self, problemset_name: str, contest_name: str, problem_name: str, submission_filename: str) -> str:
        if not problemset_name:
            problemset_name = "1"
        with open(submission_filename, 'r') as f:
            code = f.read()
        for i in range(20):
            resp = requests.post(url='https://acm.timus.ru/postsubmit.aspx', data={
                "judge_id": self.login_str,
                "space": problemset_name,
                "num": problem_name,
                "language": "cpp",
                "source": code,
            })
            content = resp.content.decode('utf-8')
            if resp.ok and content.startswith('SUCCESS'):
                submission_number = content[len('SUCCESS') + 2:]
                print(f"Submitted to Timus (problem https://acm.timus.ru/problem.aspx?space={problemset_name}&num={problem_name}). Link to the submission: https://acm.timus.ru/getsubmit.aspx/{submission_number}.cpp")
                return submission_number
            sleep(min((1 << i) * .2, 60))
        raise Exception(f"Couldn't submit problem {problem_name}: {resp.status_code=} ({resp.reason}), {content=}")

    def get_verdict(self, problemset_name: str, contest_name: str, problem_name: str, submission_number: str,
                    wait_time: float = 0) -> SubmissionResult:
        for i in range(20):
            resp = requests.get(url=f'https://acm.timus.ru/getverdict.aspx?id={submission_number}')
            content = resp.content.decode('utf-8')
            data = content.split('\r\n')
            if resp.ok and len(data) == 6 and data[0] == 'RESULT':
                data = content.split('\r\n')
                return SubmissionResult(memory=int(data[4]),
                                        time=int(data[3]),
                                        verdict=data[1])
            sleep(wait_time)
            wait_time = wait_time * 2 + .2
        raise Exception(f"Couldn't test submission {submission_number} for problem {problem_name}: {resp.status_code=} ({resp.reason}), {content=}")


def work(judge: Judge, problemset_name: str, contest_name: str, problem_name: str, filename: str):
    judge.login()
    submission_number = judge.submit(problemset_name, contest_name, problem_name, filename)
    judge.finish()
    if submission_number is None:
        return {}
    return judge.get_verdict(problemset_name, contest_name, problem_name, submission_number)


def construct_judge(judge_name: str):
    if judge_name == "codeforces":
        return Codeforces(os.environ['CODEFORCES_LOGIN'], os.environ['CODEFORCES_PASSWORD'])
    elif judge_name == 'timus':
        return TimusOnlineJudge(os.environ['TIMUS_LOGIN'], os.environ['TIMUS_PASSWORD'])
    else:
        raise Exception(f"Unknown judge: {judge_name}")


parser = argparse.ArgumentParser(
    prog="OnlineJudgesBenchmark",
    description='sends the code to an online judge and gets the verdict')

parser.add_argument("filename")
parser.add_argument("judge_name")
parser.add_argument("problem_name")
parser.add_argument("-c", "--contest", help="specify contest name", default="")
parser.add_argument("-s", "--set", help="specify problemset name", default="")
parser.add_argument("-t", "--throw", help="throw if the verdict is not as expected", default="")

args = parser.parse_args()
judge = construct_judge(args.judge_name)
result = work(judge, args.set, args.contest, args.problem_name, args.filename)
print(orjson.dumps(result).decode("utf-8"))
if args.throw and args.throw != result.verdict:
    raise Exception(f"Excepted verdict '{args.throw}', but found '{result.verdict}'")
