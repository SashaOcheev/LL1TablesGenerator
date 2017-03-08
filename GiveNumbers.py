import re
from pprint import *

def makeListOfGrammarLines(grammar):
    grammar = grammar.replace("\t", " ")
    while (" \n" in grammar):
        grammar = grammar.replace(" \n", "\n")
    while ("  " in grammar):
        grammar = grammar.replace("  ", " ")
    grammar = grammar.replace("\n ", "\n")
    grammar = grammar.replace("\n|", " |")
    grammarLines = grammar.split("\n")
    return grammarLines

def makeListOfPremisesAndLines(grammarList):
    for i in range(0, len(grammarList)):
        grammarList[i] = grammarList[i].replace(" : ", ":")
        grammarList[i] = grammarList[i].replace(" :", ":")
        grammarList[i] = grammarList[i].replace(": ", ":")
        grammarList[i] = grammarList[i].split(":")
    return grammarList

def makeListOfAlternates(premisesAndLines):
    for i in range(0, len(premisesAndLines)):
        premisesAndLines[i][1] = premisesAndLines[i][1].replace(" | ", "|")
        premisesAndLines[i][1] = premisesAndLines[i][1].replace(" |", "|")
        premisesAndLines[i][1] = premisesAndLines[i][1].replace("| ", "|")
        premisesAndLines[i][1] = premisesAndLines[i][1].split("|")
    return premisesAndLines

def getNumbers(listGrammar):
    currentNumber = 0
    out = ""
    for i in listGrammar:
        tmp = ""
        for j in i[1]:
            tmp = tmp + str(currentNumber) + " "
            currentNumber += 1
        out = out + tmp + ": "
        appender = " " * (len(tmp))
        for j in i[1]:
            for k in j.split(" "):
                out += str(currentNumber) + " "
                currentNumber += 1
            out = out[:-1] + "\n" + appender + "| "
        out = out[:-2-len(appender)]
    return out[:-1]

grammarFile = open("LL1.txt", "r")
numbersOfTokens = open("Numbers.txt", "w")

grammar = grammarFile.read()
grammar = makeListOfGrammarLines(grammar)
grammar = makeListOfPremisesAndLines(grammar)
grammar = makeListOfAlternates(grammar)
numbersOfTokens.write(getNumbers(grammar))

grammarFile.close()
numbersOfTokens.close()
