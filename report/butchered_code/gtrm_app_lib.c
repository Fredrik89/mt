#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <stddef.h>
#include "gtrm_app_lib.h"













#define MAXMSG  512
#define MESSAGE "Message from client"
#define GTRM_SOCKET "@/org/freedesktop/systemd1/"
#define NO_OF_SENDS 5
#define WAIT_FOR_REPLY 0


#define DEBUG 0


int sock, addr_len;
struct sockaddr_un *gtrm_sa;

//creates a socket and binds it with sockaddr sa using filename as sun_path. returns size of sa.
void make_named_socket (char *filename) {		
	size_t size;

	/* Create the socket. */
	sock = socket (PF_UNIX, SOCK_DGRAM|SOCK_NONBLOCK, 0);
	if (sock < 0) {
	
		#ifdef DEBUG
			fprintf(stderr,"socket() failed:\n");
		#endif
		perror ("socket");
		exit (EXIT_FAILURE);
	}
	struct sockaddr_un sa;
			
	/* Bind a name to the socket. */
	sa.sun_family = AF_FILE;
	strcpy (sa.sun_path, filename);

	/* The size of the address is
	the offset of the start of the filename,
	plus its length,
	plus one for the terminating null byte. */
	size = (offsetof (struct sockaddr_un, sun_path) + strlen (sa.sun_path) + 1);

	if (bind (sock, (struct sockaddr *) &sa,size) < 0) {
		perror ("bind");
		exit (EXIT_FAILURE);
	}
	
}





//creates and binds the applications socket to sa from filename, creates a sa for gtrm_socket
int gtrm_lib_setup_socket(char *filename){
		
	/* Make the socket. */
	make_named_socket(filename);

	/* Initialize the server socket address. */
	//read gtrm address
	char*  env;

	/*Remove*/					
	//setenv("GTRM_SOCKET","@/org/freedesktop/systemd1/gtrm",1);
	/*Remove^*/
		
	env = getenv("GTRM_SOCKET");
	
	printf("\nenv:%s\n",env);
	
	
	//log_error("found gtrm socket:%s",env);
	fprintf(stdout,"found gtrm socket:%s\n",env);
	
	
	//sometimes getenv won't give you the start of the address, so here we try and fix that.
	if(env[0]!='@')
	{
		printf("\nnot complete adress");
		char* temp;
		char *temp_two[100];	
		temp=(char*)temp_two;

		strcpy(temp,GTRM_SOCKET);

	//	printf("\ntemp:%s",

		while ( *temp!='\0')	
			temp++;		
	
		//printf("\n*(temp-1):%s",temp-1);


		strcpy(temp,strstr(env,"gtrm")); 
		printf("\nresult:%s",temp_two);
		printf("*env");

		env=(char*)temp_two;
		fprintf(stdout,"repaired socket addres:%s\n",env);
	}
	
	//log_error("repaired socket addres:%s",env);
	
	
	

	gtrm_sa = malloc(sizeof(struct sockaddr_un));
	
	strcpy(gtrm_sa->sun_path, env);		
	addr_len = strlen(gtrm_sa->sun_path) + sizeof(gtrm_sa->sun_family);
		
	gtrm_sa->sun_path[0]=0;
	gtrm_sa->sun_family = AF_UNIX;
}

void gtrm_lib_send_performance(_application_h *h,double performance){

	char msg[100];

	//data is stored in char as "pid-performance-happy-weight" 		
	printf("pid = %d\n", h->application_id);
	sprintf(msg,"%ux%fx%ux%f",h->application_id,performance,h->happy,h->weight);

	int n;	
	n = sendto(sock,msg,100,0,(struct sockaddr*)gtrm_sa,addr_len);
	if(n<0){
			perror("sendto");
			exit(EXIT_FAILURE);
	}
}

