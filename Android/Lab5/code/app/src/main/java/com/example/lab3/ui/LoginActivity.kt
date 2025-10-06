package com.example.lab3.ui

import android.content.Intent
import android.os.Bundle
import android.util.Log
import android.widget.Toast
import com.example.lab3.util.AuthLogger
import androidx.lifecycle.lifecycleScope
import com.example.lab3.databinding.ActivityLoginBinding
import com.example.lab3.db.DatabaseHandler
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext

class LoginActivity : BaseActivity() {
    private lateinit var binding: ActivityLoginBinding
    private lateinit var db: DatabaseHandler

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityLoginBinding.inflate(layoutInflater)
        setContentView(binding.root)
        Log.d("LoginActivity", "onCreate")

        if (tryAutoLogin()) return

        db = DatabaseHandler(this)
        restoreState(savedInstanceState)
        setupClickListeners()
    }

    private fun tryAutoLogin(): Boolean {
        val sharedPreferences = getSharedPreferences("session", MODE_PRIVATE)
        val savedLogin = sharedPreferences.getString("current_login", null)

        if (savedLogin != null) {
            Log.d("LoginActivity", "Auto-login for user: $savedLogin")
            AuthLogger.log(this, "Auto-login for user: $savedLogin")

            val intent = Intent(this, MenuActivity::class.java)
            intent.putExtra("user_login", savedLogin)
            startActivity(intent)
            finish()
            return true
        }
        return false
    }

    private fun restoreState(savedInstanceState: Bundle?) {
        savedInstanceState?.let {
            binding.loginEditText.setText(it.getString("login"))
            binding.passwordEditText.setText(it.getString("password"))
        }
    }

    private fun setupClickListeners() {
        binding.btnLogin.setOnClickListener {
            val login = binding.loginEditText.text.toString().trim()
            val password = binding.passwordEditText.text.toString().trim()

            if (login.isEmpty() || password.isEmpty()) {
                Toast.makeText(this, "Введите логин и пароль", Toast.LENGTH_SHORT).show()
                return@setOnClickListener
            }

            lifecycleScope.launch {
                val user = withContext(Dispatchers.IO) {
                    db.getUser(login, password)
                }

                if (user != null) {
                    getSharedPreferences("session", MODE_PRIVATE)
                        .edit().putString("current_login", user.login).apply()

                    saveThemeToPreferences(user.theme == 1)

                    AuthLogger.log(this@LoginActivity, "Login success: ${user.login} (admin=${user.isAdmin})")

                    val intent = Intent(this@LoginActivity, MenuActivity::class.java)
                    intent.putExtra("user_login", user.login)
                    startActivity(intent)
                    finish()
                } else {
                    AuthLogger.log(this@LoginActivity, "Login failed: $login")
                    Toast.makeText(this@LoginActivity, "Неверный логин или пароль", Toast.LENGTH_SHORT).show()
                }
            }
        }

        binding.btnRegister.setOnClickListener {
            val intent = Intent(this, RegisterActivity::class.java)
            startActivity(intent)
        }

        binding.btnExit.setOnClickListener {
            finish()
        }
    }

    private fun saveThemeToPreferences(isNight: Boolean) {
        prefs.edit().putBoolean("night_mode", isNight).apply()
    }

    override fun onSaveInstanceState(outState: Bundle) {
        super.onSaveInstanceState(outState)
        outState.putString("login", binding.btnLogin.text.toString())
        outState.putString("password", binding.passwordEditText.text.toString())
    }

    override fun onStart() { super.onStart(); Log.d("LoginActivity", "onStart") }
    override fun onResume() { super.onResume(); Log.d("LoginActivity", "onResume") }
    override fun onPause() { super.onPause(); Log.d("LoginActivity", "onPause") }
    override fun onStop() { super.onStop(); Log.d("LoginActivity", "onStop") }
    override fun onDestroy() { super.onDestroy(); Log.d("LoginActivity", "onDestroy") }
    override fun onRestart() { super.onRestart(); Log.d("LoginActivity", "onRestart") }
}