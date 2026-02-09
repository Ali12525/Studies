package com.example.lab3.ui

import android.content.Intent
import android.os.Bundle
import android.widget.ImageView
import androidx.activity.compose.setContent
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.material.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.unit.dp
import androidx.compose.ui.viewinterop.AndroidView
import com.example.lab3.R
import com.example.lab3.db.DatabaseHandler
import com.example.lab3.model.User
import com.example.lab3.util.AvatarHelper
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext

class AdminActivity : BaseActivity() {
    private lateinit var db: DatabaseHandler

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        db = DatabaseHandler(this)

        setContent {
            MaterialTheme {
                Surface(
                    modifier = Modifier.fillMaxSize(),
                    color = MaterialTheme.colors.background
                ) {
                    AdminScreen(db = db)
                }
            }
        }
    }
}

@Composable
fun AdminScreen(db: DatabaseHandler) {
    val ctx = LocalContext.current
    var users by remember { mutableStateOf(listOf<User>()) }
    var isLoading by remember { mutableStateOf(true) }

    LaunchedEffect(Unit) {
        try {
            users = withContext(Dispatchers.IO) {
                db.getAllUsers()
            }
        } catch (e: Exception) {
            e.printStackTrace()
        } finally {
            isLoading = false
        }
    }

    Column(modifier = Modifier.fillMaxSize().padding(12.dp)) {
        Button(onClick = {
            val intent = Intent(ctx, LogsActivity::class.java)
            ctx.startActivity(intent)
        }) {
            Text("Просмотр логов")
        }

        Spacer(modifier = Modifier.height(8.dp))

        if (isLoading) {
            Box(modifier = Modifier.fillMaxSize(), contentAlignment = Alignment.Center) {
                CircularProgressIndicator()
            }
        } else {
            LazyColumn(
                modifier = Modifier.fillMaxSize(),
                contentPadding = PaddingValues(vertical = 8.dp)
            ) {
                items(users) { user ->
                    UserListItem(
                        user = user,
                        onUserClick = { selectedUser ->
                            val intent = Intent(ctx, ProfileActivity::class.java)
                            intent.putExtra("user_login", selectedUser.login)
                            intent.putExtra("from_admin", true)
                            ctx.startActivity(intent)
                        }
                    )
                    Spacer(modifier = Modifier.height(12.dp))
                    Divider()
                }
            }
        }
    }
}

@Composable
fun UserListItem(user: User, onUserClick: (User) -> Unit) {
    Row(
        modifier = Modifier
            .fillMaxWidth()
            .padding(vertical = 12.dp)
            .clickable { onUserClick(user) }
    ) {
        AndroidView(
            factory = { context ->
                ImageView(context).apply {
                    AvatarHelper.loadAvatar(context, this, user.avatarUri)
                }
            },
            modifier = Modifier.size(56.dp)
        )

        Spacer(modifier = Modifier.width(12.dp))

        Column(modifier = Modifier.weight(1f)) {
            Text(
                text = user.login,
                style = MaterialTheme.typography.h6
            )
            Text(
                text = user.fullName,
                style = MaterialTheme.typography.body2
            )
        }

        if (user.isAdmin) {
            Text("👑 Админ")
        }
    }
}