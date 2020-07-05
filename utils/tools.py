#!/usr/bin/env python3

def read(fn):
    with open(fn, 'r', encoding="utf-8") as file:
    	return file.read()

def save(fn, z):
    d = open(fn, 'w', encoding="utf-8")
    d.write(z)
    d.close()

class bcolors:
    NO_COLOR='\033[0m'
    RED = '\033[38;5;009m'
    GREEN = '\033[38;5;010m'
    YELLOW = '\033[38;5;011m'
    ORANGE = '\033[38;5;214m'
    LIGHTPURPLE = '\033[38;5;177m'
    PURPLE = '\033[38;5;135m'
    CYAN = '\033[38;5;014m'
    LIGHTBLUE = '\033[38;5;39m'
    BLUE = '\033[38;5;27m'
    DARKBLUE = '\033[38;5;19m'
    LIGHTGRAY = '\033[38;5;252m'
    DARKGRAY = '\033[38;5;242m'
    BRIGHTRED = '\033[91m'
    BOLD = '\033[1m'
    NORMAL = '\033[0m'
    UNDERLINE = '\033[4m'

    # ENDC = '\033[0m'
    # BOLD = '\033[1m'
    # UNDERLINE = '\033[4m'
    # RED = '\033[31m'
    # BRIGHTRED = '\033[91m'
    # MAGENTA = '\033[35m'
    # PURPLE = '\033[95m'
    # ORANGE = '\033[33m'
    # YELLOW = '\033[93m'
    # BLUE = '\033[94m'
    # GREEN = '\033[92m'
