import subprocess as sp
from digParser import parseDigOutput
from dnsParser import parseDnsOutput

prog_name="./dns"
localhost="127.0.0.1"
port="53"

# stasts variables
numOfTests=0
passedTests=0
failedTests=0

# teseting values
domainNames = ("www.github.com", "www.facebook.com", "www.google.com", "github.com", "facebook.com", "google.com", "kazi.fit.vutbr.cz", "www.fit.vut.cz")

# ip4 testing values
ips4 = ("147.229.8.12","147.229.9.26")

# ip6 testing values
ips6 = ("2001:67c:1220:809::93e5:91a")


# function for colorful terminal output
def printFail(msg): print("\033[91m {}\033[00m" .format(msg),flush=True)
def printSuccess(msg): print("\033[92m {}\033[00m" .format(msg),flush=True)
def printTestSection(msg): print("\033[95m {}\033[00m" .format(msg),flush=True)

# functions for test count
def addPassedTest():
    global passedTests 
    global numOfTests
    passedTests += 1
    numOfTests += 1

def addFailedTest():
    global failedTests 
    global numOfTests
    failedTests += 1
    numOfTests += 1

def printStats():
    global passedTests 
    global failedTests 
    global numOfTests
    if(passedTests == passedTests):
        printSuccess("All tests passed.")
        return

    print("Passed tests: ", end="")
    printSuccess(passedTests)
    print("Failed tests: ", end="")
    printFail(failedTests)

# error codes
SUCCESS = 0
CONNECTION_FAILED = 1
NAME_NOT_EXIST = 2
NOT_FOUND = 3
WRONG_ARGUMENTS = 4
INTERNAL = 99

def printResult(testName: str, isOk: bool):
    print(testName, end=": ")
    if isOk:
        printSuccess("Pass")
    else:
        printFail("Fail")

def compareLists(gotList: list, expectedList: list) -> bool:
    gotList.sort()
    expectedList.sort()
    return gotList == expectedList

def compareAnswers(testName: str,gotAnswer: tuple, expectedAnswer: tuple):
    resultIsOk = True
    for answerType in range (0,3):
        listsAreSame = compareLists(gotAnswer[answerType], expectedAnswer[answerType])
        if not listsAreSame:
            resultIsOk = False 

    if resultIsOk:
        addPassedTest()
    else:
        addFailedTest()
    
    printResult(testName, resultIsOk)

def test(testName: str, digArgList: list, dnsArgList: list):
    expectedAnswer = parseDigOutput(digArgList)
    gotAnswer = parseDnsOutput(dnsArgList)
    compareAnswers(testName, gotAnswer,expectedAnswer)


# wrong arguments testing
## server not given

## address not given

# -r 

# -x
# dig @8.8.8.8 example.com +norecurse 

# -6
# dig AAAA @8.8.8.8 example.com +norecurse 

# -r -x
# dig -x @8.8.8.8 example.com 

# -r -6
# dig AAAA @8.8.8.8 example.com 

# -x -6
# dig -x AAAA @8.8.8.8 example.com +norecurse 

# -r -x -6
# dig AAAA @8.8.8.8 example.com -x


if __name__ == "__main__" :
    # this is function for new test
    test("TEST", ['@8.8.8.8', 'www.fit.vut.cz'],['-s','8.8.8.8','www.fit.vut.cz'])
    printStats()


