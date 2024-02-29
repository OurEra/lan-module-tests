##  When and where system send ACTION_LOCKED_BOOT_COMPLETED/IACTION_BOOT_COMPLETED
- In Android 7, the ACTION_LOCKED_BOOT_COMPLETED(API 24) broadcast will send in UserController.finishUserBoot(),
   ACTION_BOOT_COMPLETED broadcast will send in UserController.finishUserUnlockedCompleted();

-  In Android 6, the ACTION_BOOT_COMPLETED broadcast will send in ActivityManagerService.finishBooting();
   Application should have permission and filter broadcast to receive it:

  ```xml
   <uses-permission android:name="android.permission.RECEIVE_BOOT_COMPLETED" />
  
   <receiver android:name=".MyReceiver">
       <intent-filter>
           <action android:name="android.intent.action.BOOT_COMPLETED" />
           <action android:name="android.intent.action.LOCKED_BOOT_COMPLETED" />
       </intent-filter>
   </receiver>
  ```

Although system will bring up the process when send the broadcast, but ActivityManagerService may just   kill it since that process have no other jobs:

```java
1704 D BOOTDEBUG: collect receivers pack com.srw.realdemo
1704 I ActivityManager: Start proc 3670:com.srw.realdemo/u0a86 for broadcast com.srw.realdemo/.MyReceiver
1921 I ActivityManager: Killing 3670:com.srw.realdemo/u0a86 (adj 906): empty #4

ProcessRecord
void kill(String reason, boolean noisy) {
    if (!killedByAm) {
        Trace.traceBegin(Trace.TRACE_TAG_ACTIVITY_MANAGER, "kill");
        if (noisy) {
            Slog.i(TAG, "Killing " + toShortString() + " (adj " + setAdj + "): " + reason);
        }
        EventLog.writeEvent(EventLogTags.AM_KILL, userId, pid, processName, setAdj, reason);
        Process.killProcessQuiet(pid);
        ActivityManagerService.killProcessGroup(uid, pid);
        if (!persistent) {
            killed = true;
            killedByAm = true;
        }
        Trace.traceEnd(Trace.TRACE_TAG_ACTIVITY_MANAGER);
    }
}
```


## About odex and oat
> https://blog.csdn.net/yangwen123/article/details/18409791

```java
         javac               dx             dexopt
 *.java  ------>  *.class  ------>  *.dex  ------>  *.odex

```

```java
         javac               dx             de2oat
 *.java  ------>  *.class  ------>  *.dex  ------>  *.oat

```

 *.odex is byte code and used by Dalvik JVM, it still need
 convert to native machine code when execute(JIT);
 *.oat is machine code that converted when install(AOT), and
 execute by ART JVM;

## About matrix

>  https://blog.csdn.net/xiexiangyu92/article/details/79382650

 kMScaleX    kMSkewX     kMTransX
 kMSkewY     kMScaleY    kMTransY
 kMPersp0    kMPersp1    kMPersp2


##  About Settings
  settings content store in xml file under /data:
  settings_global.xml
  settings_system.xml
  settings_secure.xml

  details in SettingsProvider.java, the default value still handle in
  com.android.providers.settings.DatabaseHelper and migrate into xml
  file in function SettingsRegistry.migrateAllLegacySettingsIfNeeded(),
  after migrate database will drop when DROP_DATABASE_ON_MIGRATION is true.

## About DM-VERITY:

> https://nelenkov.blogspot.com/2014/05/using-kitkat-verified-boot.html


## Handler & Looper


## AsyncChannel
 CMD_CHANNEL_HALF_CONNECTED
 CMD_CHANNEL_FULL_CONNECTION
 CMD_CHANNEL_FULLY_CONNECTED
 CMD_CHANNEL_DISCONNECT
 CMD_CHANNEL_DISCONNECTED

 See details in demoapp


## StateMachine

 See details in demoapp

## property

 In general, property use mmap to share name&value between processes,
 and only allow write by init process, other process want to write a
 name&value will send command to init process through socket.

