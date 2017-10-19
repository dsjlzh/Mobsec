package com.gerald.mobsec

import android.support.v7.app.AppCompatActivity
import android.os.Bundle
//import android.text.*
//import android.text.InputType.TYPE_CLASS_TEXT
//import android.text.InputType.TYPE_TEXT_VARIATION_PASSWORD
//import android.view.Gravity
//import android.widget.TextView
//import android.widget.Button
//import android.widget.EditText

import org.jetbrains.anko.*
import org.jetbrains.anko.sdk25.coroutines.onClick
import kotlinx.android.synthetic.main.activity_main.*


class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        btn_scan.onClick {
            val msg = resultFromJNI()
            toast(msg)
        }
    }

    // keyword external in kotlin same as native in java
    external fun resultFromJNI(): String

    /* https://medium.com/@vanniktech/java-vs-kotlin-static-initializer-block-269c4902c439 */
    companion object {
        init {
            System.loadLibrary("hello-jni")
        }
    }

}
