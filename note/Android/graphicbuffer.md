```c++
BufferQueueCore 
   ||
   ||  mQueue                     --  Fifo
   ||                                  typedef Vector<BufferItem> Fifo
   ||
   ||  mSlots                     --  BufferQueueDefs::SlotsType 
   ||                                  typedef BufferSlot SlotsType[NUM_BUFFER_SLOTS]
   ||
   ||  mConsumerListener          --  sp<IConsumerListener> 
   ||
   ||  mConnectedProducerListener --  sp<IProducerListener> 
   ||
   ||

   \\---------------------------------------
     ---------------------------------------
     BufferItem 
       ||
       ||  mGraphicBuffer sp<GraphicBuffer> 
       ||  mFence         sp<Fence> 
       ||  union { int mSlot; int mBuf;}
       ||  ...
     BufferSlot 
       ||
       ||  mGraphicBuffer sp<GraphicBuffer> 
       ||  mBufferState   BufferState 
       ||                 enum BufferState {FREE = 0, DEQUEUED = 1, QUEUED = 2, ACQUIRED = 3}
       ||  mEglFence      EGLSyncKHR 
       ||  mFence         sp<Fence> 
       ||  ...
    
     IConsumerListener : public ConsumerListener   
       ||
       ||  void onFrameAvailable(const BufferItem& item) = 0;
       ||  void onFrameReplaced(const BufferItem& /* item */) {}
       ||  void onBuffersReleased() = 0;
       ||  void onSidebandStreamChanged() = 0;
    
     IProducerListener : public ProducerListener
       ||
       ||  void onBufferReleased() = 0;

```

```c++
BufferQueueProducer :
   ||    public BnGraphicBufferProducer
   ||
   ||  mCore                     --  sp<BufferQueueCore> 
   ||  mSlots                    --  BufferQueueDefs::SlotsType& 
   ||
   ||  status_t requestBuffer(int slot, sp<GraphicBuffer>* buf)
   ||
   ||  status_t dequeueBuffer(int *outSlot, sp<Fence>* outFence,
   ||               bool async, uint32_t width, uint32_t height, PixelFormat format,
   ||               uint32_t usage)
   ||
   ||  status_t queueBuffer(int slot,
   ||               const QueueBufferInput& input, QueueBufferOutput* output)
   ||
   ||  status_t connect(const sp<IProducerListener>& listener,
   ||               int api, bool producerControlledByApp, QueueBufferOutput* output)
```


```c++
 BufferQueueConsumer :
   ||    public BnGraphicBufferConsumer
   ||
   ||  mCore                     --  sp<BufferQueueCore> 
   ||  mSlots                    --  BufferQueueDefs::SlotsType& 
   ||
   ||   status_t acquireBuffer(BufferItem* outBuffer,
   ||            nsecs_t expectedPresent, uint64_t maxFrameNumber = 0)
   ||
   ||   status_t releaseBuffer(int slot, uint64_t frameNumber,
   ||            const sp<Fence>& releaseFence, EGLDisplay display,
   ||            EGLSyncKHR fence)
   ||
   ||   status_t consumerConnect(const sp<IConsumerListener>& consumer,
   ||           bool controlledByApp)
```

```c++
 ConsumerBase :
   ||    public virtual RefBase,
   ||    protected ConsumerListener 
   ||
   ||  mFrameAvailableListener   --  wp<FrameAvailableListener> 
   ||  mConsumer                 --  sp<IGraphicBufferConsumer> 
   ||  mSlots                    --  Slot [BufferQueue::NUM_BUFFER_SLOTS]
   ||
   ||  void setFrameAvailableListener(const wp<FrameAvailableListener>& listener)
   ||
   || || 
   || ||Slot  
   || || 
   || ||  mGraphicBuffer         --  sp<GraphicBuffer> 
   || ||  mFence                 --  sp<Fence> 
   ||
   || || 
   || ||FrameAvailableListener 
   || ||
   || ||  void onFrameAvailable(const BufferItem& item) = 0 
   || ||  void onFrameReplaced(const BufferItem& /* item */) 
```
  ConsumerBase inherit ConsumerListener and register to consumer by calling consumerConnect() in 
  it's constructor; In general, it's a thin wrapper around target BufferQueueConsumer:
- 1. it provide some APIs similar to IGraphicBufferConsumer and implements them by call mConsumer
         APIs;
