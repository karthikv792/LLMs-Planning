#! /usr/bin/env python

import itertools


def split_into_blocks(lines):
    block = []
    for line in lines:
        line = line.partition(";")[0].rstrip()
        if line:
            block.append(line)
        elif block:
            yield block
            block = []
    if block:
        yield block


def split_descriptions(lines):
    comments = []
    maze = []
    for block in split_into_blocks(lines):
        for line in block:
            if line.startswith(('"', "'")):
                comments.append(line)
                assert not maze
            else:
                maze.append(line)
        if maze:
            yield comments, maze
            maze = []
            comments = []


class Direction:
    def __init__(self, name, dy=0, dx=0):
        self.name = name
        self.dy = dy
        self.dx = dx

    def __call__(self, row, col):
        return (row + self.dy, col + self.dx)


def create_pddl(filename, prob_name, desc, track, hex):
    assert track in ["sequential", "temporal"], track

    comments, maze = desc
    num_rows = len(maze)
    num_cols = max(len(row) for row in maze)
    maze = [row.ljust(num_cols) for row in maze]

    player_counter = itertools.count(1)
    stone_counter = itertools.count(1)

    init = []
    objects = []
    goal = []

    if hex:
        dirs = [
            Direction("dir-northwest", dx=-1, dy=-1),
            Direction("dir-west", dx=-2),
            Direction("dir-southwest", dx=-1, dy=+1),
            Direction("dir-northeast", dx=+1, dy=-1),
            Direction("dir-east", dx=+2),
            Direction("dir-southeast", dx=+1, dy=+1),
        ]
    else:
        dirs = [
            Direction("dir-up", dy=-1),
            Direction("dir-down", dy=+1),
            Direction("dir-left", dx=-1),
            Direction("dir-right", dx=+1),
        ]

    def pos_name(row, col):
        width = len(str(max(num_rows, num_cols)))
        return "pos-%0*d-%0*d" % (width, col + 1, width, row + 1)

    for row in range(num_rows):
        for col in range(num_cols):
            cell = maze[row][col]
            assert cell in " #$.@+*", cell
            if hex and (row + col) % 2:
                # Hexoban uses only every 2nd cell.
                assert cell == " ", (row, col, cell)
                continue
            is_player = cell in "@+"
            is_stone = cell in "$*"
            is_goal = cell in ".*+"
            is_wall = cell in "#"

            pos = pos_name(row, col)
            objects.append("%s - location" % pos)

            if is_goal:
                init.append("(IS-GOAL %s)" % pos)
            else:
                init.append("(IS-NONGOAL %s)" % pos)
            if not is_player and not is_stone and not is_wall:
                init.append("(clear %s)" % pos)
            if is_player:
                player = "player-%02d" % next(player_counter)
                objects.append("%s - player" % player)
                init.append(f"(at {player} {pos})")
            if is_stone:
                stone = "stone-%02d" % next(stone_counter)
                objects.append("%s - stone" % stone)
                init.append(f"(at {stone} {pos})")
                if is_goal:
                    init.append("(at-goal %s)" % stone)
                goal.append("(at-goal %s)" % stone)
            for direction in dirs:
                row2, col2 = direction(row, col)
                if 0 <= col2 < num_cols and 0 <= row2 < num_rows:
                    if not is_wall and maze[row2][col2] != "#":
                        pos2 = pos_name(row2, col2)
                        init.append(f"(MOVE-DIR {pos} {pos2} {direction.name})")
    for direction in dirs:
        objects.append("%s - direction" % direction.name)

    out = open(filename, "w")
    for line in comments:
        print(";; %s" % line.rstrip(), file=out)
    if comments:
        print(";;", file=out)
    for line in maze:
        print(";; %s" % line.rstrip(), file=out)
    print(file=out)
    print("(define (problem %s)" % prob_name, file=out)
    print("  (:domain sokoban-%s)" % track, file=out)
    print("  (:objects", file=out)
    for objdesc in sorted(objects):
        print("    %s" % objdesc, file=out)
    print("  )", file=out)
    print("  (:init", file=out)
    for initdesc in sorted(init):
        print("    %s" % initdesc, file=out)
    if track == "sequential":
        print("    (= (total-cost) 0)", file=out)
    print("  )", file=out)
    print("  (:goal (and", file=out)
    for goaldesc in sorted(goal):
        print("    %s" % goaldesc, file=out)
    print("  ))", file=out)
    if track == "sequential":
        print("  (:metric minimize (total-cost))", file=out)
    else:
        print("  (:metric minimize (total-time))", file=out)
    print(")", file=out)
    out.close()


def translate_suite(suite, hex=False):
    input_file = open("sources/%s.data" % suite)
    for no, desc in enumerate(split_descriptions(input_file)):
        for track in ["sequential", "temporal"]:
            name = "p%03d-%s-%s" % (no + 1, suite, track)
            create_pddl("pddl/%s.pddl" % name, name, desc, track, hex)


if __name__ == "__main__":
    translate_suite("microban")
    translate_suite("multiban")
    translate_suite("hexoban", hex=True)
