from sklearn.model_selection import train_test_split
import numpy as np
import re

# Name: Adam Ten Hoeve

def Unigrams(corpus, training):

	allWords = corpus.split()
	model = {}

	# Get total number of words in the corpus.
	# Perform add 1 smoothing at this step, so that all words in full corpus start with 1 in the dictionary.
	for i in allWords:
		if i not in model.keys():
			model[i] = 1

	# Then get the words just from the training data from that spectific class
	numWords = 0
	trainingWords = training.split()
	for i in trainingWords:
		model[i] += 1
		numWords += 1

	# Convert each entry in dictionary from a count to a proprotion
	total = 0;
	for key in model:
		model[key] = model[key] / (numWords + len(model.keys()))
		total += model[key]

	# Return the dictionary of probabilities
	return model

def TestModel(posModel, negModel, probPosClass, probNegClass, test):

	ids = []
	sentences = []
	test = test.split("\n")

	# Split tests into ID and review
	for sentence in test:
		review = sentence.split("\t")
		ids.append(review[0])
		sentences.append(review[1])

	posses = 0
	negs = 0
	result = []

	# Go through each review in the test set
	for review in sentences:
		# Initialize the likelihood of either classification to 0.
		totalPosProb = 0
		totalNegProb = 0
		# Get rid of punctuation and convert to lower case.
		review = re.sub(r'[^\w\s]', '', review)
		review = review.lower()
		# Go through each word in the review to find probability in the model
		for word in review.split():
			# If word is in model, get probability. Otherwise ignore it.
			if word in posModel.keys():
				totalPosProb -= np.log(posModel[word])
				totalNegProb -= np.log(negModel[word])
		totalPosProb -= np.log(probPosClass)
		totalNegProb -= np.log(probNegClass)

		# Classify based on which value is larger. Remember that numbers closer to 0 (negative) would result in a higher log-likelihood.
		if totalPosProb < totalNegProb:
			result.append("\tPOS")
			posses += 1
		else:
			result.append("\tNEG")
			negs += 1

	# Write results to a new file.
	f = open("TenHoeve-Adam-assgn3-out.txt", "w")
	for i in range(len(result)):
		f.write(ids[i] + result[i] + "\n")
	# 	print(ids[i] + result[i])


	# print("Percent Pos:", posses / (posses + negs))
	# print("Percent Neg:", negs / (posses + negs))

	return(result)


def main():
	# Import the positive and negative files
	posText = open("hotelPosT-train.txt", encoding = "utf8").read()
	posText = posText.split("\n")
	posText = np.array(posText)
	negText = open("hotelNegT-train.txt", encoding = "utf8").read()
	negText = negText.split("\n")
	negText = np.array(negText)

	# Create training and test sets using SciPy. Will use entire files as training later so don't need.
	# posTrain, posTest = train_test_split(posText, test_size = 0.2, random_state = 99)
	# negTrain, negTest = train_test_split(negText, test_size = 0.2, random_state = 99)

	# For final implementation, import the actual test set
	posTrain = posText
	negTrain = negText
	test = open("testfile.txt", encoding = "utf8").read()

	# Track the number of positive and negative reviews to get the prob of each class.
	numPos = len(posTrain)
	numNeg = len(negTrain)
	totalReviews = numPos + numNeg
	probPosClass = numPos / totalReviews
	probNegClass = numNeg / totalReviews

	pos = []
	neg = []
	ids = []

	# Remove the review ID from each review and store in new array
	for value in posTrain:
		review = value.split("\t")
		ids.append(review[0])
		pos.append(review[1] + " ")

	for sentence in negTrain:
		review = sentence.split("\t")
		ids.append(review[0])
		neg.append(review[1] + " ")

	# To create a bag of words, add all sentences into one large string.
	posWords = ""
	negWords = ""

	for i in pos:
		posWords += i
	for i in neg:
		negWords += i

	# Strip out case and punctuation using regex.
	posWords = re.sub(r'[^\w\s]', '', posWords)
	posWords = posWords.lower()
	negWords = re.sub(r'[^\w\s]', '', negWords)
	negWords = negWords.lower()

	# Create a full corpus of both positive and negative reviews
	corpus = posWords + negWords

	# Create a unigram dictionary of words and counts for positive review
	posModel = Unigrams(corpus, posWords)
	# Then create same dictionary for negative reviews
	negModel = Unigrams(corpus, negWords)

	# Now want to test how well the models classify the sentences.

	# TestModel(posModel, negModel, probPosClass, probNegClass, posTest)
	# TestModel(posModel, negModel, probPosClass, probNegClass, negTest)
	TestModel(posModel, negModel, probPosClass, probNegClass, test);




if __name__ == "__main__":
	main()