```c++
       init   <----------------write----------------   other
              v
  /dev/socket/property_service

  mmap & PROT_READ | PROT_WRITE                 mmap & PROT_READ
              ^                                    /
  -------------\----------------------------------/---------------
          read  \  write                         / read
                 v                              v
                      /dev/__properties__/xxx

```
- 1 init process:
    - property_init()
    invoke __system_property_area_init() to mmap bunch of r/w address
    based on file /property_contexts
    - property_load_boot_defaults()
    update name&value from property file
    - start_property_service()
    create socket and listen

- 2 other process:
    - __system_properties_init()
    invoke in linker, init process specific property environment
    - property_set() / property_get()
    api provide by libcutils


 Property organize all the name&value in a hybrid trie/binary tree structure:

 ```c++
  +-----+   children    +----+   children    +--------+
  |     |-------------->| ro |-------------->| secure |
  +-----+               +----+               +--------+
                        /    \                /   |
                  left /      \ right   left /    |  prop   +===========+
                      v        v            v     +-------->| ro.secure |
                   +-----+   +-----+     +-----+            +-----------+
                   | net |   | sys |     | com |            |     1     |
                   +-----+   +-----+     +-----+            +===========+

 ```

- 1 Based on content in /property_contexts, every different context will have a
    share memory file under dir /dev/__properties__/:
    u:object_r:default_prop:s0
    u:object_r:media_settings_xml_prop:s0
    u:object_r:safemode_prop:s0
    ...

    besides, /property_contexts also describe all property prefix, two prefix can share
    the same context;

- 2 The related data structure:
    - 2.1 Every context share memory will have (128 * 1024) byte space, and
    placement new 'prop_area', infos stored start from prop_area.data_;
    - 2.2 Every name&value stored in 'prop_info', prop_area::new_prop_info()
    will use space start from prop_area.data_ +  prop_area.bytes_used_;
    - 2.3 To read/write 'prop_info', must find the corresponding 'prop_bt',
    that represents a node in the trie structure; prop_area::find_prop_bt()
    have the details, 'prop_bt' also use a prop_area::new_prop_bt() to
    placement new where under prop_area.data_ +  prop_area.bytes_used_;
    - 2.4 A 'prop_area' and context name store in 'context_node', it can
    be find from global field 'static context_node* contexts';

    based on property prefix, 'prefix_node' stored the prefix and it's
    'context_node', global field 'static prefix_node* prefixes' help
    find one spefic 'context_node';

```c++
    context_node
    | bool open(bool access_rw, bool* fsetxattr_failed)
    | context_node* next
    | char* context_
    | prop_area* pa_

    prefix_node
    | char* prefix
    | context_node* context
    | struct prefix_node* next

    prop_info
    | char value[PROP_VALUE_MAX]
    | char name[0]

    prop_bt
    | char name[0]
    | atomic_uint_least32_t prop
    | atomic_uint_least32_t left
    | atomic_uint_least32_t right
    | atomic_uint_least32_t children

    prop_area
    | uint32_t bytes_used_
    | char data_[0]
    | bool add(const char *name, unsigned int namelen,
    |         const char *value, unsigned int valuelen)

```

## A java Ixxx.Stub start and user get xxx instance

Take IWifiManager.Stub for example:
- 1. SystemServer

   SystemServiceManager.startService(WIFI_SERVICE_CLASS) // "com.android.server.wifi.WifiService"

- 2. WifiService extends SystemService

   new WifiServiceImpl

   publishBinderService(Context.WIFI_SERVICE, mImpl)

- 3. publish : ServiceManager.addService()

- 4. SystemServiceRegistry

   registerService(Context.WIFI_SERVICE, WifiManager.class,
           new CachedServiceFetcher<WifiManager>() {
       @Override
       public WifiManager createService(ContextImpl ctx) {
           IBinder b = ServiceManager.getService(Context.WIFI_SERVICE);
           IWifiManager service = IWifiManager.Stub.asInterface(b);
           return new WifiManager(ctx.getOuterContext(), service,
                   ConnectivityThread.getInstanceLooper());
       }});

- 5. ContextImpl.getSystemService:

   SystemServiceRegistry.getSystemService(this, name)

User then get the WifiManager instance.

