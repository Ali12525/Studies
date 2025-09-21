package com.example.lab3

import android.content.Intent
import android.os.Bundle
import android.util.Log
import android.widget.Button
import android.widget.EditText
import android.widget.ImageView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity

class LoginActivity : AppCompatActivity() {
    private lateinit var loginEditText: EditText
    private lateinit var passwordEditText: EditText
    private lateinit var btnLogin: Button
    private lateinit var btnRegister: Button
    private lateinit var btnExit: Button
    private lateinit var logoImage: ImageView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_login)
        Log.d("LoginActivity", "onCreate")

        logoImage = findViewById(R.id.logoImage)
        loginEditText = findViewById(R.id.loginEditText)
        passwordEditText = findViewById(R.id.passwordEditText)
        btnLogin = findViewById(R.id.btnLogin)
        btnRegister = findViewById(R.id.btnRegister)
        btnExit = findViewById(R.id.btnExit)

        savedInstanceState?.let {
            loginEditText.setText(it.getString("login"))
            passwordEditText.setText(it.getString("password"))
        }

        btnLogin.setOnClickListener {
            val login = loginEditText.text.toString().trim()
            val password = passwordEditText.text.toString().trim()

            val user = UsersRepository.findUser(login, password)

            if (user != null) {
                val intent = Intent(this, MenuActivity::class.java)
                intent.putExtra("user_login", login)
                startActivity(intent)
            } else {
                Toast.makeText(this, "Неверный логин или пароль", Toast.LENGTH_SHORT).show()
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