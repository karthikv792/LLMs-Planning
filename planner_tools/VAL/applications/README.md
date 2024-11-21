# Applications

## Analyse

Analyse a domain and problem file to give you information about type structure, relationship between arguments, overloading of predictates and functions.

## DomainView - WIP

Turn a domain into a visualisation (LaTeX document)

## HowWhatWhen

Analyse domain, problem (and plan) identifying decision making strategy of a planner.

## Instantiate

Ground the domain and problem.

## InstantiationIP

Refined veriant of instatiate.

## Parser

The PDDL Parser will find and report errors in PDDL more explicitly than validate.

Parse domain file:

`Parser <domainfile>`

Parse domain and problem file:

`Parser <domainfile> <problemfile>`

## PinguPlan

Application to create a plan for the [Pingus](https://pingus.seul.org/index.html) application

## PlanRec

Identify which action are achieving precondition of a later action in the plan.

## PlanSeqStep

Scripting of multiple plan execution and extraction of final state.

Use output of PlanToValStep output

## PlanToValStep

This utility takes a plan file as input and writes out a script in the form used by ValStep, all as one output. 
This allows you to then run all or part of the plan through ValStep easily.

`PlanToValStep myplan.pddl > myvalstepscript`

## Relax

Auto generation of a relaxed domain. Used to compare relax plan and real plan.

## TIM

Type Inference Mechanism (1998) tells which predicate are single value.

## ToFn

To function uses the TIM analysis to translate the domain from a predicate model to a multi-value model.

## TypeAnalysis

`TypeAnalysis <domainfile> <problemfile>`

The type-checking tool is reasonably robust at finding type errors in your PDDL domain/problem files.
Note that the PDDL parser will find and report errors in PDDL more explicitly.

## Validate

This utility is mainly used by planning engine developers and researchers, but it may be useful to a PDDL modeler as well.
You may get to a situation, when you are not sure why the planning engine is *not* generating the plan you are expecting.
In that case, you could handcraft the plan you expect to get and ask the Validator whether it is valid.
It usually tells you where you are missing a pre-condition, or where you are violating an over all constraint.

Validate has many command line options, but the most important first few are:

`Validate -t <number> -v <domainfile> <problemfile> <planfile....>`

Multiple plan files can be handled together. The `-t` flag allows the value of epsilon to be set.
The default value is *0.01*, but *0.001* is a good value to use for most planners.
Actions separated by epsilon or less are treated as simultaneous by Validate. 
The `-v` switch is the verbose mode.

Syntax for validating your domain model:

`Validate domain.pddl`

Syntax for validating your domain model and problem file:

`Validate domain.pddl problem.pddl`

Syntax for getting more insights into what is happening during your plan (theoretical) execution or validating a hand-coded plan:

`Validate -v -t 0.001 domain.pddl problem.pddl plan.txt`

Syntax for generating LaTeX file that visualizes the plan and changes of function values throughout the plan.

`Validate -l -f report -t 0.001 domain.pddl problem.pddl plan.txt`

The above will generate report.tex (the .tex extension is automatically added, so need not be placed on the command line).

To render the .tex file as a .pdf, download [MiKTeX](https://miktex.org/download). After you install it, its bin folder is added to your %path% and you may simply use it from a command line this way:

`pdflatex -synctex=1 -interaction=nonstopmode report.tex`

The first run will pop up a dialog asking for your consent to download additional packages needed by this .tex file. After that you should be able to just run

`pdflatex report.tex`

If you want to do the .pdf generation painlessly, you can just install the [LaTeX Preview extension for VSCode](https://marketplace.visualstudio.com/items?itemName=ajshort.latex-preview).

## ValStep

This tool is run with command line:

`ValStep [-i <input_file>] <domain> <problem>`

It gives a prompt "`?`" and expects one of the valid commands:

* `start <action> @ <time>`
* `end <id> @ <time>`
* `x`
* `w <filename>`
* `q`
* `e <line of text to echo>`

These commands do the following:

`start <action> @ <time>` queues an instantaneous action or a start of a durative action for execution at the given time (which should be higher/later than previous one). ValStep reports a number which is the action id code that must be used when queuing the corresponding end using `end <id> @ <time>`.
The command `x` causes all enqueued actions to be executed. Note that the order in which action starts and ends are added is not important – it is the time that they are specified to occur that determines when the execution will simulate them happening. Also note that `x` will only step forward to the next time at which something is specified to happen. So here is an example:

Consider this short temporal PDDL plan:

```pddl
0.00100: (boil-water) [64.00100]
61.00100: (pump-boiling-water) [3.00000]
```

```shell
ValStep.exe domain.pddl problem.pddl
?
```

Queue first action start:

```shell
? start boil-water @ 0.001
```

ValStep responds:

```shell
Posted action 1
```

Therefore _1_ is the ID of the `boil-water` action.

Execute the action and evaluate the state delta by passing the `x` command:

```shell
? x
```

ValStep responds:

```shell
Seeing 1 changed lits
boiling - now true
```

Predicate `boiling` is now `true`.

Similarly we queue and execute the second action start:

```shell
? start pump-boiling-water @ 61.001
Posted action 2
? x
Seeing 2 changed lits
boiling - now true
pumping - now true
water-temperature - now 96
```

Although this action only changes 2 literals/fluents, ValStep reports all changes since the initial state, including the already establish `boiling`==`true`.

Then we queue and execute the end of the `pump-boiling-water` using the ID `2`:

```shell
? end 2 @ 64.001
? x
Seeing 2 changed lits
boiling - now true
pumping - now false
cup-level - now 30
water-temperature - now 90
```

Last, we close the `boil-water` action:

```shell
? end 1 @ 64.002
? x
Seeing 2 changed lits
boiling - now false
pumping - now false
cup-level - now 30
water-temperature - now 90.001
```

The final state is therefore everything in the initial state plus

```shell
boiling - now false
pumping - now false
cup-level - now 30
water-temperature - now 90.001
```

In case multiple actions start at the same time, all shall be enqueued before triggering their execution using `x`.

The command `e` echoes the remaineder of the line of input.  This allows the user to annotate the ValStep output as they wish.  For example:

```shell
? e Finished pumping boiling water.
Finished pumping boiling water.
```

The command `q` quits the tool, causing it to print the current state as a PDDL problem file to the console output. If you wish to record the state before exiting, use the command `w <file>` writes the current state (and goal) as a PDDL problem file into the indicated file.

Note that the problem file written out would not replicate any _outstanding_ timed-initial effects from the original problem file. This feature is currently missing.



## ValueSeq

This tool can be used to extract the values of particular numeric state values in the course of a plan. It is used as follows:

`ValueSeq <domain> <problem> <plan> [<function>*] [REMOVE <tag>*]`

where the function expressions are state variables and the `tag`s are strings that if included in an action name, the action is removed from the output (for brevity). The square brackets indicate optional arguments and the `*` symbol indicates arguments that can appear any number of times.

An example usage:

```shell
ValueSeq domain.pddl problem_boil_and_pour1.pddl problem_boil_and_pour1.plan water-temperature
```

... outputs ...

```csv
boil-water, 35, 90.001
pump-boiling-water, 35, 90
```

Using the `-t` switch:

```shell
ValueSeq.exe -t domain.pddl problem_boil_and_pour1.pddl problem_boil_and_pour1.plan water-temperature
```

... outputs ...

```csv
0.001, boil-water, 35, 90.001
61.001, pump-boiling-water, 35, 90
```

The output shows changes in function values before and after an action execution. However, if you look up the action start time and duration in the plan file, you can reconstruct a temporal sequence and plot it. Mind the cases, where the same action appears in the plan more than once.

Using the `-T` switch:

```shell
ValueSeq.exe -T domain.pddl problem_boil_and_pour1.pddl problem_boil_and_pour1.plan water-temperature
```

... outputs ...

```csv
water-temperature
0, 35
0.001, 35
61.001, 96
64.001, 90
64.002, 90.001
```

This output may be directly pasted into a spreadsheet program e.g. Excel to render a chart.
