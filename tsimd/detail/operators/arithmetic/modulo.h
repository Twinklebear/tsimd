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

  template <typename T, int W>
  TSIMD_INLINE pack<T, W> operator%(const pack<T, W> &p1, const pack<T, W> &p2)
  {
    pack<T, W> result;

#if TSIMD_USE_OPENMP
#  pragma omp simd
#endif
    for (int i = 0; i < W; ++i)
      result[i] = (p1[i] % p2[i]);

    return result;
  }

  // Inferred pack<>/scalar operators /////////////////////////////////////////

  template <typename T,
            int W,
            typename OTHER_T,
            typename = traits::can_convert<OTHER_T, T>>
  TSIMD_INLINE pack<T, W> operator%(const pack<T, W> &p1, const OTHER_T &v)
  {
    return p1 % pack<T, W>(v);
  }

  template <typename T,
            int W,
            typename OTHER_T,
            typename = traits::can_convert<OTHER_T, T>>
  TSIMD_INLINE pack<T, W> operator%(const OTHER_T &v, const pack<T, W> &p1)
  {
    return pack<T, W>(v) % p1;
  }

  // Inferred binary operator%=() /////////////////////////////////////////////

  template <typename T, int W>
  TSIMD_INLINE pack<T, W> &operator%=(pack<T, W> &p1, const pack<T, W> &p2)
  {
    return p1 = (p1 % p2);
  }

  template <typename T,
            int W,
            typename OTHER_T,
            typename = traits::can_convert<OTHER_T, T>>
  TSIMD_INLINE pack<T, W> &operator%=(pack<T, W> &p1, const OTHER_T &v)
  {
    return p1 = (p1 % pack<T, W>(v));
  }

}  // namespace tsimd