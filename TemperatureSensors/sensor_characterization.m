data1 = xlsread('test1.xlsx',1,'A1:C1261');
data2 = xlsread('test2.xlsx',1,'A1:C1262');
data = [data1;data2];
data_length = rows(data);
sensor = data(:,1);
temperature = data(:,3);

#measurement data is in millivolts so converting to volts
#sensor = sensor./1000;




figure(1);
plot(temperature, sensor);
title("Voltage as a function of temperature");
xlabel("Temperature/C");
ylabel("Voltage/V");

#average voltage per degree C
range = 61;
counter = 0;
test = -20:1:40;

averageVoltage = zeros(1,range);
for i = 1:range
  for j = 1:data_length
    if temperature(j) == i-20
      counter = counter + 1;
      averageVoltage(i) = averageVoltage(i) + sensor(j);
    endif
  endfor
  averageVoltage(i) = averageVoltage(i)/counter;
  counter = 0;
endfor

figure(2);
plot(-20:1:40, averageVoltage);


for i = 1:range
  averageVoltage(2,i) = i-20;
endfor
