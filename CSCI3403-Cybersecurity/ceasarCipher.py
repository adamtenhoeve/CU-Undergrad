Freq = {"a":0.08, "b":0.015, "c":0.03, "d":0.04, "e":0.13, "f":0.02, "g":0.015, "h":0.06, "i":0.065, "j":0.005, "k":0.005, "l":0.035, "m":0.03,
             "n":0.07, "o":0.08, "p":0.02, "q":0.002, "r":0.065, "s":0.06, "t":0.09, "u":0.03, "v":0.01, "w":0.015, "x":0.005, "y":0.02, "z":0.002}
             # Dictionary of the frequency of each letter in english

def CeasarSolver(): # Ciphertext is "qeb mxpptloa fp nnwwnnw"
    iProbs = [] # Array to hold the probability of all 26 keys
    for i in range(26):
        # print(chr(((0-i)%26) + 97))
        phi = 0.05 * Freq[chr((0 - i)%26 + 97)] + 0.05 * Freq[chr((1-i)%26 + 97)] + 0.05 * Freq[chr((4-i)%26 + 97)] + 0.05 * Freq[chr((6-i)%26 + 97)] \
        + 0.05 * Freq[chr((11-i)%26 + 97)] + 0.05 * Freq[chr((12-i)%26 + 97)] + 0.2 * Freq[chr((13-i)%26 + 97)] + 0.05 * Freq[chr((14-i)%26 + 97)] \
        + 0.15 * Freq[chr((15-i)%26 + 97)] + 0.05 * Freq[chr((16-i)%26 + 97)] + 0.05 * Freq[chr((19-i)%26 + 97)] + 0.15 * Freq[chr((22-i)%26 + 97)] \
        + 0.05 * Freq[chr((23-i)%26 + 97)] # Takes the frequency of each letter from ciphertext and multiplies by frequency of letter in english.
        iProbs.append(phi) # Appends each probability to a list
    return iProbs

probs = CeasarSolver()
# print(probs)
first = 0
second = 0
third = 0
for i in range(len(probs)): # Finds the indexes with the greatest probability
    if (probs[i] > probs[first]):
        third = second
        second = first
        first = i
    elif (probs[i] > probs[second]):
        third = second
        second = i
    elif (probs[i] > probs[third]):
        third = i
print(first, second, third)
print(probs[first], probs[second], probs[third])
# print(probs[23]) # Correct key to decode
# probs.sort(reverse = True)
# print(probs[:3])
# print(ord("a"))
# print(ord("b"))
# print(ord("c"))
# print(ord("x"))
# print(ord("y"))
# print(ord("z"))
# print(chr(97))
# print(chr(98))
# print(chr(99))
# print((25)%25)
