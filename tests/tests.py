import subprocess as sp
from digParser import parseDigOutput

prog_name="./dns"
localhost="127.0.0.1"
port="53"

# stasts variables
num_of_tests=0
passed_tests=0
failed_tests=0

# teseting values
domainNames = ("www.github.com", "www.facebook.com", "www.google.com", "github.com", "facebook.com", "google.com", "kazi.fit.vutbr.cz", "www.fit.vut.cz")
ips4 = ("147.229.8.12","147.229.9.26")
ips6 = ("2001:67c:1220:809::93e5:91a")


# function for colorful terminal output
def printFail(msg): print("\033[91m {}\033[00m" .format(msg),flush=True)
def printSuccess(msg): print("\033[92m {}\033[00m" .format(msg),flush=True)
def printTestSection(msg): print("\033[95m {}\033[00m" .format(msg),flush=True)

# error codes
SUCCESS = 0
CONNECTION_FAILED = 1
NAME_NOT_EXIST = 2
NOT_FOUND = 3
WRONG_ARGUMENTS = 4
INTERNAL = 99



# wrong arguments testing
## server not given

## address not given

# -r 

# -x

# -6

# -r -x

# -r -6

# -x -6

# -r -x -6


if __name__ == "__main__" :
    parseDigOutput()

