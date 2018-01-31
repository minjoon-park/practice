class Stack:

    # initialize an empty list
    def __init__(self):
        self.items = []

    # push a new item to the last index
    def push(self, item):
        self.items.append(item)

    # remove the last item
    def pop(self):
        # if the stack is empty, return None
        # (it would also be reasonable to throw an exception)
        if not self.items:
            return None
        return self.items.pop()

    # see what the last item is
    def peek(self):
        if not self.items:
            return None
        return self.items[-1]


class MaxStack(object):

	def __init__(self):
		self.stack = Stack()
		self.max_stack = Stack()

	def push(self, v):
		self.stack.push(v)
		max = self.max_stack.peek()
		new_max = max
		if max is not None and max < v:
			new_max = v
		self.max_stack.push(new_max)

	def pop(self):
		self.stack.pop()
		self.max_stack.pop()

	def get_max(self):
		print self.max_stack.peek()
