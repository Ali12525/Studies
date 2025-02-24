using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Newtonsoft.Json;

namespace Lab1
{
    public partial class QuizForm : Form
    {
        private const int DISPLAY_RESULT_COUNT = 3;

        private readonly Dictionary<string, List<string>> _statementsToObjects;
        private readonly List<string> _statements;
        private readonly Dictionary<string, int> _objectsFrequency;

        private int _currentStatementIndex = 0;

        public QuizForm(Dictionary<string, List<string>> statementsToObjects)
        {
            InitializeComponent();

            _statementsToObjects = statementsToObjects;
            _statements = _statementsToObjects.Keys.ToList();

            _objectsFrequency = new Dictionary<string, int>();
            foreach (List<string> objects in _statementsToObjects.Values)
            {
                foreach (string soloObject in objects)
                {
                    if (_objectsFrequency.ContainsKey(soloObject))
                    {
                        continue;
                    }
                    _objectsFrequency.Add(soloObject, 0);
                }
            }

            UpdateQuestion();
        }

        private void UpdateQuestion()
        {
            if (_currentStatementIndex >= _statements.Count)
            {
                Finish();
                return;
            }
            QuestionNumberLabel.Text = $"Вопрос номер {_currentStatementIndex + 1}";
            QuestionLabel.Text = $"{_statements[_currentStatementIndex]}?";
        }

        private void AnalizeQuestionAnswer(bool result)
        {
            if (!result)
            {
                return;
            }
            string currentStatement = _statements[_currentStatementIndex];
            foreach (string soloObject in _statementsToObjects[currentStatement])
            {
                _objectsFrequency[soloObject]++;
            }
        }

        private void Finish()
        {
            // Сначала вычисляем для каждого объекта общее количество условий, в которых он участвует
            var totalConditions = new Dictionary<string, int>();
            foreach (var kvp in _statementsToObjects)
            {
                foreach (var obj in kvp.Value)
                {
                    if (!totalConditions.ContainsKey(obj))
                        totalConditions[obj] = 0;
                    totalConditions[obj]++;
                }
            }

            int displayCount = Math.Min(_objectsFrequency.Count, DISPLAY_RESULT_COUNT);

            // Выбираем объекты, у которых частота подтверждений > 0,
            // сортируем сначала по убыванию частоты, а затем по убыванию отношения (подтверждено/всего)
            var topObjects = _objectsFrequency
                .Where(pair => pair.Value > 0)
                .Select(pair => new
                {
                    Name = pair.Key,
                    Frequency = pair.Value,
                    Total = totalConditions[pair.Key],
                    Ratio = pair.Value / (double)totalConditions[pair.Key]
                })
                .OrderByDescending(x => x.Frequency)
                .ThenByDescending(x => x.Ratio)
                .Take(displayCount)
                .ToList();

            StringBuilder topText = new StringBuilder();
            int cdc = 0;
            foreach (var obj in topObjects)
            {
                int percent = (int)(obj.Ratio * 100);
                topText.AppendLine($"{obj.Name}");
                cdc++;
                if (cdc >= 3)
                    break;
            }

            QuestionNumberLabel.Text = "Результат";
            QuestionLabel.Text = topText.Length > 0 ? topText.ToString() : "Ничего не подходит";
            NextButton.Enabled = false;
            FinishButton.Enabled = false;
            button1.Enabled = false;

        }

        private void NextButton_Click(object sender, EventArgs e)
        {
            AnalizeQuestionAnswer(true);
            _currentStatementIndex++;
            UpdateQuestion();
        }

        private void FinishButton_Click(object sender, EventArgs e)
        {
            Finish();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            AnalizeQuestionAnswer(false);
            _currentStatementIndex++;
            UpdateQuestion();
        }

        private void QuestionNumberLabel_Click(object sender, EventArgs e)
        {

        }
    }
}
