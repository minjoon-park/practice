class Queue2Stacks(object):

	def __init__(self):
		self.s_in, self.s_out = [], []

	def enqueue(self, v):
		self.s_in.append(v)

	def dequeue(self):
		if not self.s_out:
			if not self.s_in:
				return -1
			for i in xrange(len(self.s_in), 0, -1):
				self.s_out.append(self.s_in[i - 1])
				self.s_in = self.s_in[:-1]
		v = self.s_out[-1]
		self.s_out = self.s_out[:-1]
		return v

q = Queue2Stacks()
q.enqueue(1)
q.enqueue(2)
q.enqueue(3)
q.dequeue()
