```c++
 SurfaceFlinger : public BnSurfaceComposer,
   ||             private IBinder::DeathRecipient,
   ||             private HWComposer::EventHandler
   ||
   ||
   ||  mHwc                      --  HWComposer *
   ||  mRenderEngine             --  RenderEngine *
   ||  mPrimaryDispSync          --  DispSync
   ||  mEventQueue               --  MessageQueue
   ||  mDisplays                 --  DefaultKeyedVector< wp<IBinder>, sp<DisplayDevice> >
   ||  mDrawingState             --  State
   ||  mCurrentState             --  State
   ||
   ||  sp<IDisplayEventConnection> createDisplayEventConnection()
   ||  sp<ISurfaceComposerClient>  createConnection()
   ||  sp<IBinder> createDisplay(const String8& displayName, bool secure)
   ||
   ||  status_t createLayer(const String8& name, const sp<Client>& client,
   ||            uint32_t w, uint32_t h, PixelFormat format, uint32_t flags,
   ||            sp<IBinder>* handle, sp<IGraphicBufferProducer>* gbp);
   ||
   ||  void setTransactionState(
   ||           const Vector<ComposerState>& state,
   ||           const Vector<DisplayState>& displays,
   ||           uint32_t flags)
   ||
   ||
   || ||
   || ||DispSyncSource : public VSyncSource,
   || ||                 private DispSync::Callback
   || ||
   || ||  mDispSync              --  DispSync*
   || ||  mCallback              --  sp<VSyncSource::Callback>
   || ||  mPhaseOffset           --  nsecs_t
   || ||
   || ||
   ||
   || ||
   || ||DisplayDeviceState
   || ||
   || ||  surface                --  sp<IGraphicBufferProducer>
   || ||  layerStack             --  uint32_t
   || ||  orientation            --  uint8_t
   || ||  ...
   ||
   || ||
   || ||State
   || ||
   || ||  layersSortedByZ        --  LayerVector
   || ||  displays               --  DefaultKeyedVector< wp<IBinder>, DisplayDeviceState>
   \\---------------------------------------
     ---------------------------------------
     DisplayState
       ||
       ||  uint32_t                     what
       ||  sp<IBinder>                  token
       ||  sp<IGraphicBufferProducer>   surface
       ||  uint32_t                     layerStack
       ||  uint32_t                     orientation
       ||  ...

     layer_state_t
       ||
       ||  sp<IBinder>     surface
       ||  float           x
       ||  float           y
       ||  uint32_t        z
       ||  uint32_t        w
       ||  uint32_t        h
       ||  uint32_t        layerStack
       ||  float           alpha
       ||  Rect            crop
       ||  ...

     ComposerState
       ||
       ||  layer_state_t state
       ||  ...
```

```c++
 Client : public BnSurfaceComposerClient
   ||
   ||
   ||  mLayers                   --  DefaultKeyedVector< wp<IBinder>, wp<Layer>
   ||
   ||  status_t createSurface(
   ||            const String8& name,
   ||            uint32_t w, uint32_t h,PixelFormat format, uint32_t flags,
   ||            sp<IBinder>* handle,
   ||            sp<IGraphicBufferProducer>* gbp)
   ||

 Layer
   ||
   ||  mCurrentState             --  State
   ||  mDrawingState             --  State
   ||  mActiveBuffer             --  sp<GraphicBuffer>
   ||  mSurfaceFlingerConsumer   --  sp<SurfaceFlingerConsumer>
   ||  mTextureName              --  uint32_t
   ||                                  glGenTextures() return
   ||
   ||  bool setPosition(float x, float y)
   ||  bool setLayer(uint32_t z)
   ||  bool setSize(uint32_t w, uint32_t h)
   ||
   ||
   || ||
   || ||State
   || ||
   || ||  z                      --  uint32_t
   || ||  layerStack             --  uint32_t
   || ||  alpha                  --  uint8_t
   || ||  flags                  --  uint8_t
   || ||  ...
```

  To communicate with SurfaceFlinger, APP process use SurfaceComposerClient to wrapper around the
  APIs of ISurfaceComposer and ISurfaceComposerClient, which get handle through func getService()
  and SF::createConnection() respectively;

  APP process create surface through Client::createSurface(), which will instance a Layer and return
  the GraphicBufferProducer handle to APP side;
