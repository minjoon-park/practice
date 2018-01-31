# Hamming dist
def _to_bin(i):
	if not i:
		return str(i)
	binary = []
	while i:
		r = i % 2
		binary.append(str(r))
		i //= 2
	return ''.join(binary)

def hamming_dist(i, j):
	i, j = _to_bin(i), _to_bin(j)
	i_len, j_len = len(i), len(j)
	if i_len >= j_len:
		l, s = i, j
	else:
		l, s = j, i
	hd = 0
	for k in range(len(s)):
		if l[k] != s[k]:
			hd += 1
	for k in range(len(s), len(l)):
		if l[k] == '1':
			hd += 1
	return hd


# circling?
def is_back(s):
	moves = {
		'U': (0, 1),
		'D': (0, -1),
		'L': (-1, 0),
		'R': (1, 0)
	}
	x, y = 0, 0
	for m in s:
		x += moves[m][0]
		y += moves[m][1]
	return not x and not y


# Array Partition 1
class Solution(object):

    def arrayPairSum(self, nums):
        """
        :type nums: List[int]
        :rtype: int
        """
        nums.sort()
        max_min_sum = 0
        for i in range(len(nums)):
        	if not i % 2:
        		max_min_sum += nums[i]

        return max_min_sum


# Number Complement
def _to_bin(i):
	if not i:
		return i
	binary = []
	while i:
		r = i % 2
		binary.append(r)
		i //= 2
	binary.reverse()
	return binary

class Solution(object):

    def findComplement(self, num):
        """
        :type num: int
        :rtype: int
        """
        binary = _to_bin(num)
        for i, b in enumerate(binary):
        	if b:
        		binary[i] = 0
        	else:
        		binary[i] = 1
        ans = 0
        for i in binary:
        	ans *= 2
        	ans += i
        return ans


# Keyboard Row
class Solution(object):

	rows = [
		set(['q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p']),
		set(['a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l']),
		set(['z', 'x', 'c', 'v', 'b', 'n', 'm']),
	]

    def findWords(self, words):
        """
        :type words: List[str]
        :rtype: List[str]
        """
        ans = []
        for wd in words:
        	w = wd.lower()
        	w = set(w)
        	w_0 = w.pop()
        	for r in Solution.rows:
        		if w_0 in r and not w - r:
        			ans.append(wd)
        			break

        return ans


