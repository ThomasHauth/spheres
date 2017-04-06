package com.onetwofivegames.spherestestbenchandroidvr;

import android.app.Activity;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Vibrator;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.widget.TextView;
import com.google.vr.ndk.base.AndroidCompat;
import com.google.vr.ndk.base.GvrLayout;
import com.onetwofivegames.spherestestbenchandroidvr.resources.FileBackend;
import com.onetwofivegames.spherestestbenchandroidvr.sound.SoundBackend;
import com.onetwofivegames.spherestestbenchandroidvr.exceptions.ResourceNotFound;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class SpheresVRActivity extends Activity {
    private GvrLayout gvrLayout;
    private long nativeFramework;

    private static String TAG = "SpheresVRActivity";

    static {
        System.loadLibrary("gvr");
        System.loadLibrary("gvr_audio");
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        JavaLog.info(TAG, "Setting up SpheresVR Activity");

        // create Java-side backends which will be used by the native C++ code
        m_fileBackend = new FileBackend(this);
        m_soundBackend = new SoundBackend(this);

        // Ensure fullscreen immersion.
        setImmersiveSticky();
        getWindow()
                .getDecorView()
                .setOnSystemUiVisibilityChangeListener(
                        new View.OnSystemUiVisibilityChangeListener() {
                            @Override
                            public void onSystemUiVisibilityChange(int visibility) {
                                if ((visibility & View.SYSTEM_UI_FLAG_FULLSCREEN) == 0) {
                                    setImmersiveSticky();
                                }
                            }
                        });

        // Initialize GvrLayout and the native renderer.
        gvrLayout = new GvrLayout(this);
        nativeFramework =
                nativeCreateFramework(
                        getClass().getClassLoader(),
                        this.getApplicationContext(),
                        gvrLayout.getGvrApi().getNativeGvrContext());

        // Add the GLSurfaceView to the GvrLayout.
        GLSurfaceView glSurfaceView = new GLSurfaceView(this);
        glSurfaceView.setEGLContextClientVersion(3);
        glSurfaceView.setEGLConfigChooser(8, 8, 8, 0, 0, 0);
        glSurfaceView.setPreserveEGLContextOnPause(true);
        glSurfaceView.setRenderer(
                new GLSurfaceView.Renderer() {
                    @Override
                    public void onSurfaceCreated(GL10 gl, EGLConfig config) {

                        // Our Android VR OpenGL backend needs to be created here, as this
                        // is the thread with the OpenGL conetxt
                        nativeInitializeGl(nativeFramework);
                    }

                    @Override
                    public void onSurfaceChanged(GL10 gl, int width, int height) {}

                    @Override
                    public void onDrawFrame(GL10 gl) {
                        nativeDrawFrame(nativeFramework);
                    }
                });
        glSurfaceView.setOnTouchListener(
                new View.OnTouchListener() {
                    @Override
                    public boolean onTouch(View v, MotionEvent event) {
                        if (event.getAction() == MotionEvent.ACTION_DOWN) {
                            // Give user feedback and signal a trigger event.
                            ((Vibrator) getSystemService(Context.VIBRATOR_SERVICE)).vibrate(50);
                            nativeOnTriggerEvent(nativeFramework);
                            return true;
                        }
                        return false;
                    }
                });
        gvrLayout.setPresentationView(glSurfaceView);

        // Add the GvrLayout to the View hierarchy.
        setContentView(gvrLayout);

        // Enable scan line racing.
        if (gvrLayout.setAsyncReprojectionEnabled(true)) {
            // Scanline racing decouples the app framerate from the display framerate,
            // allowing immersive interaction even at the throttled clockrates set by
            // sustained performance mode.
            AndroidCompat.setSustainedPerformanceMode(this, true);
        }

        // Enable VR Mode.
        AndroidCompat.setVrModeEnabled(this, true);

        // Prevent screen from dimming/locking.
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        JavaLog.info(TAG, "SpheresVR Activity setup complete");
    }

    @Override
    protected void onPause() {
        super.onPause();
        nativeOnPause(nativeFramework);
        gvrLayout.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        nativeOnResume(nativeFramework);
        gvrLayout.onResume();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        // Destruction order is important; shutting down the GvrLayout will detach
        // the GLSurfaceView and stop the GL thread, allowing safe shutdown of
        // native resources from the UI thread.
        gvrLayout.shutdown();
        nativeDestroyRenderer(nativeFramework);
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            setImmersiveSticky();
        }
    }

    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {
        // Avoid accidental volume key presses while the phone is in the VR headset.
        if (event.getKeyCode() == KeyEvent.KEYCODE_VOLUME_UP
                || event.getKeyCode() == KeyEvent.KEYCODE_VOLUME_DOWN) {
            return true;
        }
        return super.dispatchKeyEvent(event);
    }

    private void setImmersiveSticky() {
        getWindow()
                .getDecorView()
                .setSystemUiVisibility(
                        View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                                | View.SYSTEM_UI_FLAG_FULLSCREEN
                                | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
    }

    private native long nativeCreateFramework(
            ClassLoader appClassLoader, Context context, long nativeGvrContext);
    private native void nativeDestroyRenderer(long nativeTreasureHuntRenderer);
    private native void nativeInitializeGl(long nativeTreasureHuntRenderer);
    private native long nativeDrawFrame(long nativeTreasureHuntRenderer);
    private native void nativeOnTriggerEvent(long nativeTreasureHuntRenderer);
    private native void nativeOnPause(long nativeTreasureHuntRenderer);
    private native void nativeOnResume(long nativeTreasureHuntRenderer);



    FileBackend m_fileBackend = null;
    SoundBackend m_soundBackend = null;

    // this stuff is called from the native C++ side !

    // forwards to FileBackend
    public int loadImage(String imageName) {
        return m_fileBackend.loadTexture(imageName);
    }

    public String loadTextFile(String fileName) { return m_fileBackend.readTextFile(fileName);}

    public void freeTexture(int textureId) {
        m_fileBackend.freeTexture(textureId);
    }

    // forwards to SoundBackend
    public int playMusic(String name) throws ResourceNotFound {
        return m_soundBackend.playMusic(name);
    }

    public int playSound(String name, float direction) throws ResourceNotFound {
        return m_soundBackend.playSound(name, direction);
    }

    public void stopVibratePattern(String name) {
        //getVibrator().cancel();
    }

    public void stopAllViberatePatterns() {
        //getVibrator().cancel();
    }

    public void stopPlay(int playId) {
        m_soundBackend.stopPlay(playId);
    }

    public void pauseSound() {
        m_soundBackend.pauseSound();
    }

    public void resumeSound() {
        m_soundBackend.resumeSound();
    }
}