int gtrm_lib_set(_application_h* a, uint types, uint64_t* ert) {

	#ifdef _JOBSIGNALER_DEBUG
		fprintf(stdout, "[set] started\n");
	#endif
	int retvalue = EXIT_NORMAL;

	// Application dependant initialization
	if (types > _H_MAX_JOBS) {
		#ifdef _JOBSIGNALER_ERROR
			fprintf(stderr, "[set] jobs number exceeding maximum; limited\n");
		#endif
		types = _H_MAX_JOBS; 
	}
	a->jobs = types;
	int c_act;
	for (c_act = 0; c_act<types; ++c_act)
		a->expected_response_times[c_act] = ert[c_act];
	
	// Application independent initialization
	a->total_jobs = 0;
	a->progress_jobs = 0;
	a->completed_jobs = 0;
	a->performance_multiplier = 1;
	
	
	
	
	/*
	#ifdef _JOBSIGNALER_MULTITHREADED
		retvalue = pthread_mutex_init(&a->mutex, NULL);
		if (retvalue != 0) {
			#ifdef _JOBSIGNALER_ERROR
				fprintf(stderr, "[set] mutex error\n");
			#endif
		}
	#endif*/
	#ifdef _JOBSIGNALER_DEBUG
		fprintf(stdout, "[set] ended\n");
	#endif
	return retvalue;

}


double gtrm_lib_get_performance_number(_application_h* a, int job_type){

	// Averaging the value of last jobs of specified type: the function
	// uses all jobs of the given type that are in the latest
	// records. If empty it will return zero. If called with -1 it will
	// return the average over all type of jobs.
	double sum_performances = 0.0;
	double num_performances = 0.0;
	int c_act;
	int upvalue = a->completed_jobs;
	if (a->completed_jobs > _H_MAX_RECORDS)
		upvalue = _H_MAX_RECORDS;

	for (c_act=0; c_act<upvalue; ++c_act) {
		// this last condition in the if is used to avoid to report
		// job of type 0 that are not real jobs but initial values of
		// the vector: real jobs have a start timestamp

		if ((a->jcompleted[c_act].type == job_type || 
				job_type == -1 || job_type>=a->jobs) &&
				a->jcompleted[c_act].start_timestamp != 0) {
			int64_t deadline = a->expected_response_times[a->jcompleted[c_act].type];
			int64_t response_time = a->jcompleted[c_act].end_timestamp
				- a->jcompleted[c_act].start_timestamp;
			double this_perf = ((double) deadline / (double) response_time) - 1.0;
		
			
			
			
			/*
			#ifdef DEBUG
				fprintf(stdout,"this_perf b4 threshold:%f\n",this_perf);			
			#endif
			*/
			
			
			if (this_perf<-1.0) 
				this_perf = -1.0; // thresholds
			if (this_perf>+1.0) 
				this_perf = +1.0;
			sum_performances += this_perf;
			num_performances += 1.0;
		} 
	}
	if (num_performances == 0)
		return -1.0;
	else
		// averaging
		return sum_performances/num_performances;
}

int gtrm_lib_update_performance_multiplier(_application_h* h){

	int n;
	char buf[1024];
	char final_buf[1024];
	int got_something = 0;

	do{
		memset(buf,0,1024);
		
		n = recvfrom(sock,buf,1024,0, NULL,0);			
			
		//if no msg left
		if(n<0)
			break;
				
		if(n>1024)
			printf("too big msg! >1024, n:%d\n",n);
			
		assert(n<=1024);			
			
		strcpy(final_buf,buf);
		
		got_something = 1;
						
	} while(n>0);
	
	if(got_something!=1)
		return -1;
		
	
	
	h->performance_multiplier = atof(final_buf);	
	
	#ifdef DEBUG
		if(got_something!=0)
			fprintf(stderr,"received pmulti:%f\n",atof(final_buf));
	#endif
	
	
	
		
	return 0;
}

