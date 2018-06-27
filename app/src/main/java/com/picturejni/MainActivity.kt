package com.picturejni

import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.view.Surface
import android.view.SurfaceHolder
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity(), SurfaceHolder.Callback {
    lateinit var srcBitmap: Bitmap
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // Example of a call to a native method
        sample_text.text = stringFromJNI()

        srcBitmap = BitmapFactory.decodeResource(resources, R.mipmap.hh)
        var desBitmap = Bitmap.createBitmap(srcBitmap.width, srcBitmap.height, Bitmap.Config.ARGB_8888)
        copyBitmap(srcBitmap, desBitmap)
        ivBitmap.setImageBitmap(desBitmap)

        surfaceView.layoutParams.width = srcBitmap.width
        surfaceView.layoutParams.height = srcBitmap.height
        surfaceView.holder.addCallback(this)
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String
    external fun copyBitmap(srcBitmap: Bitmap, desBitmap: Bitmap)
    external fun jniSurface(surface: Surface, srcBitmap: Bitmap)

    companion object {

        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
    }

    override fun surfaceChanged(holder: SurfaceHolder?, format: Int, width: Int, height: Int) {
        jniSurface(holder!!.surface, srcBitmap)
    }

    override fun surfaceDestroyed(holder: SurfaceHolder?) {
    }

    override fun surfaceCreated(holder: SurfaceHolder?) {

    }
}
