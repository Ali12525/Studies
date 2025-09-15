package com.example.lab2

import android.app.Activity
import android.os.Bundle
import android.widget.Button

class HelloActivity: Activity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_helloact)

        val button0 = findViewById<Button>(R.id.button0)
        button0.setOnClickListener {
            button0.text = "НАЖАТО!"
        }
    }
}