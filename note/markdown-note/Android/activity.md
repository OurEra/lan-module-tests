```java
 ActivityManagerService extends ActivityManagerNative
   ||    implements Watchdog.Monitor,
   ||    BatteryStatsImpl.BatteryCallback
   ||
   ||  mFgBroadcastQueue         --  BroadcastQueue
   ||  mBgBroadcastQueue         --  BroadcastQueue
   ||
   ||  mStackSupervisor          --  ActivityStackSupervisor
   ||
   ||  void moveTaskToFront(int taskId, int flags, Bundle options)
```

  moveTaskToFront() can move a certain TaskRecord to front, that means all TaskRecord.mActivities
  will in front in ActivityStack;

```java
 ActivityStackSupervisor implements DisplayListener
   ||
   ||  mActivityDisplays         --  SparseArray<ActivityDisplay>
   ||  mHomeStack                --  ActivityStack
   ||  mFocusedStack             --  ActivityStack
   ||  mLastFocusedStack         --  ActivityStack
   ||
   ||  ActivityStack createStackOnDisplay(int stackId, int displayId)
   ||  ActivityStack computeStackFocus(ActivityRecord r, boolean newTask)
   ||
   || ||
   || ||ActivityDisplay
   || ||
   || ||  mDisplayId             --  int
   || ||  mStacks                --  ArrayList<ActivityStack>
```

  ASS hold the list of ActivityDisplay, when invoke ASS.setWindowManager() or display device add
  will put new item in mActivityDisplays; every ActivityDisplay hold the list of ActivityStack, it
  arrange activity in the order in which each activity is opened;

  when invoke ASS.setWindowManager() will call createStackOnDisplay(), it will new a ActivityStack
  and put in Display.DEFAULT_DISPLAY ActivityDisplay; when invoke ASS.computeStackFocus() is also
  possible to create a new ActivityStack; 

```java
ActivityStack
   ||
   ||  mService                  --  ActivityManagerService
   ||  mWindowManager            --  WindowManagerService
   ||  mStackId                  --  int
   ||  mTaskHistory              --  ArrayList<TaskRecord>
   ||  mLRUActivities            --  ArrayList<ActivityRecord>
   ||
   ||  TaskRecord createTaskRecord(int taskId, ActivityInfo info, Intent intent,
   ||           IVoiceInteractionSession voiceSession, IVoiceInteractor voiceInteractor,
   ||           boolean toTop)
   ||
   ||  void startActivityLocked(ActivityRecord r, boolean newTask,
   ||           boolean doResume, boolean keepCurTransition, Bundle options)
```

  ActivityStack hold a list of TaskRecord in mTaskHistory, a task is a collection of activities that
  users interact with when performing a certain job; user can control start activity in a new task
  by set Intent.FLAG_ACTIVITY_NEW_TASK;

```java
 TaskRecord
   ||
   ||  taskId                    --  int
   ||  mActivities               --  ArrayList<ActivityRecord>
   ||  stack                     --  ActivityStack
   ||
   ||
```

```java
 ActivityRecord
   ||
   ||  state                     --  ActivityState
   ||  appToken                  --  IApplicationToken.Stub
   ||  task                      --  TaskRecord
   ||
```

  appToken instance in ActivityRecord() with ActivityRecord$Token extends IApplicationToken.Stub

```java
 ActivityThread
   ||
   ||  mActivities               --  ArrayMap<IBinder, ActivityClientRecord>
   ||
   ||
   || ||
   || ||ActivityClientRecord
   || ||
   || ||  activity               --  Activity
   || ||  parent                 --  Activity


```

```java
 Activity
   ||
   ||  mWindow                   --  Window
   ||
   ||
   ||
```

 **ACRONYM**
   ASS                           ActivityStackSupervisor

 **REFERENCE:**
   https://developer.android.com/guide/components/activities/tasks-and-back-stack.html

