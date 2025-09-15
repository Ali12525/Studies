package com.example.lab2

import android.app.AlertDialog
import android.content.Intent
import android.os.Bundle
import android.widget.Button
import android.widget.ListView
import androidx.appcompat.app.AppCompatActivity

class UsersActivity : AppCompatActivity() {
    private lateinit var usersListView: ListView
    private lateinit var addButton: Button
    private lateinit var adapter: UserAdapter

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_users)

        initViews()
        setupAdapter()
        setupClickListeners()
    }

    private fun initViews() {
        usersListView = findViewById(R.id.usersListView)
        addButton = findViewById(R.id.addButton)
    }

    private fun setupAdapter() {
        adapter = UserAdapter(this, UsersRepository.getUsers())
        usersListView.adapter = adapter
    }

    private fun setupClickListeners() {
        addButton.setOnClickListener {
            val intent = Intent(this, RegisterActivity::class.java)
            startActivity(intent)
        }

        usersListView.setOnItemClickListener { parent, view, position, id ->
            val user = adapter.getItem(position) as User
            showUserDetails(user)
        }
    }

    private fun showUserDetails(user: User) {
        AlertDialog.Builder(this)
            .setTitle("Информация о пользователе")
            .setMessage(
                "Логин: ${user.login}\n" +
                        "ФИО: ${user.fullName}\n" +
                        "Дата рождения: ${user.birthDate}\n" +
                        "Пол: ${user.gender}"
            )
            .setPositiveButton("OK", null)
            .show()
    }

    override fun onResume() {
        super.onResume()
        adapter.notifyDataSetChanged()
    }
}