package com.example.lab3.ui

import android.content.Intent
import android.net.Uri
import android.os.Bundle
import android.view.View
import android.widget.Button
import androidx.lifecycle.lifecycleScope
import com.example.lab3.R
import com.example.lab3.db.DatabaseHandler
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext

class MenuActivity : BaseActivity() {

    private lateinit var btnStart: Button
    private lateinit var btnProfile: Button
    private lateinit var btnSettings: Button
    private lateinit var btnReport: Button
    private lateinit var btnMenuExit: Button
    private lateinit var btnAdmin: Button
    private lateinit var db: DatabaseHandler
    private var userLogin: String? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_menu)

        db = DatabaseHandler(this)
        initViews()
        loadUserData()
        setupClickListeners()
    }

    private fun initViews() {
        btnStart = findViewById(R.id.btnStart)
        btnProfile = findViewById(R.id.btnProfile)
        btnSettings = findViewById(R.id.btnSettings)
        btnReport = findViewById(R.id.btnReport)
        btnMenuExit = findViewById(R.id.btnMenuExit)
        btnAdmin = findViewById(R.id.btnAdmin)

        userLogin = intent.getStringExtra("user_login")
            ?: getSharedPreferences("session", MODE_PRIVATE).getString("current_login", null)
    }

    private fun loadUserData() {
        lifecycleScope.launch {
            try {
                val user = withContext(Dispatchers.IO) {
                    userLogin?.let { db.getUserByLogin(it) }
                }

                if (!isDestroyed && user != null) {
                    btnAdmin.visibility = if (user.isAdmin == true) View.VISIBLE else View.GONE

                    val currentTheme = prefs.getBoolean("night_mode", false)
                    val userTheme = (user.theme == 1)
                    if (currentTheme != userTheme) {
                        saveThemeToPreferences(userTheme)
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

    private fun setupClickListeners() {
        btnProfile.setOnClickListener {
            val intent = Intent(this, ProfileActivity::class.java)
            intent.putExtra("user_login", userLogin)
            startActivity(intent)
        }


        btnSettings.setOnClickListener {
            startActivity(Intent(this, SettingsActivity::class.java))
        }

        btnReport.setOnClickListener {
            val intent = Intent(Intent.ACTION_SENDTO).apply {
                data = Uri.parse("mailto:support@example.com")
                putExtra(Intent.EXTRA_SUBJECT, "Сообщение о проблеме")
            }
            startActivity(intent)
        }

        btnAdmin.setOnClickListener {
            startActivity(Intent(this, AdminActivity::class.java))
        }

        btnMenuExit.setOnClickListener {
            getSharedPreferences("session", MODE_PRIVATE).edit().clear().apply()
            val intent = Intent(this, LoginActivity::class.java)
            startActivity(intent)
            finish()
        }
    }
}
