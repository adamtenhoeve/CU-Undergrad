import cmath

def Rent(days):
	# Bill
	Bmoney = days * 100 # 100 dollars per day
	# Kate
	big = 2**days
	Kmoney = 0.01 * big # price doubles every day so same as 2^(days), and multiply that number by the starting amount.
	# John
	Jmoney = 0.
	more = 1000.
	while days > 0: # takes the initial amount and halves it for every day, then adds that amount to be added on if staying another day.
		Jmoney = Jmoney + more
		more = more / 2
		days = days - 1
	# Output
	if Bmoney > Jmoney and Bmoney > Kmoney: # Whoever pays the most gets the house. Yay!
		print "Bill"
	if Jmoney > Bmoney and Jmoney > Kmoney:
		print "John"
	if Kmoney > Bmoney and Kmoney > Jmoney:
		print "Kate"

def main():
	Rent(1)
	Rent(15)
	Rent(100)
	
if __name__== "__main__":
	main()
