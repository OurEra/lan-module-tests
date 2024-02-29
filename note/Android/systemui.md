## OVERVIEW (Android 6.0.1)


```java
CommandQueue extends IStatusBar.Stub
   ||
   ||  mCallbacks                --  Callbacks
   ||
   ||  void setIcon(int index, StatusBarIcon icon)
   ||  void removeIcon(int index)
   ||
   || ||
   || ||Callbacks
   || ||
   || ||  void addIcon(String slot, int index, int viewIndex, StatusBarIcon icon)
   || ||  void removeIcon(String slot, int index, int viewIndex)
   || ||  ...
```

```java
 BaseStatusBar extends SystemUI implements
   ||       CommandQueue.Callbacks,
   ||       ActivatableNotificationView.OnActivatedListener,
   ||       RecentsComponent.Callbacks,
   ||       ExpandableNotificationRow.ExpansionLogger,
   ||       NotificationData.Environment
   ||
   ||  mCommandQueue             --  CommandQueue
   ||  mBarService               --  IStatusBarService
   ||
   ||
   ||
```

```java
 PhoneStatusBar extends BaseStatusBar implements DemoMode,
   ||     DragDownHelper.DragDownCallback,
   ||     ActivityStarter,
   ||     OnUnlockMethodChangedListener,
   ||     HeadsUpManager.OnHeadsUpChangedListener
   ||
   ||  mIconPolicy               --  PhoneStatusBarPolicy
   ||
   ||  PhoneStatusBarView makeStatusBarView()
   ||  void addNavigationBar()
   ||
```

```java
 StatusBarManagerService extends IStatusBarService.Stub
   ||
   ||  mBar                      --  IStatusBar
   ||  mIcons                    --  StatusBarIconList
   ||
   ||  void setIcon(String slot, String iconPackage, int iconId,
   ||        int iconLevel, String contentDescription)
   ||
   ||  void registerStatusBar(IStatusBar bar, StatusBarIconList iconList,
   ||         int switches[], List<IBinder> binders)
   ||
   ||
```

  mIcons read from array config_statusBarIcons from config.xml


 **REFERRER**
   https://blog.csdn.net/zhudaozhuan/article/details/50829736
   http://wpf814533631.iteye.com/blog/1585085
