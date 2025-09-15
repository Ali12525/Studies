package com.example.lab2

import android.content.Context
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ArrayAdapter
import android.widget.ImageView
import android.widget.TextView

class UserAdapter(
    private val context: Context,
    private val users: List<User>
) : ArrayAdapter<User>(context, R.layout.item_user, users) {

    override fun getView(position: Int, convertView: View?, parent: ViewGroup): View {
        val view = convertView ?: LayoutInflater.from(context)
            .inflate(R.layout.item_user, parent, false)

        val user = getItem(position) ?: return view

        val avatarImage = view.findViewById<ImageView>(R.id.userAvatar)
        val loginTextView = view.findViewById<TextView>(R.id.userLogin)
        val fullNameTextView = view.findViewById<TextView>(R.id.userFullName)
        val genderTextView = view.findViewById<TextView>(R.id.userGender)

        avatarImage.setImageResource(user.avatarResId)
        loginTextView.text = user.login
        fullNameTextView.text = user.fullName
        genderTextView.text = user.gender

        return view
    }
}