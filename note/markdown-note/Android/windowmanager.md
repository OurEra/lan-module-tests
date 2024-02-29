```java
 Activity
   ||  mWindow                   --  Window
   ||
   || mWindow instance in attach() with PhoneWindow;
   ||
   ||
```

```java
 ActivityRecord
   ||  appToken                  --  IApplicationToken.Stub
   ||
   || appToken instance in ActivityRecord() with ActivityRecord$Token extends IApplicationToken.Stub
   ||
   ||
   ||
```

```java
 View$AttachInfo
   ||  mWindowToken              --  IBinder
   ||  mWindow                   --  IWindow
   ||
   || mWindowToken, mWindow instance in AttachInfo(), mWindow is the handle of class W;
   || mWindowToken is returned from mWindow.asBinder();
```

```java
 ViewRootImpl
   ||  mWindow                   --  class W extends IWindow.Stub
   ||  mAttachInfo               --  View.AttachInfo
   ||  mWindowSession            --  IWindowSession, a handle of com.android.server.wm.Session,
   ||                                                which represent a active client to WMS
   ||
   || mWindow instance in ViewRootImpl() with ViewRootImpl$W extends IWindow.Stub;
   || mAttachInfo instance in ViewRootImpl() with View$AttachInfo;
   ||
   || ViewRootImpl add mWindow to WMS by calling mWindowSession.addToDisplay();
   ||
```

```java
 Dialog
   ||  mWindow                   --  Window
   ||
   || mWindow instance in Dialog() width PhoneWindow;
   ||
   || call addView() in show();
   ||
```

  VolumeDialogComponent instance VolumeDialog and set window type to WindowManager.LayoutParams.
  TYPE_VOLUME_OVERLAY

```java
 PopupWindow
   ||  mWindowLayoutType         --  WindowManager.LayoutParams.TYPE_APPLICATION_PANEL
   ||
   ||
   || call addView() in invokePopup(), set WM.LayoutParams.token with View.getWindowToken()
   ||
```

```java
 WindowStateAnimator
   ||  mSurfaceControl           --  SurfaceControl
   ||  mSurfaceLayer             --  int
   ||  mAnimLayer                --  int
   ||
   ||  prepareSurfaceLocked()
   ||
```

```java
 WindowState
   ||  mWinAnimator              --  WindowStateAnimator
   ||  mAppToken                 --  AppWindowToken
   ||  mToken                    --  WindowToken
   ||  mRootToken                --  WindowToken
   ||  mBaseLayer                --  int
   ||  mSubLayer                 --  int
   ||  mLayer                    --  int
   ||  mFrame                    --  Rect
   ||  mAttachedWindow           --  WindowState
   ||
```

```java
 DisplayContent
   ||  mWindows                  --  WindowList (ArrayList<WindowState>)
   ||  mStacks                   --  ArrayList<TaskStack>
   ||
   ||
```

```java
 WindowToken
   ||  token                     --  IBinder
   ||  windows                   --  WindowList (ArrayList<WindowState>)
   ||  appWindowToken            --  AppWindowToken
   ||
   ||
```

```java
 AppWindowToken  extends WindowToken
   ||  appToken                  --  IApplicationToken
   ||  allAppWindows             --  WindowList (ArrayList<WindowState>)
   ||
   ||
```

```java
 TaskStack
   || mTasks                     --  ArrayList<Task>
   ||
   ||
```

```java
 Task
   || mAppTokens                 --  AppTokenList (ArrayList<AppWindowToken>)
   ||
   ||
```

