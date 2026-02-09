package com.example.lab3.ui

import android.content.Context
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.os.VibrationEffect
import android.os.Vibrator
import android.widget.ArrayAdapter
import android.widget.Button
import com.example.lab3.databinding.ActivityGameBinding
import kotlin.random.Random

class GameActivity : BaseActivity() {
    private lateinit var binding: ActivityGameBinding

    private val board = IntArray(9)
    private val HUMAN = 1
    private val AI = 2

    private val handler = Handler(Looper.getMainLooper())
    private val vibrator by lazy { getSystemService(Context.VIBRATOR_SERVICE) as Vibrator }

    private var gameOver: Boolean = false

    private enum class Difficulty { EASY, MEDIUM, HARD }
    private var aiDifficulty: Difficulty = Difficulty.MEDIUM

    private val PREFS_NAME = "game_prefs"
    private val KEY_DIFFICULTY = "ai_difficulty"

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityGameBinding.inflate(layoutInflater)
        setContentView(binding.root)

        prefs = getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE)
        loadDifficultyFromPrefs()
        setupDifficultySpinner()
        setupClickListeners()
        resetBoard()
    }

    private fun setupDifficultySpinner() {
        val items = listOf("Легко", "Средне", "Сложно")
        val adapter = ArrayAdapter(this, android.R.layout.simple_spinner_item, items)
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item)
        binding.spinnerDifficulty.adapter = adapter

        binding.spinnerDifficulty.setSelection(
            when (aiDifficulty) {
                Difficulty.EASY -> 0
                Difficulty.MEDIUM -> 1
                Difficulty.HARD -> 2
            }
        )

        binding.spinnerDifficulty.setOnItemSelectedListener(object : android.widget.AdapterView.OnItemSelectedListener {
            override fun onItemSelected(parent: android.widget.AdapterView<*>, view: android.view.View?, position: Int, id: Long) {
                aiDifficulty = when (position) {
                    0 -> Difficulty.EASY
                    1 -> Difficulty.MEDIUM
                    else -> Difficulty.HARD
                }
                saveDifficultyToPrefs()
                if (!gameOver) {
                    binding.tvStatus.text = when (aiDifficulty) {
                        Difficulty.EASY -> "Уровень: Легко — Ваш ход"
                        Difficulty.MEDIUM -> "Уровень: Средне — Ваш ход"
                        Difficulty.HARD -> "Уровень: Сложно — Ваш ход"
                    }
                }
            }

            override fun onNothingSelected(parent: android.widget.AdapterView<*>) { }
        })
    }

    private fun loadDifficultyFromPrefs() {
        val v = prefs.getString(KEY_DIFFICULTY, "MEDIUM") ?: "MEDIUM"
        aiDifficulty = when (v) {
            "EASY" -> Difficulty.EASY
            "HARD" -> Difficulty.HARD
            else -> Difficulty.MEDIUM
        }
    }

    private fun saveDifficultyToPrefs() {
        prefs.edit().putString(KEY_DIFFICULTY, aiDifficulty.name).apply()
    }

    private fun setupClickListeners() {
        val buttons: List<Button> = listOf(
            binding.btn00, binding.btn01, binding.btn02,
            binding.btn10, binding.btn11, binding.btn12,
            binding.btn20, binding.btn21, binding.btn22
        )

        buttons.forEachIndexed { index: Int, btn: Button ->
            btn.setOnClickListener { onCellClicked(index) }
        }

        binding.btnReset.setOnClickListener {
            resetBoard()
        }
        binding.btnBack.setOnClickListener { finish() }
    }

    private fun onCellClicked(index: Int) {
        if (gameOver) return

        if (board[index] != 0) {
            vibrateShort()
            binding.tvStatus.text = "Клетка занята"
            return
        }

        board[index] = HUMAN
        updateUI()

        if (checkWin(HUMAN)) {
            binding.tvStatus.text = "Вы выиграли!"
            vibrateWin()
            finishGame()
            return
        }

        if (isBoardFull()) {
            binding.tvStatus.text = "Ничья"
            finishGame()
            return
        }

        binding.tvStatus.text = "Ход компьютера..."
        handler.postDelayed({ aiMove() }, 400)
    }

    private fun aiMove() {
        if (gameOver) return

        val moveIndex = when (aiDifficulty) {
            Difficulty.EASY -> aiMoveEasy()
            Difficulty.MEDIUM -> aiMoveMedium()
            Difficulty.HARD -> aiMoveHard()
        }

        if (moveIndex >= 0) {
            board[moveIndex] = AI
            updateUI()
        }

        if (checkWin(AI)) {
            binding.tvStatus.text = "Вы проиграли"
            vibrateLose()
            finishGame()
            return
        }

        if (isBoardFull()) {
            binding.tvStatus.text = "Ничья"
            finishGame()
            return
        }

        binding.tvStatus.text = "Ваш ход"
    }

    private fun aiMoveEasy(): Int {
        val emptyIndices = ArrayList<Int>()
        for (i in board.indices) if (board[i] == 0) emptyIndices.add(i)
        if (emptyIndices.isEmpty()) return -1
        return emptyIndices[Random.nextInt(emptyIndices.size)]
    }

    private fun aiMoveMedium(): Int {
        val winMove = findWinningMove(AI)
        if (winMove != -1) return winMove

        val blockMove = findWinningMove(HUMAN)
        if (blockMove != -1) return blockMove

        if (board[4] == 0) return 4

        val corners = listOf(0, 2, 6, 8)
        val freeCorners = corners.filter { board[it] == 0 }
        if (freeCorners.isNotEmpty()) return freeCorners.random()

        return aiMoveEasy()
    }

    private fun aiMoveHard(): Int {
        val result = minimax(board.copyOf(), 0, true)
        return result.index
    }

    private fun findWinningMove(player: Int): Int {
        for (i in board.indices) {
            if (board[i] == 0) {
                board[i] = player
                val win = checkWin(player)
                board[i] = 0
                if (win) return i
            }
        }
        return -1
    }

    private data class MoveResult(val index: Int, val score: Int)

    private fun minimax(b: IntArray, depth: Int, isMaximizing: Boolean): MoveResult {
        when {
            checkWinOnBoard(b, AI) -> return MoveResult(-1, 10 - depth)
            checkWinOnBoard(b, HUMAN) -> return MoveResult(-1, depth - 10)
            b.all { it != 0 } -> return MoveResult(-1, 0)
        }

        val emptyIndices = ArrayList<Int>()
        for (i in b.indices) if (b[i] == 0) emptyIndices.add(i)

        if (isMaximizing) {
            var bestScore = Int.MIN_VALUE
            var bestIndex = -1
            for (idx in emptyIndices) {
                b[idx] = AI
                val result = minimax(b, depth + 1, false)
                b[idx] = 0
                if (result.score > bestScore) {
                    bestScore = result.score
                    bestIndex = idx
                }
            }
            return MoveResult(bestIndex, bestScore)
        } else {
            var bestScore = Int.MAX_VALUE
            var bestIndex = -1
            for (idx in emptyIndices) {
                b[idx] = HUMAN
                val result = minimax(b, depth + 1, true)
                b[idx] = 0
                if (result.score < bestScore) {
                    bestScore = result.score
                    bestIndex = idx
                }
            }
            return MoveResult(bestIndex, bestScore)
        }
    }

    private fun checkWinOnBoard(b: IntArray, player: Int): Boolean {
        val wins = arrayOf(
            intArrayOf(0, 1, 2), intArrayOf(3, 4, 5), intArrayOf(6, 7, 8),
            intArrayOf(0, 3, 6), intArrayOf(1, 4, 7), intArrayOf(2, 5, 8),
            intArrayOf(0, 4, 8), intArrayOf(2, 4, 6)
        )
        for (trip in wins) {
            val a = trip[0]; val b1 = trip[1]; val c = trip[2]
            if (b[a] == player && b[b1] == player && b[c] == player) return true
        }
        return false
    }

    private fun finishGame() {
        gameOver = true
        setButtonsEnabled(false)
    }

    private fun resetBoard() {
        for (i in board.indices) board[i] = 0
        gameOver = false
        updateUI()
        setButtonsEnabled(true)
        binding.tvStatus.text = when (aiDifficulty) {
            Difficulty.EASY -> "Уровень: Легко — Ваш ход"
            Difficulty.MEDIUM -> "Уровень: Средне — Ваш ход"
            Difficulty.HARD -> "Уровень: Сложно — Ваш ход"
        }
    }

    private fun updateUI() {
        val buttons: List<Button> = listOf(
            binding.btn00, binding.btn01, binding.btn02,
            binding.btn10, binding.btn11, binding.btn12,
            binding.btn20, binding.btn21, binding.btn22
        )

        buttons.forEachIndexed { i: Int, btn: Button ->
            when (board[i]) {
                0 -> btn.text = ""
                HUMAN -> btn.text = "X"
                AI -> btn.text = "O"
            }
            btn.isEnabled = !gameOver && board[i] == 0
        }
    }

    private fun setButtonsEnabled(enabled: Boolean) {
        val buttons: List<Button> = listOf(
            binding.btn00, binding.btn01, binding.btn02,
            binding.btn10, binding.btn11, binding.btn12,
            binding.btn20, binding.btn21, binding.btn22
        )
        buttons.forEachIndexed { i, btn -> btn.isEnabled = enabled && (board[i] == 0) }
    }

    private fun isBoardFull(): Boolean = board.all { it != 0 }

    private fun checkWin(player: Int): Boolean {
        val wins = arrayOf(
            intArrayOf(0, 1, 2), intArrayOf(3, 4, 5), intArrayOf(6, 7, 8),
            intArrayOf(0, 3, 6), intArrayOf(1, 4, 7), intArrayOf(2, 5, 8),
            intArrayOf(0, 4, 8), intArrayOf(2, 4, 6)
        )

        for (trip in wins) {
            val a = trip[0]; val b = trip[1]; val c = trip[2]
            if (board[a] == player && board[b] == player && board[c] == player) return true
        }
        return false
    }

    private fun vibrate(durationMs: Long) {
        try {
            if (!vibrator.hasVibrator()) return

            if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.O) {
                vibrator.vibrate(VibrationEffect.createOneShot(durationMs, VibrationEffect.DEFAULT_AMPLITUDE))
            } else {
                @Suppress("DEPRECATION")
                vibrator.vibrate(durationMs)
            }
        } catch (_: Exception) { }
    }

    private fun vibrateShort() = vibrate(100)
    private fun vibrateWin() = vibrate(180)
    private fun vibrateLose() = vibrate(350)
}