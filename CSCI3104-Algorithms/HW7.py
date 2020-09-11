# -*- coding: UTF-8 -*-
#Adam Ten Hoeve
#Algorithms

import random

sub = 10
swap = 10
insert = 1
delete = 1

# --------------------------------------------- Part 1 ----------------------------------------------------------------------------

def cost(matrix, i, j):
    cost1 = 9999999
    cost2 = 9999999
    cost3 = 9999999
    cost4 = 9999999
    if i >= 2 and j >= 2:
        cost1 = matrix[i-2][j-2] + swap  #Cost of swap
    if i-1 >= 0 and j-1 >= 0:
        cost2 = matrix[i-1][j-1] + sub  #Cost of Sub
    if i-1 >= 0:
        cost3 = matrix[i-1][j] + delete  #Cost of deleting from x
    if j-1 >= 0:
        cost4 = matrix[i][j-1] + insert  #Cost of inserting into x
    minimumCost = min(cost1, cost2, cost3, cost4)
    return minimumCost


def alignStrings(string1, string2):
    xn = len(string1)
    yn = len(string2)
    #print ("xn is", xn, "and yn is ", yn)
    s = [[0 for y in range(yn)] for x in range(xn)]
    for a in range(xn):
        s[a][0] = a * insert
    for b in range(yn):
        s[0][b] = b * insert
    for i in range(1, xn):
        for j in range(1, yn):
            if string1[i] == string2[j]: # No operation to be done, same number as diagonal
                s[i][j] = s[i-1][j-1]
            else:
                s[i][j] = cost(s, i, j)
    #for i in range(xn):
        # print (s[i])
    return s


# ----------------------------------------------------- Part 2 ------------------------------------------------------


def determineOptimalOps(matrix, x, y, i, j):
    current = matrix[i][j]
    # print "current is ", current
    up = 99999
    left = 99999
    diag = 99999
    superdiag = 99999
    ops = []
    if i > 0:
        up = matrix[i-1][j] # delete
        if up + insert == current:
            ops.append("delete")
    if j > 0:
        left = matrix[i][j-1] # insert
        if left + delete == current:
            # print "left is ", left
            ops.append("insert")
    if i > 0 and j > 0:
        diag = matrix[i-1][j-1] # sub
        if diag + sub == current:
            # print "diag is ", diag
            ops.append("sub")
    if i > 1 and j > 1:
        superdiag = matrix[i-2][j-2] # swap
        if superdiag + swap == current:
            # print "superdiag is ", superdiag
            ops.append("swap")
    if len(ops) == 0:
    #   print "no_op"
        return "no_op"
    optimal = random.choice(ops)
    return optimal

def updateIndicies(operation, i, j):
    if operation == "no_op":
        i-=1
        j-=1
    elif operation == "delete":
        i-=1
    elif operation == "insert":
        j-=1
    elif operation == "sub":
        i-=1
        j-=1
    elif operation == "swap":
        i-=2
        j-=2
    return i,j


def extractAlignments(matrix, x, y):
    a=[]
    xlength = len(x) -1
    ylength = len(y) -1
    i = xlength
    j = ylength
    while (i > 0 and j > 0):
        op = determineOptimalOps(matrix, x, y, i, j)
        a.insert(0, op) # need to read it backwards
        #print "i is ",i, " and j is ", j
        i,j = updateIndicies(op, i, j)
        # print "after update i is ", i, "and j is ", j
    # print a
    while i != 0:
        a.insert(0, "base")
        i-=1
    while j != 0:
        a.insert(0, "base")
        j-=1
    return a

    # --------------------------------------------- Part 3 -------------------------------------------------------------------------------------------------

def commonSubstrings(x, L, a):
    #print (x)
    #print (a)
    # print ("length of a is ", len(a))
    #print ("length of x is ", len(x))
    x = list(x)                             #converts string into list
    substr = ""
    substrList = []                         #list of substrings
    for i in range(len(a)):
        # print ("i is", i)
        if (a[i] == "no_op"):                #add to substring
            substr = substr + x[i]
            # print (substr)
        else:
            if substr != "":                #reset substring, and add to appendix
                if len(substr) >= L:        #only add if >= L
                    substrList.append(substr)
                substr = ""                 #resert evert time
            #handle cases for altering string to align with vector a
            if (a[i] == "insert"):          #insert random extra letter if there is an insert operation
                x.insert(0, "_")
                #print x
            # elif (a[i] == "swap"):
            #     del x[i+1]                  #want to ignore next letter
                # print x

    if substr != "":
        if len(substr) >= L:                       #if letter added on last iteration
            substrList.append(substr)
            substr = ""

    print (substrList)
    return substrList



    # -------------------------------------------- Main ------------------------------------------------------------------------------------------------------

def readFile(filename):
    p = ""
    with open(filename, encoding = "utf-8") as f:
        p = f.read().splitlines()
        for line in f:
            p = p.join(re.findall(r'\w+', line))
        f.close()
    p = "".join(p)
    return p

def main():
    string1 = readFile("string1.txt")
    #string1 = "exponential"
    string1 = "_" + string1
    string2 = readFile("string2.txt")
    #string2 = "polynomial"
    string2 =  "_" + string2
    #print (string1, "and", string2)
    matrix = alignStrings(string1, string2)
    optimal = extractAlignments(matrix, string1, string2)
    #print (optimal)
    string1 = string1[1:]
    L = int(input("Enter length of substrings: "))
    commonSubstrings(string1, L, optimal)

if __name__ == '__main__':
    main()
