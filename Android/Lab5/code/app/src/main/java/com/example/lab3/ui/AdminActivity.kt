package com.example.lab3.ui

import android.content.Intent
import android.os.Bundle
import com.example.lab3.databinding.ActivityAdminBinding
import com.example.lab3.db.DatabaseHandler
import com.example.lab3.model.User

class AdminActivity : BaseActivity() {
    private lateinit var binding: ActivityAdminBinding
    private lateinit var adapter: AdminUserAdapter
    private lateinit var db: DatabaseHandler
    private var users = mutableListOf<User>()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityAdminBinding.inflate(layoutInflater)
        setContentView(binding.root)

        db = DatabaseHandler(this)
        adapter = AdminUserAdapter(this, users)
        binding.listUsers.adapter = adapter
        setupClickListeners()
        loadUsers()
    }

    private fun setupClickListeners() {
        binding.listUsers.setOnItemClickListener { _, _, position, _ ->
            val user = users[position]
            val intent = Intent(this, ProfileActivity::class.java)
            intent.putExtra("user_login", user.login)
            intent.putExtra("from_admin", true)
            startActivity(intent)
        }

        binding.btnViewLogs.setOnClickListener {
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
