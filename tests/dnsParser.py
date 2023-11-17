import subprocess as sp
import os
from digParser import answerTemplate, soaAnswerTemplate
import sys

SUCCESS = 0
FORMAT_ERROR = 1
SERVER_FAILURE = 2
NAME_NOT_EXIST = 3
UNSUPPORTED_QUERY = 4
QUERY_REFUSED = 5
CONNECTION_FAILED = 6
WRONG_ARGUMENTS = 7
INTERNAL = 99

def createAnswer(answer: str):
    answer = answer.strip()
    answerParts = answer.split(", ")

    # skip ttl on index 3 
    if answerParts[0] == "SOA":
        return dict(soaAnswerTemplate)
    
    if len(answerParts) >= 5:
        return dict(answerTemplate,qname=answerParts[0], cls=answerParts[2], type=answerParts[1], answer=answerParts[4])
    return dict(answerTemplate)

def dnsExec(argList: list):
    cwd = os.getcwd()
    program = cwd + '/dns'
    command = [program] + argList
    result = sp.run(command, stdout=sp.PIPE, stderr=sp.PIPE)
    if result.returncode != SUCCESS:
        # print("FIRST TIME IT FAILED: ",file=sys.stderr, end="")
        # print(result.returncode, file=sys.stderr)
        result = sp.run(command, stdout=sp.PIPE, stderr=sp.PIPE)

    output = result.stdout.decode('ascii')
    lines = output.split('\n')
    return lines

def parseDnsOutput(argList: list):
    lines = dnsExec(argList=argList)

    # parse dns output to an arrays
    gotAnswers = []
    gotAuthority = []
    gotAdditional = []

    # flags for parsing
    isAnswering = False
    isAuthority = False
    isAdditional = False

    for line in lines:
        if "Answer section" in line:
            isAnswering = True
            continue

        if isAnswering:
            if "Authority section" in line:
                isAnswering = False
                isAuthority = True
                continue
            # create and object and push it to gotAuthority
            gotAnswers.append(createAnswer(line))
            continue
        
        if isAuthority:
            if "Additional section" in line:
                isAuthority = False
                isAdditional = True
                continue
            # create and object and push it to gotAuthority
            gotAuthority.append(createAnswer(line))
            continue
            
        if isAdditional:
            # create and object and push it to gotAdditional
            if line != "":
                gotAdditional.append(createAnswer(line))
            continue
    # if program havent recieved any data from server
    # if(gotAnswers == [] and gotAuthority == [] and gotAdditional == []):
    #     return ()
    return gotAnswers, gotAuthority, gotAdditional
