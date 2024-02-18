sides = ['w', 'b']

letters = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h']
possible = [i1 + str(j1) + i2 + str(j2) for i1 in letters for j1 in range(1, 9)
            for i2 in letters for j2 in range(1, 9)]
token = "6168062973:AAH-D6fdLyBNavU69CPtG3bMMbisP3ZSrZc"
