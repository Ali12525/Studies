package com.example.lab3.ui

import android.os.Bundle
import android.util.Log
import android.widget.ImageView
import android.widget.Toast
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.selection.selectable
import androidx.compose.foundation.verticalScroll
import androidx.compose.material.*
import androidx.compose.runtime.*
import androidx.compose.runtime.saveable.rememberSaveable
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.unit.dp
import androidx.compose.ui.viewinterop.AndroidView
import com.example.lab3.db.DatabaseHandler
import com.example.lab3.model.User
import com.example.lab3.util.AuthLogger
import com.example.lab3.util.AvatarHelper
import com.example.lab3.util.CameraHelper
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext

class RegisterActivity : BaseActivity() {
    private lateinit var db: DatabaseHandler
    private lateinit var cameraHelper: CameraHelper

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        db = DatabaseHandler(this)
        cameraHelper = CameraHelper(this)

        setContent {
            MaterialTheme {
                Surface(
                    modifier = Modifier.fillMaxSize(),
                    color = MaterialTheme.colors.background
                ) {
                    RegisterScreen(
                        db = db,
                        cameraHelper = cameraHelper,
                        onRegistered = { finish() }
                    )
                }
            }
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        try {
            db.close()
        } catch (e: Exception) { }
    }

    override fun onStart() { super.onStart(); Log.d("RegisterActivity", "onStart") }
    override fun onResume() { super.onResume(); Log.d("RegisterActivity", "onResume") }
    override fun onPause() { super.onPause(); Log.d("RegisterActivity", "onPause") }
    override fun onStop() { super.onStop(); Log.d("RegisterActivity", "onStop") }
    override fun onRestart() { super.onRestart(); Log.d("RegisterActivity", "onRestart") }
}

@Composable
fun RegisterScreen(
    db: DatabaseHandler,
    cameraHelper: CameraHelper,
    onRegistered: () -> Unit
) {
    val ctx = LocalContext.current
    val scope = rememberCoroutineScope()

    var login by rememberSaveable { mutableStateOf("") }
    var password by rememberSaveable { mutableStateOf("") }
    var fullName by rememberSaveable { mutableStateOf("") }
    var birthDate by rememberSaveable { mutableStateOf("") }
    var gender by rememberSaveable { mutableStateOf("") }
    var avatarUri by rememberSaveable { mutableStateOf(AvatarHelper.getDefaultAvatarUri(0)) }
    var isLoading by remember { mutableStateOf(false) }
    var showAvatarDialog by remember { mutableStateOf(false) }

    Column(
        modifier = Modifier
            .fillMaxSize()
            .padding(16.dp)
            .verticalScroll(rememberScrollState()),
        verticalArrangement = Arrangement.spacedBy(12.dp)
    ) {
        AndroidView(
            factory = { context ->
                ImageView(context).apply {
                    AvatarHelper.loadAvatar(context, this, avatarUri)
                    setOnClickListener { showAvatarDialog = true }
                }
            },
            update = { imageView ->
                try {
                    AvatarHelper.loadAvatar(imageView.context, imageView, avatarUri)
                } catch (_: Exception) {}
            },
            modifier = Modifier.size(100.dp)
        )

        OutlinedTextField(
            value = login,
            onValueChange = { login = it },
            label = { Text("Логин") },
            modifier = Modifier.fillMaxWidth()
        )

        OutlinedTextField(
            value = password,
            onValueChange = { password = it },
            label = { Text("Пароль") },
            modifier = Modifier.fillMaxWidth()
        )

        OutlinedTextField(
            value = fullName,
            onValueChange = { fullName = it },
            label = { Text("ФИО") },
            modifier = Modifier.fillMaxWidth()
        )

        OutlinedTextField(
            value = birthDate,
            onValueChange = { birthDate = it },
            label = { Text("Дата рождения (дд.мм.гггг)") },
            modifier = Modifier.fillMaxWidth()
        )

        GenderRadioButtons(gender = gender, onGenderSelected = { gender = it })

        Spacer(modifier = Modifier.height(8.dp))

        RegisterButton(
            isLoading = isLoading,
            onRegisterClick = {
                registerUser(
                    login = login,
                    password = password,
                    fullName = fullName,
                    birthDate = birthDate,
                    gender = gender,
                    avatarUri = avatarUri,
                    db = db,
                    ctx = ctx,
                    scope = scope,
                    onSuccess = onRegistered,
                    onLoadingChanged = { isLoading = it }
                )
            }
        )
    }

    if (showAvatarDialog) {
        AvatarSelectionDialog(
            onDismiss = { showAvatarDialog = false },
            onAvatarSelected = { uri -> avatarUri = uri },
            cameraHelper = cameraHelper
        )
    }
}

@Composable
private fun GenderRadioButtons(
    gender: String,
    onGenderSelected: (String) -> Unit
) {
    Row(Modifier.fillMaxWidth(), horizontalArrangement = Arrangement.Start) {
        Row(Modifier.selectable(selected = gender == "М", onClick = { onGenderSelected("М") }).padding(end = 12.dp)) {
            RadioButton(selected = gender == "М", onClick = { onGenderSelected("М") })
            Spacer(modifier = Modifier.width(6.dp))
            Text(text = "Мужской", modifier = Modifier.padding(top = 8.dp))
        }
        Row(Modifier.selectable(selected = gender == "Ж", onClick = { onGenderSelected("Ж") })) {
            RadioButton(selected = gender == "Ж", onClick = { onGenderSelected("Ж") })
            Spacer(modifier = Modifier.width(6.dp))
            Text(text = "Женский", modifier = Modifier.padding(top = 8.dp))
        }
    }
}

