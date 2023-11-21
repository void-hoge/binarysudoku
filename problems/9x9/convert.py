#!/usr/bin/env python3

data = open("HardestDatabase110626.txt").read().split('\n')
# print(data)
writefile = open("hardestdatabase.txt", "w")

for line in data:
	tmp = ""
	for ch in line:
		if ch == '.':
			tmp += '-'
		else:
			tmp += ch
		tmp += ' '
	writefile.write(tmp+'\n')
