    package com.example.lab3

    object UsersRepository {
        private val users = mutableListOf<User>()

        fun addUser(user: User) {
            users.add(user)
        }

        fun getUsers(): MutableList<User> {
            return users
        }

        fun findUser(login: String, password: String): User? {
            return users.find { it.login == login && it.password == password }
        }

        fun getUserByLogin(login: String): User? {
            return users.find { it.login == login }
        }

        fun clearUsers() {
            users.clear()
        }
    }