from collections import Counter
import numpy as np


def Unigram():
	# Open the data and split into a list of seperate words
	text = open("berp-training.txt").read()
	# text = "<s> red fish </s> <s> blue fish </s> <s> one fish </s> <s> two fish </s> "

	words = text.split()

	# Get total number of words in the corpus
	N = len(words)

	# Get the number of times each word is used in corpus
	freqs = Counter(words)

	# Create another dictionary to store frequency of <unk> words
	unkModel = {"<unk>":0}

	# For all values in original sample, add to <unk> model if frequency is > 1 or increase <unk> count if frequency == 1.
	for value in freqs:
		if freqs[value] == 1:
			unkModel["<unk>"] += 1
		else:
			unkModel[value] = freqs[value]
	# print(unkModel)

	# Convert the count of each word to the proportion so we know the probability of getting that word.
	for key in unkModel:
		unkModel[key] = unkModel[key] / N

	return unkModel


def UniSentenceProb(freqs, sentence):
	# Split test sentence into words
	sentence = sentence.split()
	# Initialize the starting probability to 1 because will be multiplying by numbers [0,1].
	prob = 1
	# Interate over all words in the test sentence
	for word in sentence:
		# If the word is in the corpus, use that probability
		if word in freqs.keys():
			prob = prob * freqs[word]
		# If not in corpus, then multiply by probability of "<unk>".
		else:
			prob = prob * freqs["<unk>"]
	return prob


# Function to take the corpus and create a dictionary of bigram probabilities
def Bigram():
	# Open the data and split into a list of seperate words
	text = open("berp-training.txt").read()
	# text = "<s> red fish blue fish one fish two fish </s>"
	# text = "<s> red fish </s> <s> blue fish </s> <s> one fish </s> <s> two fish </s> "
	corpus = text.split()

	# Get initial count of number of times each word is used. Prefix unigram counts will be used to determine probabilities
	# of bigrams.
	unigrams = Counter(corpus)

	# Create an <unk> model again for words only used once.
	unkModel = {"<unk>":0}
	for key in unigrams:
		if unigrams[key] == 1:
			unkModel["<unk>"] += 1
		else:
			unkModel[key] = unigrams[key]

	# Rewrite the original corpus, replacing rare words with <unk>.
	for index, word in enumerate(corpus):
		if word not in unkModel.keys():
			corpus[index] = "<unk>"

	# Create libary for bigrams. Is a libary of libraries. The current word is the first key, then then following word is the second key.
	# This lets us figure out what words are conditioned on which words.
	
	# Start by creating our dictionary with add-one smoothing. This means each word will already have 1 occurance for each possible bigram.
	bigrams = {}
	# Take the keys from our unigram unkModel because that contains all the unique words, already filted with unk.
	uniqueWords = set(corpus)
	# Go through unique words and initialize a dictionary bigram pair for each
	for initWord in uniqueWords:
		bigrams[initWord] = {}
		for secWord in uniqueWords:
			bigrams[initWord][secWord] = 1

	# Loop over all words in the corpus to assign counts to each bigram.
	for index, word in enumerate(corpus):
		# Skip the last word in the corpus as it throws an error and will always be the </s> character.
		if index == len(corpus)-1:
			continue
		# Increment each bigram pair of words.
		bigrams[word][corpus[index + 1]] += 1

	# print("Numbers")
	# print("Length of corpus", len(corpus))
	# print("Unigram entires", unkModel)
	# print("Bigram entries", bigrams)


	# Now we have our library of bigrams counts. Want to convert that to probabilities.
	# Loop over all words in dictionary.
	for initWord in bigrams:  
		# For each bigram, change from the count to the proportion of total counts given the initial word.
		for nextWord in bigrams[initWord]:
			bigrams[initWord][nextWord] = (bigrams[initWord][nextWord]) / (unkModel[initWord] + (1 * (len(unkModel.keys()))))
	# Now we have a dictionary of proportions. Can start to figure out the probabilities of sentences occuring.

	return bigrams


