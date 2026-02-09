package com.example.lab3.ui

import android.app.AlertDialog
import android.os.Bundle
import android.widget.Button
import android.widget.Toast
import androidx.lifecycle.lifecycleScope
import com.example.lab3.databinding.ActivityProfileBinding
import com.example.lab3.db.DatabaseHandler
import com.example.lab3.util.AuthLogger
import com.example.lab3.util.AvatarHelper
import com.example.lab3.util.CameraHelper
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext

class ProfileActivity : BaseActivity() {
    private lateinit var binding: ActivityProfileBinding
    private lateinit var db: DatabaseHandler
    private var viewingLogin: String? = null
    private var currentLogin: String? = null
    private var fromAdmin: Boolean = false
    private lateinit var cameraHelper: CameraHelper

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityProfileBinding.inflate(layoutInflater)
        setContentView(binding.root)

        db = DatabaseHandler(this)
        cameraHelper = CameraHelper(this)

        setupClickListeners()
        extractIntentAndPreferences()
        loadProfile()
    }

    private fun setupClickListeners() {
        binding.btnMakeAdmin.setOnClickListener { confirmMakeAdmin() }
        binding.btnDeleteUser.setOnClickListener { confirmDeleteUser() }
        binding.profileAvatar.setOnClickListener { selectAvatar() }
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
                        AvatarHelper.loadAvatar(this@ProfileActivity, binding.profileAvatar, it.avatarUri)
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

    private fun selectAvatar() {
        if (viewingLogin != currentLogin) {
            Toast.makeText(this, "Вы можете изменять только свой аватар", Toast.LENGTH_SHORT).show()
            return
        }

        val avatarNames = arrayOf("Камера", "Галерея", "Настройки", "Сделать фото")

        AlertDialog.Builder(this)
            .setTitle("Изменить аватар")
            .setItems(avatarNames) { _, which ->
                when (which) {
                    0, 1, 2 -> {
                        val newAvatarUri = AvatarHelper.getDefaultAvatarUri(which)
                        lifecycleScope.launch(Dispatchers.IO) {
                            viewingLogin?.let { login ->
                                db.updateUserAvatar(login, newAvatarUri)
                            }
                        }
                        AvatarHelper.loadAvatar(this, binding.profileAvatar, newAvatarUri)
                        Toast.makeText(this, "Аватар изменен", Toast.LENGTH_SHORT).show()
                        loadProfile()
                    }
                    3 -> {
                        cameraHelper.takePhoto(
                            onSuccess = { photoUri ->
                                updateAvatarInDatabase(photoUri)
                                AvatarHelper.loadAvatar(this, binding.profileAvatar, photoUri)
                                Toast.makeText(this, "Фото обновлено!", Toast.LENGTH_SHORT).show()
                                loadProfile()
                            }
                        )
                    }
                }
            }
            .show()
    }

    private fun updateAvatarInDatabase(avatarUri: String) {
        lifecycleScope.launch(Dispatchers.IO) {
            viewingLogin?.let { login ->
                db.updateUserAvatar(login, avatarUri)
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
                            AuthLogger.log(this@ProfileActivity, "Admin $currentLogin deleted user $viewingLogin")
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