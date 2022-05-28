#!/bin/bash
for i in {2..7}
do
	python3 ReasoningTasks.py --task t$i --engine davinci
done

