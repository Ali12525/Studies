package com.example.lab3

import android.content.Intent
import android.net.Uri
import android.os.Bundle
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity

class MenuActivity : AppCompatActivity() {

    private lateinit var btnStart: Button
    private lateinit var btnProfile: Button
    private lateinit var btnSettings: Button
    private lateinit var btnReport: Button
    private lateinit var btnMenuExit: Button

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_menu)

        btnStart = findViewById(R.id.btnStart)
        btnProfile = findViewById(R.id.btnProfile)
        btnSettings = findViewById(R.id.btnSettings)
        btnReport = findViewById(R.id.btnReport)
        btnMenuExit = findViewById(R.id.btnMenuExit)

        btnProfile.setOnClickListener {
            val login = intent.getStringExtra("user_login")
            val intent = Intent(this, ProfileActivity::class.java)
            intent.putExtra("user_login", login)
            startActivity(intent)
        }


        btnSettings.setOnClickListener {
            startActivity(Intent(this, SettingsActivity::class.java))
        }

        btnReport.setOnClickListener {
            val intent = Intent(Intent.ACTION_SENDTO).apply {
                data = Uri.parse("mailto:support@example.com")
                putExtra(Intent.EXTRA_SUBJECT, "Сообщение о проблеме")
            }
            startActivity(intent)
        }

        btnMenuExit.setOnClickListener {
            finishAffinity()
        }
    }
}
