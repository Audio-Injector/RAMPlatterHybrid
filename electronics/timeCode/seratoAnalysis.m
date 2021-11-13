function seratoAnalysis
fn='Serato Control CD.wav';
[s,fs]=audioread(fn);
tau=1; % analysis seconds
N=tau*fs; % samples
tauD=10e-3; % display seconds
Nd=tauD*fs; % samples
f=linspace(0,fs,N+1); f(end)=[];

Ns=size(s,1);
n=randi(Ns-N)+(0:N-1);
x=s(n,:);
X=fft(x);
subplot(211);
plot(x); grid on; xlabel('block sample (n)'); ylabel('Amplitude'); title('Serato control signal');
set(gca,'xlim',[1 Nd]);
subplot(212);
semilogx(f, 20*log10(abs(X))); grid on; xlabel('f (Hz)'); ylabel('dB');
print -dpng /tmp/TimeCodeExample.png
end