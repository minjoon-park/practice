stock_prices_yesterday = [7, 9, 5, 6, 3, 2]

a = []
a.append(stock_prices_yesterday[0])
a.append(stock_prices_yesterday[1])
a.append(a[0] if a[0] < a[1] else a[1])
for i in xrange(2, len(stock_prices_yesterday)):
	if stock_prices_yesterday[i] < a[2]:
		a[2] = stock_prices_yesterday[i]
	elif stock_prices_yesterday[i] - a[2] > a[1] - a[0]:
		a[1] = stock_prices_yesterday[i]
		a[0] = a[2]
	elif stock_prices_yesterday[i] > a[1]:
		a[1] = stock_prices_yesterday[i]
print a[1] - a[0]
