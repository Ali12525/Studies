package com.example.lab3

import android.os.Bundle
import android.widget.ImageView
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity

class ProfileActivity : AppCompatActivity() {
    private lateinit var avatarImage: ImageView
    private lateinit var loginText: TextView
    private lateinit var fullNameText: TextView
    private lateinit var birthDateText: TextView
    private lateinit var genderText: TextView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_profile)

        avatarImage = findViewById(R.id.profileAvatar)
        loginText = findViewById(R.id.profileLogin)
        fullNameText = findViewById(R.id.profileFullName)
        birthDateText = findViewById(R.id.profileBirthDate)
        genderText = findViewById(R.id.profileGender)

        val login = intent.getStringExtra("user_login")
        val user = login?.let { UsersRepository.getUserByLogin(it) }

        user?.let {
            avatarImage.setImageResource(it.avatarResId)
            loginText.text = it.login
            fullNameText.text = it.fullName
            birthDateText.text = it.birthDate
            genderText.text = it.gender
        }

    }
}