import os
import string
import random

fd = open("test_init.txt", "w")
fn = open("all_words.txt", "w")

words = ["amb", "ambk", "ambkl", "ambtt", "ambtg", "jklmno", "jklmnop"]
for j in range(10000):
    word1 = random.choice(words)
    word2 = random.choice(words)
    if (j<5000):
        word_len = random.randint(3, 12)
        value = random.randint(1, 1000)
        for j in range(2, word_len):
            word1 += random.choice(string.ascii_lowercase)
        fd.write(word1 + "\n")
        fd.write(str(value)+'\n')
        fn.write(word1 + "\n")
    else:
        word_len = random.randint(5, 12)
        value = random.randint(1, 1000)
        for j in range(2, word_len):
            word2 += random.choice(string.ascii_lowercase)
        fd.write(word2 + "\n")
        fd.write(str(value)+'\n')
        fn.write(word2 + "\n")
fd.write('--\n')
fn.close()
fd.close()


lines = open("all_words.txt").read().splitlines()
lines = lines[:-1]

fd = open("test_find.txt", "w")
for j in range(10000):
    fd.write(random.choice(lines) + "\n")
fd.write('--\n')
fd.close()

# fd = open("test_pref.txt", "w")
# word1 = "ab"
# word2 = "ac"
# for j in range(10):
#     temp = word1 + random.choice(string.ascii_lowercase) + '\n'
#     fd.write(temp)
#     temp = word2 + random.choice(string.ascii_lowercase) + '\n'
#     fd.write(temp)
# fd.write('--\n')
# fd.close()

# fd = open("test_rm.txt", "w")
# for j in range(2000):
#     word = random.choice(lines)
#     fd.write(word+"\n")
# fd.write("--\n")
# fd.close()
