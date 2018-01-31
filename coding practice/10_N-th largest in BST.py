class Node:

    def __init__(self, value):
        self.value = value
        self.left  = None
        self.right = None

    def insert_left(self, value):
        self.left = Node(value)
        return self.left

    def insert_right(self, value):
        self.right = Node(value)
        return self.right

root = Node(10)
root.insert_left(5)
root.insert_right(15)
root.left.insert_left(3)
root.left.insert_right(6)
root.right.insert_left(14)
root.right.insert_right(16)
root.left.left.insert_left(2)
root.left.right.insert_right(7)
root.right.left.insert_left(12)
root.right.right.insert_right(17)
root.right.left.left.insert_right(13)



class NthLargest(object):

	def __init__(self, root, n):
		self.loc = 0
		self.root = root
		self.n = n

	def _traverse(self, node):
		if not node:
			return
		self._traverse(node.right)
		self.loc += 1
		if self.loc == self.n:
			print '====  ', node.value
			return
		self._traverse(node.left)

	def run(self):
		self._traverse(self.root)
