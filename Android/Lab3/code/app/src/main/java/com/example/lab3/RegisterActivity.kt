package com.example.lab3

import android.app.AlertDialog
import android.os.Bundle
import android.util.Log
import android.widget.Button
import android.widget.EditText
import android.widget.ImageView
import android.widget.RadioGroup
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import kotlin.text.isEmpty
import kotlin.text.matches
import kotlin.text.toRegex
import kotlin.text.trim

class RegisterActivity : AppCompatActivity() {
    private lateinit var avatarImage: ImageView
    private lateinit var loginEditText: EditText
    private lateinit var passwordEditText: EditText
    private lateinit var fullNameEditText: EditText
    private lateinit var birthDateEditText: EditText
    private lateinit var genderRadioGroup: RadioGroup
    private lateinit var registerButton: Button
    private var selectedAvatarResId: Int = android.R.drawable.ic_menu_gallery

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_register)
        Log.d("RegisterActivity", "onCreate")

        initViews()
        setupClickListeners()

        savedInstanceState?.let {
            loginEditText.setText(it.getString("login"))
            passwordEditText.setText(it.getString("password"))
            fullNameEditText.setText(it.getString("fullName"))
            birthDateEditText.setText(it.getString("birthDate"))
            selectedAvatarResId = it.getInt("avatarResId", android.R.drawable.ic_menu_gallery)
            avatarImage.setImageResource(selectedAvatarResId)

            val genderId = it.getInt("genderId", -1)
            if (genderId != -1) genderRadioGroup.check(genderId)
        }
    }

    override fun onSaveInstanceState(outState: Bundle) {
        super.onSaveInstanceState(outState)
        outState.putString("login", loginEditText.text.toString())
        outState.putString("password", passwordEditText.text.toString())
        outState.putString("fullName", fullNameEditText.text.toString())
        outState.putString("birthDate", birthDateEditText.text.toString())
        outState.putInt("avatarResId", selectedAvatarResId)
        outState.putInt("genderId", genderRadioGroup.checkedRadioButtonId)
    }

    private fun initViews() {
        avatarImage = findViewById(R.id.avatarImage)
        loginEditText = findViewById(R.id.loginEditText)
        passwordEditText = findViewById(R.id.passwordEditText)
        fullNameEditText = findViewById(R.id.fullNameEditText)
        birthDateEditText = findViewById(R.id.birthDateEditText)
        genderRadioGroup = findViewById(R.id.genderRadioGroup)
        registerButton = findViewById(R.id.registerButton)
    }

    private fun setupClickListeners() {
        findViewById<Button>(R.id.selectAvatarButton).setOnClickListener {
            selectAvatar()
        }

        avatarImage.setOnClickListener {
            selectAvatar()
        }

        registerButton.setOnClickListener {
            registerUser()
        }
    }

    private fun selectAvatar() {

        val avatars = listOf(
            android.R.drawable.ic_menu_camera,
            android.R.drawable.ic_menu_gallery,
            android.R.drawable.ic_menu_manage
        )

        val avatarNames = arrayOf("Камера", "Галерея", "Настройки")

        AlertDialog.Builder(this)
            .setTitle("Выберите аватар")
            .setItems(avatarNames) { _, which ->
                selectedAvatarResId = avatars[which]
                avatarImage.setImageResource(selectedAvatarResId)
            }
            .show()
    }

    private fun registerUser() {
        val login = loginEditText.text.toString().trim()
        val password = passwordEditText.text.toString().trim()
        val fullName = fullNameEditText.text.toString().trim()
        val birthDate = birthDateEditText.text.toString().trim()

        val selectedGenderId = genderRadioGroup.checkedRadioButtonId
        val gender = when(selectedGenderId) {
            R.id.maleRadioButton -> "М"
            R.id.femaleRadioButton -> "Ж"
            else -> ""
        }

        if (!validateInput(login, password, fullName, birthDate, gender)) return

        val existingUser = UsersRepository.getUserByLogin(login)
        if (existingUser != null) {
            Toast.makeText(this, "Пользователь с таким логином уже существует!", Toast.LENGTH_SHORT).show()
            return
        }

        val user = User(login, password, fullName, birthDate, gender, selectedAvatarResId)
        UsersRepository.addUser(user)

        Toast.makeText(this, "Пользователь зарегистрирован!", Toast.LENGTH_SHORT).show()
        finish()
    }

    private fun validateInput(
        login: String,
        password: String,
        fullName: String,
        birthDate: String,
        gender: String
    ): Boolean {
        if (login.isEmpty()) {
            Toast.makeText(this, "Введите логин", Toast.LENGTH_SHORT).show()
            return false
        }
        if (password.isEmpty()) {
            Toast.makeText(this, "Введите пароль", Toast.LENGTH_SHORT).show()
            return false
        }
        if (fullName.isEmpty()) {
            Toast.makeText(this, "Введите ФИО", Toast.LENGTH_SHORT).show()
            return false
        }
        if (birthDate.isEmpty()) {
            Toast.makeText(this, "Введите дату рождения", Toast.LENGTH_SHORT).show()
            return false
        }
        if (!validDate(birthDate)) {
            Toast.makeText(this, "Неправильный формат даты", Toast.LENGTH_SHORT).show()
            return false
        }
        if (gender.isEmpty()) {
            Toast.makeText(this, "Выберите пол", Toast.LENGTH_SHORT).show()
            return false
        }
        return true
    }

    fun validDate(birthDate : String) : Boolean {
        var regex = "\\d{2}.\\d{2}.\\d{4}".toRegex()
        return birthDate.matches(regex)
    }

    override fun onStart() { super.onStart(); Log.d("RegisterActivity", "onStart") }
    override fun onResume() { super.onResume(); Log.d("RegisterActivity", "onResume") }
    override fun onPause() { super.onPause(); Log.d("RegisterActivity", "onPause") }
    override fun onStop() { super.onStop(); Log.d("RegisterActivity", "onStop") }
    override fun onDestroy() { super.onDestroy(); Log.d("RegisterActivity", "onDestroy") }
    override fun onRestart() { super.onRestart(); Log.d("RegisterActivity", "onRestart") }

}