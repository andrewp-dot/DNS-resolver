import subprocess as sp
from digParser import parseDigOutput
from dnsParser import parseDnsOutput

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
    if(passedTests == numOfTests):
        printSuccess("All tests passed.")
        return

    print("Passed tests: ", end="")
    printSuccess(passedTests)
    print("Failed tests: ", end="")
    printFail(failedTests)

def printResult(testName: str, isOk: bool, gotAnswer: tuple, expectedAnswer: tuple):
    print('%20s' % testName, end=": ")
    if isOk:
        printSuccess("Pass")
    else:
        printFail("Fail")
        print("Expected: ")
        print(expectedAnswer)
        print(" Got: ")
        print(gotAnswer)

def compareLists(gotList: list, expectedList: list) -> bool:
    if len(gotList) != len(expectedList):
        return False  # If the lengths are different, the lists are not the same

    for dict1, dict2 in zip(gotList, expectedList):
        if dict1 != dict2:
            return False  # If any dictionaries are not the same, the lists are not the same

    return True  # If no differences were found, the lists are the same

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
    
    printResult(testName, resultIsOk, gotAnswer, expectedAnswer)

def test(testName: str, digArgList: list, dnsArgList: list):
    expectedAnswer = parseDigOutput(digArgList)
    gotAnswer = parseDnsOutput(dnsArgList)
    compareAnswers(testName, gotAnswer,expectedAnswer)


# wrong arguments testing
## server not given

## address not given

# -r 
def recursionOnlyTests():
    printTestSection("Recursive only queries [-r]")
    digArgs = ['@8.8.8.8']
    dnsArgs = ['-r', '-s','8.8.8.8']
    for domain in domainNames:
        digArgs.append(domain)
        dnsArgs.append(domain)
        test(domain,digArgs,dnsArgs)
        digArgs.pop()
        dnsArgs.pop()

# -x
# dig @8.8.8.8 example.com +norecurse 
def invertedQueriesTests():
    printTestSection("Inverted queries [-x]")
    digArgs = ['@8.8.8.8', '+norecurse']
    dnsArgs = ['-x', '-s','8.8.8.8']
    for domain in ips4:
        digArgs.append(domain)
        dnsArgs.append(domain)
        test(domain,digArgs,dnsArgs)
        digArgs.pop()
        dnsArgs.pop()

# -6
# dig AAAA @8.8.8.8 example.com +norecurse 
def ipv6Quries():
    printTestSection("IPv6 queries [-6]")
    digArgs = ['AAAA','@8.8.8.8', '+norecurse']
    dnsArgs = ['-x', '-s','8.8.8.8', '-6']
    for domain in domainNames:
        digArgs.append(domain)
        dnsArgs.append(domain)
        test(domain,digArgs,dnsArgs)
        digArgs.pop()
        dnsArgs.pop()

# -r -x
# dig -x @8.8.8.8 example.com 
def invertedRecursionQueries():
    printTestSection("Inverted recursive queries [-r, -x]")
    digArgs = ['AAAA','@8.8.8.8']
    dnsArgs = ['-x', '-s','8.8.8.8', '-6']
    for domain in domainNames:
        digArgs.append(domain)
        dnsArgs.append(domain)
        test(domain,digArgs,dnsArgs)
        digArgs.pop()
        dnsArgs.pop()

# -r -6
# dig AAAA @8.8.8.8 example.com 

# -x -6
# dig -x AAAA @8.8.8.8 example.com +norecurse 

# -r -x -6
# dig AAAA @8.8.8.8 example.com -x

# function for run all tests
def runAll():
    recursionOnlyTests()
    invertedQueriesTests()
    ipv6Quries()

if __name__ == "__main__" :
    runAll()
    # test('TEST:',['@8.8.8.8', 'www.facebook.com'],['-s','8.8.8.8','www.facebook.com'])
    printStats()


