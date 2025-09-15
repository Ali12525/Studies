package com.example.lab2

import android.app.AlertDialog
import android.os.Bundle
import android.widget.Button
import android.widget.EditText
import android.widget.ImageView
import android.widget.RadioGroup
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity

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

        initViews()
        setupClickListeners()
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
        //Toast.makeText(this, "Выбор аватара", Toast.LENGTH_SHORT).show()
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

        if (validateInput(login, password, fullName, birthDate, gender)) {
            val user = User(login, password, fullName, birthDate, gender)
            UsersRepository.addUser(user)

            Toast.makeText(this, "Пользователь зарегистрирован!", Toast.LENGTH_SHORT).show()
            finish()
        }
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
        if (gender.isEmpty()) {
            Toast.makeText(this, "Выберите пол", Toast.LENGTH_SHORT).show()
            return false
        }
        return true
    }
}