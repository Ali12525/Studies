package com.example.lab2

import android.app.Activity
import android.os.Bundle
import android.widget.ArrayAdapter
import android.widget.Button
import android.widget.ListView

class HelloActivity: Activity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_helloact)

        val button1 = findViewById<Button>(R.id.button1)
        button1.setOnClickListener {
            button1.text = "НАЖАТО!"
        }

        val myStringArray = ArrayList<String>()
        myStringArray.add("Element_0")
        myStringArray.add("Element_1")
        myStringArray.add("Element_2")
        myStringArray.add("Element_3")
        val textAdapter: ArrayAdapter<String> = ArrayAdapter(this, R.layout.item,
            R.id.itemContent, myStringArray)
        val textList = findViewById<ListView>(R.id.textList)
        textList.adapter = textAdapter
    }
}