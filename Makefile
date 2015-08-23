swarm:
	cc -Wall -ggdb -o swarm swarm.c config.c errors.c pipeset.c wipe.c

clean:
	rm swarm
