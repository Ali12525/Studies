package com.example.lab3.model

import android.net.Uri

data class User(
    val id: Int = -1,
    val login: String,
    val password: String,
    val fullName: String,
    val birthDate: String,
    val gender: String,
    val avatarUri: String? = null,
    val isAdmin: Boolean = false,
    val theme: Int = 0
)