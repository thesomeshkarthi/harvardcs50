from cs50 import get_float

# Set change to a number that allows it to enter while loop
change = -1

# While loop that keeps asking for valid change number > 0
while change < 0:
    change = get_float("Change owed: ")

# Multiply change by 100 to get rid of floating point
change = change * 100

# Set initial amount of coins to 0
coins = 0

# All possible coins and add coins while entering each while loop
while change >= 25:
    coins += 1
    change = change - 25

while change >= 10:
    coins += 1
    change = change - 10

while change >= 5:
    coins += 1
    change = change - 5

while change >= 1.0:
    coins += 1
    change = change - 1

# Print out number of coins
print(coins)