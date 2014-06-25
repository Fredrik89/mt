
static int manager_dispatch_gtrm_fd(sd_event_source *source, int fd,
uint32_t revents, void *userdata) {

Manager *m = userdata;    		    
//will contain the read message
char buf[1024];
//will be set to the byte size of the read message
int n;
//will contain the senders adress
struct sockaddr_un *from;
socklen_t fromlen;
//stores RM data about the applications being managed
rm_app_t *app;
rm_app_t *app2;	

fromlen = 1024;

do{		

	memset(buf,'\0',1023);
	from = calloc(1,sizeof(struct sockaddr_un));
	app = calloc(1,sizeof(rm_app_t));	
	n = recvfrom(fd,buf,1024,0,(struct sockaddr *) from,&fromlen);

	//n is negative if there was no message if socket is non blocking
	n = recvfrom(fd,buf,1024,0,(struct sockaddr *) from,&fromlen);
	if(n<0)
		break;
	if(n>1024){//2do avoid magic numbers
		log_error("manager_dispatch_gtrm_fd:received too big message");
	}	

	gtrm_char2gtrmstruct(buf, app);
	pid_t pid = app->tid;
	app->sa = from;

	if(hashmap_get(m->gtrm_apps, pid) == NULL) {
		hashmap_put(m->gtrm_apps, pid, app);
	} else {
		app2 = hashmap_get(m->gtrm_apps, pid);
		gtrm_update_rm_struct(app,app2);
	}															

}while(n>0);

m->update_gtrm = true;

return 0;
}