```java
 WindowManagerService
   ||  mWindowMap                --  HashMap<IBinder, WindowState>
   ||  mSessions                 --  ArraySet<Session>
   ||  mAnimator                 --  WindowAnimator
   ||  mPolicy                   --  PhoneWindowManager
   ||  mActivityManager          --  IActivityManager
   ||  mDisplayContents          --  SparseArray<DisplayContent>
   ||  mTokenMap                 --  HashMap<IBinder, WindowToken>
   ||  mDestroySurface           --  ArrayList<WindowState>
   ||  mStackIdToStack           --  SparseArray<TaskStack>
   ||  mTaskIdToTask             --  SparseArray<Task>
   ||
   || ActivityStackSupervisor$ActivityContainer.attachToDisplay() will call attachStack():
   ||   1. find correspond TaskStack from mStackIdToStack, call TaskStack.attachDisplayContent(),
   ||      and call DisplayContent.attachStack(), which update DisplayContent.mStacks;
   ||   2. if find from mStackIdToStack failed, will instance TaskStack() and put mStackIdToStack;
   ||
   || ActivityStackSupervisor.restoreRecentTaskLocked() will call addAppToken():
   ||   1. instance AppWindowToken() and update mTokenMap;
   ||   2. find corresponding Task from mTaskIdToTask and call Task.addAppToken(), which will update
   ||      Task.mAppTokens;
   ||   3. if find from mTaskIdToTask failed, call createTaskLocked(): instance Task() and update
   ||      mTaskIdToTask, also call TaskStack.addTask() which update TaskStack.mTasks;
   ||
   ||
   || ActivityStack.startActivityLocked() call setAppTask()
   ||   1. find corresponding Task from mTaskIdToTask and call Task.addAppToken(), which update
   ||      Task.mAppTokens;
   ||   2. if find from mTaskIdToTask failed, will call createTaskLocked(), instance Task() and
   ||      update mTaskIdToTask, also call TaskStack.addTask() which update TaskStack.mTasks;
   ||
   ||
   || WMS handle a new window in addWindow(), it save the caring resource in a WindowState instance
   || and put in mWindowMap
   ||
```

  WMS need to manage different type of windows, according to the implements of AM side:
    1. every display window have an ViewRootImpl object, which contain a ViewRootImpl.mWindow field;
    2. every activity have an ActivityRecord object, which contain a ActivityRecord.appToken field;
    3. a collection of activities which perform a certain job correspond to object TaskRecord;
    4. all TaskRecord managed by object ActivityStack;
  in order to manage these objects, WMS have correspond objects:
    1. WindowToken object correspond to ViewRootImpl, field WindowToken.token record ViewRootImpl.
       mWindow;
    2. AppWindowToken object correspond to ActivityRecord, field AppWindowToken.appToken record
       ActivityRecord.appToken;
    3. Task correspond to TaskRecord, Task.mTaskId is the value of TaskRecord.taskId;
    4. TaskStack correspond to ActivityStack, TaskStack.mStackId is the value of ActivityStack.
       mStackId;

  WMS record all WindowToken in WMS.mTokenMap, every display device's WindowToken are record in
  DisplayContent.mWindows, a Task's WindowToken are record in Task.mAppTokens.

  One of WMS's job is to manage every window's display order: an PopupWindow always render on the
  top of the anchor view's page; an Dialog always render on the top of others, etc; in general, WMS
  handle this render order by:
    1. in AM side, every display page will hold a instance of PhoneWindow extends Window,
       the Window.mWindowAttributes field is a instance of WindowManager.LayoutParams;
       WindowManager.LayoutParams.type will be assign to value like:
           WindowManager.LayoutParams.TYPE_VOLUME_OVERLAY
           WindowManager.LayoutParams.TYPE_APPLICATION_PANEL
           WindowManager.LayoutParams.FIRST_APPLICATION_WINDOW
           WindowManager.LayoutParams.TYPE_APPLICATION_STARTING

    2. in WMS side, every display page have a instance of WindowState, AM WM.LayoutParams.type are
       convert to WindowState.mBaseLayer and WindowState.mSubLayer by func PWM.windowTypeToLayerLw()
       and PWM.subWindowTypeToLayerLw() respectively, if WindowManager.LayoutParams.type value are
       between FIRST_SUB_WINDOW and LAST_SUB_WINDOW, then mBaseLayer&mSubLayer will convert from
       WindowState.mAttachedWindow.mAttrs.type; later WMS.assignLayersLocked() will assign field
       WindowState.mLayer according to WindowState.mBaseLayer, mLayer eventually will be used in
       WindowStateAnimator and set to SurfaceFlinger through SurfaceControl.setLayer();
    
       in order to arrange windows, WMS keep enough gap between different layer type and the same
       layer type; when assign mBaseLayer, value from windowTypeToLayerLw() will first multiply by
       TYPE_LAYER_MULTIPLIER; then add a TYPE_LAYER_OFFSET, which guarantee enough gap between
       different layer; when assign mLayer in assignLayersLocked(), if mBaseLayer is the same, then
       add the WINDOW_LAYER_MULTIPLIER;

  A page/widget need to display, need to add view to WMS by calling WindowManagerImpl.addView(),this
  function then invoke WindowManagerGlobal.addView();
    1. instance ViewRootImpl() and update WindowManagerGlobal.mRoots;
    2. invoke ViewRootImpl.setView() function:
      2.1 call requestLayout() which schedule views traversal, in performTraversals() will call
          relayoutWindow(), func then call Session.relayout(), at last call to WMS.relayoutWindow();
      2.2 call Session.addToDisplay(), which will invoke WMS.addWindow();

  WindowManagerService.relayoutWindow() will instance SurfaceControl by calling WindowStateAnimator.
  createSurfaceLocked(), the view's display order, position all need to pass to SF by SurfaceControl's
  APIs;

  WMS.addWindow() first do many checks, for example, a application window(FIRST_APPLICATION_WINDOW
  - LAST_APPLICATION_WINDOW) must find WindowToken from mTokenMap; a sub window(FIRST_SUB_WINDOW -
    LAST_SUB_WINDOW) must find a WindowState instance from mWindowMap, which the sub window attached
    to; then instance a WindowState and update mWindowMap. Next this new WindowState will be put in a
    proper position in DisplayContent.mWindows, then assign the WindowState.mLayer, which handled by:
  1. addWindowToListInOrderLocked()/addInputMethodWindowToListLocked()
     1.1 when incoming window is the inpput method, call addInputMethodWindowToListLocked(), it will
         first call findDesiredInputMethodWindowIndexLocked() to find a proper position and place
         the WindowState in it directly;
     1.2 when use addWindowToListInOrderLocked(), there still cases need treat differently: if it's
         a sub window, addAttachedWindowToListLocked() will handle the job; if it's a window have
         application token will invoke addAppWindowToListLocked(), otherwise will invoke function
         addFreeWindowToListLocked();
  2. after place the WindowState in position, assignLayersLocked() iterate the WindowList and assign
     every mLayer, which organized the display order the way user intend to render.

  In WMS, there are several occasion will destroy a display window:
  1. when user dismiss a window, like call Dialog.dismiss(), will invoke WMS.removeWindow(), then
     WMS.removeWindowInnerLocked() call WindowState.removeLocked(), which call destroySurfaceLocked();
  2. when WindowStateAnimator.stepAnimationLocked() check animation is done will invoke finishExit()
     and update the WMS.mDestroySurface, in WMS.performLayoutAndPlaceSurfacesLockedInner() whill
     check mDestroySurface and also call destroySurfaceLocked().
  3. in WMS.relayoutWindow() after check a certain WindowState's conditions, will also invoke
     destroySurfaceLocked();

  WindowStateAnimator.destroySurfaceLocked() will invoke SurfaceControl.destroy(), which then call
  corresponding native API to destroy surface.

 **ACRONYM**
   WMS                           WindowManagerService
   WM                            WindowManager
   AM                            ActivityManager
   PWM                           PhoneWindowManager

 **REFERENCE**
   http://blog.csdn.net/Luoshengyang/article/details/8570428

#######################################################
#######################################################
WMS.rebuildAppWindowListLocked()

