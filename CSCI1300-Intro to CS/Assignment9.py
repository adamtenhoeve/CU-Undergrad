'''
Author: Adam Ten Hoeve
TA: Amber Womack
Assignment 9
'''

# Problem 1

def CalcNewPopulation(pop):
    births = 31540000 / 7 # 31540000 seconds per year, so find how many births, deads, and immigrants per year and adds them to the population parameter.
    deaths = 31540000 / 13
    immigrants = 31540000 / 35
    return pop + births - deaths + immigrants

# Problem 2

def BreakoutTime(time): # takes the parameter of the time, in seconds, and subtracts days (in seconds), hours (in seconds), minutes and the remaining time parameter.
    days = 0
    hours = 0
    minute = 0
    while time > 86400:
        days = days + 1
        time = time - 86400
    while time > 3600:
        hours = hours + 1
        time = time - 3600
    while time > 60:
        minute = minute + 1
        time = time - 60
    print "Days:", days
    print "Hours:", hours
    print "Minutes:", minute
    print "Seconds:", time

# Problem 3

def ListCodonPositions(larger, smaller): # Takes the larger string and in a while loop makes a substring of it the length of the smaller sting. Then compares the smaller string to the substring to see if they are the same, outputing any locations where they are.
    for i in range(len(larger)):
        sub_str = larger[i:i+len(smaller)]
        if sub_str == smaller:
            print i+1

# problem 4

def MadLib (select):

	story1 = "Be kind to your <noun1>-footed <plural noun>, or a duck may be somebody's <noun2>." # Has 5 strings that are each stored in their own string.
	story2 = "It was the <adjective1> of <noun1>, it was the <adjective2> of <noun2>."
	story3 = "<plural noun1>? I don't have to show you any <adjective> <plural noun2>!"
	story4 = "My <relative> always said <noun1> was like a box of <noun2>. You never know what you're gonna get."
	story5 = "One <time of day>, I <verb> a <noun> in my pajamas. How he got in my pajamas, I don't know."
	story = ""
	if select == 1: # For whatever the input parameter is, the story string will become that assigned story.
		story = story1
	if select == 2:
		story = story2
	if select == 3:
		story = story3
	if select == 4:
		story = story4
	if select == 5:
		story = story5
	c = "<"
	index = 0
	while index < len(story): # Loop that goes through the entire length of the string finding all of the < characters. When on is found, then finds the closest > character. Then creates a substring from those two characters and replaces that substring with the input string.
		if story[index] == c:
			d = story.find(">",index)
			part_of_speech = story[index + 1:d]
			newStr = raw_input("Enter " + part_of_speech + ": ")
			story = story.replace ("<"+part_of_speech+">", newStr)
		index = index + 1
	return story

# main

def main():
    new_pop = CalcNewPopulation(100000)
    print new_pop
    BreakoutTime(70000)
    ListCodonPositions("CGCAAATTTGCCGGATTTCCTTTGCTGTTCCTGCATGTAGTTTAAACGAGATTGCCAGCACCGGGTATCATTCACCATTTTTCTTTTCGTTAACTTGCCGTCAGCCTTTTCTTTGACCTCTTCTTTCTGTTCATGTGTATTTGCTGTCTCTTAGCCCAGACTTCCCGTGTCCTTTCCACCGGGCCTTTGAGAGGTCACAGGGTCTTGATGCTGTGGTCTTCATCTGCAGGTGTCTGACTTCCAGCAACTGCTGGCCTGTGCCAGGGTGCAGCTGAGCACTGGAGTGGAGTTTTCCTGTGGAGAGGAGCCATGCCTAGAGTGGGATGGGCCATTGTTCATG", "CCG")
    story = MadLib(1)
    print story

if __name__ == '__main__':
    main ()
