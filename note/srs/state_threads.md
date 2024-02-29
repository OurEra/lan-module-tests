```
st_init()
    _st_this_vp.idle_thread = st_thread_create(_st_idle_thread_start, NULL, 0, 0, "idle thread");

    thread = (_st_thread_t *) calloc(1, sizeof(_st_thread_t) + (ST_KEYS_MAX * sizeof(void *)));
    if (!thread)
        return -1;
    thread->private_data = (void **) (thread + 1);
    thread->state = _ST_ST_RUNNING;
    thread->flags = _ST_FL_PRIMORDIAL;
    strcpy(thread->name, "Init Thread");
    _ST_SET_CURRENT_THREAD(thread);
    _st_active_count++;
#ifdef DEBUG
    _ST_ADD_THREADQ(thread);
#endif
```
> 初始化时，初始化一个 idle 线程，供后续调度使用； 同时，创建一个裸的 thread 对象添加到队列；
> 第一次 thread 被调度的时机，需要等到第一个包含 _ST_SWITCH_CONTEXT 的阻塞函数来触发，比如:
> st_poll()

```
_ST_INIT_CONTEXT

#define MD_INIT_CONTEXT(_thread, _sp, _main) \
    ST_BEGIN_MACRO                             \
    if (MD_SETJMP((_thread)->context))         { \
        _main();                                \
    }                                           \
    MD_GET_SP(_thread) = (long) (_sp);         \
    ST_END_MACRO
```

> 调用 st_thread_create 创建 thread 对象时，将会设置 jump 返回执行 _main；触发时机为当前 thread 在 RUNQ 中被调度
> _main 实际上是一个公共入口，为 _st_thread_main，在其中才会调用用户设置的 thread->start


```
_ST_SWITCH_CONTEXT

#define _ST_SWITCH_CONTEXT(_thread)       \
    ST_BEGIN_MACRO                        \
    ST_SWITCH_OUT_CB(_thread);            \
    if (!MD_SETJMP((_thread)->context)) { \
        _st_vp_schedule();                  \
    }                                     \
    ST_DEBUG_ITERATE_THREADS();           \
    ST_SWITCH_IN_CB(_thread);             \
    ST_END_MACRO

```

```
void _st_vp_schedule(void)
{
    _st_thread_t *thread;
    
    if (_ST_RUNQ.next != &_ST_RUNQ) {
        #if defined(DEBUG) && defined(DEBUG_STATS)
        ++_st_stat_thread_run;
        #endif

        /* Pull thread off of the run queue */
        thread = _ST_THREAD_PTR(_ST_RUNQ.next);
        _ST_DEL_RUNQ(thread);
    } else {
        #if defined(DEBUG) && defined(DEBUG_STATS)
        ++_st_stat_thread_idle;
        #endif

        /* If there are no threads to run, switch to the idle thread */
        thread = _st_this_vp.idle_thread;
    }
    ST_ASSERT(thread->state == _ST_ST_RUNNABLE);
    
    /* Resume the thread */
    thread->state = _ST_ST_RUNNING;
    _ST_RESTORE_CONTEXT(thread);
}
```

> _ST_SWITCH_CONTEXT 就是设置当前 thread 的 jump 点，然后使用 _st_vp_schedule 来 jump 到 RUNQ 中下一个 thread; 当前 thread 返回时继续执行
> 触发时机主要在阻塞函数中调用，去切换其他 thread；或者在 idle 线程中 _st_idle_thread_start 来触发


```
_ST_RESTORE_CONTEXT
#define _ST_RESTORE_CONTEXT(_thread)   \
    ST_BEGIN_MACRO                     \
    _ST_SET_CURRENT_THREAD(_thread);   \
    MD_LONGJMP((_thread)->context, 1); \
    ST_END_MACRO
```

> _ST_RESTORE_CONTEXT 被封装在函数 _st_vp_schedule 中，实际就是找到需要调度的 thread 后去 jump 到 thread 之前设置的 jump 点，完成类似调度的功能
