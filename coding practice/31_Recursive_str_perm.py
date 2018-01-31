# Recursive permutation
def permute(in_str):
	if len(in_str) == 1:
		return set([in_str])
	s = set()
	for i, a in enumerate(in_str):
		for x in permute(in_str[:i] + in_str[i + 1:]):
			s.add(a + x)
	return s
