#!/bin/bash
for i in {1..7}
do
	python3 ReasoningTasks.py --task t$i --engine davinci
done