# Determine the sentence probability from the bigram model.
def BigramSentenceProb(bigrams, sentence):
	# Split the sentence into a list of words
	sentence = sentence.split()
	# initialize probability of sentence to 1
	prob = 1
	# Go through each word to compute probabilities given previous word
	for index, word in enumerate(sentence):
		# Ignore the last word or else index goes out of bounds. And will always be the same character of </s>.
		if index == len(sentence) - 1:
			continue
		# Check if the inital word exists in the corpus. If it does not, treat it as <unk>.
		if word in bigrams.keys():
			# Check if the following word exists in the corpus. If it does not, treat as <unk>.
			if sentence[index + 1] in bigrams[word].keys():
				prob = prob * bigrams[word][sentence[index + 1]]
			else:
				prob = prob * bigrams[word]["<unk>"]
		else:
			# Check if following word is in the corpus. If it is not, treat as <unk>.
			if sentence[index + 1] in bigrams["<unk>"].keys():
				prob = prob * bigrams["<unk>"][sentence[index + 1]]
			else:
				prob = prob * bigrams["<unk>"]["<unk>"]
	return prob


# Generate random sentences from the model
def Shannon(model):
	# Initialize the word to the beginning of setence character.
	word = "<s>"
	sentence = "<s>"
	# Going to keep adding words until we hit the end of sentence character
	while(word != "</s>"):
		# Generate a random float between 0 and 1. This is how we will pick our random word.
		rand = np.random.uniform(0, 1)
		# Iterate through all the possible bigrams from the current word
		for secWord in model[word].keys():
			# print("Shipping up to Boston")
			# Subtract the probability of getting that bigram from our random number to see if we will use it. If the result is negative,
			# we will use that word.
			if rand - model[word][secWord] <= 0:
				sentence = sentence + " " + secWord
				word = secWord
				break;
			# If the result is not negative, then subtract the value from the random number. This is similar to moving up a bar
			# shown in class.
			else:
				rand -= model[word][secWord]
	return(sentence)

def ShannonWrite(model, n):
	# Opens the file in write mode. Will overwrite any data alreayd in it.
	f = open("TenHoeve-Adam-assgn2-bigram-rand-corpus.txt", "w")
	# Write n sentences to the file
	for i in range(n):
		f.write(Shannon(model) + "\n")

def main():
	# Generate a dictionary of probabilities of unigrams.
	unigrams = Unigram()
	
	# # Check different test sentences
	# test1 = "<s> i want food </s>"
	# test2 = "<s> i want english food </s>"
	# test3 = "<s> i want chinese food </s>"
	# # This sentence has a word that is not in the original corpus. Should still work using <unk>.
	# test4 = "<s> tell me about pasta jays </s>"

	# print("Tests for unigram model:")
	# print(test1, ":", UniSentenceProb(unigrams, test1))
	# print(test2, ":", UniSentenceProb(unigrams, test2))
	# print(test3, ":", UniSentenceProb(unigrams, test3))
	# print(test4, ":", UniSentenceProb(unigrams, test4))

	# Creates a dictionary of probabilities of bigram pairs.
	bigrams = Bigram()
	# print(bigrams)

	testSet = open("testSet.txt", "r")
	unigramProb = open("TenHoeve-Adam-assgn2-unigram-out.txt", "w")
	bigramProb = open("TenHoeve-Adam-assgn2-bigram-out.txt", "w")
	for line in testSet:
		unigramProb.write(str(UniSentenceProb(unigrams, line)) + "\n")
		bigramProb.write(str(BigramSentenceProb(bigrams, line)) + "\n")

	# print("Tests for bigram model")
	# print(test1, ":", BigramSentenceProb(bigrams, test1))
	# print(test2, ":", BigramSentenceProb(bigrams, test2))
	# print(test3, ":", BigramSentenceProb(bigrams, test3))
	# print(test4, ":", BigramSentenceProb(bigrams, test4))

	# Let's generate some random sentences using the Shannon Method and our bigrams model
	print("Shannon Generated Sentences")
	# print(Shannon(bigrams))
	# print(Shannon(bigrams))
	# print(Shannon(bigrams))
	# Write 100 sentences to file
	# ShannonWrite(bigrams, 100)

if __name__ == "__main__":
	main()