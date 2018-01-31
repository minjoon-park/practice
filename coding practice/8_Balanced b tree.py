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

root = Node(0)
root.insert_left(1)
root.insert_right(2)
root.left.insert_left(3)
root.left.insert_right(4)
root.right.insert_left(1)
root.right.insert_right(2)
root.left.left.insert_left(1)
root.left.right.insert_right(2)
root.right.left.insert_left(1)
root.right.right.insert_right(2)
root.right.left.left.insert_right(2)


def get_depths(root):
	range = [0, 1000 * 1000]

	def _traverse(d, node):
		if not node.left and not node.right:
			if d + 1 > range[0]:
				range[0] = d + 1
			if d + 1 < range[1]:
				range[1] = d + 1
			return
		if node.left:
			_traverse(d + 1, node.left)
		if node.right:
			_traverse(d + 1, node.right)

	_traverse(0, root)

	print range
