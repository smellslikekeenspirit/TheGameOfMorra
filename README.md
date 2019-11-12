
#Warning
The makefile does not have --prefix /usr/local/ tag added to the make statements. Add them if necessary.

# Usual compilation
        make morra
#If the program is running on a local machine:
		
        make run_local ROUNDS=X PLAYERS=Y
		
#If the program is running on a cluster of machines:
	
		make run_cluster ROUNDS=X PLAYERS=Y HOSTFILE=Z
	
	
#If execution is divided among the processors in round-robin:

		make run_round_robin ROUNDS=X PLAYERS=Y HOSTFILE=Z

