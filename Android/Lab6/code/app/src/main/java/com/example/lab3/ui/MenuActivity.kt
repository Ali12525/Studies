package com.example.lab3.ui

import android.content.Intent
import android.net.Uri
import android.os.Bundle
import android.view.View
import androidx.lifecycle.lifecycleScope
import com.example.lab3.databinding.ActivityMenuBinding
import com.example.lab3.db.DatabaseHandler
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext

class MenuActivity : BaseActivity() {
    private lateinit var binding: ActivityMenuBinding
    private lateinit var db: DatabaseHandler
    private var userLogin: String? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMenuBinding.inflate(layoutInflater)
        setContentView(binding.root)

        db = DatabaseHandler(this)
        userLogin = intent.getStringExtra("user_login")
            ?: getSharedPreferences("session", MODE_PRIVATE).getString("current_login", null)
        loadUserData()
        setupClickListeners()
    }

    private fun loadUserData() {
        lifecycleScope.launch {
            try {
                val user = withContext(Dispatchers.IO) {
                    userLogin?.let { db.getUserByLogin(it) }
                }

                if (!isDestroyed && user != null) {
                    binding.btnAdmin.visibility = if (user.isAdmin == true) View.VISIBLE else View.GONE
                }
            } catch (e: Exception) {
                e.printStackTrace()
            }
        }
    }

    private fun setupClickListeners() {
        binding.btnProfile.setOnClickListener {
            val intent = Intent(this, ProfileActivity::class.java)
            intent.putExtra("user_login", userLogin)
            startActivity(intent)
        }


        binding.btnSettings.setOnClickListener {
            startActivity(Intent(this, SettingsActivity::class.java))
        }

        binding.btnReport.setOnClickListener {
            val intent = Intent(Intent.ACTION_SENDTO).apply {
                data = Uri.parse("mailto:support@example.com")
                putExtra(Intent.EXTRA_SUBJECT, "Сообщение о проблеме")
            }
            startActivity(intent)
        }

        binding.btnAdmin.setOnClickListener {
            startActivity(Intent(this, AdminActivity::class.java))
        }

        binding.btnMenuExit.setOnClickListener {
            getSharedPreferences("session", MODE_PRIVATE).edit().clear().apply()
            val intent = Intent(this, LoginActivity::class.java)
            startActivity(intent)
            finish()
        }

        binding.btnStart.setOnClickListener {
            startActivity(Intent(this, GameActivity::class.java))
        }
    }
}
