# Module: tests.py
# Author: Adrián Ponechal (xponec01@stud.fit.vut.cz)
# Date: 2023-11-18
# Test program for testing functionality of current dns program

from digParser import parseDigOutput
from dnsParser import parseDnsOutput

# stasts variables
numOfTests=0
passedTests=0
failedTests=0

# teseting values
# domainNames = ("www.fit.vutbr.cz", "kazi.fit.vutbr.cz", "www.vut.cz", "merlin.fit.vutbr.cz", "facebook.com")
domainNames = ("www.fit.vutbr.cz", "kazi.fit.vutbr.cz", "www.facebook.com")

# ip4 testing values
ips4 = ("147.229.8.12","147.229.9.26")

# ip6 testing values
ips6 = ("2001:67c:1220:809:0:0:93e5:91a", "2a03:2880:f107:83:face:b00c:0:25de")

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

    # if one of the programs havent recieved data, skip test
    if len(gotAnswer) == 0 or len(expectedAnswer) == 0:
        return
    
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

def testSet(name: str, serchedNames, digArgs: list, dnsArgs: list):
    printTestSection(name)
    # parse results from file
    for domain in serchedNames:
        digArgs.append(domain)
        dnsArgs.append(domain)
        test(domain,digArgs,dnsArgs)
        digArgs.pop()
        dnsArgs.pop()

# wrong arguments testing
## server not given

## address not given

# -r 
def recursionOnlyTests():
    digArgs = ['@8.8.8.8']
    dnsArgs = ['-s','8.8.8.8', '-r']
    testSet("Recursive only queries [-r]",domainNames, digArgs ,dnsArgs)

# -x
# dig @8.8.8.8 example.com +norecurse 
def invertedQueriesTests():
    digArgs = ['@8.8.8.8', '+norecurse','-x']
    dnsArgs = ['-s','8.8.8.8' , '-x']
    testSet("Inverted queries [-x]", ips4, digArgs ,dnsArgs)

# -6
# dig AAAA @8.8.8.8 example.com +norecurse 
def ipv6Quries():
    digArgs = ['AAAA','@8.8.8.8', '+norecurse']
    dnsArgs = ['-s','8.8.8.8', '-6']
    testSet("IPv6 queries [-6]", domainNames, digArgs ,dnsArgs)

# -r -x
# dig -x @8.8.8.8 example.com 
def invertedRecursionQueries():
    digArgs = ['@8.8.8.8','-x']
    dnsArgs = ['-s','8.8.8.8','-r', '-x']
    testSet("Inverted recursive queries [-r, -x]", ips4, digArgs ,dnsArgs)

# -r -6
# dig AAAA @8.8.8.8 example.com 
def recursionIPv6Queries():
    digArgs = ['AAAA','@8.8.8.8']
    dnsArgs = ['-s','8.8.8.8','-r', '-6']
    testSet("Recursive IPv6 queries [-r, -6]", domainNames, digArgs ,dnsArgs)

# -x -6
# dig -x  @8.8.8.8 example.com +norecurse 
def invertedIPv6Queries():
    digArgs = ['@8.8.8.8','+norecurse','-x']
    dnsArgs = ['-s','8.8.8.8','-x', '-6']
    testSet("Inverted IPv6 queries [-x, -6]", ips6, digArgs ,dnsArgs)

# -r -x -6
# dig @8.8.8.8 -x example.com 
def invertedRecursiveIPv6Queries():
    digArgs = ['@8.8.8.8','-x']
    dnsArgs = ['-s','8.8.8.8','-r', '-6', '-x']
    testSet("Inverted IPv6 queries [-r, -6, -x]", ips6, digArgs ,dnsArgs)

# function for run all tests
def runAll():
    recursionOnlyTests()
    invertedQueriesTests()
    ipv6Quries()
    invertedRecursionQueries()
    recursionIPv6Queries()
    invertedIPv6Queries()
    invertedRecursiveIPv6Queries()

if __name__ == "__main__" :
    runAll()
    printStats()


