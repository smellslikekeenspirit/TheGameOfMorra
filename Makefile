SOURCE_FILE=morra.c
OUTPUT_FILE=morra

# defaults
PLAYERS=2
HOSTFILE=hostfile
ROUNDS=1

# compiling morra
morra: ${SOURCE_FILE}
        mpicc -ggdb -Wall -Wextra -pedantic ${SOURCE_FILE} -o ${OUTPUT_FILE}

# clean up
clean:
        rm ${OUTPUT_FILE}

# for distributing among local machines
run_local:
        mpiexec -n ${PLAYERS} ./${OUTPUT_FILE} ${ROUNDS}

# for distributing among other machines
run_cluster:
        mpiexec --mca btl_tcp_if_include eno1 --hostfile ${HOSTFILE} -n ${PLAYERS} ./${OUTPUT_FILE} ${ROUNDS}

# for distributing among cluster machines in round-robin
run_round_robin:
        mpiexec --map-by node:SPAN --hostfile ${HOSTFILE} -n ${PLAYERS} ./${OUTPUT_FILE} ${ROUNDS}
