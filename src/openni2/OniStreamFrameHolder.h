/*****************************************************************************
 *                                                                            *
 *  OpenNI 2.x Alpha                                                          *
 *  Copyright (C) 2012 PrimeSense Ltd.                                        *
 *                                                                            *
 *  This file is part of OpenNI.                                              *
 *                                                                            *
 *  Licensed under the Apache License, Version 2.0 (the "License");           *
 *  you may not use this file except in compliance with the License.          *
 *  You may obtain a copy of the License at                                   *
 *                                                                            *
 *      http://www.apache.org/licenses/LICENSE-2.0                            *
 *                                                                            *
 *  Unless required by applicable law or agreed to in writing, software       *
 *  distributed under the License is distributed on an "AS IS" BASIS,         *
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
 *  See the License for the specific language governing permissions and       *
 *  limitations under the License.                                            *
 *                                                                            *
 *****************************************************************************/
#ifndef _ONI_IMPL_STREAM_FRAME_HOLDER_H_
#define _ONI_IMPL_STREAM_FRAME_HOLDER_H_

#include "OniCommon.h"
#include "OniFrameHolder.h"

ONI_NAMESPACE_IMPLEMENTATION_BEGIN

class VideoStream;

class StreamFrameHolder : public FrameHolder {
  public:
  // Constructor.
  StreamFrameHolder(FrameManager &frameManager, VideoStream *pStream);

  // Destructor.
  virtual ~StreamFrameHolder();

  // Get the next frame belonging to a stream.
  virtual OniStatus readFrame(VideoStream *pStream, OniFrame **pFrame);

  // Process a newly received frame.
  virtual OniStatus processNewFrame(VideoStream *pStream, OniFrame *pFrame);

  // Peek at next frame.
  virtual OniFrame *peekFrame(VideoStream *pStream);

  // Clear all the frame in the holder.
  virtual void clear();

  // Return list of streams which are members of the stream group.
  virtual void getStreams(VideoStream **ppStreams, int *pNumStreams);

  // Return number of streams which are members of the stream group.
  virtual int getNumStreams();

  //
  virtual void setStreamEnabled(VideoStream * /*pStream*/, OniBool /*enabled*/);

  private:
  VideoStream *m_pStream;

  OniFrame *m_pLastFrame;
};

ONI_NAMESPACE_IMPLEMENTATION_END

#endif // _ONI_IMPL_STREAM_FRAME_HOLDER_H_
