
## SURFACEs

```java
 SurfaceView extends View
   ||
   ||  mSurface                  --  Surface
   ||  mSurfaceHolder            --  SurfaceHolder
   ||
```

```java
 GLSurfaceView extends SurfaceView
   ||    implements SurfaceHolder.Callback
   ||
   ||
   ||
   ||
```


```java
 SurfaceTexture
   ||
   ||  mSurfaceTexture           --  long
   ||  mProducer                 --  long
   ||  mFrameAvailableListener   --  long
   ||
```

  SurfaceTexture is just a java class that conjunction with the native BufferQueue that created in
  native SurfaceTexture_init() function; if give SurfaceTexture.mProducer to others and let queue
  buffer, then the SurfaceTexture will receive frame available and process it;

```java
 HardwareLayer
   ||
   ||  mRenderer                 --  HardwareRenderer
   ||  mFinalizer                --  VirtualRefBasePtr
   ||
   ||  void setSurfaceTexture(SurfaceTexture surface)
   ||  void updateSurfaceTexture()
```

  HardwareLayer is a java class that represents native hwui DeferredLayerUpdater, it can be instance
  through func ThreadedRenderer.createTextureLayer(); mRenderer is the reference of ThreadedRenderer
  , mFinalizer hold the native DeferredLayerUpdater reference and keep a strong reference;

  when set a SurfaceTexture to native DeferredLayerUpdater through setSurfaceTexture() and invoke
  updateSurfaceTexture() when content update, the SurfaceTexture's buffer can draw as one texture


```java
 TextureView extends View
   ||
   ||  mLayer                    --  HardwareLayer
   ||  mSurface                  --  SurfaceTexture
   ||
```

## VIEW & DRAW part


```java
 Activity
   ||
   ||  mWindow                   --  Window
   ||
   ||
   ||
```

  Through Instrumentation.newActivity() can instance Activity and invoke Activity.attach(), in
  Activity.attach() will instance PhoneWindow and record in Activity.mWindow;

  After new activity will callback Activity.onCreate(), user will overwrite it and call function
  setContentView() to render it's views on the activity page; Activity.setContentView() then call
  PhoneWindow.setContentView();

  ActivityThread.handleLaunchActivity() will call performLaunchActivity() and handleResumeActivity()
  in order; handleResumeActivity() will assign PhoneWindow.mDecor to Activity.mDecor and add it to
  WindowManager by calling WindowManager.addView() or Activity.makeVisible();

```java
 PhoneWindow extends Window
   ||    implements MenuBuilder.Callback
   ||
   ||  mDecor                    --  DecorView
   ||  mContentParent            --  ViewGroup
   ||  mContentRoot              --  ViewGroup
   ||
   ||  void installDecor()
   ||
   || ||
   || ||DecorView extends FrameLayout
   || ||    implements RootViewSurfaceTaker
   || ||
```

   PhoneWindow implements abstract class android.view.Window, which responsible for a top-level
   window look and behavior policy;

   PhoneWindow.mDecor is a FrameLayout, when invoke installDecor() will add system defined view to
   this FrameLayout; for instance, when add layout R.layout.screen_custom_title to mDecor according
   to user defined feature, then mContentRoot is the top LinearLayout in layout file, mContentParent
   is the FrameLayout that has the ID_ANDROID_CONTENT name; when invoke setContentView() will add
   user defined view under mContentParent, with system view and user defined view formed the display
   hierarchy of one page;

