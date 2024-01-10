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
            "http://sns-video-exp1-qn.xhscdn.com",
            Qcdn.Strategy(
                "http://sns-video-exp4-qn.xhscdn.com",
                "http://sns-video-exp4-qn.xhscdn.com"
            )
        )
        val proxyUrl =
            qcdn.makeVodUrl("http://sns-video-exp1-qn.xhscdn.com/stream/110/408/01e4e8140c1269b5010370038a2af4a014_408.mp4")
        Log.i("Qcdn", "url: " + proxyUrl)

        Thread(object : Runnable {
            override fun run() {
                run {
                    val request: Request = Request.Builder().url(proxyUrl).build()
                    val response = OkHttpClient().newCall(request).execute()
                    Log.i("Qcdn", "res: " + response.message)
                }

                run {
                    val request: Request = Request.Builder().url(proxyUrl).build()
                    val response = OkHttpClient().newCall(request).execute()
                    Log.i("Qcdn", "res: " + response.message)
                }

            }
        }).start()
    }
}