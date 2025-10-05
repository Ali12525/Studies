package com.example.lab3.ui

import android.os.Bundle
import android.widget.Button
import android.widget.ImageView
import android.widget.TextView
import androidx.appcompat.app.AlertDialog
import androidx.lifecycle.lifecycleScope
import com.example.lab3.R
import com.example.lab3.db.DatabaseHandler
import com.example.lab3.util.AuthLogger
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext

class ProfileActivity : BaseActivity() {
    private lateinit var avatar: ImageView
    private lateinit var loginTv: TextView
    private lateinit var nameTv: TextView
    private lateinit var birthTv: TextView
    private lateinit var genderTv: TextView
    private lateinit var btnMakeAdmin: Button
    private lateinit var btnDeleteUser: Button

    private lateinit var db: DatabaseHandler

    private var viewingLogin: String? = null
    private var currentLogin: String? = null
    private var fromAdmin: Boolean = false

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_profile)

        db = DatabaseHandler(this)
        initViews()
        setupClickListeners()
        extractIntentAndPreferences()
        loadProfile()
    }

    private fun initViews() {
        avatar = findViewById(R.id.profileAvatar)
        loginTv = findViewById(R.id.profileLogin)
        nameTv = findViewById(R.id.profileFullName)
        birthTv = findViewById(R.id.profileBirthDate)
        genderTv = findViewById(R.id.profileGender)
        btnMakeAdmin = findViewById(R.id.btnMakeAdmin)
        btnDeleteUser = findViewById(R.id.btnDeleteUser)
    }

    private fun setupClickListeners() {
        btnMakeAdmin.setOnClickListener { confirmMakeAdmin() }
        btnDeleteUser.setOnClickListener { confirmDeleteUser() }
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
                    if (user != null) {
                        avatar.setImageResource(user.avatarResId)
                        loginTv.text = user.login
                        nameTv.text = user.fullName
                        birthTv.text = user.birthDate
                        genderTv.text = user.gender

                        val showMake = (currentUser?.isAdmin == true) && user.login != currentLogin && !user.isAdmin
                        btnMakeAdmin.visibility = if (showMake) Button.VISIBLE else Button.GONE
                        btnDeleteUser.visibility = if (showMake) Button.VISIBLE else Button.GONE
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
                            btnMakeAdmin.visibility = Button.GONE
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
                            btnMakeAdmin.visibility = Button.GONE
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