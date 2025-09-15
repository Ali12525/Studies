package com.example.lab1

import android.app.Activity
import android.os.Bundle
import android.widget.Button
import android.widget.TextView

class HelloActivity : Activity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_helloact)

        val button1 = findViewById<Button>(R.id.button1)
        val button2 = findViewById<Button>(R.id.button2)
        val textCount1 = findViewById<TextView>(R.id.textCount1)
        val textCount2 = findViewById<TextView>(R.id.textCount2)
        val textCountTotal = findViewById<TextView>(R.id.textCountTotal)

        var countB1 = 0
        var countB2 = 0
        var countTotal = 0

        button1.setOnClickListener {
            countB1++
            countTotal++
            textCount1.text = countB1.toString()
            textCountTotal.text = countTotal.toString()
        }

        button2.setOnClickListener {
            countB2++
            countTotal++
            textCount2.text = countB2.toString()
            textCountTotal.text = countTotal.toString()
        }
    }
}