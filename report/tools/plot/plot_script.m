%A=zeros(100,6);
%fid=fopen(getenv("LOG_NAME"),"o");
%for i=1:10
%	A=scanf(fid,
%end



%TEST="test3"
NO_OF_PLOTS = 5





 T=dlmread(sprintf("logs/%s/gtrm.log",TEST),",\t");
 T_start=T;
 T2=T;
 
        
v_handle = figure
f_handle = figure
sl_handle = figure


close all

my_colors = ["b","r","g","m","k"]




my_legend = cell(NO_OF_PLOTS,1);
	
for i=1:NO_OF_PLOTS	
	my_legend(i,1) = sprintf("app%d",i) 	;
end
 


for i =1:NO_OF_PLOTS
	
	if (size(T2)==0)
		break
	endif
	
	
	
	%pick out the first pid
	indeces= find(T2(:,2)==T2(1,2));
		
		

	log_name = sprintf("logs/%s/%d.log",TEST,T2(1,2));
		
	T=T2(indeces,:);
	
	%remove from T2
	T2(indeces,:) = 0;

	
	save_indeces=find(T2(:,2));
			
	T2=T2(save_indeces,:);
	
	TimeCol = ones(size(T(:,1)))*T_start(1,1);

	%we want the differences and not the actual time
	T(:,1)=(T(:,1)-TimeCol);

	T(:,1)=T(:,1)/10^9;%convert to sec

	%T(:,3)=T(:,3)./T(:,3);
	T(2,2)=0;

	figure(v_handle)
	hold on
	plot(T(:,1),T(:,3),my_colors(i))
	title('virtual platform')
	legend(my_legend)
	axis("tight");
	hold off
			
	figure(f_handle)
	hold on
	
	
	plot(T(:,1),T(:,4),sprintf("%s",my_colors(i)))
	title('performance')
	legend(my_legend)
	axis("tight");
	hold off	

	S=dlmread(log_name,",\t");
	
	TimeCol = ones(size(S(:,1)))*T_start(1,1);

	%we want the differences and not the actual time
	S(:,1)=(S(:,1)-TimeCol(1,1));	
	S(:,1)=S(:,1)/10^9;%convert to seconds
	

	figure(sl_handle)	
	hold on
	plot(S(:,1),S(:,3),my_colors(i))
	title('service level')	
	legend(my_legend)
	axis("tight");
	hold off		
end