- 1 For SF, the Layer reference will record in Client.mLayers and SF.mCurrentState.layersSortedByZ;
  with the consumer that record in Layer.mSurfaceFlingerConsumer, SF can get the available buffer
  and do the composition;

- 2 For APP, with the producer reference there are two important class will be create:
    - 2.1 SurfaceControl is create after SurfaceComposerClient::createSurface() return, the reference
      is record in SurfaceControl.mNativeObject; SurfaceControl provide APIs to control the layer's
      attribute and state;

  The API SF provide to set layer's attrs and display device's attrs is setTransactionState(), two
  vector of struct ComposerState and struct DisplayState as parameter; these two structs defined the
  possible attributes of layer and displsy device, so APP side can pass the attributes that user set
  to SF side;

  ComposerState.state.z, state.layerStack, state.alpha, etc are set from WindowStateAnimator through
  SurfaceControl's corresponding APIs; SF::setClientStateLocked() handle and parse the struct, the
  position, size, alpha, etc attributes will update to Layer.mCurrentState's field through Layer's
  corresponding APIs; when come to z and layerStack, except update Layer's field, SF.mCurrentState.
  layersSortedByZ will also be update;

  DisplayState.surface and layerStack are set from DMS.performTraversalInTransactionLocked(), by
  calling SurfaceControl.setDisplaySurface() and setDisplayLayerStack() respectively; In SF side,
  SF::setDisplayStateLocked() will handle DisplayState changes, which parse field from DisplayState
  and assign corresponding field in SF.mCurrentState.displays on by one;

    - 2.2 Surface is create in WMS.relayoutWindow() by calling Surface.copyFrom(SurfaceControl other),
Surface in native layer also hold the procedure reference; native Surface provide APIs that
inherit from ANativeWindow, implement them by calling GraphicBufferProducer's APIs;

```c++
 DispSync
   ||
   ||  mResyncSamples            --  nsecs_t []
   ||  mPeriod                   --  nsecs_t
   ||  mPhase                    --  nsecs_t
   ||  mError                    --  nsecs_t
   ||  mThread                   --  sp<DispSyncThread>
   ||
   ||  mThread                   --  sp<DispSyncThread>
   ||
   ||  status_t addEventListener(nsecs_t phase, const sp<Callback>& callback)
   ||
```

```c++
 MessageQueue
   ||
   ||  mLooper                   --  sp<Looper>
   ||  mEvents                   --  sp<IDisplayEventConnection>
   ||  mEventTube                --  sp<BitTube>
   ||  mFlinger                  --  sp<SurfaceFlinger>
   ||
   ||
```

   MessageQueue notify SurfaceFlinger through SF.onMessageReceived()

```c++
 EventThread : public Thread,
   ||          private VSyncSource::Callback
   ||
   ||  mDisplayEventConnections   --  SortedVector< wp<Connection> >
   ||
   ||
   || ||
   || ||Connection : public BnDisplayEventConnection
   || ||
   || ||
   || ||  count                  --  int32_t
   || ||  mEventThread           --  sp<EventThread> const
   || ||  mChannel               --  sp<BitTube> const
   || ||
   || ||  status_t postEvent(const DisplayEventReceiver::Event& event)
   || ||
```

```c++
 DisplayDevice
   ||
   ||  mNativeWindow             --  sp<ANativeWindow>
   ||  mSurface                  --  EGLSurface
   ||  mDisplaySurface           --  sp<DisplaySurface>
   ||  mLayerStack               --  uint32_t
   ||  mType                     --  DisplayType
   ||  mVisibleLayersSortedByZ   --  Vector< sp<Layer> >
   ||
   ||  EGLBoolean makeCurrent(EGLDisplay dpy, EGLContext ctx)
   ||  void swapBuffers(HWComposer& hwc)
   ||
```

```c++
 FramebufferSurface :
   ||    public ConsumerBase,
   ||    public DisplaySurface
   ||
   ||  void onFrameAvailable(const BufferItem& item)
   ||
   ||
```

  SurfaceFlinger save the DisplayDevice reference for every display device in SF.mDisplays; every
  DisplayDevice hold a EGLSurface handle and a DisplaySurface handle:
- 1 when the display device need composite with GLES, through DisplayDevice.makeCurrent(), mSurface
  can set as the current GLES context's window buffer; then the layers which has HWC_FRAMEBUFFER
  composite type will draw it's current display buffer into the mSurface as a texture; after draw
  every layers, SF will invoke DisplayDevice.swapBuffers(), it call eglSwapBuffers() which handle
  the completed window buffer to correspond consumer;

