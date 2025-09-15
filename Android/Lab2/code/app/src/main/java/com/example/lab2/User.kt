package com.example.lab2

data class User(
    val login: String,
    val password: String,
    val fullName: String,
    val birthDate: String,
    val gender: String,
    val avatarResId: Int = android.R.drawable.ic_menu_gallery
)
