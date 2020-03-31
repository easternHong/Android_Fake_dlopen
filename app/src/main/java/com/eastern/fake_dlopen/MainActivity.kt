package com.eastern.fake_dlopen

import android.os.Bundle
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import com.eastern.hook.What

class MainActivity : AppCompatActivity() {


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // Example of a call to a native method
        val tv = findViewById<TextView>(R.id.sample_text)
        tv.setOnClickListener { Thread.sleep(6000) }
        val what = What()
        what.doHook()
        what.testLog()
        what.hookDumpAll()
    }
}