- 2 mDisplaySurface is a reference that implements the DisplaySurface's APIs, DisplayDevice will
  notify it the different composite stage through these APIs;

  when DisplayDevice.mType is DISPLAY_PRIMARY/DISPLAY_EXTERNAL, the DisplayDevice.mSurface is return
  from eglCreateWindowSurface(), correspond native window is the GraphicBufferProducer which created
  along with instance DisplayDevice; mDisplaySurface is the reference of FramebufferSurface, besides
  implements DisplaySurface's APIs, it's also the consumer of DisplayDevice.mNativeWindow; So, after
  eglSwapBuffers(), FramebufferSurface.onFrameAvailable() will be callback, it then call HWComposer
  ::fbPost(), HWComposer put the completed buffer in HWC_FRAMEBUFFER_TARGET layer and let platform
  composite it with other layers like HWC_OVERLAY;

```c++
 VirtualDisplaySurface :
   ||    public DisplaySurface,
   ||    public BnGraphicBufferProducer,
   ||    private ConsumerBase
   ||
   ||  mSource                   --  sp<IGraphicBufferProducer> [2]
   ||
   ||                                mSource[SOURCE_SINK]    // producer of remote display
   ||                                mSource[SOURCE_SCRATCH] // producer of frame buffer
   ||                                                        // (created along with DisplayDevice)
   ||
   ||  mFbProducerSlot           --  int
   ||  mOutputProducerSlot       --  int
   ||  mCompositionType          --  CompositionType
   ||
   ||  status_t dequeueBuffer(int* pslot, sp<Fence>* fence, bool async,
   ||               uint32_t w, uint32_t h, PixelFormat format, uint32_t usage)
   ||
   ||  status_t queueBuffer(int pslot,
   ||               const QueueBufferInput& input, QueueBufferOutput* output)
   ||
   ||  status_t beginFrame(bool mustRecompose)
   ||  status_t prepareFrame(CompositionType compositionType)
   ||  status_t advanceFrame()
   ||  status_t compositionComplete()
   ||  void onFrameCommitted()
   ||
```

  when DisplayDevice.mType is DISPLAY_VIRTUAL, DisplayDevice.mNativeWindow and mDisplaySurface are
  both the reference of VirtualDisplaySurface, VirtualDisplaySurface also hold a producer that the
  correspond consumer is running in another process, the consumer transport the complete buffer to
  a virtual display device;

  VirtualDisplaySurface manages most of the buffer issues, when layers need draw into correspond
  DisplayDevice.mSurface as texture, egl dequeue/queue buffer will go through it's API and it will
  choose to get the buffer from SOURCE_SCRATCH or SOURCE_SINK according to VDS.mCompositionType;
  since VDS is also the consumer of producer SOURCE_SCRATCH, so when DisplayDevice.mSurface queue
  a new buffer, it can acquire the buffer from GBC directly; VDS also set the hdc_1_t.outbuf by
  calling HWComposer::setOutputBuffer() and let platfrom fill the completed buffer; mFbProducerSlot
  record the GLES composite buffer, VDS will set it to the HWC_FRAMEBUFFER_TARGET layer by calling
  HWComposer::fbPost();

  From one frame composte begin to it complete, SF will notify different stages to DisplayDevice,
  then DisplayDevice notify to mDisplaySurface; the notify sequence and SF stages are:

- 1 beginFrame()
  Notify when SurfaceFlinger::setUpHWComposer(), before everything begin;
  VDS will dequeue buffer from SOURCE_SINK and record in VDS.mOutputProducerSlot;
  VDS also set mOutputProducerSlot as output buffer through HWComposer::setOutputBuffer();

- 2 prepareFrame()
  Notify when SurfaceFlinger::setUpHWComposer(), after HWComposer::prepare() finish;
  VDS will update it's own mCompositionType field;

- 3 advanceFrame()
  Notify when SurfaceFlinger::doComposition(), end of SurfaceFlinger::doDisplayComposition(), it
  means GLES composition is complete for this frame;
  VDS call HWComposer::setOutputBuffer() with valid fence and call HWComposer::fbPost();

- 4 compositionComplete()
  Notify when SurfaceFlinger::doComposition(), after SurfaceFlinger::doDisplayComposition();
  VDS has empty implements;

