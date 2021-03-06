// Copyright (c) 2017-2021 RAM Platter Hybrid Authors. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//    * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//    * Neither the name of mad chops coder AU nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "PlatterALSA.H"

using namespace ALSA;

template<typename SAMPLE_TYPE, unsigned int FS>
PlatterALSA<SAMPLE_TYPE, FS>::PlatterALSA() : N(64), M(2), direction(1) {
  printf("PlatterALSA constructed\n");
}

template<typename SAMPLE_TYPE, unsigned int FS>
PlatterALSA<SAMPLE_TYPE, FS>::~PlatterALSA(){
  printf("PlatterALSA destructed\n");
}

template<typename SAMPLE_TYPE, unsigned int FS>
int PlatterALSA<SAMPLE_TYPE, FS>::open(const char *devName){
  // first check the audio is loaded before opening the audio device
  if (PlatterAudio<SAMPLE_TYPE, FS>::audioFwd.rows()==0 || PlatterAudio<SAMPLE_TYPE, FS>::audioFwd.cols()==0)
    return RAMPlatterDebug().evaluateError(RAMPLATTER_AUDIOEMPTY_ERROR, "forward audio empty.\n");
  if (PlatterAudio<SAMPLE_TYPE, FS>::audioRev.rows()==0 || PlatterAudio<SAMPLE_TYPE, FS>::audioRev.cols()==0)
    return RAMPlatterDebug().evaluateError(RAMPLATTER_AUDIOEMPTY_ERROR, "reverse audio empty.\n");
  // we are setting up for 4 byte audio sample playback - assume fixed, LE
  if (sizeof(SAMPLE_TYPE)!=4)
    return RAMPlatterDebug().evaluateError(RAMPLATTER_BYTEDEPTH_ERROR);

  int blocking=0; // set this blocking write
  int res=Playback::open(devName, blocking);
  if (res<0)
    return ALSADebug().evaluateError(res);

  // set the format, access, sample rate and channel count
  unsigned int ch=PlatterAudio<SAMPLE_TYPE, FS>::audioFwd.cols();
  if ((res=setFARC(SND_PCM_FORMAT_S32_LE, SND_PCM_ACCESS_RW_INTERLEAVED, FS, ch))<0)
    return res;

  if ((res=setBufSize(N, M))<0) // We want M periods of size N samples per channel
    return ALSADebug().evaluateError(res);

  if ((res=setParams())<0) // set the params to the hardware device
    return ALSADebug().evaluateError(res);

  // print out the state of ALSA
  dumpState(); // outputs the current ALSA params

  if (getPeriodSize()!=N || getPeriods()!=M){ // check we have acquired the desired period size
    printf("Wanted a period size of %d samples, but got %d samples.\n", N, getPeriodSize());
    printf("Wanted %d periods, but got %d.\n", M, getPeriods());
    return RAMPlatterDebug().evaluateError(RAMPLATTER_BLOCKSIZE_ERROR);
  }

  return 0;
}

template<typename SAMPLE_TYPE, unsigned int FS>
int PlatterALSA<SAMPLE_TYPE, FS>::play(){
  int res=ALSA::changeThreadPriority(-1);
  if (res<0)
    return RAMPlatterDebug().evaluateError(res, ". When trying to max out scheduling priority.\n");

  if (!prepared()){
    printf("ALSA state : %s\n", getStateName());
    return RAMPlatterDebug().evaluateError(RAMPLATTER_ALSA_STATE_ERROR, ". Expecting prepared state.\n");
  }
  int n=0, ch=PlatterAudio<SAMPLE_TYPE, FS>::audioFwd.cols();
  int step=N*ch*4;
  int Nm1=N-1;
  char *aFwd=(char*)PlatterAudio<SAMPLE_TYPE, FS>::audioFwd.data();
  char *aRev=(char*)PlatterAudio<SAMPLE_TYPE, FS>::audioRev.data();
  direction=1;
  bool firstRun=true;
  while (1){
    if (direction>0) {
      writeBuf(aFwd,N,firstRun); // play the audio data
      aFwd+=step;
    } else {
      writeBuf(aRev,N, firstRun); // play the audio data
      aRev-=step;
    }
    firstRun=false;
  }
  return 0;
}

template class PlatterALSA<int, 48000>;
template class PlatterALSA<int, 96000>;
