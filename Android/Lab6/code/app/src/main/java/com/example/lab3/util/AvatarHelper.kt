package com.example.lab3.util

import android.content.Context
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.net.Uri
import android.widget.ImageView
import java.io.InputStream

object AvatarHelper {
    private val defaultAvatars = listOf(
        "android.resource://android/drawable/ic_menu_camera",
        "android.resource://android/drawable/ic_menu_gallery",
        "android.resource://android/drawable/ic_menu_manage"
    )

    fun loadAvatar(context: Context, imageView: ImageView, avatarUri: String?) {
        try {
            when {
                avatarUri == null -> {
                    imageView.setImageResource(android.R.drawable.ic_menu_camera)
                }
                avatarUri.startsWith("android.resource://android/") -> {
                    when {
                        avatarUri.contains("ic_menu_camera") -> imageView.setImageResource(android.R.drawable.ic_menu_camera)
                        avatarUri.contains("ic_menu_gallery") -> imageView.setImageResource(android.R.drawable.ic_menu_gallery)
                        avatarUri.contains("ic_menu_manage") -> imageView.setImageResource(android.R.drawable.ic_menu_manage)
                        else -> imageView.setImageResource(android.R.drawable.ic_menu_camera)
                    }
                }
                avatarUri.startsWith("file://") -> {
                    try {
                        val uri = Uri.parse(avatarUri)
                        val inputStream: InputStream? = context.contentResolver.openInputStream(uri)
                        val bitmap: Bitmap? = BitmapFactory.decodeStream(inputStream)
                        inputStream?.close()
                        bitmap?.let { imageView.setImageBitmap(it) } ?: setDefaultAvatar(imageView)
                    } catch (e: Exception) {
                        setDefaultAvatar(imageView)
                    }
                }
                else -> {
                    setDefaultAvatar(imageView)
                }
            }
        } catch (e: Exception) {
            setDefaultAvatar(imageView)
        }
    }

    private fun setDefaultAvatar(imageView: ImageView) {
        try {
            imageView.setImageResource(android.R.drawable.ic_menu_camera)
        } catch (e: Exception) {
        }
    }

    fun getDefaultAvatarUri(index: Int): String {
        return defaultAvatars.getOrElse(index) { defaultAvatars.first() }
    }
}