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

#include "PlatterAudio.H"

template<typename SAMPLE_TYPE, unsigned int FS>
PlatterAudio<SAMPLE_TYPE, FS>::PlatterAudio(){
  printf("PlatterAudio constructed\n");
}

template<typename SAMPLE_TYPE, unsigned int FS>
PlatterAudio<SAMPLE_TYPE, FS>::~PlatterAudio(){
  printf("PlatterAudio destructed\n");
}

template<typename SAMPLE_TYPE, unsigned int FS>
int PlatterAudio<SAMPLE_TYPE, FS>::loadFile(const std::string fn){
  int ret=audioFwd.loadFile(fn);
  if (ret<0)
    return ret;
  Eigen::Matrix<SAMPLE_TYPE, Eigen::Dynamic, Eigen::Dynamic> *aM=&audioRev;
  *aM=audioFwd.colwise().reverse();
  return 0;
}

template class PlatterAudio<int, 48000>;