int gtrm_lib_signalend(_application_h* a, uint id) {

	// Get actual time
	struct timespec time_info;
	int64_t time;
	clock_gettime(CLOCK_REALTIME, &time_info);
	time = (int64_t) time_info.tv_sec*1000000000 + (int64_t) time_info.tv_nsec;
	int retvalue = EXIT_FAILURE_JOBNOTFOUND;

	#ifdef _JOBSIGNALER_MULTITHREADED
		pthread_mutex_lock(&a->mutex);
	#endif

	// Looking for the job to be terminated
	int c_act;
	for (c_act=0; c_act<(a->progress_jobs % _H_MAX_RECORDS); ++c_act) {
		if (a->jprogress[c_act].id == id) {
			
			// Writing it
			uint index_completed = a->completed_jobs % _H_MAX_RECORDS;
			int type = a->jprogress[c_act].type;
			int64_t start = a->jprogress[c_act].start_timestamp;
			a->jcompleted[index_completed].id = a->total_jobs;
			a->jcompleted[index_completed].type = type;
			a->jcompleted[index_completed].start_timestamp = start;
			a->jcompleted[index_completed].end_timestamp = time;
			a->completed_jobs++;

			// Clearing up the progress log
			a->jprogress[c_act].id = 0;
			a->jprogress[c_act].type = 0;
			a->jprogress[c_act].start_timestamp = 0;
			a->jprogress[c_act].end_timestamp = 0;
			a->progress_jobs--;

			// Done
			#ifdef _JOBSIGNALER_DEBUG
				fprintf(stdout, "[stop] removed job %d into %d\n", c_act, index_completed);
			#endif
			retvalue = EXIT_NORMAL;
		}
	}

	#ifdef _JOBSIGNALER_MULTITHREADED
		pthread_mutex_unlock(&a->mutex);
	#endif
	return retvalue;

}

int gtrm_lib_jobsignaler_signalstart(_application_h* a, uint type) {

	// Get actual time
	struct timespec time_info;
	int64_t time;
	clock_gettime(CLOCK_REALTIME, &time_info);
	time = (int64_t) time_info.tv_sec*1000000000 + (int64_t) time_info.tv_nsec;
	uint job_id = a->total_jobs;

	#ifdef _JOBSIGNALER_MULTITHREADED
		pthread_mutex_lock(&a->mutex);
	#endif

	// The number of jobs in progress should never exceed the max number
	// Otherwise jobs will be overwritten and will never finish
	uint index_in_progress = a->progress_jobs % _H_MAX_RECORDS;
	a->jprogress[index_in_progress].id = a->total_jobs;
	a->jprogress[index_in_progress].type = type;
	a->jprogress[index_in_progress].start_timestamp = time;
	a->jprogress[index_in_progress].end_timestamp = time;
	a->total_jobs++;
	a->progress_jobs++;

	#ifdef _JOBSIGNALER_DEBUG
		fprintf(stdout, "[start] added job %d\n", index_in_progress);
	#endif

	#ifdef _JOBSIGNALER_MULTITHREADED
		pthread_mutex_unlock(&a->mutex);
	#endif
	return job_id;

}


// ****************************************************************************
int gtrm_lib_jobsignaler_terminate(_application_h* a) {


	return EXIT_NORMAL;

}


// ****************************************************************************
int gtrm_lib_jobsignaler_signalend(_application_h* a, uint id) {

	// Get actual time
	struct timespec time_info;
	int64_t time;
	clock_gettime(CLOCK_REALTIME, &time_info);
	time = (int64_t) time_info.tv_sec*1000000000 + (int64_t) time_info.tv_nsec;
	int retvalue = EXIT_FAILURE_JOBNOTFOUND;

	#ifdef _JOBSIGNALER_MULTITHREADED
		pthread_mutex_lock(&a->mutex);
	#endif

	// Looking for the job to be terminated
	int c_act;
	for (c_act=0; c_act<(a->progress_jobs % _H_MAX_RECORDS); ++c_act) {
		if (a->jprogress[c_act].id == id) {
			
			// Writing it
			uint index_completed = a->completed_jobs % _H_MAX_RECORDS;
			int type = a->jprogress[c_act].type;
			int64_t start = a->jprogress[c_act].start_timestamp;
			a->jcompleted[index_completed].id = a->total_jobs;
			a->jcompleted[index_completed].type = type;
			a->jcompleted[index_completed].start_timestamp = start;
			a->jcompleted[index_completed].end_timestamp = time;
			a->completed_jobs++;

			// Clearing up the progress log
			a->jprogress[c_act].id = 0;
			a->jprogress[c_act].type = 0;
			a->jprogress[c_act].start_timestamp = 0;
			a->jprogress[c_act].end_timestamp = 0;
			a->progress_jobs--;

			// Done
			#ifdef _JOBSIGNALER_DEBUG
				fprintf(stdout, "[stop] removed job %d into %d\n", c_act, index_completed);
			#endif
			retvalue = EXIT_NORMAL;
		}
	}

	#ifdef _JOBSIGNALER_MULTITHREADED
		pthread_mutex_unlock(&a->mutex);
	#endif
	return retvalue;

}