# Reverse Words in a String III
class Solution(object):
    def reverseWords(self, s):
        """
        :type s: str
        :rtype: str
        """
        s = list(s)

        def _reverse_word(i, j):
        	for k in range(i, (i + j) // 2 + 1):
        		temp = s[k]
        		s[k] = s[j - (k - i)]
        		s[j - (k - i)] = temp

        start = 0
        for i, c in enumerate(s):
        	if c == ' ':
        		_reverse_word(start, i - 1)
        		start = i + 1
        _reverse_word(start, len(s) - 1)

        return ''.join(s)


# Merge Two Binary Trees

# Definition for a binary tree node.
class TreeNode(object):
    def __init__(self, x):
        self.val = x
        self.left = None
        self.right = None


class Solution(object):
    def mergeTrees(self, t1, t2):
        """
        :type t1: TreeNode
        :type t2: TreeNode
        :rtype: TreeNode
        """
        if not t1:
        	return t2
        if not t2:
        	return t1

        self._visit(t1, t2)
        return t1

    def _visit(self, n1, n2):
    	n1.val += n2.val
    	if n2.left:
	    	if not n1.left:
	    		n1.left = TreeNode(0)
    		self._visit(n1.left, n2.left)
    	if n2.right:
	    	if not n1.right:
	    		n1.right = TreeNode(0)
    		self._visit(n1.right, n2.right)


t1 = TreeNode(1)
t1.left = TreeNode(3)
t1.right = TreeNode(2)
t1.left.left = TreeNode(5)
t2 = TreeNode(2)
t2.left = TreeNode(1)
t2.right = TreeNode(3)
t2.left.right = TreeNode(4)
t2.right.right = TreeNode(7)

s = Solution()
x = s.mergeTrees(t1, t2)


# Baseball Game
class Solution(object):
    def calPoints(self, ops):
        """
        :type ops: List[str]
        :rtype: int
        """
        nums = []
        for o in ops:
        	try:
        		o = int(o)
        		nums.append(o)
        	except ValueError:
        		if o == 'C':
        			nums.pop()
        		elif o == 'D':
        			nums.append(2 * nums[-1])
        		elif o == '+':
        			nums.append(nums[-1] + nums[-2])

        return sum(nums)


# Max area of island
def DFS(n, data):
	visited = set()

	def _DFS(i, j):
		visited.add((i, j))
		c = 1
		for m in [(-1, 0), (1, 0), (0, -1), (0, 1)]:
			x, y = i + m[0], j + m[1]
			if 0 <= x < n and 0 <= y < n and data[x][y] and (x, y) not in visited:
				c += _DFS(x, y)
		return c

	m = 0
	for i in range(n):
		for j in range(n):
			if data[i][j] and (i, j) not in visited:
				c = _DFS(i, j)
				if c > m:
					m = c

	print(m)


data = [
	[1, 1, 0, 0, 0],
	[1, 0, 1, 1, 0],
	[0, 0, 1, 1, 1],
	[1, 0, 1, 0, 0],
	[0, 0, 1, 1, 0]
]

DFS(4, data)


# Reshape the Matrix
class Solution(object):
    def matrixReshape(self, nums, r, c):
        """
        :type nums: List[List[int]]
        :type r: int
        :type c: int
        :rtype: List[List[int]]
        """
        if not nums or len(nums) * len(nums[0]) != r * c:
            return nums

        cnt, x = 0, 0
        ans = [[] for i in range(r)]
        for i in range(len(nums)):
            for j in range(len(nums[0])):
                ans[x].append(nums[i][j])
                if cnt % c == c - 1:
                    x += 1
                cnt += 1
        return ans


# Trim a Binary Search Tree

# Definition for a binary tree node.
class TreeNode(object):
    def __init__(self, x):
        self.val = x
        self.left = None
        self.right = None


class Solution(object):
    def trimBST(self, root, L, R):
        """
        :type root: TreeNode
        :type L: int
        :type R: int
        :rtype: TreeNode
        """
        if not root:
            return None

        if root.left:
            root.left = self.trimBST(root.left, L, R)
        if root.right:
            root.right = self.trimBST(root.right, L, R)


        if root.val < L:
            if root.right:
                root = root.right
                self.trimBST(root, L, R)
            else:
                return None
        elif root.val > R:
            if root.left:
                root = root.left
                self.trimBST(root, L, R)
            else:
                return None
        else:
            if root.left:
                self._trim_L(root.left, root)
            if root.right:
                self._trim_R(root.right, root)
        return root


root = TreeNode(3)
root.left = TreeNode(0)
root.right = TreeNode(4)
root.left.right = TreeNode(2)
root.left.right.left = TreeNode(1)
L, R = 1, 3

s = Solution()
x = s.trimBST(root, L, R)




root = TreeNode(1)
root.left = TreeNode(0)
root.right = TreeNode(2)
L, R = 1, 2










class Solution(object):
    def maxDepth(self, root):
        """
        :type root: TreeNode
        :rtype: int
        """
        self.max_d = 0
        self._visit(root, self.max_d)
        return self.max_d

    def _visit(self, node, n):
        n += 1
        if not node.left and not node.right:
            if n > self.max_d:
                self.max_d = n
            return
        if node.left:
            self._visit(node.left, n)
        if node.right:
            self._visit(node.right, n)




class Solution(object):
    def averageOfLevels(self, root):
        """
        :type root: TreeNode
        :rtype: List[float]
        """
        ans = []

        if not root:
            return ans

        queue = []
        [root] + queue

        while queue:
            x = queue.pop()
            avg = 0.
            if x.left:







# Distribute Candies
class Solution(object):
    def distributeCandies(self, candies):
        """
        :type candies: List[int]
        :rtype: int
        """
        cnt = {}
        inv_idx = {}
        for i in candies:
        	if i not in cnt:
        		cnt[i] = 0
        	cnt[i] += 1
        for i in cnt:
        	if cnt[i] not in inv_idx:
        		inv_idx[cnt[i]] = 0
        	inv_idx[cnt[i]] += 1
        i, j = 0, len(candies) // 2
        for d, c in inv_idx.items():
        	if c >= j:
        		i += j
        		break
        	i += c
        	j -= c
        	if d != 1:
        		if d - 1 not in inv_idx:
        			inv_idx[d - 1] = 0
	        	inv_idx[d - 1] += c
        	del inv_idx[d]
        return i




class Solution(object):
    def islandPerimeter(self, grid):
        """
        :type grid: List[List[int]]
        :rtype: int
        """
        self.grid = grid
        self.n = len(grid)
        self.visited = set()
        self.perimeter = 0
        for i in range(self.n):
			for j in range(self.n):
				if grid[i][j]:
					p = (i, j)
					if p not in self.visited:
						self.perimeter += 4
						self.visited.add(p)
						self._dfs(p)
		return self.perimeter

    def _check(self, p):
    	if p not in self.visited:
    		self.perimeter += 2
    		self.visited.add(p)
    		self._dfs(p)

    def _dfs(self, p):
    	x, y = p[0], p[1]
    	if 0 <= x + 1 < self.n and self.grid[x + 1][y]:
	    	self._check((p[0] + 1, p[1]))
	    if 0 <= x - 1 < self.n and self.grid[x - 1][y]:
    		self._check((p[0] - 1, p[1]))
    	if 0 <= y + 1 < self.n and self.grid[x][y + 1]:
    		self._check((p[0], p[1] + 1))
    	if 0 <= y - 1 < self.n and self.grid[x][y - 1]:
    		self._check((p[0], p[1] - 1))


class Solution(object):
    def nextGreaterElement(self, findNums, nums):
        """
        :type findNums: List[int]
        :type nums: List[int]
        :rtype: List[int]
        """
        n = len(nums)
        done = -1
        ans = {}
        for i in range(1, n):
            y = nums[i]
            k = done
            for j in range(done + 1, i):
                z = nums[j]
                if y > z:
                    ans[z] = y
                    k = j
            done = k
        res = []
        for i in range(len(findNums)):
            res.append(ans[findNums[i]] if findNums[i] in ans else -1)
        return res




class Node(object):

    def __init__(self, v):
        self.val = v
        self.p, self.left, self.right = None, None, None


def insert(node, v):
    if node.v > v:
        if not node.left:
            n = Node(v)
            node.left = n
            return n
        return insert(node.left, v)
    if node.v < v:
        ...
        return insert(node.right, v)
    return node

insert(None, root, v)


def search(node, v):
    if not node:
        return None
    if node.v > v:
        return search(node.left, v)
    if node.v < v:
        return search(node.right, v)
    return node


def delete(root, v):
    n = search(root, v)
    if not n:
        return
    if not n.left and not n.right:
        if not n.p:
            n = None
            return
        swap(n, None)
        return
    if n.left and n.right:
        nn = n.right
        while nn.left:
            nn = nn.left
        n.v = nn.v
        swap(nn, nn.right)
        return
    if n.left:
        n.left.p = n.p
        swap(n, n.left)
    if n.right:
        n.right.p = n.p
        swap(n, n.right)


def swap(n, t):
    if n.p.left is n:
        n.p.left = t
    elif n.p.right is n:
        n.p.right = t


def pred(n):
    if n.left:
        nn = n.left
        while nn.right:
            nn = nn.right
        return nn
    if n.p:
        while n.p.right is not n:
            n = n.p
        return n.p
    return None


def in_order_trav(node):
    if not node:
        return
    in_order_trav(node.left)
    print node.v
    in_order_trav(node.right)




def checkInclusion(s1, s2):
    m = {}
    for c in s1:
        if c not in m:
            m[c] = 0
        m[c] += 1
    i = 0
    while i < len(s2):
        t = m.copy()
        while True:
            if s2[i] in t:
                t[s2[i]] -= 1
                if not t[s2[i]]:
                    del t[s2[i]]
                i += 1
            else:
                break
        if not t:
            return True
    return False















from collections import defaultdict

#This class represents a directed graph using adjacency list representation
class Graph:

    def __init__(self,vertices):
        self.V = vertices #No. of vertices
        self.V_org = vertices
        self.graph = defaultdict(list) # default dictionary to store graph

    # function to add an edge to graph
    def addEdge(self,u,v,w):
        if w == 1:
            self.graph[u].append(v)
        else:
            '''split all edges of weight 2 into two
            edges of weight 1 each.  The intermediate
            vertex number is maximum vertex number + 1,
            that is V.'''
            self.graph[u].append(self.V)
            self.graph[self.V].append(v)
            self.V = self.V + 1

    # To print the shortest path stored in parent[]
    def printPath(self, parent, j):
        Path_len = 1
        if parent[j] == -1 and j < self.V_org : #Base Case : If j is source
            print(j),
            return 0 # when parent[-1] then path length = 0
        l = self.printPath(parent , parent[j])

        #incerement path length
        Path_len = l + Path_len

        # print node only if its less than original node length.
        # i.e do not print any new node that has been added later
        if j < self.V_org :
            print(j),

        return Path_len

    ''' This function mainly does BFS and prints the
        shortest path from src to dest. It is assumed
        that weight of every edge is 1'''
    def findShortestPath(self,src, dest):

        # Mark all the vertices as not visited
        # Initialize parent[] and visited[]
        visited =[False]*(self.V)
        parent =[-1]*(self.V)

        # Create a queue for BFS
        queue=[]

        # Mark the source node as visited and enqueue it
        queue.append(src)
        visited[src] = True

        while queue :

            # Dequeue a vertex from queue
            s = queue.pop(0)

            # if s = dest then print the path and return
            if s == dest:
                return self.printPath(parent, s)


            # Get all adjacent vertices of the dequeued vertex s
            # If a adjacent has not been visited, then mark it
            # visited and enqueue it
            for i in self.graph[s]:
                if visited[i] == False:
                    queue.append(i)
                    visited[i] = True
                    parent[i] = s


# Create a graph given in the above diagram
g = Graph(4)
g.addEdge(0, 1, 2)
g.addEdge(0, 2, 2)
g.addEdge(1, 2, 1)
g.addEdge(1, 3, 1)
g.addEdge(2, 0, 1)
g.addEdge(2, 3, 2)
g.addEdge(3, 3, 2)

src = 0; dest = 3
print ("Shortest Path between %d and %d is  " %(src, dest))
l = g.findShortestPath(src, dest)
print ("\nShortest Distance between %d and %d is %d " %(src, dest, l))















































