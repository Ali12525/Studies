package com.example.lab3.ui

import android.content.Context
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.appcompat.app.AppCompatDelegate

open class BaseActivity : AppCompatActivity() {
    protected lateinit var prefs: android.content.SharedPreferences

    override fun onCreate(savedInstanceState: Bundle?) {
        prefs = getSharedPreferences("settings", Context.MODE_PRIVATE)
        applyThemeFromPrefs()
        super.onCreate(savedInstanceState)
    }

    protected fun applyThemeFromPrefs() {
        val isNight = prefs.getBoolean("night_mode", false)
        applyTheme(isNight)
    }

    protected fun applyTheme(isNight: Boolean) {
        AppCompatDelegate.setDefaultNightMode(
            if (isNight) AppCompatDelegate.MODE_NIGHT_YES
            else AppCompatDelegate.MODE_NIGHT_NO
        )
    }
}