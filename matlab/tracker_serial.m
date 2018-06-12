%% Sketch

delete(instrfindall);
%%

s=serial('COM8','BAUD', 256000);
fopen(s);

figure();cla
hold on 
counter = 1;
clear tracking;
clear counters;
max_values = 500;

%% 
for x=1:10000
      i = fscanf(s, '%s');
      disp(i);
      tracking(counter) = str2double(i);
      counters(counter) = counter;
      
      counter = counter +1;
      
      if mod(counter,2)==1
        if counter > max_values
            cla
            plot(counters(end-max_values+1:end),tracking(end-max_values+1:end),"-k")
            axis tight
        else
            plot(counters,tracking,"-k")
        end
        drawnow
      end
      
      end

fclose(s);