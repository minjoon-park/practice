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




def check_bst(root):
	node_min_max = {}

	def _build_node_min_max(node):
		print '====  ', node.value
		if not node.left and not node.right:
			node_min_max[node] = (node.value, node.value, node.value)
			return

		left_min, left_max = 1000 * 1000, 0
		if node.left:
			_build_node_min_max(node.left)
			left_min = node_min_max[node.left][0]
			left_max = node_min_max[node.left][1]
		right_min, right_max = 1000 * 1000, 0
		if node.right:
			_build_node_min_max(node.right)
			right_min = node_min_max[node.right][0]
			right_max = node_min_max[node.right][1]

		min, max = node.value, node.value
		if left_min < min:
			min = left_min
		if right_min < min:
			min = right_min
		if left_max > max:
			max = left_max
		if right_max > max:
			max = right_max
		node_min_max[node] = (min, max, node.value)

	_build_node_min_max(root)
	print node_min_max

	def _do_check(node):
		print '****  ', node.value
		if not node.left and not node.right:
			return True

		is_left_ok = True
		if node.left:
			if node_min_max[node.left][1] > node.value:
				return False
			is_left_ok = _do_check(node.left)
		is_right_ok = True
		if node.right:
			if node_min_max[node.right][0] < node.value:
				return False
			is_right_ok = _do_check(node.right)

		return is_left_ok and is_right_ok

	return _do_check(root)








INT_MAX = 4294967296
INT_MIN = -4294967296

# A binary tree node
class Node:

    # Constructor to create a new node
    def __init__(self, data):
        self.data = data
        self.left = None
        self.right = None


# Returns true if the given tree is a binary search tree
# (efficient version)
def isBST(node):
    return (isBSTUtil(node, INT_MIN, INT_MAX))

# Retusn true if the given tree is a BST and its values
# >= min and <= max
def isBSTUtil(node, mini, maxi):

    # An empty tree is BST
    if node is None:
        return True

    # False if this node violates min/max constraint
    if node.data < mini or node.data > maxi:
        return False

    # Otherwise check the subtrees recursively
    # tightening the min or max constraint
    return (isBSTUtil(node.left, mini, node.data -1) and
          isBSTUtil(node.right, node.data+1, maxi))

# Driver program to test above function
root = Node(4)
root.left = Node(2)
root.right = Node(5)
root.left.left = Node(1)
root.left.right = Node(3)

if (isBST(root)):
    print "Is BST"
else:
    print "Not a BST"






'''Please note that it's Function problem i.e.
you need to write your solution in the form of Function(s) only.
Driver Code to call/invoke your function would be added by GfG's Online Judge.'''


# Node definition
"""
class Node:
    def __init__(self, value):
        self.left = None
        self.data = value
        self.right = None
"""
# Your task is to complete this function
# Function should return an 1/0 or True/False
def isBST(root):
    arr = []

    def _walk(node):
        if node.left:
            if not _walk(node.left):
                return False
        if arr and arr[-1] > node.data:
            return False
        arr.append(node.data)
        if node.right:
            if not _walk(node.right):
                return False
        return True

    return _walk(root)



