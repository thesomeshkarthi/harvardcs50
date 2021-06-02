from cs50 import get_string


text = get_string("Text: ")


# Count letters
letters = 0
for i in range(len(text)):
    if text[i].isupper() or text[i].islower():
        letters = letters + 1

# Count words
words = 0
for i in range(len(text)):
    if text[i] == ' ':
        words = words + 1

words = words + 1

# Count sentences
sentences = 0
for i in range(len(text)):
    if text[i] == '?' or text[i] == '!' or text[i] == '.':
        sentences = sentences + 1


# Calculate averages
L = (100.0 / words) * letters
S = (100.0 / words) * sentences

# Execute formula
rawindex = round(0.0588 * L - 0.296 * S - 15.8)
index = int(rawindex)

# Select desired output based on calculated index value
if index >= 1 and index <= 16:
    print("Grade " + str(index))
elif index < 1:
    print("Before Grade 1")
else:
    print("Grade 16+")

