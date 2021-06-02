import csv
import sys

# Function that retrieves maximum amount of STR repeates with STR name and Dna sample


def getmax(dna, key):
    s = len(key)
    maximum = 0
    # Iterates through each character in DNA - the length of STR to ensure no overshooting
    for i in range(len(dna) - s):
        pos = 0
        multiplier = 1
        count = 0
        # Position and multiplier variables are used to make the dna index relative to the original i point at all times
        while dna[i + pos:i + multiplier * s] == key:
            count = count + 1
            # pos is added with s to go to the next substring starting point
            pos = pos + s
            # multiplier is added by one to shift the ending of the substring to the next segment relative to original i 
            multiplier = multiplier + 1
        # if the new count is greater, a new maximum is reached 
        if count > maximum:
            maximum = count
    return(maximum)
    

# Checks for command-line error
if len(sys.argv) != 3:
    sys.exit("Command Line Error!")

# Sets list with names 
names = []
with open(sys.argv[1]) as file:
    reader = csv.DictReader(file)
    
    # Checks each name in csv file and appends all names to list
    for name in reader:
        names.append(name)

# Opens .csv file and copies first line to memory
with open(sys.argv[2]) as file:
    dna = file.readline()

# Copies all the STR information with getmax function 
AGATC = str(getmax(dna, "AGATC"))
TTTTTTCT = str(getmax(dna, "TTTTTTCT"))
AATG = str(getmax(dna, "AATG"))
TCTAG = str(getmax(dna, "TCTAG"))
GATA = str(getmax(dna, "GATA"))
TATC = str(getmax(dna, "TATC"))
GAAA = str(getmax(dna, "GAAA"))
TCTG = str(getmax(dna, "TCTG"))

# Outputted variable to check if no name has been outputted, (means there were no matches)
outputted = 0

# Iterates through each person's information 
for i in range(len(names)):
    # The small.csv only checks for 3 different CSV files while large has more
    if sys.argv[1] == "databases/small.csv":
        if AGATC == names[i]["AGATC"] and AATG == names[i]["AATG"] and TATC == names[i]["TATC"]:
            print(names[i]["name"])
            outputted = 1
    else:
        if AGATC == names[i]["AGATC"] and AATG == names[i]["AATG"] and TATC == names[i]["TATC"]:
            if TTTTTTCT == names[i]["TTTTTTCT"] and TCTAG == names[i]["TCTAG"] and GATA == names[i]["GATA"] and GAAA == names[i]["GAAA"] and TCTG == names[i]["TCTG"]:
                print(names[i]["name"])
                outputted = 1

if outputted == 0:
    print("No match")

