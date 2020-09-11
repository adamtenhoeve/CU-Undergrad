'''
Adam Ten Hoeve
Python Project
TA: Amber Womack
Section 114
'''

import random # Needed to pull a random word out of a list.

class Setup: # Class that creates a complex dictionary of lists of strings. 
	def __init__(self): # Initializes the empty dictionary and the word that they will be guessing.
		self.Oxford = {}
		self.word = ""
		
	def create_list(self,filename): # Opens a text file that has a bunch of words that can be guessed. Creates a list of each word in that file.
		ReadMe = open(filename,"r")
		Webster = []
		for line in ReadMe.readlines(): # for each word in the text file, takes the word and strips it and takes off the comma at the end.
			line = line.strip()
			line = str.lower(line)
			line = line.split(",")
			if line[0] not in Webster: # Adds the final word to the list that contains all the words from that file.
				Webster.append(line[0])
		ReadMe.close()
		return Webster
	
	def create_dictionary(self): # Preforms the above function for each of the 4 categories and puts each of those into a dictionary as the value with a key that is the category.
		movies = self.create_list("Movies.txt")
		self.Oxford["Movies"] = movies
		animals = self.create_list("Animals.txt")
		self.Oxford["Animals"] = animals
		cities = self.create_list("Cities.txt")
		self.Oxford["Cities"] = cities
		foods = self.create_list("Foods.txt")
		self.Oxford["Foods"] = foods
		
	def select_word(self, choice):
		n = (random.randint(0, len(self.Oxford[choice]))) # generates a random number
		first = self.Oxford[choice] # List of all possible words from the catalog that they choose
		self.word = first[n] # Pulls a random word from the catalog based on the random number
		return self.word
		
class Game: # A class that handles the numbers part of the game.
	def __init__(self):
		self.miss = 0 # how mnay wrong guesses they have had
		self.total = 0 # the total number of guesses
		self.Guessed_letters = [] # Keeps track of which letters the player has guesses so they don't accidentally guess them again.
		
	def GetGuess(self):
		return self.total # returns the number of guesses
		
	def GetMiss(self):
		return self.miss # returns the number of wrong guesses
		
	def Guess(self):
		self.total = self.total + 1 # increases every time the player makes a guess
		
	def Miss_Letter(self, letter):
		print "The letter '", letter, "' is not in the word. Guess again." # If the player guesses a letter that is not in the word, this tells them that they guessed wrong and increments the misses count.
		self.miss = self.miss + 1
		
	def Miss_Word(self):
		print "Incorrect. Guess agian." # If they guess the wrong word, tells them that the word was wrong. Increments the misses count.
		self.miss = self.miss + 1
		
	def TellMe(self):
		print "You have guessed", self.total, "times. You have", self.miss, "misses." # Prints the total number of guesses and misses.
	
	def guess_letters(self, letter): # This adds to the list of what letters the player has guessed. If they already guessed the letter, will return none rather than the list.
		if letter not in self.Guessed_letters:
			self.Guessed_letters.append(letter)
			self.Guessed_letters.sort()
			print "You have guessed:", self.Guessed_letters
			return 0
		return None
	
	def Print_Misses(self): # This function outputs the visual hangman part by checking how many misses the player has and printing the 
		if self.miss == 1:
			print '''
			     --------
			     |      |
			     |      O
			     | 
			     |
			     |
			     |
			     |
			     |
			   ------------
			   '''
		if self.miss == 2:
			print '''
			     --------
			     |      |
			     |      O
			     |      |
			     |      |
			     |
			     |
			     |
			     |
			   ------------
			   '''
		if self.miss == 3:
			print '''
			     --------
			     |      |
			     |      O
			     |      |/
			     |      |
			     |
			     |
			     |
			     |
			   ------------
			   '''
		if self.miss == 4:
			print '''
			     --------
			     |      |
			     |      O
			     |     \|/
			     |      |
			     |
			     |
			     |
			     |
			   ------------
			   '''
		if self.miss == 5:
			print '''
			     --------
			     |      |
			     |      O
			     |     \|/
			     |      |
			     |     /  
			     |
			     |
			     |
			   ------------
			   '''
		if self.miss == 6:
			print '''
			     --------
			     |      |
			     |      O
			     |     \|/
			     |      | 
			     |     / \  
			     |
			     |
			     |
			   ------------
			   '''

