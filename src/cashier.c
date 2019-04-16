#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/types.h>
#include <semaphore.h>
#include "common.h"

/* cmd args validation
    ./cashier -s serviceTime -b breakTime -m shmid	*/
int cmd_validate(int argc, char const *argv[], long *serviceTime, long *shmid, long *breakTime);

int main(int argc, char const *argv[]) {
	long serviceTime, shmid, breakTime;
	/* cmd args validation
	    ./cashier -s serviceTime -m shmid -b breakTime */
	if (cmd_validate(argc, argv, &serviceTime, &shmid, &breakTime) == 1) {
		fprintf(stderr,
		        "Incorrect args supplied. Usage: ./cashier -s serviceTime -b breakTime -m shmid\n");
		return 1;
	}
	printf("DEBUG s is %li, m is %li, b is %li\n",serviceTime, shmid, breakTime);

	/*TODO*/
	// get shmid and access it
	// check cur_n_cashiers against MaxNumOfCashiers if not exit
	// deal with client in [1....serviceTime]
	//
	// if no client currently waiting (cur_n_clients_wait_cashier = 0)
	//  go to break for [1....breakTime]
	//  and come back to check for clients waiting
	//      if no client then go for break again
	// deal with semaphores P() and V() funcs
	// exit



	/* name of the shared memory object */
	const char* name = "OS";

	/* shared memory file descriptor */
	int shm_fd;

	/* pointer to shared memory object */
	void* ptr;

	/* open the shared memory object */
	shm_fd = shm_open(name, O_RDONLY, 0666);

	/* memory map the shared memory object */
	ptr = mmap(0, MAX_SHM_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

	/* read from the shared memory object */
	printf("%s", (char*)ptr);

	/* remove the shared memory object */
	munmap(ptr, MAX_SHM_SIZE);
	close(shm_fd);
	shm_unlink(name);


	return 0;
}

/* function for validating the cmd line args input */
int cmd_validate(int argc, char const *argv[], long *serviceTime, long *shmid, long *breakTime) {
	int serviceTime_found = 0;
	int shmid_found = 0;
	int breakTime_found = 0;

	if ( argc != 7 ) {
		return 1;
	}

	for (int i = 1; i < argc-1; i++) {
		if (strcmp(argv[i], "-s") == 0) {
			if (isdigit_all(argv[i+1], strlen(argv[i+1]))) {
				return 1;
			}
			serviceTime_found += 1;
			*serviceTime = strtol(argv[i+1], NULL, 10);
		}
		else if (strcmp(argv[i], "-b") == 0) {
			if (isdigit_all(argv[i+1], strlen(argv[i+1]))) {
				return 1;
			}
			breakTime_found += 1;
			*breakTime = strtol(argv[i+1], NULL, 10);
		}
		else if (strcmp(argv[i], "-m") == 0) {
			if (isdigit_all(argv[i+1], strlen(argv[i+1]))) {
				return 1;
			}
			shmid_found += 1;
			*shmid = strtol(argv[i+1], NULL, 10);
		}
	}
	if (serviceTime_found == 1 && shmid_found == 1 && breakTime_found == 1) {
		return 0;
	}
	return 1; // if the correct args were not found
}
