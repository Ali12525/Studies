package com.example.lab3.ui

import android.os.Bundle
import android.widget.Button
import android.widget.ImageView
import android.widget.TextView
import androidx.appcompat.app.AlertDialog
import androidx.lifecycle.lifecycleScope
import com.example.lab3.R
import com.example.lab3.databinding.ActivityProfileBinding
import com.example.lab3.db.DatabaseHandler
import com.example.lab3.util.AuthLogger
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext

class ProfileActivity : BaseActivity() {
    private lateinit var binding: ActivityProfileBinding
    private lateinit var db: DatabaseHandler
    private var viewingLogin: String? = null
    private var currentLogin: String? = null
    private var fromAdmin: Boolean = false

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityProfileBinding.inflate(layoutInflater)
        setContentView(binding.root)

        db = DatabaseHandler(this)
        setupClickListeners()
        extractIntentAndPreferences()
        loadProfile()
    }

    private fun setupClickListeners() {
        binding.btnMakeAdmin.setOnClickListener { confirmMakeAdmin() }
        binding.btnDeleteUser.setOnClickListener { confirmDeleteUser() }
    }

    private fun extractIntentAndPreferences() {
        viewingLogin = intent.getStringExtra("user_login")
        fromAdmin = intent.getBooleanExtra("from_admin", false)
        currentLogin = getSharedPreferences("session", MODE_PRIVATE).getString("current_login", null)
    }

    private fun loadProfile() {
        lifecycleScope.launch {
            try {
                val (user, currentUser) = withContext(Dispatchers.IO) {
                    val user = viewingLogin?.let { db.getUserByLogin(it) }
                    val currentUser = currentLogin?.let { db.getUserByLogin(it) }
                    Pair(user, currentUser)
                }

                if (!isDestroyed) {
                    user?.let {
                        binding.profileAvatar.setImageResource(it.avatarResId)
                        binding.profileLogin.text = it.login
                        binding.profileFullName.text = it.fullName
                        binding.profileBirthDate.text = it.birthDate
                        binding.profileGender.text = it.gender

                        val showMake = (currentUser?.isAdmin == true) && it.login != currentLogin && !it.isAdmin
                        binding.btnMakeAdmin.visibility = if (showMake) android.view.View.VISIBLE else android.view.View.GONE
                        binding.btnDeleteUser.visibility = if (showMake) android.view.View.VISIBLE else android.view.View.GONE
                    }
                }
            } catch (e: Exception) {
                e.printStackTrace()
            }
        }
    }

    private fun confirmMakeAdmin() {
        AlertDialog.Builder(this)
            .setTitle("Подтверждение")
            .setMessage("Сделать этого пользователя администратором?")
            .setPositiveButton("Да") { _, _ ->
                lifecycleScope.launch {
                    try {
                        withContext(Dispatchers.IO) {
                            viewingLogin?.let {
                                db.makeAdminByLogin(it)
                            }
                        }

                        if (!isDestroyed) {
                            AuthLogger.log(this@ProfileActivity, "Promoted to admin: $viewingLogin")
                            binding.btnMakeAdmin.visibility = Button.GONE
                            loadProfile()
                        }
                    } catch (e: Exception) {
                        e.printStackTrace()
                    }
                }
            }
            .setNegativeButton("Отмена", null)
            .show()
    }

    private fun confirmDeleteUser() {
        AlertDialog.Builder(this)
            .setTitle("Подтверждение")
            .setMessage("Удалить этого пользователя?")
            .setPositiveButton("Да") { _, _ ->
                lifecycleScope.launch {
                    try {
                        withContext(Dispatchers.IO) {
                            viewingLogin?.let {
                                db.deleteUser(it)
                            }
                        }

                        if (!isDestroyed) {
                            AuthLogger.log(this@ProfileActivity, "Admin $currentLogin delevit user $viewingLogin")
                            binding.btnMakeAdmin.visibility = Button.GONE
                            loadProfile()
                        }
                    } catch (e: Exception) {
                        e.printStackTrace()
                    }
                }
            }
            .setNegativeButton("Отмена", null)
            .show()
    }
}