def main():
	
	# Creating the actual dictionary
	
	Mix = Setup()
	Mix.create_dictionary()
	
	# When the game starts
	
	playing = True
	while playing == True:	# They can keep playing until the 
		print "Welcome to hangman."
		try: # an exception. Tries to make the user pick a category. If they do something that isn't allowed, makes them pick again.
			choice = input("Pick a catagory. Press 1 for Movies, 2 for animals, 3 for cities, 4 for foods")
			while choice != 1 and choice != 2 and choice != 3 and choice != 4:
				choice = input("That is not a category, please enter 1, 2, 3, or 4.")
		except:
			choice = input("That was not a number from the selection. Please enter a number.")
		if choice == 1:
			catalog = "Movies"
		elif choice == 2:
			catalog = "Animals"
		elif choice == 3:
			catalog = "Cities"
		elif choice == 4:
			catalog = "Foods"
		word = Mix.select_word(catalog) # creates of key of the player's category choice and selects that word.
		
		# The game now has the word it needs, so the player can start guessing.
		
		Play = Game()
		win = None
		findme = list(word) #Makes the word into a list of each individual letter so each can be compared easier.
		blank = []
		for i in findme: # Creates a second list which is blank but the same length as the word the player is trying to find. As they guess letters, the blank list will be filled in without affecting the original word.
			if i == " ":
				blank.append(" ")
			if i != " ":
				blank.append("_")
				
		# Starts guessing
		print blank
		while blank != findme: # game will contiue until the blank list is the same as the word that they're searching for.
			guess = raw_input("Guess a letter or the word.") # The player can either guess a letter to get closer or the word entirely.
			str.lower(guess)
			Play.Guess()
			if len(guess) > 1: # If the guess is more than one letter long, will assume that they're trying to guess the word and compare it to the overall word, rather than each individual letter.
				tryme = list(guess)
				if tryme == findme:
					win = True
					break # If they get it right, breaks out of the loops and they win.
				else:
					Play.Miss_Word() # Or else treats it the same as a missed letter.
					Play.Print_Misses()
					Play.TellMe()
					if Play.GetMiss() == 6: # If the number of wrong guesses reaches 6 then the player looses and the game loops stops.
						print "You lose."
						break
					continue
			temp = Play.guess_letters(guess) # If the player's guess is only one letter long, then they are guessing a letter which will be compared to each individual character of the word.
			while temp == None: # If they guess the same letter twice, will tell them that.
				guess = raw_input("You already guessed that letter. Guess another letter.")
				temp = Play.guess_letters(guess)
			index = 0
			correct = False 
			for i in findme: # compares the guessed letter to each character in the word.
				if guess == i:
					blank[index] = guess # If the player guessed right, replaces the blank list with the guessed letter.
					correct = True	
				index = index + 1
							
			if correct == False: # If they guessed wrong, incrememnts the wrong counter and prints out the hangman image.
				Play.Miss_Letter(guess)
				Play.Print_Misses()
			Play.TellMe() # After every guess, tells the player the total number of guesses and the total number of misses.
			if Play.GetMiss() == 6: # They still lose if wrong guesses reaches 6.
				print "You lose."
				break
			print blank # Prints the outline list that has changed based on how the player has guessed.
			if blank == findme:
				win = True # Only breaks the loop if they guess the word or they reach too many wrong guesses.
		if win == True:
			print "Correct!" 
			Play.TellMe()
			Winner = open("Champions.txt","w") # If they win, puts their name and the game information into a new text file.
			winstr = "You won! Your word was" + word + "and you got it with" + str(Play.GetGuess()) + "guesses."
			Winner.write(winstr)
			Winner.close()
		fool = input("Do you want to play again? Enter 1 for yes or 2 for no.") # The player can then play again if they want to where they can pick a new category and get a new word.
		if fool == 2:
			break
	print "Thanks for playing."
		
	
main()
	
	
	
