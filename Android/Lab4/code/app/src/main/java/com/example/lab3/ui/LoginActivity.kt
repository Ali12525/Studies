package com.example.lab3.ui

import android.content.Intent
import android.os.Bundle
import android.util.Log
import android.widget.Button
import android.widget.EditText
import android.widget.ImageView
import android.widget.Toast
import com.example.lab3.util.AuthLogger
import androidx.lifecycle.lifecycleScope
import com.example.lab3.R
import com.example.lab3.db.DatabaseHandler
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext

class LoginActivity : BaseActivity() {
    private lateinit var loginEditText: EditText
    private lateinit var passwordEditText: EditText
    private lateinit var btnLogin: Button
    private lateinit var btnRegister: Button
    private lateinit var btnExit: Button
    private lateinit var logoImage: ImageView
    private lateinit var db: DatabaseHandler

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_login)
        Log.d("LoginActivity", "onCreate")

        if (tryAutoLogin()) {
            return
        }

        db = DatabaseHandler(this)
        initViews()
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

    private fun initViews() {
        logoImage = findViewById(R.id.logoImage)
        loginEditText = findViewById(R.id.loginEditText)
        passwordEditText = findViewById(R.id.passwordEditText)
        btnLogin = findViewById(R.id.btnLogin)
        btnRegister = findViewById(R.id.btnRegister)
        btnExit = findViewById(R.id.btnExit)
    }

    private fun restoreState(savedInstanceState: Bundle?) {
        savedInstanceState?.let {
            loginEditText.setText(it.getString("login"))
            passwordEditText.setText(it.getString("password"))
        }
    }

    private fun setupClickListeners() {
        btnLogin.setOnClickListener {
            val login = loginEditText.text.toString().trim()
            val password = passwordEditText.text.toString().trim()

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

        btnRegister.setOnClickListener {
            val intent = Intent(this, RegisterActivity::class.java)
            startActivity(intent)
        }

        btnExit.setOnClickListener {
            finish()
        }
    }

    private fun saveThemeToPreferences(isNight: Boolean) {
        prefs.edit().putBoolean("night_mode", isNight).apply()
    }

    override fun onSaveInstanceState(outState: Bundle) {
        super.onSaveInstanceState(outState)
        outState.putString("login", loginEditText.text.toString())
        outState.putString("password", passwordEditText.text.toString())
    }

    override fun onStart() { super.onStart(); Log.d("LoginActivity", "onStart") }
    override fun onResume() { super.onResume(); Log.d("LoginActivity", "onResume") }
    override fun onPause() { super.onPause(); Log.d("LoginActivity", "onPause") }
    override fun onStop() { super.onStop(); Log.d("LoginActivity", "onStop") }
    override fun onDestroy() { super.onDestroy(); Log.d("LoginActivity", "onDestroy") }
    override fun onRestart() { super.onRestart(); Log.d("LoginActivity", "onRestart") }
}