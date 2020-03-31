package com.eastern.hook;

public class What {
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }
    public native void sysDump();
    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native void  testDlopen();
    public native void  dumpToLog();
    public native void  invokeFunPoint();
    public native void  testLog();
    public native void  doHook();
    public native void  doUnHook();
    public native void  hookDumpAll();
}
