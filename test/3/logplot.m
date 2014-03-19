%A=zeros(100,6);
%fid=fopen(getenv("LOG_NAME"),"o");
%for i=1:10
%	A=scanf(fid,
%end



 T=dlmread(LOG_NAME,",\t");


%cpu_usage current_time, performance, service_level, 
%cpu_requirement, mem_requirement, id);

%T(1:5,1)



TimeCol = ones(size(T(:,2)))*T(1,2);

%we want the differences and not the actual time, convert to seconds
T(:,2)=(T(:,2)-TimeCol)/(10^9);

%create cpu_usage/time, instead of the cpu_time total
CPU_Col = ones(size(T(:,1)));

%create diffcpu_usage/diffTime
for i = 2:1:max(size(CPU_Col))
	CPU_Col(i,1) = CPU_Col(i,1)-CPU_Col(i-1,1);
	CPU_Col(i,1) = CPU_Col(i,1)/(T(i,2)-T(i-1,2));
end 


%cpu_usage current_time, performance, service_level, 
%cpu_requirement, mem_requirement, id);

figure

subplot(2,2,1);
plot(T(:,2),T(:,1))
title('cpu usage')

subplot(2,2,2)
plot(T(:,2),T(:,3))
title('performance')
subplot(2,2,3);
plot(T(:,2),T(:,4))
title('service level')
subplot(2,2,4);
plot(T(:,2),T(:,5))
title('cpu requirement')
%subplot(3,2,5);
%plot(T(:,2),T(:,6))
%title('mem req')
%subplot(3,2,6);
%plot(T(:,2),T(:,7))
%title('id')
