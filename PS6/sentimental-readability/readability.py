from cs50 import get_string

sentence = get_string("Text: ")
letter_count = 0
word_count = 1
sentence_count = 0

for char in range(len(sentence)):
    if sentence[char].isalnum():
        letter_count += 1
    elif sentence[char] == ' ' and letter_count > 0:
        word_count += 1
    elif sentence[char] in ['.', '?', '!']:
        sentence_count += 1
    else:
        continue

L = (letter_count / word_count) * 100
S = (sentence_count / word_count) * 100

index = 0.0588 * L - 0.296 * S - 15.8

if 0 < index < 16:
    print(f"Grade {round(index)}")
elif index < 1:
    print(f"Before Grade 1")
else:
    print("Grade 16+")
