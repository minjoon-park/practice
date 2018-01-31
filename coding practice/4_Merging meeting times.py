input = [(0, 1), (3, 5), (4, 8), (10, 12), (9, 10)]
input = sorted(input, key=lambda x: x[0])
output = [input[0]]
for i in xrange(1, len(input)):
	candidate = input[i]
	current_last = output[-1]
	if current_last[1] < candidate[0]:
		output.append(candidate)
		continue
	if current_last[1] == candidate[0] or current_last[1] < candidate[1]:
		output[-1] = (current_last[0], candidate[1])
print output


input = [(0, 1), (3, 5), (4, 8), (10, 12), (9, 10)]
input = sorted(input, key=lambda x: x[0], reverse=True)
if len(input) == 1:
	print input[0]
for i in xrange(0, len(input) - 1):
	current = input[i]
	next_to_current = input[i + 1]
	if next_to_current[1] < current[0]:
		continue
	if next_to_current[1] == current[0] or next_to_current[1] < current[1]:
		input[i + 1] = (next_to_current[0], current[1])
	input[i] = None
input.reverse()
for i in xrange(0, len(input)):
	if input[i]:
		print input[i]