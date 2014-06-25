#include "gtrm_lib.h"

#define GTRM_SOCKET "@/org/freedesktop/systemd1/gtrm"

static int manager_dispatch_gtrm_fd(sd_event_source *source, int fd, uint32_t revents, void *userdata);





static void manager_clean_environment(Manager *m) {
        assert(m);

        /* Let's remove some environment variables that we
         * need ourselves to communicate with our clients */
        strv_env_unset_many(
                        m->environment,
                        "GTRM_SOCKET",                        
                        "NOTIFY_SOCKET",
                        "MAINPID",
                        "MANAGERPID",
                        "LISTEN_PID",
                        "LISTEN_FDS",
                        "WATCHDOG_PID",
                        "WATCHDOG_USEC",
                        NULL);
}



 m->pin_cgroupfs_fd = m->gtrm_fd = m->notify_fd = m->signal_fd = m->time_change_fd = m->dev_autofs_fd = m->private_listen_fd = m->kdbus_fd = -1;
 
	m->gtrm_apps = hashmap_new(trivial_hash_func, trivial_compare_func);
	
	
static int manager_setup_gtrm(Manager *m) {
        union {
                struct sockaddr sa;
                struct sockaddr_un un;
        } sa = {
                .sa.sa_family = AF_UNIX,
        };
       int r;


	
	
        if (m->gtrm_fd < 0) {
                _cleanup_close_ int fd = -1;

                /* First free all secondary fields */
                free(m->gtrm_socket);
                m->gtrm_socket = NULL;
                m->gtrm_event_source = sd_event_source_unref(m->gtrm_event_source);

		
                fd = socket(AF_UNIX, SOCK_DGRAM|SOCK_CLOEXEC|SOCK_NONBLOCK, 0);
                if (fd < 0) {
                        log_error("Failed to allocate notification socket: %m");
                        return -errno;
                }

                if (getpid() != 1 || detect_container(NULL) > 0)
                        snprintf(sa.un.sun_path, sizeof(sa.un.sun_path), GTRM_SOCKET "/%" PRIx64, random_u64());
                else
                        strncpy(sa.un.sun_path, GTRM_SOCKET, sizeof(sa.un.sun_path));
                sa.un.sun_path[0] = 0;

                r = bind(fd, &sa.sa, offsetof(struct sockaddr_un, sun_path) + 1 + strlen(sa.un.sun_path+1));
                if (r < 0) {
                        log_error("bind() failed: %m");
                        return -errno;
                }

//                r = setsockopt(fd, SOL_SOCKET, SO_PASSCRED, &one, sizeof(one));
//                if (r < 0) {
//                      log_error("SO_PASSCRED failed: %m");
//                      return -errno;
//                }		
		

                sa.un.sun_path[0] = '@';
                m->gtrm_socket = strdup(sa.un.sun_path);
                if (!m->gtrm_socket)
                        return log_oom();

                m->gtrm_fd = fd;
                fd = -1;

                log_debug("Using gtrm socket %s", m->gtrm_socket);
        }
        else
	        log_error("in setup_gtrm: m->gtrm_fd <0, (socket= %s)", m->gtrm_socket);

        if (!m->gtrm_event_source) {
                r = sd_event_add_io(m->event, &m->gtrm_event_source, m->gtrm_fd, EPOLLIN, manager_dispatch_gtrm_fd, m);
                if (r < 0) {
                        log_error("Failed to allocate gtrm event source: %s", strerror(-r));
                        return -errno;
                }

                /* Process signals a bit earlier than SIGCHLD, so that we can
                 * still identify to which service an exit message belongs */
                r = sd_event_source_set_priority(m->gtrm_event_source, -7);
                if (r < 0) {
                        log_error("Failed to set priority of gtrm event source: %s", strerror(-r));
                        return r;
                }
        }

        return 0;
}


//<<<<<<<<<<<<<<<<<<in void manager_free(Manager *m) {


	hashmap_free_free_free(m->gtrm_apps);
	
  sd_event_source_unref(m->gtrm_event_source);
  free(m->gtrm_socket);
  manager_setup_gtrm(m);
//<<<<<<<<<<<<<<<<<in void manager_free(Manager *m) {
  