```java
 View implements Drawable.Callback,
   ||    KeyEvent.Callback,
   ||    AccessibilityEventSource
   ||
   ||  mLayerType                --  int
   ||  mAttachInfo               --  AttachInfo
   ||  mParent                   --  ViewParent
   ||  mLeft                     --  int
   ||  mRight                    --  int
   ||  mTop                      --  int
   ||  mBottom                   --  int
   ||  mPaddingLeft              --  int
   ||  mPaddingRight             --  int
   ||  mPaddingTop               --  int
   ||  mPaddingBottom            --  int
   ||  mMeasuredWidth            --  int
   ||  mMeasuredHeight           --  int
   ||  mLayoutParams             --  ViewGroup.LayoutParams
   ||
   ||  mRenderNode               --  RenderNode
   ||
   ||  void assignParent(ViewParent parent)
   ||  void measure(int widthMeasureSpec, int heightMeasureSpec)
   ||  void onMeasure(int widthMeasureSpec, int heightMeasureSpec)
   ||  void layout(int l, int t, int r, int b)
   ||  void onLayout(boolean changed, int left, int top, int right, int bottom)
   ||  void draw(Canvas canvas)
   ||  void onDraw(Canvas canvas)
   ||  boolean draw(Canvas canvas, ViewGroup parent, long drawingTime)
   ||
   ||  RenderNode updateDisplayListIfDirty()
   ||
   ||  int getMeasuredWidth()
   ||  int getMeasuredHeight()
   ||  int getWidth()
   ||  int getHeight()
   ||
   || ||
   || ||AttachInfo
   || ||
   || ||  mHardwareRenderer      --  HardwareRenderer
   || ||  mHardwareAccelerated   --  boolean
```

  PhoneWindow.mDecor's mParent field is assign in ViewRootImpl.setView() through assignParent();
  other view's mParent field is assign in ViewGroup.addViewInner();

  View's mAttachInfo will be assign when the first time call ViewRootImpl.performTraversals() by
  calling dispatchAttachedToWindow(), ViewGroup then dispatch to child views; when add new views,
  ViewGroup will also dispatch the mAttachInfo to it's child in ViewGroup.addViewInner();

```java
 ViewGroup extends View
   ||    implements ViewParent, ViewManager
   ||
   ||  mChildren                 --  View[]
   ||
   ||  void addView(View child, int index, LayoutParams params)
   ||
   ||  void measureChildWithMargins(View child,
   ||           int parentWidthMeasureSpec, int widthUsed,
   ||           int parentHeightMeasureSpec, int heightUsed)
   ||
   ||  void dispatchDraw(Canvas canvas)
   ||  void dispatchGetDisplayList()
   ||
   || ||
   || ||LayoutParams
   || ||
   || ||  width                  --  int
   || ||  height                 --  int
   || ||
   ||
   || ||MarginLayoutParams extends ViewGroup.LayoutParams
   || ||
   || ||  leftMargin             --  int
   || ||  topMargin              --  int
   || ||  rightMargin            --  int
   || ||  bottomMargin           --  int
```

  ViewParent is a interface that defines the responsibilities for a class that will be a parent of
  a View;

```java
 LinearLayout extends ViewGroup
   ||
   ||  void onMeasure(int widthMeasureSpec, int heightMeasureSpec)
   ||  void onLayout(boolean changed, int l, int t, int r, int b)
   ||  LayoutParams generateLayoutParams(AttributeSet attrs)
   ||
   ||
   || ||
   || ||LayoutParams extends ViewGroup.MarginLayoutParams
   || ||
   || ||  weight                 --  float
   || ||  gravity                --  int
```

```java
 FrameLayout extends ViewGroup
   ||
   ||  void onMeasure(int widthMeasureSpec, int heightMeasureSpec)
   ||  void onLayout(boolean changed, int left, int top, int right, int bottom)
   ||  LayoutParams generateLayoutParams(AttributeSet attrs)
   ||
   ||
   || ||
   || ||LayoutParams extends MarginLayoutParams
   || ||
   || ||  gravity                --  int
```

