import android.provider.BaseColumns

object DBContract {
    object UserEntry : BaseColumns {
        const val TABLE_NAME = "users"
        const val COLUMN_ID = "id"
        const val COLUMN_LOGIN = "login"
        const val COLUMN_PASSWORD = "password"
        const val COLUMN_FULLNAME = "fullName"
        const val COLUMN_BIRTHDATE = "birthDate"
        const val COLUMN_GENDER = "gender"
        const val COLUMN_AVATAR_URI = "avatarUri"
        const val COLUMN_IS_ADMIN = "isAdmin"
        const val COLUMN_THEME = "theme"
    }
}