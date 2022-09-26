clear ; clc; close all; 
J=0.28;
b=8.5*10^(-3);
K=0.5093;
R=0.13;
L=1.6*10^(-3);
Cr=0;

Ziegler_L = 0.015;
Ziegler_R = (22-0)/(0.17-0.02);

Kp_PID = 1.2/(Ziegler_L*Ziegler_R);
Ti_PID = 2*(2*Ziegler_L);
Td_PID = 0.5*Ziegler_L;

Kp_PI =  0.9/(Ziegler_L*Ziegler_R);
Ti_PI = 3.3*Ziegler_L;

Kp_P =  1/(Ziegler_L*Ziegler_R);

Kpos=6;