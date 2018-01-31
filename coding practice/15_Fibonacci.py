def fib(n):
	if n in (1, 2):
		return 1
	return fib(n - 1) + fib(n - 2)

def fib_2(n):
	if n in (0, 1):
		return 1

	ans = [1, 1]
	for i in xrange(2, n + 1):
		ans.append(ans[i - 1] + ans[i - 2])

	return ans[n]
