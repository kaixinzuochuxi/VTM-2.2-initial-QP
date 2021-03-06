/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.
 *
 * Copyright (c) 2010-2018, ITU/ISO/IEC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  * Neither the name of the ITU/ISO/IEC nor the names of its contributors may
 *    be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

/** \file     Mv.cpp
    \brief    motion vector class
*/

#include "Mv.h"

#include "Common.h"
#include "Slice.h"

void roundMV( Mv & rMV, unsigned imvShift )
{
  CHECK( imvShift == 0, "roundMV called for imvShift=0" );
#if !REMOVE_MV_ADAPT_PREC
  if (rMV.highPrec) imvShift += VCEG_AZ07_MV_ADD_PRECISION_BIT_FOR_STORE;
#endif
  int offset = 1 << ( imvShift - 1 );

  rMV.setHor( ( ( rMV.getHor() + offset ) >> imvShift ) << imvShift );
  rMV.setVer( ( ( rMV.getVer() + offset ) >> imvShift ) << imvShift );
}

void roundAffineMv( int& mvx, int& mvy, int nShift )
{
  const int nOffset = 1 << (nShift - 1);
  mvx = mvx >= 0 ? (mvx + nOffset) >> nShift : -((-mvx + nOffset) >> nShift);
  mvy = mvy >= 0 ? (mvy + nOffset) >> nShift : -((-mvy + nOffset) >> nShift);
}

void clipMv( Mv& rcMv, const Position& pos, const SPS& sps )
{
#if !REMOVE_MV_ADAPT_PREC
  int iMvShift = 2 + (rcMv.highPrec ? VCEG_AZ07_MV_ADD_PRECISION_BIT_FOR_STORE : 0);
#else
  int iMvShift = 2;
#endif
#if REMOVE_MV_ADAPT_PREC
  iMvShift += VCEG_AZ07_MV_ADD_PRECISION_BIT_FOR_STORE;
#endif
  int iOffset = 8;
  int iHorMax = ( sps.getPicWidthInLumaSamples() + iOffset - ( int ) pos.x - 1 ) << iMvShift;
  int iHorMin = ( -( int ) sps.getMaxCUWidth()   - iOffset - ( int ) pos.x + 1 ) << iMvShift;

  int iVerMax = ( sps.getPicHeightInLumaSamples() + iOffset - ( int ) pos.y - 1 ) << iMvShift;
  int iVerMin = ( -( int ) sps.getMaxCUHeight()   - iOffset - ( int ) pos.y + 1 ) << iMvShift;

  rcMv.setHor( std::min( iHorMax, std::max( iHorMin, rcMv.getHor() ) ) );
  rcMv.setVer( std::min( iVerMax, std::max( iVerMin, rcMv.getVer() ) ) );
}

//! \}
