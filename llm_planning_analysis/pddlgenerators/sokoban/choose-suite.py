#! /usr/bin/env python

from rolling_stone_data import rs_data

rs_nodes = {
    instance: nodes for instance, moves, pushes, nodes in rs_data if moves is not None
}


def r(begin, end=None):
    if end is None:
        end = begin
    return set(range(begin, end + 1))


solved_by_base = (
    r(1, 34)
    | r(36, 77)
    | r(79, 92)
    | r(94, 97)
    | r(100, 104)
    | r(110)
    | r(115, 116)
    | r(119, 120)
    | r(124, 125)
    | r(127, 132)
    | r(134, 136)
    | r(142)
    | r(147, 149)
    | r(151, 152)
    | r(154, 155)
)


def key(instance):
    hard = instance not in solved_by_base
    too_hard = instance not in rs_nodes
    return (hard, too_hard, rs_nodes.get(instance))


instances = list(range(1, 156))
instances.sort(key=key)

for no, instance in enumerate(instances):
    print("%3d  instance #%03d: %s" % ((no + 1), instance, key(instance)))

assert len(instances) == 155, len(instances)
assert len(solved_by_base) == 123, len(solved_by_base)
assert len(rs_nodes) == 145, len(rs_nodes)
assert set(solved_by_base - set(rs_nodes)) == {154}

# "instances" now contains, in sequence:
# 122 instances solved by blind search and Rolling Stone.
#   1 instance (#154) solved by blind search, but not by Rolling Stone.
#  23 instances solved by Rolling Stone, but not blind search.
#   9 instance not solved by anyone.
#
# Within the first and third range, instances are sorted by ascending
# node expansion count for Rolling Stone (ties broken by original
# problem number). Within the fourth range, they are ordered by
# original problem number.

opt_easy = instances[3:113:11]
opt_middle = instances[113:123]
opt_hard = instances[123:133]

opt = opt_easy + opt_middle + opt_hard

sat_easy = opt_easy[1::2]
sat_middle = opt_middle[1::2]
sat_hard = instances[123:143]

sat = sat_easy + sat_middle + sat_hard

print("sat_range = [%s]" % ", ".join(map(str, sat)))
print("opt_range = [%s]" % ", ".join(map(str, opt)))
