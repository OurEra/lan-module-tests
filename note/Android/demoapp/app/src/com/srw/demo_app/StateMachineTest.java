package com.srw.demo_app;

import android.os.Handler;
import android.os.Message;
import android.util.Log;

import com.android.internal.util.State;
import com.android.internal.util.StateMachine;

public class StateMachineTest extends StateMachine {

    private static final String TAG = "srwTestApp";

    private InitialState mInitialState;
    private IdleState mIdelState;
    private ErrorState mErrorState;
    private ActiveState  mActiveState;
    private ActiveC1State mActiveC1State;
    private ActiveC2State mActiveC2State;

    static final int SM_CMD_INIT        = 1;
    static final int SM_CMD_START       = 2;
    static final int SM_CMD_START_ACT1  = 3;
    static final int SM_CMD_START_ACT2  = 4;
    static final int SM_CMD_STOP        = 5;
    static final int SM_CMD_DUMMY       = 6;

    private static StateMachineTest gSm;
    public static StateMachineTest makeSM() {

        if (gSm == null)
            gSm = new StateMachineTest();
        return gSm;
    }

    /**
     *         InitialState                 ErrorState
     *      /               \
     *     ActiveStat         IdleStatee
     *   /           \
     * ActiveC1State ActiveC2State
     *
     **/
    private StateMachineTest() {
        super("StateMachineTest");

        mInitialState = new InitialState();
        addState(mInitialState);

        mActiveState = new ActiveState();
        addState(mActiveState, mInitialState);

        mIdelState = new IdleState();
        addState(mIdelState, mInitialState);

        mActiveC1State = new ActiveC1State();
        mActiveC2State = new ActiveC2State();
        addState(mActiveC1State, mActiveState);
        addState(mActiveC2State, mActiveState);

        mErrorState = new ErrorState();
        addState(mErrorState);

        setInitialState(mActiveC2State);
    }

    public void destory() {
        gSm.quit();
    }

    @Override
    protected void onHalting() {

        Log.d(TAG, "StateMachineTest onHalting");
    }

    @Override
    protected void onQuitting() {

        Log.d(TAG, "StateMachineTest onQuitting");
    }

    class InitialState extends State {

        @Override
        public void enter() {

            Log.d(TAG, "StateMachineTest InitialState enter");
        }

        @Override
        public boolean processMessage(Message message) {

            Log.d(TAG, "StateMachineTest InitialState processMessage " + message.what);
            return true;
        }

        @Override
        public void exit() {

            Log.d(TAG, "StateMachineTest InitialState exit");
        }
    }

    class IdleState extends State {

        @Override
        public void enter() {

            Log.d(TAG, "StateMachineTest IdleState enter");
        }

        @Override
        public boolean processMessage(Message message) {

            Log.d(TAG, "StateMachineTest IdleState processMessage " + message.what);
            return true;
        }

        @Override
        public void exit() {

            Log.d(TAG, "StateMachineTest IdleState exit");
        }
    }

    class ErrorState extends State {

        @Override
        public void enter() {

            Log.d(TAG, "StateMachineTest ErrorState enter");
        }

        @Override
        public boolean processMessage(Message message) {

            Log.d(TAG, "StateMachineTest ErrorState processMessage " + message.what);
            return true;
        }

        @Override
        public void exit() {

            Log.d(TAG, "StateMachineTest ErrorState exit");
        }
    }


    class ActiveState extends State {

        @Override
        public void enter() {

            Log.d(TAG, "StateMachineTest ActiveState enter");
        }

        @Override
        public boolean processMessage(Message message) {

            Log.d(TAG, "StateMachineTest ActiveStat processMessage " + message.what);
            return true;
        }

        @Override
        public void exit() {

            Log.d(TAG, "StateMachineTest ActiveStat exit");
        }
    }

    class ActiveC1State extends State {

        @Override
        public void enter() {

            Log.d(TAG, "StateMachineTest ActiveC1State enter");
        }

        @Override
        public boolean processMessage(Message message) {

            Log.d(TAG, "StateMachineTest ActiveC1State processMessage " + message.what);
            return true;
        }

        @Override
        public void exit() {

            Log.d(TAG, "StateMachineTest ActiveC1State exit");
        }
    }

    class ActiveC2State extends State {

        @Override
        public void enter() {

            Log.d(TAG, "StateMachineTest ActiveC2State enter");
        }

        @Override
        public boolean processMessage(Message message) {

            Log.d(TAG, "StateMachineTest ActiveC2State processMessage " + message.what);
            return true;
        }

        @Override
        public void exit() {

            Log.d(TAG, "StateMachineTest ActiveC2State exit");
        }
    }

}
