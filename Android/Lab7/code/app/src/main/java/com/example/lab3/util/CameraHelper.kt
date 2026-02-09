package com.example.lab3.util

import android.app.Activity
import android.content.Intent
import android.content.pm.PackageManager
import android.net.Uri
import android.provider.MediaStore
import android.util.Log
import android.widget.Toast
import androidx.activity.ComponentActivity
import androidx.activity.result.contract.ActivityResultContracts
import androidx.core.content.ContextCompat
import androidx.core.content.FileProvider
import java.io.File
import java.io.IOException
import java.text.SimpleDateFormat
import java.util.*

class CameraHelper(private val activity: ComponentActivity) {
    var currentPhotoPath: String? = null
    private var onPhotoTaken: ((String) -> Unit)? = null
    private var onPhotoCanceled: (() -> Unit)? = null

    private val requestPermissionLauncher = activity.registerForActivityResult(
        ActivityResultContracts.RequestPermission()
    ) { isGranted ->
        if (isGranted) {
            dispatchTakePictureIntent()
        } else {
            Toast.makeText(activity, "Разрешение на камеру необходимо для съемки фото", Toast.LENGTH_SHORT).show()
        }
    }

    private val takePictureResult = activity.registerForActivityResult(
        ActivityResultContracts.StartActivityForResult()
    ) { result ->
        if (result.resultCode == Activity.RESULT_OK) {
            currentPhotoPath?.let { path ->
                val photoUri = "file://$path"
                Log.d("CameraHelper", "Фото сделано, путь: $photoUri")
                onPhotoTaken?.invoke(photoUri)
            } ?: run {
                Log.e("CameraHelper", "currentPhotoPath is null!")
                Toast.makeText(activity, "Ошибка: путь к фото не сохранен", Toast.LENGTH_SHORT).show()
            }
        } else {
            Log.d("CameraHelper", "Съемка фото отменена")
            onPhotoCanceled?.invoke()
        }
    }

    fun takePhoto(
        onSuccess: (String) -> Unit,
        onCancel: () -> Unit = { Toast.makeText(activity, "Съемка фото отменена", Toast.LENGTH_SHORT).show() }
    ) {
        this.onPhotoTaken = onSuccess
        this.onPhotoCanceled = onCancel

        Log.d("CameraHelper", "Запуск камеры...")
        when {
            ContextCompat.checkSelfPermission(
                activity,
                android.Manifest.permission.CAMERA
            ) == PackageManager.PERMISSION_GRANTED -> {
                Log.d("CameraHelper", "Разрешение на камеру есть")
                dispatchTakePictureIntent()
            }
            else -> {
                Log.d("CameraHelper", "Запрос разрешения на камеру")
                requestPermissionLauncher.launch(android.Manifest.permission.CAMERA)
            }
        }
    }

    private fun dispatchTakePictureIntent() {
        Log.d("CameraHelper", "Создание Intent для камеры")
        Intent(MediaStore.ACTION_IMAGE_CAPTURE).also { takePictureIntent ->
            takePictureIntent.resolveActivity(activity.packageManager)?.also {
                Log.d("CameraHelper", "Найдено приложение для камеры")
                val photoFile: File? = try {
                    createImageFile()
                } catch (ex: IOException) {
                    Log.e("CameraHelper", "Ошибка создания файла", ex)
                    Toast.makeText(activity, "Ошибка создания файла для фото", Toast.LENGTH_SHORT).show()
                    null
                }

                photoFile?.also {
                    Log.d("CameraHelper", "Файл создан: ${it.absolutePath}")
                    val photoURI: Uri = FileProvider.getUriForFile(
                        activity,
                        "${activity.packageName}.fileprovider",
                        it
                    )
                    Log.d("CameraHelper", "URI создан: $photoURI")

                    takePictureIntent.putExtra(MediaStore.EXTRA_OUTPUT, photoURI)
                    takePictureResult.launch(takePictureIntent)
                } ?: run {
                    Log.e("CameraHelper", "Файл не создан")
                    Toast.makeText(activity, "Не удалось создать файл для фото", Toast.LENGTH_SHORT).show()
                }
            } ?: run {
                Log.e("CameraHelper", "Не найдено приложение для камеры")
                Toast.makeText(activity, "Не найдено приложение для камеры", Toast.LENGTH_SHORT).show()
            }
        }
    }

    @Throws(IOException::class)
    private fun createImageFile(): File {
        val timeStamp: String = SimpleDateFormat("yyyyMMdd_HHmmss", Locale.getDefault()).format(Date())
        val storageDir: File? = activity.getExternalFilesDir(null)

        return File.createTempFile(
            "JPEG_${timeStamp}_",
            ".jpg",
            storageDir
        ).apply {
            currentPhotoPath = absolutePath
            Log.d("CameraHelper", "Файл создан и путь сохранен: $absolutePath")
        }
    }
}