package com.example.lab2

object UsersRepository {
    private val users = mutableListOf<User>()

    fun addUser(user: User) {
        users.add(user)
    }

    fun getUsers(): MutableList<User> {
        return users
    }

    fun clearUsers() {
        users.clear()
    }
}