denoms = [1, 3, 5]
tot = 11
ans = [1000 * 1000 * 1000] * (tot + 1)
ans[0] = 0
for i in xrange(1, tot + 1):
	for j in xrange(0, len(denoms)):
		if denoms[j] <= i and ans[i - denoms[j]] + 1 < ans[i]:
			ans[i] = ans[i - denoms[j]] + 1
print ans[tot]



denoms = [2, 3, 5, 6]
m = len(denoms)
n = 10
tbl = [[0] * m] * (n + 1)
for i in xrange(1, n + 1):
	tbl[i][0] = 1 if n % denoms[0] == 0 else 0
	for j in xrange(1, m):
		tbl[i][j] = (tbl[i - denoms[j]][j] if i > denoms[j] else 0) + tbl[i][j - 1]
print tbl[n][m - 1]