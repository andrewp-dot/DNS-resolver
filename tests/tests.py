import subprocess as sp
import sys
import re

prog_name="./dns"
localhost="127.0.0.1"
port="53"

#stasts variables
num_of_tests=0
passed_tests=0
failed_tests=0

#funckcie pre farebne zobrazenie v terminali
def printFail(msg): print("\033[91m {}\033[00m" .format(msg),flush=True)
def printSuccess(msg): print("\033[92m {}\033[00m" .format(msg),flush=True)
def printTestSection(msg): print("\033[95m {}\033[00m" .format(msg),flush=True)
