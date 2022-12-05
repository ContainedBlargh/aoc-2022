from sys import argv

duplicates = []
with open(argv[1]) as fp:
    for line in fp:
        l = len(line)
        (left, right) = line[:l // 2], line[l // 2:]
        local_duplicates = set()
        for r in right:
            if r in left:
                local_duplicates.add(r)
        duplicates += list(local_duplicates)

def score(letter):
    letter = ord(letter)
    if letter >= ord('a'):
        return letter - ord('a') + 1
    elif letter >= ord('A'):
        return letter - ord('A') + 27

duplicates = [(d, score(d)) for d in duplicates]
sum = sum([s for (_, s) in duplicates])

# for (d, s) in duplicates:
    # print(f"{d}: {s}")

print(sum)

def divide_chunks(l, n):
    # looping till length l
    for i in range(0, len(l), n):
        yield l[i:i + n]

g_sum = 0

with open(argv[1]) as fp:
    for chunk in divide_chunks([line.strip() for line in fp], 3):
        group = set(chunk[0])
        for items in chunk[1:]:
            group = group.intersection(set(items))
        group = sorted(list(group))
        g_score = score(group[0])
        # print(f"'{group[0]}': {g_score}")
        g_sum += g_score
print(g_sum)