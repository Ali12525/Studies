package com.example.lab3.ui

import android.os.Bundle
import android.widget.Button
import android.widget.TextView
import com.example.lab3.R
import com.example.lab3.util.AuthLogger

class LogsActivity : BaseActivity() {
    private lateinit var tvLogs: TextView
    private lateinit var btnBack: Button

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_logs)

        initViews()
        tvLogs.text = AuthLogger.readAll(this).ifEmpty { "Логов нет" }
        setupClickListeners()
    }

    private fun initViews() {
        tvLogs = findViewById(R.id.tvLogs)
        btnBack = findViewById(R.id.btnBackLogs)
    }

    private fun setupClickListeners() {
        btnBack.setOnClickListener { finish() }
    }
}