- 5 onFrameCommitted()
  Notify when SurfaceFlinger::doComposition(), call in SurfaceFlinger::postFramebuffer(), after
  HWComposer::commit() finish;
  VDS queue mOutputProducerSlot buffer to SOURCE_SINK producer;

  According to different VDS.mCompositionType, VirtualDisplaySurface handles accordingly;

- 1 GLES-only composition:
  when invoke VDS::dequeueBuffer(), VDS return mOutputProducerSlot that dequeued from SOURCE_SINK
  in beginFrame();
  when invoke VDS::queueBuffer(), VDS hold it in mFbProducerSlot, it's actually the same buffer
  as mOutputProducerSlot;
  when invoke VDS::onFrameCommitted(), VDS queue mOutputProducerSlot to SOURCE_SINK;

- 2 HWC-only composition
  beginFrame() dequeue from SOURCE_SINK and pass to HWC in advanceFrame(); after composition is
  complete, the buffer is queued to the sink in onFrameCommitted();

- 3 MIXED composition:
  when invoke VDS::dequeueBuffer(), VDS dequeue and return a new buffer from SOURCE_SCRATCH;
  when invoke VDS::queueBuffer(), VDS queued the GLES complete buffer to SOURCE_SCRATCH and
  immediately acquired from consumer, mFbProducerSlot also record it;
  when invoke VDS::onFrameCommitted(), VDS queue mOutputProducerSlot to SOURCE_SINK, and release
  mFbProducerSlot, cause this time it's another buffer from SOURCE_SCRATCH;

```c++
 HWComposer
   ||
   ||  mFbDev                     --  framebuffer_device_t*
   ||  mHwc                       --  struct hwc_composer_device_1*
   ||  mLists                     --  struct hwc_display_contents_1* [MAX_HWC_DISPLAYS]
   ||  mDisplayData               --  DisplayData [MAX_HWC_DISPLAYS]
   ||
   ||  status_t prepare()
   ||  status_t commit()
   ||  status_t createWorkList(int32_t id, size_t numLayers)
   ||  void invalidate()
   ||  void vsync(int disp, int64_t timestamp)
   ||  void hotplug(int disp, int connected)
   ||
   ||  status_t setOutputBuffer(int32_t id, const sp<Fence>& acquireFence,
   ||           const sp<GraphicBuffer>& buf)
   ||
   ||  int fbPost(int32_t id, const sp<Fence>& acquireFence,
   ||           const sp<GraphicBuffer>& buf)
   ||
   ||
   || ||
   || ||DisplayData
   || ||
   || ||  list                   --  hwc_display_contents_1*
   || ||  framebufferTarget      --  hwc_layer_1*
   || ||  fbTargetHandle         --  buffer_handle_t
   || ||  lastRetireFence        --  sp<Fence>
   || ||  lastDisplayFence       --  sp<Fence>
   || ||  outbufHandle           --  buffer_handle_t
   || ||  outbufAcquireFence     --  sp<Fence>
   || ||
   ||
   || ||
   || ||HWCLayerInterface
   || ||
   || ||  void setBlending(uint32_t blending) = 0
   || ||  void setBuffer(const sp<GraphicBuffer>& buffer) = 0
   || ||  void setTransform(uint32_t transform) = 0
   || ||  ...
   ||
   || ||
   || ||HWCLayer : public HWCLayerInterface
   || ||
   || ||  status_t setLayer(size_t index) = 0
   \\---------------------------------------
     ---------------------------------------
     template<typename CONCRETE, typename HWCTYPE>
     Iterable : public HWComposer::HWCLayer
       ||
       ||  HWCTYPE* const mLayerList
       ||  HWCTYPE* mCurrentLayer
       ||
       ||  status_t setLayer(size_t index)
       ||  HWCTYPE* getLayer()
       ||

     HWCLayerVersion1 :
       ||    public Iterable<HWCLayerVersion1, hwc_layer_1_t>
       ||
       ||  void setTransform(uint32_t transform)
       ||  void setBlending(uint32_t blending)
       ||  void setCrop(const FloatRect& crop)
       ||  ...
```

  HWComposer use HWCLayerInterface to abstract HAL's hwc_layer_1_t functionality; HWCLayer interface
  use to manage list of hwc_layer_1_t; template class Iterable implements the iterable part of
  HWCLayer, while HWCLayerVersion1 implements the HWCLayerInterface's APIs;


