This generator has not been used for IPC 2011.
All the problems have been taken from IPC 2008.

The IPC 2011 organizers



* pddl-generators note

`build-program.py` converts microban / multiban / hexoban data into PDDL.

`choose-suite.py` analyzes instances in `rolling_stone_data.py` and
select problem instances for satisficing and optimal track.

From the message above, it seems these problem generators (which are based on
existing microban benchmarks etc.) are not used for IPC2011.



Scripts in `random/` are randomized generators that are used for generating
instances for IPC2008, which was also used for IPC2011.


usage:

OPTIONS   DESCRIPTIONS

-n <num>    grid size (minimal 5)
-b <num>    number of boxes (minimal 1)
-w <num>    number of walls (minimal 0)
-s <num>    random seed

