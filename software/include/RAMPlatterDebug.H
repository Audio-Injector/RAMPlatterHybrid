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

#ifndef RAMPLATTERDEBUG_H_
#define RAMPLATTERDEBUG_H_

#include <Debug.H>
#include <string>

#define RAMPLATTER_BYTEDEPTH_ERROR MIN_ERROR_OFFSET-1
#define RAMPLATTER_AUDIOEMPTY_ERROR MIN_ERROR_OFFSET-2
#define RAMPLATTER_FS_ERROR MIN_ERROR_OFFSET-3
#define RAMPLATTER_BLOCKSIZE_ERROR MIN_ERROR_OFFSET-4
#define RAMPLATTER_ALSA_STATE_ERROR MIN_ERROR_OFFSET-5

/** Class to handle errors
*/
class RAMPlatterDebug : virtual public Debug {
public:
  /** Constructor
  */
  RAMPlatterDebug(){
    #ifndef NDEBUG
    errors[RAMPLATTER_BYTEDEPTH_ERROR]=std::string("The word byte depth is incorrect.");
    errors[RAMPLATTER_AUDIOEMPTY_ERROR]=std::string("The audio hasn't been loaded yet, load audio first.");
    errors[RAMPLATTER_FS_ERROR]=std::string("The audio hardware's sample rate is incorrect.");
    errors[RAMPLATTER_BLOCKSIZE_ERROR]=std::string("The ALSA block size is incorrect.");
    errors[RAMPLATTER_ALSA_STATE_ERROR]=std::string("The ALSA system is in the wrong state.");
    #endif
  }

  /** Destructor
  */
  virtual ~RAMPlatterDebug(){}
};

#endif // RAMPLATTERDEBUG_H_
