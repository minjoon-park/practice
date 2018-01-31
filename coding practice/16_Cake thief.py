# Don't allow multiples
def max_profit(weights, prices, cap):
	num_items = len(weights)
	profits = [[0 for i in xrange(cap + 1)] for j in xrange(num_items + 1)]
	for i in xrange(1, num_items + 1):
		for j in xrange(1, cap + 1):
			if weights[i - 1] > j:
				profits[i][j] = profits[i - 1][j]
			else:
				profits[i][j] = max(
					profits[i - 1][j - weights[i - 1]] + prices[i - 1],
					profits[i - 1][j]
				)
	print profits[num_items][cap]

p = [60, 100, 120]
w = [10, 20, 30]
max_profit(w, p, 50)


# Allow multiples
def max_profit(weights, prices, cap):
	num_items = len(weights)
	profits = [[0 for i in xrange(cap + 1)] for j in xrange(num_items + 1)]
	for i in xrange(1, num_items + 1):
		for j in xrange(1, cap + 1):
			if weights[i - 1] > j:
				profits[i][j] = profits[i - 1][j]
			else:
				profits[i][j] = max(
					profits[i][j - weights[i - 1]] + prices[i - 1],
					profits[i - 1][j]
				)
	print profits[num_items][cap]
