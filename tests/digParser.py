import subprocess as sp

answerTemplate = {"qname": "","qclass": "", "type": "", "answer": ""}

def removeEmptyStrings(stringParts: list) -> str:
    while "" in stringParts:
        stringParts.remove("")
    return stringParts


def createAnswer(str: str) -> dict:
    strParts = str.split('\t')
    strParts = removeEmptyStrings(stringParts=strParts)
    # skip the ttl, ttl is on index 1
    return dict(answerTemplate,qname=strParts[0], qclass=strParts[2], type=strParts[3], answer=strParts[4])


def parseDigOutput():
    # split output to array
    result = sp.run(['dig','@8.8.8.8', 'www.github.com'], stdout=sp.PIPE, stderr=sp.PIPE)
    output = result.stdout.decode('ascii')
    lines = output.split('\n')

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
            # print("BEGIN|" + line + "|END")
            # create and object and push it to expectedAnswers
            expectedAnswers.append(createAnswer(line))
            continue
        
        if isAuthority:
            if line.strip() == "":
                isAuthority = False
            # print("BEGIN|" + line + "|END")
            continue
            
        if isAdditional:
            if line.strip() == "":
                isAdditional = False
            # print("BEGIN|" + line + "|END")
            continue

    print(expectedAnswers)
