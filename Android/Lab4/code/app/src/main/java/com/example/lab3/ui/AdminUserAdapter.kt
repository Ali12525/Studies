package com.example.lab3.ui

import android.content.Context
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.*
import com.example.lab3.R
import com.example.lab3.model.User

class AdminUserAdapter(private val ctx: Context, private val users: List<User>) : BaseAdapter() {
    override fun getCount(): Int = users.size
    override fun getItem(position: Int): Any = users[position]
    override fun getItemId(position: Int): Long = position.toLong()
    override fun getView(position: Int, convertView: View?, parent: ViewGroup?): View {
        val view = convertView ?: LayoutInflater.from(ctx).inflate(R.layout.item_user_admin, parent, false)
        val user = users[position]
        val iv = view.findViewById<ImageView>(R.id.userAvatar)
        val tvLogin = view.findViewById<TextView>(R.id.userLogin)
        val tvFull = view.findViewById<TextView>(R.id.userFullName)
        val tvAdmin = view.findViewById<TextView>(R.id.userAdmin)

        iv.setImageResource(user.avatarResId)
        tvLogin.text = user.login
        tvFull.text = user.fullName
        tvAdmin.text = if (user.isAdmin) "👑 Админ" else ""

        return view
    }
}