##  SurfaceFlinger VSYNC part

  From Android4.4, DispSync is add to maintains a model of the periodic hardware-based VSYNC events
  of a display and uses that model to execute periodic callbacks at specific phase offsets from the
  hardware VSYNC events;

  DispSync handle two kind input data:

- 1 VSYNC event timestamp given by DispSync.addResyncSample(): when HWComposer get vsync()
         callback will invoke SurfaceFlinger.onVSyncReceived(), if SF.mPrimaryHWVsyncEnabled is true,
         addResyncSample() will be called;
- 2 DispSync also accept feedback of the last display frame's hdc_1_t.retireFenceFd signal
      timestamp, which signalled by platform Hardware Composer after specific frame is displayed
      on the panel; it's given by DispSync.addPresentFence() from SF.postComposition(). 

  Everytime a new frame is post to composition, addPresentFence() will adjust DispSync's internal
  stats by updateErrorLocked() and decide a new VSYNC event is needed; what DispSync is trying to
  do is guarantee the real display period get from retireFenceFd and the post composition period's
  phase offset always less then kErrorThreshold's squared; when DispSync.mError exceed this value,
  SurfaceFlinger.mPrimaryHWVsyncEnabled will become true and a new hardware-based VSYNC event will
  add into DispSync by addResyncSample().

  DispSync provide addEventListener() to accept listeners register DispSync.Callback, it fire
  callback in it's internal thread DispSyncThread. Since post vsync event to listeners is through
  EventThread, so DispSyncSource is responsible to connect with DispSync and EventThread

- 1 DispSyncSource register callback to DispSync through DispSync.addEventListener();
- 2 EventThread register callback to DispSyncSource through VSyncSource.setCallback();

  when DispSyncSource get callback from DispSync in DispSyncSource.onDispSyncEvent(), it forward it
  to EventThread through VSyncSource.Callback.onVSyncEvent();

  After get vsync timestamp, EventThread post to EventThread.mDisplayEventConnections in it's own
  thread runtime funntion EventThread.threadLoop(); mDisplayEventConnections is the vector of the
  instance EventThread.Connection, listeners should create a Connection handle by calling function
  EventThread.createEventConnection(), then get the receive fd of Connection.mChannel, which it's
  a receive fd from socketpair(), listeners select/poll the receive fd to get the vsync timestamp
  from EventThread; listeners can also control the vsync post type:

- 1  when set Connection.count >=1 by calling Connection.setVsyncRate(), EventThread will post
         continuous event;
- 2 when call requestNextVsync() everytime, EventThread will post one-shot event.

  The dynamic adjusted vsync event from DispSync will finally callback to two consumers:
  SurfaceFlinger and Choreographer;

- 1 SurfaceFlinger:
  SurfaceFlinger register a Connection in it's own message looper instance MessageQueue,
  and poll the receive fd; then MessageQueue just notify the vsync event by SurfaceFlinger.
  onMessageReceived(), SF then begin to composition;

- 2 Choreographer:
  APP side use Choreographer to coordinates the timing of view drawing. DisplayEventReceiver
  register a connection through SF.createDisplayEventConnection(), NativeDisplayEventReceiver
  get the receive fd from Connection and poll the vsync event; vsync event then callback to
  java environment, by calling Java|DisplayEventReceiver.dispatchVsync(), then Choreographer.
  FrameDisplayEventReceiver, which extends DisplayEventReceiver get the vsync event in func
  onVsync(). Next, Choreographer callback the ViewRootImpl to let traversal and draw views;

  By default, on a VSYNC event, the display begins showing frame N while SurfaceFlinger begins
  compositing windows for frame N+1; the app handles pending input and generates frame N+2.
  Additionally, there are parameter can set:

- 1 VSYNC_EVENT_PHASE_OFFSET_NS can set to extend period of vsync event that MessageQueue
  receives, which means allow long GPU composition time;

