import subprocess as sp

answerTemplate = {"qname": "","cls": "", "type": "", "answer": ""}
soaAnswerTemplate = {"None": "not implemented"}

def removeEmptyStrings(stringParts: list) -> str:
    while "" in stringParts:
        stringParts.remove("")
    return stringParts


def createAnswer(answer: str) -> dict:
    answerParts = answer.split()
    answerParts = removeEmptyStrings(stringParts=answerParts)

    # if answer is type SOA
    if answerParts[0] == "SOA":
        return dict(soaAnswerTemplate)

    # skip the ttl, ttl is on index 1
    if len(answerParts) >= 5:
        return dict(answerTemplate,qname=answerParts[0], cls=answerParts[2], type=answerParts[3], answer=answerParts[4])
    return dict(answerTemplate)

def digExec(argList: list):
    command = ['dig'] + argList
    # ['dig','@8.8.8.8', 'www.github.com']

    result = sp.run(command, stdout=sp.PIPE, stderr=sp.PIPE)
    output = result.stdout.decode('ascii')
    lines = output.split('\n')
    return lines

def parseDigOutput(argList: list):
    # split output to array
    # result = sp.run(['dig','@8.8.8.8', 'www.github.com'], stdout=sp.PIPE, stderr=sp.PIPE)
    # output = result.stdout.decode('ascii')
    lines = digExec(argList=argList)

    # parse dig output to an arrays
    expectedAnswers = []
    expectedAuthority = []
    expectedAdditional = []

    # flags for parsing
    isAnswering = False
    isAuthority = False
    isAdditional = False

    for line in lines:
        if line == ";; ANSWER SECTION:":
            isAnswering = True
            continue
        elif line == ";; AUTHORITY SECTRION:":
            isAuthority = True
            continue
        elif line == ";; ADDITIONAL SECTION:":
            isAdditional = True
            continue

        if isAnswering:
            if line.strip() == "":
                isAnswering = False
                continue
            # create and object and push it to expectedAuthority
            expectedAnswers.append(createAnswer(line))
            continue
        
        if isAuthority:
            if line.strip() == "":
                isAuthority = False
                continue
            # create and object and push it to expectedAuthority
            expectedAuthority.append(createAnswer(line))
            continue
            
        if isAdditional:
            if line.strip() == "":
                isAdditional = False
                continue
            # create and object and push it to expectedAdditional
            expectedAdditional.append(createAnswer(line))
            continue

    return expectedAnswers,expectedAuthority,expectedAdditional
