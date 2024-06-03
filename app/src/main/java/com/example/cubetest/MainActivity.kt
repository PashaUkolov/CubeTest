package com.example.cubetest

import android.os.Bundle
import android.view.MotionEvent
import android.app.NativeActivity

class MainActivity : NativeActivity() {
    companion object {
        init {
            System.loadLibrary("native-activity")
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
    }

    private var previousX: Float = 0f
    private var previousY: Float = 0f

    override fun onTouchEvent(event: MotionEvent): Boolean {
        val x: Float = event.x
        val y: Float = event.y

        when (event.action) {
            MotionEvent.ACTION_MOVE -> {
                handleTouchEvent(event.x, event.y)
            }
        }
        return super.onTouchEvent(event)
    }

    external fun handleTouchEvent(x: Float, y: Float);
}