int manager_set_cpu_shares(Manager *m, pid_t pid, int shares){
		Unit* u;
		CGroupContext* c;
		
		
		log_error("manager_get_unit_by_pid:pid%d\n",pid);
		
		
		u = manager_get_unit_by_pid(m, pid);
		if(u == NULL) {
			log_error("U = null\n");
			hashmap_remove(m->gtrm_apps, pid);
			reset_virtual_platforms(m->gtrm_apps);
			return 1;
		}

		c = unit_get_cgroup_context(u);
		c->cpu_shares = shares;
		cgroup_context_apply(c, CGROUP_CPU, u->cgroup_path);
		return 0;
}
  
        
static int manager_dispatch_gtrm_fd(sd_event_source *source, int fd, uint32_t revents, void *userdata) {
	Manager *m = userdata;    	
	assert(m);
    assert(m->gtrm_fd == fd);  
    
    if (revents != EPOLLIN) {
			log_warning("Got unexpected poll event for gtrm");
			return 0;
	}
    
	char buf[1024];
	int n;
	struct sockaddr_un *from;
	
	socklen_t fromlen;
	rm_app_t *app;
	rm_app_t *app2;



	char last_was_from[124];//keeps the adress (sun_path) from last sender
	
		
	//fromlen = (socklen_t) sizeof(struct sockaddr_un);
	fromlen = 1024;
	log_error("gtrm:inside main loop systemd \n");





	do{		
	
		memset(buf,'\0',1023);
		from = calloc(1,sizeof(struct sockaddr_un));
		app = calloc(1,sizeof(rm_app_t));
	
//		memset(from,'\0',sizeof(struct sockaddr_un));

		
		n = recvfrom(fd,buf,1024,0,(struct sockaddr *) from,&fromlen);
		
		//n is negative if there was no message if socket is non blocking
		n = recvfrom(fd,buf,1024,0,(struct sockaddr *) from,&fromlen);
		if(n<0)
			break;
		if(n>1024){//2do avoid magic numbers
			log_error("manager_dispatch_gtrm_fd:received to big message");
		}
			
		//log_error("received msg:%s from%s\n",buf,from->sun_path);
		//gtrm_print_struct(app);
		assert((size_t) n < sizeof(buf));
	
		gtrm_char2gtrmstruct(buf, app);
		pid_t pid = app->tid;
		app->sa = from;
		//log_error("re_pid = %d, hashmapsize = %d\n", pid, hashmap_size(m->gtrm_apps));
		if(hashmap_get(m->gtrm_apps, pid) == NULL) {
			log_error("add new app! pid = %d\n", pid);
			hashmap_put(m->gtrm_apps, pid, app);
			//reset_virtual_platforms(m->gtrm_apps);
		} else {
			app2 = hashmap_get(m->gtrm_apps, pid);
			gtrm_update_rm_struct(app,app2);
			//hashmap_update(m->gtrm_apps, &pid, app2);
		}
	
	
	
				
	/*
		if(!hashmap_contains(m->gtrm_apps, key)) {					
				hashmap_put(m->gtrm_apps, key, app);				
				// reset_virtual_platforms*/											
				
		
	}while(n>0);

	m->update_gtrm = true;

    return 0;
}


int gtrm_compute_virtual_platforms(Hashmap *apps, gtrm_t *gtrm_t) {
	unsigned int offset = gtrm_t->num_apps != gtrm_t->prev_apps;
	if(offset)
		reset_virtual_platforms(apps);
	double c1 = gtrm_t->c1;
	double c2 = gtrm_t->c2;
	//log_error("c1 = %f, c2 = %f, offset = %d\n", c1, c2, offset);
	double epsilon = gtrm_get_epsilon(gtrm_t->iterations, offset, c1, c2);
	float sumlambdafi = 0.0;
    Iterator i;
    rm_app_t* a;
	bool all_happy = true;
    HASHMAP_FOREACH(a, apps, i) {
		sumlambdafi += (1 - a->weight) * a->performance;
	//	log_error("pids: %d\n", a->tid);
	}

	// sumlambdafi has the correct value now

    HASHMAP_FOREACH(a, apps, i) {
		all_happy = a->happy && all_happy;
		//log_error("a->happy = %d\n", a->happy);
		a->vp_old = a->vp;
		float vp_old = a->vp_old / (_MAX_ASSIGNABLE);
		//log_error("vp = %f, vp_old = %f\n", a->vp, a->vp_old);
		float tmp = vp_old - epsilon * ((1 - a->weight) * a->performance - (sumlambdafi * vp_old)); // computed as sum to one
		tmp = tmp * _MAX_ASSIGNABLE;// scaled
		if(tmp < _MIN_SINGLE_ASSIGNABLE) 
			tmp = _MIN_SINGLE_ASSIGNABLE;
		else if(tmp > _MAX_SINGLE_ASSIGNABLE)
			tmp = _MAX_SINGLE_ASSIGNABLE;
		a->vp = tmp;
		//a->vp_old = vp_old * _MAX_ASSIGNABLE;
		//log_error("pid = %d, epsilon = %f, weight = %f, tmp = %f, sumlamdafi = %f, performance = %f, vp_old = %f, vp = %f\n", a->tid,epsilon,a->weight, tmp, sumlambdafi, a->performance, a->vp_old, a->vp);
	}
	gtrm_t->all_happy = all_happy;
	return 0;
}


