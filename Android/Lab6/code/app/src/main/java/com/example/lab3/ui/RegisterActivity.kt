package com.example.lab3.ui

import android.app.AlertDialog
import android.os.Bundle
import android.util.Log
import android.widget.Toast
import androidx.lifecycle.lifecycleScope
import com.example.lab3.databinding.ActivityRegisterBinding
import com.example.lab3.model.User
import com.example.lab3.db.DatabaseHandler
import com.example.lab3.util.AuthLogger
import com.example.lab3.util.AvatarHelper
import com.example.lab3.util.CameraHelper
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext

class RegisterActivity : BaseActivity() {
    private lateinit var binding: ActivityRegisterBinding
    private var selectedAvatarUri: String? = null
    private lateinit var db: DatabaseHandler
    private lateinit var cameraHelper: CameraHelper

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityRegisterBinding.inflate(layoutInflater)
        setContentView(binding.root)
        Log.d("RegisterActivity", "onCreate")

        db = DatabaseHandler(this)
        cameraHelper = CameraHelper(this)

        selectedAvatarUri = AvatarHelper.getDefaultAvatarUri(0)
        AvatarHelper.loadAvatar(this, binding.avatarImage, selectedAvatarUri)

        setupClickListeners()
        restoreState(savedInstanceState)
    }

    override fun onSaveInstanceState(outState: Bundle) {
        super.onSaveInstanceState(outState)
        outState.putString("login", binding.loginEditText.text.toString())
        outState.putString("password", binding.passwordEditText.text.toString())
        outState.putString("fullName", binding.fullNameEditText.text.toString())
        outState.putString("birthDate", binding.birthDateEditText.text.toString())
        outState.putString("avatarUri", selectedAvatarUri)
        outState.putInt("genderId", binding.genderRadioGroup.checkedRadioButtonId)
    }

    private fun restoreState(savedInstanceState: Bundle?) {
        savedInstanceState?.let {
            binding.loginEditText.setText(it.getString("login"))
            binding.passwordEditText.setText(it.getString("password"))
            binding.fullNameEditText.setText(it.getString("fullName"))
            binding.birthDateEditText.setText(it.getString("birthDate"))

            selectedAvatarUri = it.getString("avatarUri") ?: AvatarHelper.getDefaultAvatarUri(0)
            AvatarHelper.loadAvatar(this, binding.avatarImage, selectedAvatarUri)

            val genderId = it.getInt("genderId", -1)
            if (genderId != -1) binding.genderRadioGroup.check(genderId)
        }
    }

    private fun setupClickListeners() {
        binding.selectAvatarButton.setOnClickListener { selectAvatar() }
        binding.avatarImage.setOnClickListener { selectAvatar() }
        binding.registerButton.setOnClickListener { registerUser() }
    }

    private fun selectAvatar() {
        val avatarNames = arrayOf("Камера", "Галерея", "Настройки", "Сделать фото")

        AlertDialog.Builder(this)
            .setTitle("Выберите аватар")
            .setItems(avatarNames) { _, which ->
                when (which) {
                    0, 1, 2 -> {
                        selectedAvatarUri = AvatarHelper.getDefaultAvatarUri(which)
                        AvatarHelper.loadAvatar(this, binding.avatarImage, selectedAvatarUri)
                        Toast.makeText(this, "Аватар выбран: ${avatarNames[which]}", Toast.LENGTH_SHORT).show()
                    }
                    3 -> {
                        cameraHelper.takePhoto(
                            onSuccess = { photoUri ->
                                selectedAvatarUri = photoUri
                                AvatarHelper.loadAvatar(this, binding.avatarImage, selectedAvatarUri)
                                Toast.makeText(this, "Фото сделано!", Toast.LENGTH_SHORT).show()
                            }
                        )
                    }
                }
            }
            .show()
    }

    private fun registerUser() {
        val login = binding.loginEditText.text.toString().trim()
        val password = binding.passwordEditText.text.toString().trim()
        val fullName = binding.fullNameEditText.text.toString().trim()
        val birthDate = binding.birthDateEditText.text.toString().trim()

        val selectedGenderId = binding.genderRadioGroup.checkedRadioButtonId
        val gender = when(selectedGenderId) {
            com.example.lab3.R.id.maleRadioButton -> "М"
            com.example.lab3.R.id.femaleRadioButton -> "Ж"
            else -> ""
        }

        if (!validateInput(login, password, fullName, birthDate, gender)) return

        lifecycleScope.launch {
            try {
                val existingUser = withContext(Dispatchers.IO) {
                    db.getUserByLogin(login)
                }

                if (existingUser != null) {
                    Toast.makeText(this@RegisterActivity, "Такой пользователь уже есть", Toast.LENGTH_SHORT).show()
                    return@launch
                }

                val isFirstUser = withContext(Dispatchers.IO) {
                    db.countUsers() == 0
                }

                val user = User(
                    login = login,
                    password = password,
                    fullName = fullName,
                    birthDate = birthDate,
                    gender = gender,
                    avatarUri = selectedAvatarUri,
                    isAdmin = isFirstUser
                )

                val result = withContext(Dispatchers.IO) {
                    db.addUser(user)
                }

                if (result != -1L) {
                    AuthLogger.log(this@RegisterActivity, "Registered: $login (admin=$isFirstUser)")
                    Toast.makeText(this@RegisterActivity, "Пользователь зарегистрирован!", Toast.LENGTH_SHORT).show()
                    finish()
                } else {
                    Toast.makeText(this@RegisterActivity, "Ошибка при сохранении пользователя", Toast.LENGTH_SHORT).show()
                }

            } catch (e: Exception) {
                Log.e("RegisterActivity", "Registration error", e)
                Toast.makeText(this@RegisterActivity, "Ошибка регистрации: ${e.message}", Toast.LENGTH_LONG).show()
            }
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
        if (!validDate(birthDate)) {
            Toast.makeText(this, "Неправильный формат даты (дд.мм.гггг)", Toast.LENGTH_SHORT).show()
            return false
        }
        if (gender.isEmpty()) {
            Toast.makeText(this, "Выберите пол", Toast.LENGTH_SHORT).show()
            return false
        }
        return true
    }

    fun validDate(birthDate: String): Boolean {
        val regex = "\\d{2}\\.\\d{2}\\.\\d{4}".toRegex()
        return birthDate.matches(regex)
    }

    override fun onStart() { super.onStart(); Log.d("RegisterActivity", "onStart") }
    override fun onResume() { super.onResume(); Log.d("RegisterActivity", "onResume") }
    override fun onPause() { super.onPause(); Log.d("RegisterActivity", "onPause") }
    override fun onStop() { super.onStop(); Log.d("RegisterActivity", "onStop") }
    override fun onDestroy() { super.onDestroy(); Log.d("RegisterActivity", "onDestroy") }
    override fun onRestart() { super.onRestart(); Log.d("RegisterActivity", "onRestart") }
}