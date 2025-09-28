package com.example.lab3.ui

import android.content.Context
import android.os.Bundle
import android.widget.Switch
import androidx.lifecycle.lifecycleScope
import com.example.lab3.R
import com.example.lab3.db.DatabaseHandler
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext

class SettingsActivity : BaseActivity() {

    private lateinit var switchTheme: Switch
    private lateinit var db: DatabaseHandler
    private var currentLogin: String? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_settings)

        db = DatabaseHandler(this)
        loadCurrentUser()
        initViews()
        setupThemeSwitch()
        loadUserTheme()
    }

    private fun loadCurrentUser() {
        val sessionPrefs = getSharedPreferences("session", Context.MODE_PRIVATE)
        currentLogin = sessionPrefs.getString("current_login", null)
    }

    private fun initViews() {
        switchTheme = findViewById(R.id.switchTheme)
    }

    private fun setupThemeSwitch() {
        switchTheme.setOnCheckedChangeListener { _, isChecked ->
            val newTheme = if (isChecked) 1 else 0

            lifecycleScope.launch(Dispatchers.IO) {
                currentLogin?.let { login ->
                    db.updateUserTheme(login, newTheme)
                }

                withContext(Dispatchers.Main) {
                    saveThemeToPreferences(isChecked)
                    applyTheme(isChecked)
                }
            }
        }
    }

    private fun loadUserTheme() {
        lifecycleScope.launch {
            try {
                val user = withContext(Dispatchers.IO) {
                    currentLogin?.let { db.getUserByLogin(it) }
                }

                if (user != null) {
                    switchTheme.isChecked = user.theme == 1
                    if (prefs.getBoolean("night_mode", false) != (user.theme == 1)) {
                        saveThemeToPreferences(user.theme == 1)
                    }
                }
            } catch (e: Exception) {
                e.printStackTrace()
            }
        }
    }

    private fun saveThemeToPreferences(isNight: Boolean) {
        prefs.edit().putBoolean("night_mode", isNight).apply()
    }
}