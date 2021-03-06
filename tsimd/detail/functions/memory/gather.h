// ========================================================================== //
// The MIT License (MIT)                                                      //
//                                                                            //
// Copyright (c) 2017 Intel Corporation                                       //
//                                                                            //
// Permission is hereby granted, free of charge, to any person obtaining a    //
// copy of this software and associated documentation files (the "Software"), //
// to deal in the Software without restriction, including without limitation  //
// the rights to use, copy, modify, merge, publish, distribute, sublicense,   //
// and/or sell copies of the Software, and to permit persons to whom the      //
// Software is furnished to do so, subject to the following conditions:       //
//                                                                            //
// The above copyright notice and this permission notice shall be included in //
// in all copies or substantial portions of the Software.                     //
//                                                                            //
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR //
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   //
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL    //
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER //
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING    //
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER        //
// DEALINGS IN THE SOFTWARE.                                                  //
// ========================================================================== //

#pragma once

#include "../../pack.h"

namespace tsimd {

  template <typename PACK_T, typename OFFSET_T>
  TSIMD_INLINE PACK_T gather(void *_src,
                             const pack<OFFSET_T, PACK_T::static_size> &o)
  {
    auto *src = (const typename PACK_T::value_t *)_src;
    PACK_T result;

#if TSIMD_USE_OPENMP
#  pragma omp simd
#endif
    for (int i = 0; i < PACK_T::static_size; ++i)
      result[i] = src[o[i]];

    return result;
  }

  template <typename PACK_T, typename OFFSET_T>
  TSIMD_INLINE PACK_T
  gather(void *_src,
         const pack<OFFSET_T, PACK_T::static_size> &o,
         const mask<typename PACK_T::value_t, PACK_T::static_size> &m)
  {
    auto *src = (const typename PACK_T::value_t *)_src;
    PACK_T result;

#if TSIMD_USE_OPENMP
#  pragma omp simd
#endif
    for (int i = 0; i < PACK_T::static_size; ++i)
      if (m[i])
        result[i] = src[o[i]];

    return result;
  }

}  // namespace tsimd
