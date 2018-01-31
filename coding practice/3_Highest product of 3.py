input = [7, 9, 5, 6, 3, 2]

a = sorted(input[:3])
for i in xrange(3, len(input)):
	if input[i] > a[0]:
		a[0] = input[i]
		a = sorted(a)
print a[0] * a[1] * a[2]
