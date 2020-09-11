import math
import random
import string
import re
import matplotlib.pyplot as plt
import numpy as np


def openAndSplit(filename):
    with open(filename) as ReadMe:
        L = [line.strip() for line in ReadMe]
        ReadMe.close()
    random.shuffle(L)
    halfLength = len(L)/2
    return L[:halfLength]

def hashSum1(name, buckets):
    total = 0
    for i in name:
        total = total + ord(i) - 64
    return total % buckets

def hashSum2(name, buckets):
    total = 0;
    a = random.randint(0, buckets)
    for i in name:
        total = total + ((ord(i) - 64) * a)
    total = total % buckets
    return total

def plothist(hash, title, save):
    fig, ax = plt.subplots(nrows=1, ncols = 1, figsize=(10,5))
    ax.bar(range(len(hash)), hash)
    axes = plt.gca()
    axes.set_xlim(0, 5701)
    plt.title(title)
    plt.xlabel("Distribution of hash values")
    plt.ylabel("Frequency")
    fig.savefig(save)

def plotChaining(list, title, save):
    fig, ax = plt.subplots(nrows=1, ncols = 1, figsize=(10,5))
    plt.plot(list)
    plt.title(title)
    plt.xlabel("Number of strings hashed n")
    plt.xlabel("Length of the longest chain")
    fig.savefig(save)

def findPrimes(upto):
    n = set(range(upto, 1, -1))
    primes = []
    while n:
        p = n.pop()
        primes.append(p)
        n.difference_update(set(range(p*2, upto+1, p)))
    return primes

def plotCollisions(list, primes, title, save):
    fig, ax = plt.subplots(nrows=1, ncols = 1, figsize = (10,5))
    plt.plot(primes, list)
    axes = plt.gca()
    axes.set_ylim([39000, 45000])
    plt.title(title)
    plt.xlabel("Number of buckets l")
    plt.ylabel("Number of Collisions")
    fig.savefig(save)

ReadMe = openAndSplit("data.txt")
buckets = 5701
h1 = [0] * buckets
h2 = [0] * buckets
chain1 = []
chain2 = []
length1 = 0
length2 = 0
for line in ReadMe:
    line = line.split("\t")
    hash1 = hashSum1(line[0], buckets)
    hash2 = hashSum2(line[0], buckets)
    h1[hash1] = h1[hash1] + 1
    if h1[hash1] > length1:
        length1 = h1[hash1]
        chain1.append(length1)
    else:
        chain1.append(length1)
    h2[hash2] = h2[hash2] + 1
    if h2[hash2] > length2:
        length2 = h2[hash2]
        chain2.append(length2)
    else:
        chain2.append(length2)

plothist(h1, "Distribution of values for h1(x)", "hist1.png")
plothist(h2, "Distribution of values for h2(x)", "hist2.png")
plotChaining(chain1, "Length of longest string for h1(x)", "chainplot1.png")
plotChaining(chain2, "Length of longest string for h2(x)", "chainplot2.png")

primes = findPrimes(buckets)
Col1 = []
Col2 = []
for x in primes:
    h1 = [0] * x
    h2 = [0] * x
    collisions1 = 0
    collisions2 = 0
    for line in ReadMe:
        line = line.split("\t")
        hash1 = hashSum1(line[0], x)
        hash2 = hashSum2(line[0], x)
        h1[hash1] = h1[hash1] + 1
        if h1[hash1] != 1:
            collisions1+=1
        h2[hash2] = h2[hash2] + 1
        if h2[hash2] != 1:
            collisions2+=1
    Col1.append(collisions1)
    Col2.append(collisions2)
plotCollisions(Col1, primes, "Number of Collisions based on Number of Buckets for h1(x)", "collplot1.png")
plotCollisions(Col2, primes, "Number of Collisions based on Number of Buckets for h2(x)", "collplot2.png")
