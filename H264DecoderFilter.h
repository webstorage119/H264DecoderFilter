/** @file

MODULE:
 
TAG: 

FILE NAME: 

DESCRIPTION: 

COPYRIGHT: (c)CSIR 2007-2018 all rights reserved

LICENSE: Software License Agreement (BSD License)

RESTRICTIONS: 
Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this 
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
this list of conditions and the following disclaimer in the documentation and/or 
other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may 
be used to endorse or promote products derived from this software without specific 
prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
OF THE POSSIBILITY OF SUCH DAMAGE.
===========================================================================

*/
#pragma once
#include <DirectShowExt/CustomBaseFilter.h>
#include <DirectShowExt/DirectShowMediaFormats.h>
#include <DirectShowExt/NotifyCodes.h>
#include "VersionInfo.h"

// {4F722923-7E4D-4263-926D-2A1F405B2619}
static const GUID CLSID_VPP_H264Decoder = 
{ 0x4f722923, 0x7e4d, 0x4263, { 0x92, 0x6d, 0x2a, 0x1f, 0x40, 0x5b, 0x26, 0x19 } };

// Forward
class ICodecv2;

class H264DecoderFilter : public CCustomBaseFilter
{
public:
  /// Constructor
  H264DecoderFilter();
  /// Destructor
  ~H264DecoderFilter();

  /// Static object-creation method (for the class factory)
  static CUnknown * WINAPI CreateInstance(LPUNKNOWN pUnk, HRESULT *pHr); 

  /**
  * Overriding this so that we can set whether this is an RGB24 or an RGB32 Filter
  */
  HRESULT SetMediaType(PIN_DIRECTION direction, const CMediaType *pmt);

  /**
  * Used for Media Type Negotiation 
  * Returns an HRESULT value. Possible values include those shown in the following table.
  * <table border="0" cols="2"><tr valign="top"><td><b>Value</b></td><td><b>Description</b></td></TR><TR><TD>S_OK</TD><TD>Success</TD></TR><TR><TD>VFW_S_NO_MORE_ITEMS</TD><TD>Index out of range</TD></TR><TR><TD>E_INVALIDARG</TD><TD>Index less than zero</TD></TR></TABLE>
  * The output pin's CTransformOutputPin::GetMediaType method calls this method. The derived class must implement this method. For more information, see CBasePin::GetMediaType.
  * To use custom media types, the media type can be manipulated in this method.
  */
  HRESULT GetMediaType(int iPosition, CMediaType *pMediaType);

  /// Buffer Allocation
  /**
  * The output pin's CTransformOutputPin::DecideBufferSize method calls this method. The derived class must implement this method. For more information, see CBaseOutputPin::DecideBufferSize. 
  * @param pAlloc Pointer to the IMemAllocator interface on the output pin's allocator.
  * @param pProp Pointer to an ALLOCATOR_PROPERTIES structure that contains buffer requirements from the downstream input pin.
  * @return Value: Returns S_OK or another HRESULT value.
  */
  HRESULT DecideBufferSize(IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *pProp);

  /**
  * The CheckTransform method checks whether an input media type is compatible with an output media type.
  * <table border="0" cols="2"> <tr valign="top"> <td  width="50%"><b>Value</b></td> <td width="50%"><b>Description</b></td> </tr> <tr valign="top"> <td width="50%">S_OK</td> <td width="50%">The media types are compatible.</td> </tr> <tr valign="top"> <td width="50%">VFW_E_TYPE_NOT_ACCEPTED</td> <td width="50%">The media types are not compatible.</td> </tr> </table>
  */
  HRESULT CheckTransform(const CMediaType *mtIn, const CMediaType *mtOut);

  virtual void doGetVersion(std::string& sVersion)
  {
    sVersion = VersionInfo::toString();
  }
  ///Overridden from CSettingsInterface
  virtual void initParameters(){;}
  ///Overridden from CSettingsInterface
  STDMETHODIMP GetParameter( const char* type, int* length, void* value );
  ///Overridden from CSettingsInterface
  STDMETHODIMP SetParameter( const char* type, const char* value);


  /// Overridden from CCustomBaseFilter
  virtual void InitialiseInputTypes();

private:
  /**
  * This method decodes the frame from H.264 to RGB24
  * @param[in] pBufferIn The source buffer
  * @param[in] lInBufferSize The size of the in buffer
  * @param[in] lActualDataLength The size of the actual frame inside the in buffer
  * @param[in,out] pBufferOut The destination buffer
  * @param[in] lOutBufferSize The size of the destination buffer
  * @param[in,out] lOutActualDataLength The size of the data encoded into the destination buffer
  */
  virtual HRESULT ApplyTransform(BYTE* pBufferIn, long lInBufferSize, long lActualDataLength, BYTE* pBufferOut, long lOutBufferSize, long& lOutActualDataLength);

  HRESULT initialiseH264Decoder();
  void resizeEncodedPictureBufferIfNecessary(long lActualDataLength);

  ICodecv2* m_pCodec;

  unsigned char* m_pSeqParamSet;
  unsigned m_uiSeqParamSetLen;
  unsigned char* m_pPicParamSet;
  unsigned m_uiPicParamSetLen;

  // only set if media subtype == MEDIASUBTYPE_AVC1
  // and stores length of length field preceding H264 data
  unsigned m_uiAvc1PrefixLength;
  // buffer in case we have to copy the incoming data
  BYTE* m_pEncodedPictureBuffer;
  unsigned m_uiEncodedPictureBufferLength;
};
