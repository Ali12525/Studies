package com.example.lab3.ui

import android.content.Context
import android.os.Bundle
import androidx.lifecycle.lifecycleScope
import com.example.lab3.databinding.ActivitySettingsBinding
import com.example.lab3.db.DatabaseHandler
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext

class SettingsActivity : BaseActivity() {
    private lateinit var binding: ActivitySettingsBinding
    private lateinit var db: DatabaseHandler
    private var currentLogin: String? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivitySettingsBinding.inflate(layoutInflater)
        setContentView(binding.root)

        db = DatabaseHandler(this)
        loadCurrentUser()
        setupThemeSwitch()
        loadUserTheme()
    }

    private fun loadCurrentUser() {
        val sessionPrefs = getSharedPreferences("session", Context.MODE_PRIVATE)
        currentLogin = sessionPrefs.getString("current_login", null)
    }

    private fun setupThemeSwitch() {
        binding.switchTheme.setOnCheckedChangeListener { _, isChecked ->
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
                    binding.switchTheme.isChecked = user.theme == 1
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