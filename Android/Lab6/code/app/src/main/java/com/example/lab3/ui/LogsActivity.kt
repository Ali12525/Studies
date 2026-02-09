package com.example.lab3.ui

import android.os.Bundle
import com.example.lab3.databinding.ActivityLogsBinding
import com.example.lab3.util.AuthLogger

class LogsActivity : BaseActivity() {
    private lateinit var binding: ActivityLogsBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityLogsBinding.inflate(layoutInflater)
        setContentView(binding.root)

        binding.tvLogs.text = AuthLogger.readAll(this).ifEmpty { "Логов нет" }
        setupClickListeners()
    }

    private fun setupClickListeners() {
        binding.btnBackLogs.setOnClickListener { finish() }
    }
}