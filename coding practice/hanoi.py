start = ((3, 2, 1), (), ())
end = [(), (), (3, 2, 1)]
been_there = set([start])
path = []

def hanoi(been_there_set, state, path_list):
	print 'start:  ', state
	for i, pole in enumerate(state):
		if pole:
			state_copy = list(state)
			top = pole[-1]
			state_copy[i] = pole[:-1]
			for j in [k for k in range(3) if k != i]:
				if not state_copy[j] or state_copy[j][-1] > top:
					new_state = list(state_copy)
					new_state[j] = state_copy[j] + (top,)
					path_list.append(new_state)
					print 'middle new:  ', new_state
					if new_state == end:
						return path_list
					if tuple(new_state) not in been_there_set:
						been_there_set.add(tuple(new_state))
						return hanoi(been_there_set, tuple(new_state), path_list)
			removed_path = path_list.pop()
			been_there_set.remove(tuple(removed_path))
	return None

if __name__ == '__main__':
	print hanoi(been_there, start, path)