- 2 SF_VSYNC_EVENT_PHASE_OFFSET_NS can set to extend period of vsync event that Choreographer
  receives, which means allow long application render time;

  As mentioned before, DispSync adjust internal stats accordind to display frame's hdc_1_t.
  retireFenceFd signal time, this FenceFd plus with every layer's hwc_layer_1.acquireFenceFd,
  hwc_layer_1.releaseFenceFd are the descriptor based on Android "Synchronization framework", the
  framework consists of three main building blocks: sync_timeline, sync_pt, and sync_fence, Qcom
  have an implementation in kgsl_sync.c.

  The signal timestamp of retire fences must match VSYNC, PRESENT_TIME_OFFSET_FROM_VSYNC_NS can be
  set to coordinates it.

##  SurfaceFlinger composition part

  Since SurfaceFlinger begin a frame's composite on a VSYNC event arrive; so SF and Layer hold two
  related infos, one record current composte frame's infos, another one record next frame's infos;

  The composite begin from SF::onMessageReceived():
- 1 handleMessageTransaction() is responsible for parse current state and commit to draw state;
       the details are in func handleTransactionLocked():
    - 1.1 iterate SF.mCurrentState.layersSortedByZ, call Layer::doTransaction() to update every
           Layer.mDrawingState from Layer.mCurrentState; if mCurrentState.alpha/z, etc has changed
           in last VSYNC period, from now on it will take effect;
    - 1.2 compare SF.mCurrentState.displays and SF.mDrawingState.displays to handle display add/
           remove:
        - 1.2.1 if display removes, erase correspond DisplayDevice from SF.mDisplays;
        - 1.2.2 if current display's layerStack/orientation, etc has changed, update to correspond
                     DisplayDevice;
        - 1.2.3 if display added, instance correspond DisplaySurface: FramebufferSurface/VDS; and
                       instance DisplayDevice, then update in SF.mDisplays;
    - 1.3 update every layer's transform according to it's DisplayDevice; if the same layer is
                   mirrored in different DisplayDevice then use the default display's transfrom;
    - 1.4 handle layer remove and update correspond DisplayDevice.dirtyRegion;
    - 1.5 call SF::commitTransaction() to update SF.mDrawingState from SF.mCurrentState;

- 2 handleMessageInvalidate() is responsible for acquire every layer's next render buffer that
       use to do the composite; the details are in function handlePageFlip():

       iterate every layer in mDrawingState.layersSortedByZ and determine whether need to composte
       according to SF.mQueuedFrames and the DispSync; then call Layer::latchBuffer() of the vaild
       layers, latchBuffer() will invoke Layer.mSurfaceFlingerConsumer.updateTexImage(), it will
       acquire a GraphicBuffer from layer's consumer, then bind the Layer.mTextureName through func
       glBindTexture(), also bind GraphicBuffer through func glEGLImageTargetTexture2DOES(), at last
       update the GraphicBuffer in Layer.mActiveBuffer;

