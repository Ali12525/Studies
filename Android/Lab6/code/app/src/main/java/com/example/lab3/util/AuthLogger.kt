package com.example.lab3.util

import android.content.Context
import java.io.File
import java.text.SimpleDateFormat
import java.util.*

object AuthLogger {
    private const val LOG_FILE = "auth_log.txt"
    private val sdf = SimpleDateFormat("yyyy-MM-dd HH:mm:ss", Locale.getDefault())

    fun log(context: Context, message: String) {
        try {
            val file = File(context.filesDir, LOG_FILE)
            val line = "${sdf.format(Date())} - $message\n"
            file.appendText(line)
        } catch (e: Exception) {
        }
    }

    fun readAll(context: Context): String {
        val file = File(context.filesDir, LOG_FILE)
        return if (file.exists()) file.readText() else ""
    }
}