@Composable
private fun RegisterButton(
    isLoading: Boolean,
    onRegisterClick: () -> Unit
) {
    Button(
        onClick = onRegisterClick,
        modifier = Modifier.fillMaxWidth(),
        enabled = !isLoading
    ) {
        Text(if (isLoading) "Сохранение..." else "Зарегистрироваться")
    }
}

@Composable
private fun AvatarSelectionDialog(
    onDismiss: () -> Unit,
    onAvatarSelected: (String) -> Unit,
    cameraHelper: CameraHelper
) {
    val avatarOptions = listOf("Камера", "Галерея", "Настройки", "Сделать фото")

    AlertDialog(
        onDismissRequest = onDismiss,
        title = { Text("Выберите аватар") },
        buttons = {
            Column(Modifier.padding(8.dp)) {
                avatarOptions.forEachIndexed { index, label ->
                    TextButton(
                        onClick = {
                            onDismiss()
                            selectAvatar(
                                optionIndex = index,
                                cameraHelper = cameraHelper,
                                onAvatarSelected = onAvatarSelected
                            )
                        },
                        modifier = Modifier.fillMaxWidth()
                    ) {
                        Text(label)
                    }
                }
                Spacer(modifier = Modifier.height(4.dp))
                TextButton(onClick = onDismiss, modifier = Modifier.fillMaxWidth()) {
                    Text("Отмена")
                }
            }
        }
    )
}

private fun selectAvatar(
    optionIndex: Int,
    cameraHelper: CameraHelper,
    onAvatarSelected: (String) -> Unit
) {
    when (optionIndex) {
        0, 1, 2 -> {
            onAvatarSelected(AvatarHelper.getDefaultAvatarUri(optionIndex))
        }
        3 -> {
            cameraHelper.takePhoto(onSuccess = { uri ->
                onAvatarSelected(uri)
            })
        }
    }
}

private fun registerUser(
    login: String,
    password: String,
    fullName: String,
    birthDate: String,
    gender: String,
    avatarUri: String,
    db: DatabaseHandler,
    ctx: android.content.Context,
    scope: kotlinx.coroutines.CoroutineScope,
    onSuccess: () -> Unit,
    onLoadingChanged: (Boolean) -> Unit
) {
    if (!validateInput(login, password, fullName, birthDate, gender, ctx)) return

    onLoadingChanged(true)
    scope.launch {
        try {
            val existingUser = withContext(Dispatchers.IO) {
                db.getUserByLogin(login)
            }
            if (existingUser != null) {
                withContext(Dispatchers.Main) {
                    Toast.makeText(ctx, "Такой пользователь уже есть", Toast.LENGTH_SHORT).show()
                }
                onLoadingChanged(false)
                return@launch
            }

            val isFirstUser = withContext(Dispatchers.IO) {
                db.countUsers() == 0
            }

            val user = User(
                login = login,
                password = password,
                fullName = fullName,
                birthDate = birthDate,
                gender = gender,
                avatarUri = avatarUri,
                isAdmin = isFirstUser
            )

            val result = withContext(Dispatchers.IO) {
                db.addUser(user)
            }

            if (result != -1L) {
                AuthLogger.log(ctx, "Registered: $login (admin=$isFirstUser)")
                withContext(Dispatchers.Main) {
                    Toast.makeText(ctx, "Пользователь зарегистрирован!", Toast.LENGTH_SHORT).show()
                    onSuccess()
                }
            } else {
                withContext(Dispatchers.Main) {
                    Toast.makeText(ctx, "Ошибка при сохранении пользователя", Toast.LENGTH_SHORT).show()
                }
            }
        } catch (e: Exception) {
            e.printStackTrace()
            withContext(Dispatchers.Main) {
                Toast.makeText(ctx, "Ошибка регистрации: ${e.message}", Toast.LENGTH_LONG).show()
            }
        } finally {
            onLoadingChanged(false)
        }
    }
}

private fun validateInput(
    login: String,
    password: String,
    fullName: String,
    birthDate: String,
    gender: String,
    ctx: android.content.Context
): Boolean {
    if (login.isEmpty()) {
        Toast.makeText(ctx, "Введите логин", Toast.LENGTH_SHORT).show()
        return false
    }
    if (password.isEmpty()) {
        Toast.makeText(ctx, "Введите пароль", Toast.LENGTH_SHORT).show()
        return false
    }
    if (fullName.isEmpty()) {
        Toast.makeText(ctx, "Введите ФИО", Toast.LENGTH_SHORT).show()
        return false
    }
    if (birthDate.isEmpty()) {
        Toast.makeText(ctx, "Введите дату рождения", Toast.LENGTH_SHORT).show()
        return false
    }
    if (!validDate(birthDate)) {
        Toast.makeText(ctx, "Неправильный формат даты (дд.мм.гггг)", Toast.LENGTH_SHORT).show()
        return false
    }
    if (gender.isEmpty()) {
        Toast.makeText(ctx, "Выберите пол", Toast.LENGTH_SHORT).show()
        return false
    }
    return true
}

private fun validDate(birthDate: String): Boolean {
    val regex = "\\d{2}\\.\\d{2}\\.\\d{4}".toRegex()
    return birthDate.matches(regex)
}