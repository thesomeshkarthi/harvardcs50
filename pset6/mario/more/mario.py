from cs50 import get_int

# Initlizes height to 0 to enter while loop
height = 0

# Keep prompting user until height between 1-8 is entered
while height < 1 or height > 8:
    height = get_int("Height: ")

# Iterate through each layer and add hashes, space, and formatting accordingly 
for i in range(height):
    for j in range(height - (i + 1)):
        print(" ", end='')
    for k in range(i + 1):
        print("#", end='')
    
    print("  ", end='')
    
    for m in range(i + 1):
        print("#", end='')
    
    print("\n", end='')