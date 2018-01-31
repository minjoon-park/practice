message = 'find you will pain only go you recordings security the into if'


class Reverse(object):

	def __init__(self, m):
		self.l = []
		self.len = len(m)
		for i in xrange(self.len):
			self.l.append(m[i])

	def _reverse_word(self, a, b):
		for i in xrange(a, (a + b) / 2):
			t = self.l[i]
			self.l[i] = self.l[a + b - 1 - i]
			self.l[a + b - 1 - i] = t

	def reverse(self):
		s, e = 0, 0
		for i in xrange(self.len):
			if self.l[i] == ' ':
				self._reverse_word(s, e)
				s = i + 1
			e += 1
		if s != e:
			self._reverse_word(s, e)

		self._reverse_word(0, self.len)

		print ''.join(self.l)