```java
 ViewRootImpl implements ViewParent,
   ||    View.AttachInfo.Callbacks,
   ||    HardwareRenderer.HardwareDrawCallbacks
   ||
   ||  mAttachInfo               --  View.AttachInfo
   ||  mWindow                   --  W
   ||  mSurface                  --  Surface
   ||  mView                     --  View
   ||  mWinFrame                 --  Rect
   ||  mWidth                    --  int
   ||  mHeight                   --  int
   ||  mChoreographer            --  Choreographer
   ||
   ||  void enableHardwareAcceleration(WindowManager.LayoutParams attrs)
   ||
   || ||
   || ||W extends IWindow.Stub
   || ||
   || ||
```

  ViewRootImpl is instance when add PhoneWindow.mDecor to WindowManager, after instance it mDecor
  will give to ViewRootImpl by calling ViewRootImpl.setView(), mView then hold the reference;

  There are three major steps to perform views updates and drawing, the are all process in function
  ViewRootImpl.performTraversals(); there are several route will invoke performTraversals():
  1. when invoke ViewRootImpl.setView() will call requestLayout(), it then do perfrom traversal;
  2. when a child view update it's content and call it's invalidate() function, then will call it's
     parent's invalidateChild() function; in invalidateChild() will call every view's own parent's
     invalidateChildInParent() in the view hierarchy layer by layer, at last call to ViewRootImpl.
     invalidateChildInParent() function and perform traversal;

  Actually, no matter through requestLayout() or invalidate(), etc in ViewRootImpl will first invoke
  scheduleTraversals(), then will call performTraversals() at a certain time; the time will perform
  traversal is on a vsync event, cause scheduleTraversals() will invoke Choreographer.postCallback()
  , so when Choreographer receive the next vsync event will callback ViewRootImpl.mTraversalRunnable
  ; another call in scheduleTraversals() is also important: notifyRendererOfFramePending(), it will
  notify hwui there is frame to draw;

  what performTraversals() wrapper around process steps are:
  1. performMeasure()
     measure is try to determine every view's View.mMeasuredWidth and View.mMeasuredHeight field;
     measure start from ViewRootImpl.mView in top-to-bottom order, parent view's measure size and
     mode will pass to child through MeasureSpec, every view decide it's own measure size according
     to parent's MeasureSpec and it's own LayoutParams;

     the ViewRootImpl.mWindowAttributes is copy from PhoneWindow.mWindowAttributes in setView(), the
     default value of ViewGroup.width and ViewGroup.height when instance mWindowAttributes are both
     MATCH_PARENT; so in ViewRootImpl.measureHierarchy(), the MeasureSpec size is display size, mode
     is MeasureSpec.EXACTLY;

     base class View has function View.measure() and View.onMeasure(); ViewGroup implements class
     FrameLayout, LinearLayout will overwrite onMeasure() to handle it's childs measure; widget view
     like TextView, ImageView will also overwrite onMeasure(); View.measure() mainly check the old
     measure result to prevent unnecessary measure, then invoke view's own onMeasure();

  2. performLayout()
     layout() is try to determine every view's View.mLeft, View.mRight, View.mTop and View.mBottom
     field; base class View's function View.layout() assign these fields through View.setFrame(),
     then it call View.onLayout() if necessary; some widget class like TextView overwrite onLayout()
     to update it's internal data; ViewGroup implements class like FrameLayout, LinearLayout, etc
     need to overwrite onLayout() to traversal it's child views and call child view's layout();

     layout process start from ViewRootImpl.mView, the previous measure result will save in it's
     position field when View.layout(); then in the order of view's hierarchy, every view will be
     determine it's position according to it's measure result and padding, margin setting; besides,
     LinearLayout, RelativeLayout, etc have it's own attributes which determine it's child position;

     for example, when child's gravity value is Gravity.CENTER_HORIZONTAL | Gravity.CENTER_VERTICAL
     in a FrameLayout; the child position will be calculate:
             _________________________________________________
            |PARENT TOP                                       |
            |      _____________________________________      |
            | #1  |CHILD TOP                            | #2  |
            |     |                                     |     |
            |     |                                     |     |
      PARENT|     |CHILD                           CHILD|     |PARENT
        LEFT|     |LEFT                            RIGHT|     |RIGHT
            |     |                                     |     |
            |     |_____________________________________|     |
            |     CHILD BOTTOM                                |
            |_________________________________________________|
            PARENT BOTTOM

            #1 : PARENT LEFT PADDING  + CHILD LEFT MARGIN  + (SPACE TO ALIGN)
            #2 : PARENT RIGHT PADDING + CHILD RIGHT MARGIN + (SPACE TO ALIGN)
    
            (PR - PL) = parent measure width
            (PB - PT) = parent measure height
    
            (CR - CL) = child measure width
            (CB - CT) = child measure height

     when a child need layout center in HORIZONTAL, the child left value will be:

        parentLeft  = getPaddingLeftWithForeground();
        parentRight = right - left - getPaddingRightWithForeground();
        width       = child.getMeasuredWidth();
        height      = child.getMeasuredHeight();

        parentLeft + (parentRight - parentLeft - width) / 2 + lp.leftMargin - lp.rightMargin
            $1                       $2                                     $3

     $2 is the average space that child should left in left and right side;
     $1 + S2 is the left positon include parent left padding, relative to parent;
     $3 means child will adjust it's position by setting margin value;

     so child right position is child left plus with child measure width; the top and bottom will
     be calculate similarly;

  3. performDraw()

   3.1 mAttachInfo.mHardwareRenderer.draw()

       ThreadedRenderer.updateRootDisplayList()
         ThreadedRenderer.updateViewTreeDisplayList()
           ViewRootImpl.mView.updateDisplayListIfDirty()
    
         DisplayListCanvas canvas = ThreadedRenderer.mRootNode.start()
    
         canvas.drawRenderNode(ViewRootImpl.mView.updateDisplayListIfDirty())
    
         ThreadedRenderer.mRootNode.end(canvas)


       nSyncAndDrawFrame(mNativeProxy, frameInfo, frameInfo.length)


       View.updateDisplayListIfDirty()
         DisplayListCanvas canvas = View.mRenderNode.start(width, height)
    
         View.draw(Canvas canvas)
           onDraw(canvas) if overwrite
           dispatchDraw(canvas) if is ViewGroup
    
         View.mRenderNode.end(canvas)
    
       ViewGroup.dispatchDraw(Canvas canvas)
    
         ViewGroup.drawChild(Canvas canvas, View child, long drawingTime)
           View.draw(Canvas canvas, ViewGroup parent, long drawingTime)
    
             draw with render node:
               RenderNode renderNode = updateDisplayListIfDirty()
    
               cache exist:
               canvas.drawBitmap(cache, 0.0f, 0.0f, mLayerPaint)
               or:
               ((DisplayListCanvas) canvas).drawRenderNode(renderNode)
    
             no render node:
    
               cache exist:
               canvas.drawBitmap(cache, 0.0f, 0.0f, mLayerPaint)
               or:
               draw(canvas)


   3.2 drawSoftware()

