package com.example.lab3

import android.R

data class User(
    val login: String,
    val password: String,
    val fullName: String,
    val birthDate: String,
    val gender: String,
    val avatarResId: Int = R.drawable.ic_menu_gallery
)
