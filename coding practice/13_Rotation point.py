input = [
    'ptolemaic',
    'retrograde',
    'supplant',
    'undulate',
    'xenoepist',
    'asymptote', # <-- rotates here!
    'babka',
    'banoffee',
    'engender',
    'karpatka',
    'othellolagkage',
]
n = len(input)
min = 0
max = n - 1
first = input[0]
while True:
	m = (min + max) / 2
	middle = input[m]
	if middle > first:
		if middle < input[m + 1]:
			min = m
		else:
			print m + 1
			break
	else:
		if input[m - 1] < middle:
			max = m
		else:
			print m
			break