## HWUI part (SDK = 23)

 HardwareRenderer                                          #Java
   ||
   ||  HardwareRenderer create(Context context, boolean translucent)
   ||
   ||  abstract boolean initialize(Surface surface)
   ||
   ||  abstract  void draw(View view, View.AttachInfo attachInfo,
   ||                       HardwareDrawCallbacks callbacks)
   ||
   ||  ...

 ThreadedRenderer extends HardwareRenderer                 #Java
   ||
   ||  mNativeProxy              --  long
   ||  mRootNode                 --  RenderNode
   ||
   ||  void updateRootDisplayList(View view, HardwareDrawCallbacks callbacks)

 RenderNode                                                #Java
   ||
   ||  mNativeRenderNode         --  long
   ||
   ||  RenderNode create(String name, @Nullable View owningView)
   ||  DisplayListCanvas start(int width, int height)
   ||  void end(DisplayListCanvas canvas)
   ||
   ||  boolean setLayerType(int layerType)
   ||  boolean setLayerPaint(Paint paint)
   ||  boolean setRotation(float rotation)
   ||  ...

  Java RenderNode set***() functions will set to native RenderNode.mStagingProperties through JNI
  directly;

 Canvas                                                    #Java
   ||
   ||  mNativeCanvasWrapper      --  long
   ||
   ||  void drawText(@NonNull char[] text, int index, int count, float x, float y,
   ||         @NonNull Paint paint)
   ||
   ||  void drawBitmap(@NonNull Bitmap bitmap, float left, float top, @Nullable Paint paint)
   ||
   ||  void drawRect(@NonNull RectF rect, @NonNull Paint paint)
   ||  ...


 DisplayListCanvas extends Canvas                          #Java
   ||
   ||  sPool                     --  SynchronizedPool<DisplayListCanvas>
   ||
   ||  DisplayListCanvas obtain(@NonNull RenderNode node)
   ||  long finishRecording()
   ||


 Canvas
   ||
   ||  void drawBitmap(const SkBitmap& bitmap, float left, float top,
   ||         const SkPaint* paint) = 0
   ||
   ||  void drawRect(float left, float top, float right, float bottom,
   ||          const SkPaint& paint) = 0
   ||
   ||  void drawText(const uint16_t* glyphs, const float* positions, int count,
   ||         const SkPaint& paint, float x, float y,
   ||         float boundsLeft, float boundsTop, float boundsRight, float boundsBottom,
   ||         float totalAdvance) = 0
   ||
   ||  ...

   From SDK23, Android removed GLES20Canvas.java and related implements; instead, Android choose to
   seperate hwui and skia canvas in native; so in native, SkiaCanvas and DisplayListCanvas are both
   inherit Canvas, then implements the functions separately; in java, DisplayListCanvas only has few
   different functions than Canvas;

 SkiaCanvas : public Canvas
   ||
   ||
   ||

 DisplayListCanvas : public Canvas,
   ||    public CanvasStateClient
   ||
   ||  mDisplayListData          --  DisplayListData*
   ||  mDeferredBarrierType      --  DeferredBarrierType
   ||
   ||  size_t addOpAndUpdateChunk(DisplayListOp* op)
   ||  size_t addStateOp(StateOp* op)
   ||  size_t addDrawOp(DrawOp* op)
   ||  size_t addRenderNodeOp(DrawRenderNodeOp* op)

  DisplayListCanvas is responsible to record all draw operations and clip, rorate, etc from the view
  to mDisplayListData;

 DisplayListOp
   ||
   ||  void defer(DeferStateStruct& deferStruct, int saveCount, int level,
   ||         bool useQuickReject) = 0
   ||
   ||  void replay(ReplayStateStruct& replayStruct, int saveCount, int level,
   ||         bool useQuickReject) = 0
   ||
   ||  void output(int level, uint32_t logFlags = 0) const = 0

 DrawOp : public DisplayListOp
   ||
   ||  void applyDraw(OpenGLRenderer& renderer, Rect& dirty) = 0
   ||

 StateOp : public DisplayListOp
   ||
   ||  void applyState(OpenGLRenderer& renderer, int saveCount) const = 0
   ||

 DisplayListData
   ||
   ||  displayListOps            --  Vector<DisplayListOp*>
   ||  mChildren                 --  Vector<DrawRenderNodeOp*>
   ||  chunks                    --  Vector<Chunk>
   ||
   || ||
   || ||Chunk
   || ||
   || ||  beginOpIndex           --  size_t
   || ||  endOpIndex             --  size_t
   || ||  beginChildIndex        --  size_t
   || ||  endChildIndex          --  size_t

  The functions in DisplayListCanvas will be treat in three ways:
  1. drawBitmap(), drawText(), drawLines(), etc, DisplayListCanvas will instance correspond DrawOp
     and save in mDisplayListData.displayListOps through DisplayListCanvas::addDrawOp();
  2. rotate(), clipPath(), scale(), etc, DisplayListCanvas will instance correspond StateOp and
     save in mDisplayListData.displayListOps through DisplayListCanvas::addDrawOp();
  3. drawRenderNode(), DisplayListCanvas will instance DrawRenderNodeOp and save both in mChildren
     and displayListOps;

  Except update DisplayListData.displayListOps, DisplayListCanvas also update DisplayListData.chunks
  when DisplayListCanvas::addOpAndUpdateChunk(); according mDeferredBarrierType field, the item in
  Vector<DisplayListOp*> will be organize into Chunk, Chunk will record the item's index in Vector;

 RenderNode : public VirtualLightRefBase
   ||
   ||  mLayer                    --  Layer*
   ||  mDisplayListData          --  DisplayListData*
   ||  mStagingDisplayListData   --  DisplayListData*
   ||
   ||  void pushLayerUpdate(TreeInfo& info)


 RootRenderNode : public RenderNode,
   ||    ErrorHandler
   ||
   ||  mLooper                   --  sp<Looper>
   ||
   ||


 DeferredDisplayList
   ||
   ||  mBatches                  --  Vector<Batch*>
   ||
   ||  void flush(OpenGLRenderer& renderer, Rect& dirty)
   ||  void addDrawOp(OpenGLRenderer& renderer, DrawOp* op)
   ||

  addDrawOp() is DrawOp defer will invoke;
  flush is when OpenGLRenderer drawRenderNode() finish defer will invoke;

 RenderProxy
   ||
   ||  mRenderThread             --  RenderThread&
   ||  mContext                  --  CanvasContext*
   ||  mDrawFrameTask            --  DrawFrameTask
   ||
   ||  int syncAndDrawFrame()
   ||
   \\---------------------------------------
     ---------------------------------------
     CREATE_BRIDGE4 macro expansion:

     typedef struct {
    
         RenderThread* thread;
         bool translucent;
         RenderNode* rootRenderNode;
         IContextFactory* contextFactory;
    
     } createContextArgs;
    
     static void* Bridge_createContext(createContextArgs* args) {
    
         return new CanvasContext(*args->thread, args->translucent,
                 args->rootRenderNode, args->contextFactory);
     }
    
     SETUP_TASK macro expansion:
    
     MethodInvokeRenderTask* task = new MethodInvokeRenderTask((RunnableMethod) Bridge_createContext);
     createContextArgs *args = (createContextArgs *) task->payload();

 RenderThread : public Thread,
   ||    protected Singleton<RenderThread>
   ||
   ||  mLooper                   --  sp<Looper>
   ||  mDisplayEventReceiver     --  DisplayEventReceiver*
   ||  mQueue                    --  TaskQueue
   ||  mFrameCallbacks           --  std::set<IFrameCallback*>
   ||  mPendingRegistrationFrameCallbacks  --  std::set<IFrameCallback*>
   ||  mRenderState              --  RenderState*
   ||  mEglManager               --  EglManager*
   ||
   ||  void queue(RenderTask* task)
   ||
   || ||
   || ||TaskQueue
   || ||
   || ||  mHead           --  RenderTask*
   || ||  mTail           --  RenderTask*
   || ||  void queue(RenderTask* task)


  RenderThread use a looper to deal with several jobs async:
  1. RenderThread hold a TaskQueue, it provide queue() API to allow post RenderTask to RenderThread
     and get it done async; many APIs call from java in RenderProxy will wrapper the real job func
     in a MethodInvokeRenderTask and post to RenderThread; a helpful macro SETUP_TASK is define to
     do this kind of work;
  2. DrawFrameTask is also a RenderTask that implements virtual function run(), the RenderProxy func
     syncAndDrawFrame() will invoke it's drawFrame() and it will post itself to RenderThread to run;
  3. RenderThread also hold a DisplayEventReceiver reference, so it will requestNextVsync() and draw
     frame on a vsync event arrives;

  mFrameCallbacks::doFrame() -> CanvasContext::draw


  RenderProxy::syncAndDrawFrame() -> DrawFrameTask::drawFrame() -> CanvasContext::draw

 DrawFrameTask : public RenderTask
   ||
   ||  mLayers                   --  std::vector< sp<DeferredLayerUpdater> >
   ||
   ||  void pushLayerUpdate(DeferredLayerUpdater* layer);
   ||  int drawFrame()
   ||  void run() override

 CanvasContext : public IFrameCallback
   ||
   ||  mEglManager               --  RenderThread&
   ||  mCanvas                   --  OpenGLRenderer*
   ||  mRootRenderNode           --  sp<RenderNode>
   ||  mNativeWindow             --  sp<ANativeWindow>
   ||  mEglSurface               --  EGLSurface
   ||
   ||  bool initialize(ANativeWindow* window)
   ||  void draw()
   ||  void doFrame() override

  mNativeWindow is set from java ViewRootImpl.mSurface through initialize(); mEglSurface is create
  through EglManager::createSurface();

 OpenGLRenderer : public CanvasStateClient
   ||
   ||
   ||  void drawRenderNode(RenderNode* displayList, Rect& dirty, int32_t replayFlags = 1)
   ||

 EglManager
   ||
   ||  mEglContext               --  EGLContext
   ||
   ||  void initialize()
   ||  EGLSurface createSurface(EGLNativeWindowType window)
   ||

  initialize()

 LayerRenderer: public OpenGLRenderer
   ||
   ||
   ||  Layer* createTextureLayer(RenderState& renderState)
   ||  Layer* createRenderLayer(RenderState& renderState, uint32_t width, uint32_t height)
   ||  bool resizeLayer(Layer* layer, uint32_t width, uint32_t height)
   ||  void updateTextureLayer(Layer* layer, uint32_t width, uint32_t height,
   ||         bool isOpaque, bool forceFilter, GLenum renderTarget, float* textureTransform)
   ||  bool copyLayer(RenderState& renderState, Layer* layer, SkBitmap* bitmap)

 DeferredLayerUpdater : public VirtualLightRefBase
   ||
   ||  mSurfaceTexture           --  sp<GLConsumer>
   ||  mLayer                    --  Layer*
   ||
   ||  void setSurfaceTexture(const sp<GLConsumer>& texture, bool needsAttach)

 Layer : public VirtualLightRefBase
   ||
   ||  type                      --  Type
   ||  texture                   --  Texture
   ||  renderer                  --  std::unique_ptr<OpenGLRenderer>
   ||
   ||  void updateDeferred(RenderNode* renderNode, int left, int top, int right, int bottom)
   ||  void requireRenderer()



 **REFERRER**
   https://developer.android.com/guide/topics/graphics/hardware-accel.html
   https://source.android.com/devices/graphics/arch-sv-glsv
   http://blog.csdn.net/luoshengyang/article/details/45601143
   https://www.inovex.de/blog/android-graphics-pipeline-from-button-to-framebuffer-part-1/
   https://www.kancloud.cn/alex_wsc/androids/473793