- 3 handleMessageRefresh() is responsible for the frame's composte, the key steps are:

    - 3.1 call rebuildLayerStacks(), it iterate SF.mDisplays and compute whether the layers in SF.
           mDrawingState.layersSortedByZ is visible on DisplayDevice; at last update the result to
           every DisplayDevice.mVisibleLayersSortedByZ;

    - 3.2 call setUpHWComposer() to setup infos of the HAL's structs:
        - 3.2.1 iterate DisplayDevice.mVisibleLayersSortedByZ in SF.mDisplays and invoke Layer::
                 setGeometry(), it will set fields that in every layer's Layer.mDrawingState to
                 fields in hwc_layer_1_t through abstract APIs of HWCLayerInterface; hwc_layer_1_t
                 is get from HWComposer.mDisplayData[].list->hwLayers[];

        - 3.2.2 iterate DisplayDevice.mVisibleLayersSortedByZ in SF.mDisplays and invoke Layer::
                 setPerFrameData(), it will update Layer.mActiveBuffer to correspond hwc_layer_1_t.
                 handle through abstract API of HWCLayerInterface;
    
        - 3.2.3 invoke HWComposer::prepare(), it iterate HWComposer.mDisplayData[] and let mLists
                 pointer to correspond DisplayData.list; then invoke HAL API prepare() with mLists
                 as it's parameter, after this API return, HAL will decide every layer's composite
                 type according to platform's capability;

    - 3.3 call doComposition() to draw Layer which have HWC_FRAMEBUFFER composite type and invoke
       HAL API commit() to display frame to panel:
        - 3.3.1 invoke doDisplayComposition() to handle draw different composite type layers; the
             drawing details are in doComposeSurfaces(), if need use GLES to composite will
             invoke DisplayDevice::makeCurrent() to set the DisplayDevice.mSurface as egl draw
             surface; then call Layer::draw() that will eventually call glDrawArrays() to draw
             layer as texture to the DisplayDevice.mSurface; after doComposeSurfaces() will
             invoke DisplayDevice::swapBuffers(), it then call eglSwapBuffers() that will queue
             egl complete buffer, if DisplayDevice.mSurface's consumer is FramebufferSurface,
             FBS will set the egl composite buffer to HWC_FRAMEBUFFER_TARGET index in hwc_1_t.
             hwLayers[] through HWComposer::fbPost();

        - 3.3.2 call postFramebuffer() to invoke HAL API commit() to let platform to the rest layer
             composite and display the complete frame to panel;

    - 3.4 postComposition() provide last present fence to DispSync and decide whether need turn off
       hardware vsync dynamically;

  In the composite process, hwc_layer_1.acquireFenceFd are set when SF::doComposeSurfaces() through
  Layer::setAcquireFence(), it is get from buffer consumer's fence, HAL can wait this fence till it
  is available to access; during HAL API set() hwc_layer_1.releaseFenceFd are provide by HAL, it set
  to correspond layer in SF::postFramebuffer() through Layer::onLayerDisplayed(), when the buffer
  released to buffer queue will carry on this fence object, if producer dequeue this buffer can wait
  the fence till it's available; hdc_1_t.retireFenceFd are also set during API set(), after set()
  in HWComposer::commit(), will save this fence in mDisplayData[].lastDisplayFence , it will provide
  to DispSync through HWComposer::getDisplayFence() to adjust the VSYNC event;


 **ACRONYM**
   hdc_1_t                       hwc_display_contents_1_t
   SF                            SurfaceFlinger
   GBP                           GraphicBufferProducer
   GBC                           GraphicBufferConsumer
   VDS                           VirtualDisplaySurface
   FBS                           FramebufferSurface


 **REFERENCE:**
   https://source.android.com/devices/graphics/implement-vsync
   http://blog.csdn.net/jinzhuojun/article/details/39698317

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

 Emmmmmm, the mediarouter in supportV7 library is allow manufacturers
 to enable playback on their devices through a standardized interface

 MediaRouter
   ||
   ||
   ||  sGlobal            --  GlobalMediaRouter
   ||
   ||
   ||
   ||
   || ||
   || ||GlobalMediaRouter
   || ||  implements SystemMediaRouteProvider.SyncCallback,
   || ||             RegisteredMediaRouteProviderWatcher.Callback
   || ||
   || ||  mRoutes                     --  ArrayList<RouteInfo>
   || ||  mProviders                  --  ArrayList<ProviderInfo>
   || ||  mSystemProvider             --  SystemMediaRouteProvider
   || ||  mRegisteredProviderWatcher  --  RegisteredMediaRouteProviderWatcher
   || ||  mDiscoveryRequest           --  MediaRouteDiscoveryRequest
   || ||


 abstract MediaRouteProvider
   ||
   ||  mDiscoveryRequest --  MediaRouteDiscoveryRequest
   ||  mCallback         --  Callback
   ||  mDescriptor       --  MediaRouteProviderDescriptor
   ||
   ||  RouteController onCreateRouteController(String routeId)
   ||
   ||
   ||
   || ||
   || ||abstract RouteController
   || ||
   || ||
   || ||
   ||
   ||
   || ||
   || ||abstract Callback
   || ||
   || ||  void onDescriptorChanged(MediaRouteProvider provider,
   || ||                MediaRouteProviderDescriptor descriptor)
   || ||
   || ||


 abstract MediaRouteProviderService
   ||               extends Service
   ||
   ||  mProvider                   --  MediaRouteProvider
   ||  mCompositeDiscoveryRequest  --  MediaRouteDiscoveryRequest
   ||  mProviderCallback           --  ProviderCallback
   ||  mClients                    --  ArrayList<ClientRecord>
   ||
   ||  abstract MediaRouteProvider onCreateMediaRouteProvider()
   ||
   || ||
   || ||ProviderCallback
   || ||    extends MediaRouteProvider.Callback
   || ||
   || ||
   ||
   ||
   || ||
   || ||ClientRecord
   || ||
   || ||  mControllers             --  SparseArray<MediaRouteProvider.RouteController>
   || ||
   || ||  createRouteController(String routeId, int controllerId)
   || ||