void gtrm_apply_virtual_platforms(Manager* m) {
	int shares;
    Iterator i;
    rm_app_t* a;
	HASHMAP_FOREACH(a, m->gtrm_apps, i) {
		shares = a->vp * _TOTAL_SHARES;
//		log_error("apply vp = %f\n", a->vp);
		manager_set_cpu_shares(m, a->tid, shares);
	}
}


//<<<<<<<<<<<<<<<<<< int manager_loop(Manager *m) {

;
		gtrm_t *gtrm_t = calloc(1,sizeof(struct gtrm_t));
		gtrm_t->num_apps = 0;
		gtrm_t->prev_apps = 0;
		gtrm_t->iterations = 0;
		gtrm_t->all_happy = true;
		gtrm_t->c1 = 0.1;
		gtrm_t->c2 = 10;
		
		
		
		
		
		
		 while (m->exit_code == MANAGER_RUNNING) {
                usec_t wait_usec;
                if (m->runtime_watchdog > 0 && m->running_as == SYSTEMD_SYSTEM)
                        watchdog_ping();

                if (!ratelimit_test(&rl)) {
                        /* Yay, something is going seriously wrong, pause a little */
                        log_warning("Looping too fast. Throttling execution a little.");
                        sleep(1);
                        continue;
                }

                if (manager_dispatch_load_queue(m) > 0)
                        continue;

                if (manager_dispatch_gc_queue(m) > 0)
                        continue;

                if (manager_dispatch_cleanup_queue(m) > 0)
                        continue;

                if (manager_dispatch_cgroup_queue(m) > 0)
                        continue;

                if (manager_dispatch_dbus_queue(m) > 0)
                        continue;

                /* Sleep for half the watchdog time */
                if (m->runtime_watchdog > 0 && m->running_as == SYSTEMD_SYSTEM) {
                        wait_usec = m->runtime_watchdog / 2;
                        if (wait_usec <= 0)
                                wait_usec = 1;
                } else
                        wait_usec = (usec_t) -1;

                r = sd_event_run(m->event, wait_usec);
                if (r < 0) {
                        log_error("Failed to run event loop: %s", strerror(-r));
                        return r;
                }
//				log_error("all happy = %d\n", gtrm_t->all_happy);
				if ((!(gtrm_t->all_happy) || m->update_gtrm) && !hashmap_isempty(m->gtrm_apps)) {
						gtrm_t->prev_apps = gtrm_t->num_apps;
						gtrm_t->num_apps = hashmap_size(m->gtrm_apps);
//						log_error("gtrm num apps %d, prev apps = %d\n",gtrm_t->num_apps, gtrm_t->prev_apps);
						gtrm_compute_virtual_platforms(m->gtrm_apps, gtrm_t);
						gtrm_t->iterations++;
						gtrm_apply_virtual_platforms(m); // set_shares according to platform
						m->update_gtrm=false;																	

						gtrm_update_performance_multipliers(m->gtrm_fd,m->gtrm_apps);
						gtrm_write_log(m->gtrm_apps, gtrm_t->num_apps);
						log_error("done with gtrm\n");			

				}
				
        }
		log_error("free gtrm_t\n");
		free(gtrm_t);
        return m->exit_code;
    
//<<<<<<<<<<<<<<<<<< int manager_loop(Manager *m) {
      if (m->gtrm_fd >= 0) {
                int copy;

                copy = fdset_put_dup(fds, m->gtrm_fd);
                if (copy < 0)
                        return copy;

                fprintf(f, "gtrm-fd=%i\n", copy);
                fprintf(f, "gtrm-socket=%s\n", m->gtrm_socket);
        }
        
        
        
        
//>>>>>>>>>>>>>>>>>>>>>><< IN int manager_deserialize(Manager *m, FILE *f, FDSet *fds) {        
          else if (startswith(l, "gtrm-fd=")) {
                        int fd;

                        if (safe_atoi(l + 10, &fd) < 0 || fd < 0 || !fdset_contains(fds, fd))
                                log_debug("Failed to parse gtrm fd: %s", l + 10);
                        else {
                                if (m->gtrm_fd >= 0) {
                                        m->gtrm_event_source = sd_event_source_unref(m->gtrm_event_source);
                                        close_nointr_nofail(m->gtrm_fd);
                                }

                                m->gtrm_fd = fdset_remove(fds, fd);
                        }

                } else if (startswith(l, "gtrm-socket=")) {
                        char *n;

                        n = strdup(l+14);
                        if (!n) {
                                r = -ENOMEM;
                                goto finish;
                        }

                        free(m->gtrm_socket);
                        m->gtrm_socket = n;

                } 
                
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<int manager_deserialize(Manager *m, FILE *f, FDSet *fds) {            



//IN>>>>>>>>>>>>>>>>>>>>>>>>>>>>int manager_reload(Manager *m) {

 /* Re-register gtrm_fd as event source */
        q = manager_setup_gtrm(m);
        if (q < 0)
                r = q;


//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<int manager_reload(Manager *m) {



    
                
