Simple example of threaded code for running on a leon3mp system

Tested using 4 CPU system on Xilinx ml605.

create_thread - finds an unactive (READY) CPU and executes and function specified
join_thread   - blocks while CPU specified is ACTIVE

Notes:
CPU0 is in control of system execution, when started this starts all available CPUs and then sets up some status and function pointer global arrays

Problems:
I need to work out a better way of starting all processors and then setting up the global arrays. It seems that whenever a processor is started it zeros out the global variables, which is a bit annoying.
