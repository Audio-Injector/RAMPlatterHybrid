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

#ifndef PLATTERAUDIO_H_
#define PLATTERAUDIO_H_

#include "Audio.H"

/** Class to hold and manage audio.
Inherits an Eigen matrix, where columns are channels and rows are frames (samples).
\param SAMPLE_TYPE The type of the audio samples
\param FS The hard set sample rate of the audio in Hz
*/
template<typename SAMPLE_TYPE, unsigned int FS>
class PlatterAudio {
  Audio<SAMPLE_TYPE> audioFwd; ///< The forward playing audio
  Audio<SAMPLE_TYPE> audioRev; ///< The backward playing audio
public:
  /** Constructor
  */
  PlatterAudio();

  /** Destructor
  */
  virtual ~PlatterAudio();

  /** Given a file name, attempt to load ALL of the audio to RAM.
  Forwards version to the audioFwd, revers version to the audioRev
  \param fn The file to load
  \returns 0 on success, otherwise error
  */
  int loadFile(const std::string fn);
};
#endif // PLATTERAUDIO_H_
