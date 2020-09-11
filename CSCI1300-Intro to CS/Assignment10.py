def CreateDictionary(filename):
	ReadMe = open(filename, "r") # takes the input file and turns it into a string
	Webster = {} # empty dictionary
	for line in ReadMe.readlines():
		line = line.strip() # For each line in the file, takes out the white space and seperates each line into individual words in a list.
		line = line.split(",")
		slang = line[0] # Assigns the first word of the list to key of the dictionary, and the second word to be the value if not alreay in the dictionary.
		english = line[1]
		if (slang not in Webster):
			Webster[slang] = english
	ReadMe.close()
	return Webster

def Deslang (slang, Oxford):
	SlangList = slang.split() # Takes the input string and splits it at each space into a list"
	GoodEnglish = ""
	for word in SlangList: # for each word in the list, checks to see if it's in the dictionary. If it is, reassigns it to the word's value and adds the word to the final string.
		if word in Oxford: # If the word is not in the dictionary, then just adds the word to the final string without any alterations.
			word = Oxford[word]
		GoodEnglish = GoodEnglish + " " + word
	GoodEnglish = GoodEnglish.strip()
	return GoodEnglish

def main ():
	wordDictionary = CreateDictionary("textToEnglish.txt") # creates the dictionary for all of the slang
	
	line = raw_input("Enter a slang word")
	while line != "quit": # While loop so the user can input as much slang as they want. Will continually analyze it and see if it's in the dictionary.
		if line in wordDictionary:
			print wordDictionary[line]
		else:
			print "Not Found"
		line = raw_input("Enter a slang word")
		
	notEnglish = raw_input("Enter a bunch of slang") # Can take a bunch of slang in a single string.
	while notEnglish != "quit": # another while loop to keep going until the user tells the loop to quit.
		english = Deslang(notEnglish, wordDictionary)
		print english 
		notEnglish = raw_input("Enter a bunch of slang")

if __name__ == "__main__":
	main()
