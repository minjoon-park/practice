class Mode(object):

	def __init__(self):
		self.counter = {}
		self.inv_counter = {}
		self.max_count = 0

	def insert(self, key):
		if key in self.counter:
			old_count = self.counter[key]
			self.counter[key] += 1
			self.inv_counter[old_count].remove(key)
			if old_count + 1 in self.inv_counter:
				self.inv_counter[old_count + 1].add(key)
			else:
				self.inv_counter[old_count + 1] = set([key])
				self.max_count = old_count + 1
		else:
			self.counter[key] = 1
			if 1 in self.inv_counter:
				self.inv_counter[1].add(key)
			else:
				self.inv_counter[1] = set([key])
				self.max_count = 1

	def get_modes(self):
		return self.inv_counter[self.max_count]

m = Mode()
for i in (1, 2, 4, 3, 5, 3, 7, 1, 4, 5, 9, 11 ,5, 3, 7, 9):
	m.insert(i)
m.get_modes()




class ModeHeap(object):

	def __init__(self):
		self.counter = {}
		self.container = []
		self.inv_idx = {}
		self.heap_size = 0

	def _swap(self, i, j):
		temp = self.container[i]
		self.inv_idx[temp] = j
		self.container[i] = self.container[j]
		self.inv_idx[self.container[i]] = i
		self.container[j] = temp

	def _max_heapify(self, container, i, heap_size):
		left_idx, right_idx, largest = 2 * i + 1, 2 * i + 2, i
		if left_idx <= heap_size and self.counter[container[left_idx]] > self.counter[container[largest]]:
			largest = left_idx
		if right_idx <= heap_size and self.counter[container[right_idx]] > self.counter[container[largest]]:
			largest = right_idx
		if largest != i:
			self._swap(i, largest)
			self._max_heapify(container, largest, heap_size)

	def insert(self, key):
		if key not in self.counter:
			i = self.heap_size
			self.heap_size += 1
			self.counter[key] = 1
			self.inv_idx[key] = i
			self._max_heapify(self.container, i, self.heap_size)
		else:
			self.counter[key] += 1
			parent_idx = (self.inv_idx[key] - 1) / 2
			while parent_idx and self.counter[self.container[parent_idx]] < self.counter[key]:
				self._swap(parent_idx, self.inv_idx[key])
				parent_idx = (parent_idx - 1) / 2

	def get_modes(self):
		heap = self.container.copy()
		modes = heap[:1]
		heap = heap[1:]
		self._max_heapify(heap, 0, len(heap))
		while len(heap) and self.counter[heap[0]] == self.counter[modes[0]]:
			modes.append(heap[:1])
			heap = heap[1:]
			self._max_heapify(heap, 0, len(heap))


mh = ModeHeap()
for i in (1, 2, 4, 3, 5, 3, 7, 1, 4, 5, 9, 11 ,5, 3, 7, 9):
	mh.insert(i)
mh.get_modes()




class BasicMaxHeap(object):

	def __init__(self):
		self.container = []
		self.heap_size = 0

	def _swap(self, i, j):
		temp = self.container[i]
		self.container[i] = self.container[j]
		self.container[j] = temp

	def insert(self, key):
		i = self.heap_size
		self.container.append(key)
		self.heap_size += 1
		while i and self.container[(i - 1) / 2] < key:
			self._swap(i, (i - 1) / 2)
			i = (i - 1) / 2

	def _max_heapify(self, i):
		left_idx, right_idx, largest = 2 * i + 1, 2 * i + 2, i
		if left_idx <= self.heap_size and self.container[left_idx] > self.container[i]:
			largest = left_idx
		if right_idx < self.heap_size and self.container[right_idx] > self.container[largest]:
			largest = right_idx
		if largest != i:
			self._swap(i, largest)
			self._max_heapify(largest)

bmh = BasicMaxHeap()
for i in (1, 2, 3, 4, 7, 8, 9, 10, 14, 16):
	bmh.insert(i)
bmh.container
