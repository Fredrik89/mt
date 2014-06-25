#include <sys/types.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include "util.h"
// Using the library:
#include "gtrm_app_lib.h"



#define TOTAL_JOBS 100
#define NOISE_PERCENTAGE 0.0
#define MINIMUM_SERVICE_LEVEL 0.0001
#define ERROR_APPLICATION 0
#define LOGGING_APPLICATION 0
#define EXIT_APPLICATIONFAILURE -1
#define DEBUG 0

uint id;
_application_h* myself;
	

double generate_random_0_1() {
...
}

int do_work(int64_t cpu, int64_t mem, double noise) {
...
}

int main(int argc, char* argv[]) {


	signal(SIGINT, signal_callback_handler);
	float service_level;
	float a_cpu, b_cpu;
	float a_mem, b_mem;
	double epsilon, weight;
	double deadline_seconds;

	myself = calloc(1,sizeof(_application_h));
	
	// Parameter parsing
	service_level = atof(argv[1]);
	a_cpu = atof(argv[2]);
	b_cpu = atof(argv[3]);
	a_mem = atof(argv[4]);
	b_mem = atof(argv[5]);
	epsilon = atof(argv[6]);
	weight = atof(argv[7]);
	deadline_seconds = atof(argv[8]);

	char* sock_path = "/root/temp/app";
	unsigned int r_nbr = random_u64();
	char* sock_name[100];


	sprintf(sock_name,"%s/%u",sock_path,r_nbr);
	gtrm_lib_setup_socket(sock_name);

	
	uint64_t deadline = (unsigned int) ((double)1000000000 * deadline_seconds);
	uint64_t ert[1] = {deadline};
	gtrm_lib_set(myself, 1, ert);
	myself->weight = weight;
	myself->application_id = getpid();
	int jobs;
	double performance;

	for(;;) {
	
				
		int type = 0;
		id = gtrm_lib_jobsignaler_signalstart(myself, type);
		
		
		performance = gtrm_lib_get_performance_number(myself, type);
		
		// I want to adapt only if needed
		if (performance < -0.01 || performance > 0.01) {
			myself->happy = false;
			
			
			// send performance to systemd
			gtrm_lib_send_performance(myself, performance);				
			
			//if there is no multiplier to read use the simple SL adaption
			if(gtrm_lib_update_performance_multiplier(myself) == 0)
				service_level+= epsilon*service_level*(myself->performance_multiplier-1);
			else
				service_level += epsilon * (performance * service_level);	
					
				
				
			if (service_level < MINIMUM_SERVICE_LEVEL)
				service_level = MINIMUM_SERVICE_LEVEL;
			if (service_level != service_level) // avoid nans
				service_level = 1.0;

		} else if(myself->happy==false) {			
			myself->happy = true;
			gtrm_lib_send_performance(myself, performance);
		}

		int64_t cpu_requirement = a_cpu * service_level + b_cpu;
		int64_t mem_requirement = a_mem * service_level + b_mem;

		// Do the required work	
		do_work(cpu_requirement, mem_requirement, NOISE_PERCENTAGE);
		gtrm_lib_jobsignaler_signalend(myself, id);
		id = 0;
	}	
}
