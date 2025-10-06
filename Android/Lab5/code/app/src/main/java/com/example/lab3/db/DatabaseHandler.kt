package com.example.lab3.db

import android.content.ContentValues
import android.content.Context
import android.database.Cursor
import android.database.sqlite.SQLiteDatabase
import android.database.sqlite.SQLiteOpenHelper
import com.example.lab3.model.User

class DatabaseHandler(context: Context) :
    SQLiteOpenHelper(context, DATABASE_NAME, null, DATABASE_VERSION) {

    companion object {
        private const val DATABASE_VERSION = 1
        private const val DATABASE_NAME = "Users.db"
    }

    override fun onCreate(db: SQLiteDatabase) {
        val createTable = """
            CREATE TABLE ${DBContract.UserEntry.TABLE_NAME} (
                ${DBContract.UserEntry.COLUMN_ID} INTEGER PRIMARY KEY AUTOINCREMENT,
                ${DBContract.UserEntry.COLUMN_LOGIN} TEXT UNIQUE,
                ${DBContract.UserEntry.COLUMN_PASSWORD} TEXT,
                ${DBContract.UserEntry.COLUMN_FULLNAME} TEXT,
                ${DBContract.UserEntry.COLUMN_BIRTHDATE} TEXT,
                ${DBContract.UserEntry.COLUMN_GENDER} TEXT,
                ${DBContract.UserEntry.COLUMN_AVATAR} INTEGER,
                ${DBContract.UserEntry.COLUMN_IS_ADMIN} INTEGER DEFAULT 0,
                ${DBContract.UserEntry.COLUMN_THEME} INTEGER DEFAULT 0
            )
        """.trimIndent()
        db.execSQL(createTable)
    }

    override fun onUpgrade(db: SQLiteDatabase, oldVersion: Int, newVersion: Int) {
        db.execSQL("DROP TABLE IF EXISTS ${DBContract.UserEntry.TABLE_NAME}")
        onCreate(db)
    }

    fun addUser(user: User): Long {
        val db = writableDatabase
        val values = ContentValues().apply {
            put(DBContract.UserEntry.COLUMN_LOGIN, user.login)
            put(DBContract.UserEntry.COLUMN_PASSWORD, user.password)
            put(DBContract.UserEntry.COLUMN_FULLNAME, user.fullName)
            put(DBContract.UserEntry.COLUMN_BIRTHDATE, user.birthDate)
            put(DBContract.UserEntry.COLUMN_GENDER, user.gender)
            put(DBContract.UserEntry.COLUMN_AVATAR, user.avatarResId)
            put(DBContract.UserEntry.COLUMN_IS_ADMIN, if (user.isAdmin) 1 else 0)
        }
        return db.insert(DBContract.UserEntry.TABLE_NAME, null, values)
    }

    fun getUser(login: String, password: String): User? {
        val db = readableDatabase
        val cursor = db.query(
            DBContract.UserEntry.TABLE_NAME,
            null,
            "${DBContract.UserEntry.COLUMN_LOGIN}=? AND ${DBContract.UserEntry.COLUMN_PASSWORD}=?",
            arrayOf(login, password), null, null, null
        )
        val user = if (cursor.moveToFirst()) parseUser(cursor) else null
        cursor.close()
        return user
    }

    fun getUserByLogin(login: String): User? {
        val db = readableDatabase
        val cursor = db.query(
            DBContract.UserEntry.TABLE_NAME,
            null,
            "${DBContract.UserEntry.COLUMN_LOGIN}=?",
            arrayOf(login), null, null, null
        )
        val user = if (cursor.moveToFirst()) parseUser(cursor) else null
        cursor.close()
        return user
    }

    fun getAllUsers(): List<User> {
        val users = mutableListOf<User>()
        val db = readableDatabase
        val cursor = db.rawQuery("SELECT * FROM ${DBContract.UserEntry.TABLE_NAME} ORDER BY ${DBContract.UserEntry.COLUMN_FULLNAME} COLLATE NOCASE", null)
        if (cursor.moveToFirst()) {
            do {
                users.add(parseUser(cursor))
            } while (cursor.moveToNext())
        }
        cursor.close()
        return users
    }

    fun countUsers(): Int {
        val db = readableDatabase
        val cursor = db.rawQuery("SELECT COUNT(*) FROM ${DBContract.UserEntry.TABLE_NAME}", null)
        var count = 0
        if (cursor.moveToFirst()) count = cursor.getInt(0)
        cursor.close()
        return count
    }

    fun makeAdminByLogin(login: String) {
        val db = writableDatabase
        val values = ContentValues().apply { put(DBContract.UserEntry.COLUMN_IS_ADMIN, 1) }
        db.update(DBContract.UserEntry.TABLE_NAME, values, "${DBContract.UserEntry.COLUMN_LOGIN}=?", arrayOf(login))
    }

    fun deleteUser(login: String) {
        val db = writableDatabase
        val deleteRows = db.delete(DBContract.UserEntry.TABLE_NAME, "${DBContract.UserEntry.COLUMN_LOGIN}=?", arrayOf(login))
    }

    private fun parseUser(cursor: Cursor): User {
        return User(
            id = cursor.getInt(cursor.getColumnIndexOrThrow(DBContract.UserEntry.COLUMN_ID)),
            login = cursor.getString(cursor.getColumnIndexOrThrow(DBContract.UserEntry.COLUMN_LOGIN)),
            password = cursor.getString(cursor.getColumnIndexOrThrow(DBContract.UserEntry.COLUMN_PASSWORD)),
            fullName = cursor.getString(cursor.getColumnIndexOrThrow(DBContract.UserEntry.COLUMN_FULLNAME)),
            birthDate = cursor.getString(cursor.getColumnIndexOrThrow(DBContract.UserEntry.COLUMN_BIRTHDATE)),
            gender = cursor.getString(cursor.getColumnIndexOrThrow(DBContract.UserEntry.COLUMN_GENDER)),
            avatarResId = cursor.getInt(cursor.getColumnIndexOrThrow(DBContract.UserEntry.COLUMN_AVATAR)),
            isAdmin = cursor.getInt(cursor.getColumnIndexOrThrow(DBContract.UserEntry.COLUMN_IS_ADMIN)) == 1,
            theme = cursor.getInt(cursor.getColumnIndexOrThrow(DBContract.UserEntry.COLUMN_THEME))
        )
    }

    fun updateUserTheme(login: String, theme: Int) {
        val db = writableDatabase
        val values = ContentValues().apply {
            put(DBContract.UserEntry.COLUMN_THEME, theme)
        }
        db.update(
            DBContract.UserEntry.TABLE_NAME,
            values,
            "${DBContract.UserEntry.COLUMN_LOGIN}=?",
            arrayOf(login)
        )
    }

}