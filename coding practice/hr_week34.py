def _convert_to_num(d):
    return int(''.join([str(i) for i in d]))


def onceInATram(x):
    if x == 999999:
        return None

    digits = []
    for i in str(x):
        digits.append(int(i))
    f = digits[0] + digits[1] + digits[2]
    b = digits[3] + digits[4] + digits[5]

    if f > b:
        diff = f - b
        for i in xrange(5, 2, -1):
            if diff <= 9 - digits[i]:
                digits[5] += diff
                return _convert_to_num(digits)
            diff -= (9 - digits[i])
            digits[i] = 9
    else:
        if digits[3] > f:
            digits[2] = digits[3] - f + digits[2]
            for j in xrange(4, 6):
                digits[j] = 0
            return _convert_to_num(digits)
        if digits[3] == f:
            digits[2] += 1
            digits[3] += 1
            for j in xrange(4, 6):
                digits[j] = 0
            return _convert_to_num(digits)
        if digits[3] + digits[4] > f:
            diff = digits[3] + digits[4] - f
            digits[3] += 1
            digits[4] -= (diff + 1)
            digits[5] = 0
            return _convert_to_num(digits)
        if digits[3] + digits[4] == f:
            digits[3] += 1
            digits[4] -= 1
            digits[5] = 0
            return _convert_to_num(digits)
        if f < b:
            diff = b - f
            digits[4] += 1
            digits[5] -= (diff + 1)
            return _convert_to_num(digits)
        digits[4] += 1
        digits[5] -= 1
        return _convert_to_num(digits)



class Node(object):

    def __init__(self, data=None, next_node=None, prev_node = None):
        self.data = data
        self.next = next_node
        self.prev = prev_node



# Reverse a linked list
def Reverse(head):
    if not head or not head.next:
        return head

    c = head
    n = head.next
    c.next = None

    while n:
        nn = n.next
        n.next = c
        c = n
        n = nn

    return c


# Reverse a doubly linked list
def Reverse(head):
    if not head or not head.next:
        return head

    c = head
    n = head.next
    c.next = None
    c.prev = n

    while n:
        nn = n.next
        n.next = c
        n.prev = nn
        c = n
        n = nn

    return c
