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
PlatterALSA<SAMPLE_TYPE, FS>::PlatterALSA() : N(16), M(1) {
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

  int res;
  int blocking=1; // set to blocking write
  printf("Opening the device %s\n",devName);
  Playback::open(devName, blocking);

  if ((res=resetParams())<0) // we don't want defaults so reset and refil the params
  return res;

  // we are setting up for 4 byte audio sample playback - assume fixed, LE
  if (sizeof(SAMPLE_TYPE)!=4)
    return RAMPlatterDebug().evaluateError(RAMPLATTER_BYTEDEPTH_ERROR);
  if ((res=setFormat(SND_PCM_FORMAT_S32_LE))<0)
    return ALSADebug().evaluateError(res);

  if ((res=setAccess(SND_PCM_ACCESS_RW_INTERLEAVED))<0)
    return ALSADebug().evaluateError(res);

  unsigned int fs=FS;
  if (fs!=getSampleRate()){
    printf("sample rate mismatch, file = %d Hz and ALSA = %d",fs,getSampleRate());
    if ((res=setSampleRate(fs))<0)
      return ALSADebug().evaluateError(res);
    fs=getSampleRate();
  }
  printf("Sample Rate %d Hz\n",fs);

  unsigned int ch=PlatterAudio<SAMPLE_TYPE, FS>::audioFwd.cols();
  if ((res=setBufSize(N*ch, M))<0)
    return ALSADebug().evaluateError(res);

  if ((res=setChannels(ch))<0){
    printf("Couldn't set the channels to %d\n", ch);
    return ALSADebug().evaluateError(res);
  }

  if ((res=setParams())<0)
    return ALSADebug().evaluateError(res);

  snd_pcm_format_t format;
  if ((res=getFormat(format))<0)
    return ALSADebug().evaluateError(res);
  printf("format %s\n",getFormatName(format));
  printf("channels %d\n",getChannels());
  snd_pcm_uframes_t pSize;
  if ((res=getPeriodSize(&pSize))<0)
    return ALSADebug().evaluateError(res);
  printf("period size %ld\n",pSize);
  printf("block count %d\n",M);
  printf("latency = %f s\n",(float)pSize/(float)FS);
  printf("Sample Rate %d Hz\n",FS);

  res=0;
  if (pSize!=N){
    printf("Wanted a period size of %d samples, but got %ld samples, error - aborting.\n", N, pSize);
    res=-1;
  }

  dumpState();
  return 0;
}

template class PlatterALSA<int, 48000>;
