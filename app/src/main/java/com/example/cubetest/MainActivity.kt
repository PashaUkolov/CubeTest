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
                handleMoveAction(event.x, event.y)
            }
            MotionEvent.ACTION_DOWN -> {
                handleDownAction(event.x, event.y)
            }
            MotionEvent.ACTION_UP -> {
                handleUpAction(event.x, event.y)
            }
        }
        return super.onTouchEvent(event)
    }

    external fun handleMoveAction(x: Float, y: Float);
    external fun handleDownAction(x: Float, y: Float);
    external fun handleUpAction(x: Float, y: Float);
}