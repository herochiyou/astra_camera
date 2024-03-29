#ifndef _ONI_SENSOR_H_
#define _ONI_SENSOR_H_

#include "OniCommon.h"
#include "OniDriverHandler.h"
#include "OniFrameManager.h"
#include <Driver/OniDriverTypes.h>
#include <XnErrorLogger.h>
#include <XnEvent.h>
#include <XnList.h>
#include <XnOSCpp.h>

ONI_NAMESPACE_IMPLEMENTATION_BEGIN

/** Contains stuff that is common to several streams created on the same sensor
 */
class Sensor : OniStreamServices {
  public:
  Sensor(xnl::ErrorLogger &errorLogger, FrameManager &frameManager,
         const DriverHandler &driverHandler);
  ~Sensor();

  void setDriverStream(void *streamHandle);
  OniStatus setFrameBufferAllocator(OniFrameAllocBufferCallback alloc,
                                    OniFrameFreeBufferCallback free,
                                    void *pCookie);
  void setRequiredFrameSize(int requiredFrameSize);

  xnl::Event1Arg<OniFrame *>::Interface &newFrameEvent() {
    return m_newFrameEvent;
  }
  void *streamHandle() const { return m_streamHandle; }

  int m_streamCount;
  int m_startedStreamCount;
  xnl::CriticalSection m_refCountCS;

  private:
  XN_DISABLE_COPY_AND_ASSIGN(Sensor)

  // stream services implementation
  int getDefaultRequiredFrameSize();
  OniFrame *acquireFrame();

  static int ONI_CALLBACK_TYPE
  getDefaultRequiredFrameSizeCallback(void *streamServices);
  static OniFrame *ONI_CALLBACK_TYPE acquireFrameCallback(void *streamServices);
  static void ONI_CALLBACK_TYPE releaseFrameCallback(void *streamServices,
                                                     OniFrame *pFrame);
  static void ONI_CALLBACK_TYPE addFrameRefCallback(void *streamServices,
                                                    OniFrame *pFrame);

  void resetFrameAllocator();

  // frame buffer management
  void *allocFrameBufferFromPool(int size);
  void releaseFrameBufferToPool(void *pBuffer);
  void releaseAllFrames();

  static void *ONI_CALLBACK_TYPE
  allocFrameBufferFromPoolCallback(int size, void *pCookie);
  static void ONI_CALLBACK_TYPE releaseFrameBufferToPoolCallback(void *pBuffer,
                                                                 void *pCookie);
  static void ONI_CALLBACK_TYPE freeFrameBufferMemoryCallback(void *pBuffer,
                                                              void *pCookie);
  static void ONI_CALLBACK_TYPE
  frameBackToPoolCallback(OniFrameInternal *pFrame, void *pCookie);

  static void ONI_CALLBACK_TYPE newFrameCallback(void *streamHandle,
                                                 OniFrame *pFrame,
                                                 void *pCookie);

  xnl::ErrorLogger &m_errorLogger;
  FrameManager &m_frameManager;
  const DriverHandler &m_driverHandler;

  xnl::Event1Arg<OniFrame *> m_newFrameEvent;
  void *m_streamHandle;

  int m_requiredFrameSize;

  // following members are for the frame buffer pool that is used by default
  xnl::CriticalSection m_framesCS;
  xnl::List<void *> m_allFrameBuffers;
  xnl::List<void *> m_availableFrameBuffers;
  xnl::List<OniFrameInternal *> m_currentStreamFrames;

  // following members point to current allocation functions
  OniFrameAllocBufferCallback m_allocFrameBufferCallback;
  OniFrameFreeBufferCallback m_freeFrameBufferCallback;
  void *m_frameBufferAllocatorCookie;
};

ONI_NAMESPACE_IMPLEMENTATION_END

#endif //_ONI_SENSOR_H_