package com.example.kotlinqcdn

import android.os.Bundle
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import okhttp3.OkHttpClient
import okhttp3.Request


class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val qcdn = Qcdn(null)
        qcdn.setStrategy(
            "http://base.com",
            Qcdn.Strategy(
                "http://back.com",
                "http://bacp.com"
            )
        )
        val proxyUrl =
            qcdn.makeVodUrl("http://base.com/test.mp4")
        Log.i("Test", "url: " + proxyUrl)

        Thread(object : Runnable {
            override fun run() {
                run {
                    val request: Request = Request.Builder().url(proxyUrl).build()
                    val response = OkHttpClient().newCall(request).execute()
                    Log.i("Test", "res: " + response.message)
                }

                run {
                    val request: Request = Request.Builder().url(proxyUrl).build()
                    val response = OkHttpClient().newCall(request).execute()
                    Log.i("Test", "res: " + response.message)
                }

            }
        }).start()
    }
}
