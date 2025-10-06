package com.example.lab3.ui

import android.content.Context
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.*
import com.example.lab3.databinding.ItemUserAdminBinding
import com.example.lab3.model.User

class AdminUserAdapter(private val ctx: Context, private val users: List<User>) : BaseAdapter() {
    override fun getCount(): Int = users.size
    override fun getItem(position: Int): Any = users[position]
    override fun getItemId(position: Int): Long = position.toLong()

    override fun getView(position: Int, convertView: View?, parent: ViewGroup?): View {
        val binding: ItemUserAdminBinding
        val view: View

        if (convertView == null) {
            binding = ItemUserAdminBinding.inflate(LayoutInflater.from(ctx), parent, false)
            view = binding.root
            view.tag = binding
        } else {
            view = convertView
            binding = view.tag as ItemUserAdminBinding
        }

        val user = users[position]

        binding.userAvatar.setImageResource(user.avatarResId)
        binding.userLogin.text = user.login
        binding.userFullName.text = user.fullName
        binding.userAdmin.text = if (user.isAdmin) "👑 Админ" else ""

        return view
    }
}
