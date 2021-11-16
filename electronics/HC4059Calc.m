function HC4059Calc
% 74HC4059 settings calculator
% Matt Flax <flatmax@>
% Flatmax (C) 2021
target=12000/2;
mode=2;
mults=[1 10 100 1000];
stages=[4 9 9 9]; % stages 1 to 4. Stage 5 is stage 1 div. 2.
N=mode*sum(mults.*[stages(2:end) floor(stages(1)/2)])+stages(1);
fprintf("\ntarget = %d     N = %d   target-N = %d\n",target, N, target-N);
fprintf('mode %d\n',mode);
binAll=[];
for m=1:length(stages)
    bin{m}=fliplr(dec2bin(stages(m),4));
    binAll=[binAll bin{m}];
    fprintf('stage %d : %d : 0b %s\n',m,stages(m),bin{m});
end
bin{5}=fliplr(dec2bin(floor(stages(1)/2),3));
fprintf('stage 5 : %d : 0b %s\n',floor(stages(1)/2),bin{5});

K{2} ='111';
K{4} ='011';
K{5} ='101';
K{8} ='001';
K{10}='110';
for m=1:length(binAll)
    fprintf('J%.2d : %c\n',m,binAll(m));
end
fprintf('mode : %d : Ka %c\n',mode, K{mode}(1))
fprintf('mode : %d : Kb %c\n',mode, K{mode}(2))
fprintf('mode : %d : Kc %c\n',mode, K{mode}(3))
end
