package com.example.lab3.model

import android.R

data class User(
    val id: Int = -1,
    val login: String,
    val password: String,
    val fullName: String,
    val birthDate: String,
    val gender: String,
    val avatarResId: Int = R.drawable.ic_menu_gallery,
    val isAdmin: Boolean = false,
    val theme: Int = 0
)