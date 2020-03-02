
clc;
close all;
delete(instrfindall);
comport=serial('COM5','BaudRate',9600,'DataBits',8);
set(comport,'Parity','none');

fopen(comport);
x=1;
axis([0 400 -4500 4500]);
while(x<inf)
    
    %axis([0 400 -4500 4500]);
    x=x+1;
    if(x>400)
        f=400;
    end
    if(x<401)
        f=x;
    end
    y1(f)=fscanf(comport,'%d');
    plot(y1,'r','linewidth',2);
    grid on;
    hold on;
    drawnow;
    hold off;
    
    if(x>=400)
        loop=0;
        while(loop<399)
            loop=loop+1;
            y1(loop)=y1(loop+1);
        end
        
    end
end
fclose(comport);
delete(comport);

            
    
    
