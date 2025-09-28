package com.example.lab3.ui

import android.content.Intent
import android.os.Bundle
import android.widget.Button
import android.widget.ListView
import com.example.lab3.R
import com.example.lab3.db.DatabaseHandler
import com.example.lab3.model.User

class AdminActivity : BaseActivity() {
    private lateinit var listView: ListView
    private lateinit var btnLogs: Button
    private lateinit var adapter: AdminUserAdapter
    private lateinit var db: DatabaseHandler
    private var users = mutableListOf<User>()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_admin)

        db = DatabaseHandler(this)
        initViews()
        setupClickListeners()
        loadUsers()
    }

    private fun initViews() {
        listView = findViewById(R.id.listUsers)
        btnLogs = findViewById(R.id.btnViewLogs)
        adapter = AdminUserAdapter(this, users)
        listView.adapter = adapter
    }

    private fun setupClickListeners() {
        listView.setOnItemClickListener { _, _, position, _ ->
            val user = users[position]
            val intent = Intent(this, ProfileActivity::class.java)
            intent.putExtra("user_login", user.login)
            intent.putExtra("from_admin", true)
            startActivity(intent)
        }

        btnLogs.setOnClickListener {
            startActivity(Intent(this, LogsActivity::class.java))
        }
    }

    override fun onResume() {
        super.onResume()
        loadUsers()
    }

    private fun loadUsers() {
        Thread {
            val list = db.getAllUsers()
            runOnUiThread {
                users.clear()
                users.addAll(list)
                adapter.notifyDataSetChanged()
            }
        }.start()
    }
}