- 2. the callback from BufferQueueConsumer it notify outside by it's own mFrameAvailableListener;


```c++
 GLConsumer :
   ||    public ConsumerBase
   ||
   ||  mCurrentTextureImage      --  sp<EglImage> 
   ||  mTexName                  --  uint32_t 
   ||                                  Layer.mTextureName
   ||  mEglContext               --  EGLContext 
   ||  mEglSlots                 --  EglSlot [BufferQueue::NUM_BUFFER_SLOTS]
   ||
   ||  status_t updateTexImage()
   ||  status_t bindTextureImageLocked()
   ||
   || ||
   || ||EglImage 
   || ||
   || ||  mGraphicBuffer         --  sp<GraphicBuffer> 
   || ||  mEglImage              --  EGLImageKHR 
   || ||
   || ||  status_t createIfNeeded(EGLDisplay display,
   || ||                             const Rect& cropRect,
   || ||                             bool forceCreate = false)
   || ||
   || ||  void bindToTextureTarget(uint32_t texTarget)
   || ||
   ||
   || ||
   || ||EglSlot 
   || ||
   || ||  mEglImage              --  sp<EglImage> 
   || ||  mEglFence              --  EGLSyncKHR 
```

  GLConsumer help bind the target Layer.mTextureName to the current GraphicBuffer, which use in
  GLES composition mode; invoke procedure are similar to GLConsumer::updateTexImage():
- 1. call GLConsumer::acquireBufferLocked(), which acquire an BufferItem from consumer and instance
     an EglImage; the EglImage hold the reference of BufferItem.mGraphicBuffer and save mEglSlots
     for record;
- 2. call GLConsumer::updateAndReleaseLocked(), create an EGLImageKHR handle with the GraphicBuffer
     through eglCreateImageKHR(); also update GLConsumer current state from BufferItem field;
- 3. call GLConsumer::bindTextureImageLocked(), first use glBindTexture() to bind to the mTexName,
     then use glEGLImageTargetTexture2DOES() to bind to the EGLImageKHR handle; 

```c++
 SurfaceFlingerConsumer :
   ||    public GLConsumer
   ||
   ||  mContentsChangedListener  --  wp<ContentsChangedListener> 
   ||
   ||  void setContentsChangedListener(const wp<ContentsChangedListener>& listener)
   ||
   ||  status_t updateTexImage(BufferRejecter* rejecter, const DispSync& dispSync,
   ||           uint64_t maxFrameNumber = 0)
   || ||
   || ||ContentsChangedListener:
   || ||    public FrameAvailableListener
   || ||
   || ||  void onSidebandStreamChanged() = 0
```

  SurfaceFlingerConsumer() wrapper around GLConsumer, it provide ContentsChangedListener listener
  which notify onSidebandStreamChanged() to Layer; also provide updateTexImage() to update texture;

 **REFERRER**
   https://software.intel.com/en-us/articles/using-opengl-es-to-accelerate-apps-with-legacy-2d-guis

## MISC
- 1. 7.0 add gralloc1.h which provide APIs like 
     gralloc1_function_pointer_t (*getFunction)(struct gralloc1_device* device,
             int32_t /*gralloc1_function_descriptor_t*/ descriptor);
   in struct gralloc1_device_t; user get symbol address through enum gralloc1_function_descriptor_t.

- 2. 7.0 still use the old gralloc implementation in HAL, framework has adapter to hook different 
   platform implementation;
   2.1 GraphicBuffer do the work through two singleton class GraphicBufferAllocator and
       GraphicBufferMapper;
   2.2 the two class both hold reference of Loader and Device; Loader get HAL module version and 
       decide to use adapter or not, which means get gralloc1_device_t pointer in different way;
       the Device hold a gralloc1_device_t pointer; the two class call APIs of Device, Device then 
       call the corresponding function through gralloc1_device_t pointer;
   2.3 when platform support gralloc1 APIs, then gralloc1_device_t is just the real pointer return 
       from gralloc1_open();
   2.4 when platform don't support gralloc1 APIs, then gralloc1_device_t is the reference of class 
       Gralloc1On0Adapter, which hook gralloc1 with gralloc APIs, the gralloc implementation is 
       return from gralloc_open();


- 3. GraphicBuffer allocate & lock common logic
   
- 4. gralloc implementation in qcom is in libgralloc.

GRALLOC_HARDWARE_FB0
GRALLOC_HARDWARE_GPU0
