using System;
using System.Collections.Generic;
using System.Windows.Forms;
using Newtonsoft.Json;

namespace Lab1
{
    public partial class FormChangeData : Form
    {
        // Списки для хранения объектов и их утверждений
        private List<string> _objects;
        private Dictionary<string, List<string>> _objectStatements;

        public FormChangeData()
        {
            InitializeComponent();

            // Инициализируем структуры данных
            _objects = new List<string>();
            _objectStatements = new Dictionary<string, List<string>>();
        }

        public FormChangeData(List<string> _objects, Dictionary<string, List<string>> _objectStatements)
        {
            InitializeComponent();

            // Инициализируем структуры данных
            this._objects = _objects;
            this._objectStatements = _objectStatements;
            foreach (var item in _objects)
            {
                lstObjects.Items.Add(item);
            }
            foreach (var item in _objectStatements.Values)
            {
                foreach (var item2 in item)
                {
                    lstStatements.Items.Add(item2);
                }
            }
        }

        // === ОБРАБОТЧИКИ ДЛЯ ОБЪЕКТОВ ===

        private void btnAddObject_Click(object sender, EventArgs e)
        {
            // Пример: просто показать диалог ввода (или сделайте свою форму ввода)
            string newObject = PromptInput("Добавить утверждение", "Введите название утверждения:");
            if (!string.IsNullOrWhiteSpace(newObject) && !_objects.Contains(newObject))
            {

                _objects.Add(newObject);
                _objectStatements[newObject] = new List<string>();
                lstObjects.Items.Add(newObject);
            }
        }

        private void btnEditObject_Click(object sender, EventArgs e)
        {
            if (lstObjects.SelectedItem == null) return;

            string oldObject = lstObjects.SelectedItem.ToString();
            string newObject = PromptInput("Изменить утверждение", "Введите новое название утверждения:", oldObject);
            if (!string.IsNullOrWhiteSpace(newObject) && !_objects.Contains(newObject))
            {
                // Переносим утверждения
                var oldStatements = _objectStatements[oldObject];
                _objectStatements.Remove(oldObject);
                _objectStatements[newObject] = oldStatements;

                // Обновляем список
                int index = _objects.IndexOf(oldObject);
                _objects[index] = newObject;

                // Обновляем элемент в ListBox
                int lbIndex = lstObjects.SelectedIndex;
                lstObjects.Items[lbIndex] = newObject;
            }
        }

        private void btnRemoveObject_Click(object sender, EventArgs e)
        {
            if (lstObjects.SelectedItem == null) return;

            string selectedObject = lstObjects.SelectedItem.ToString();
            _objects.Remove(selectedObject);
            foreach (var item in _objectStatements[selectedObject])
            {
                lstStatements.Items.Remove(item); // Очищаем связанные 
            }
            _objectStatements.Remove(selectedObject);

            lstObjects.Items.Remove(selectedObject);
        }

        // === ОБРАБОТЧИКИ ДЛЯ УТВЕРЖДЕНИЙ ===

        private void btnAddStatement_Click(object sender, EventArgs e)
        {
            if (lstObjects.SelectedItem == null) return;

            string selectedObject = lstObjects.SelectedItem.ToString();
            string newStatement = PromptInput("Добавить объект", "Введите текст объекта:");
            if (!string.IsNullOrWhiteSpace(newStatement) && !_objectStatements[selectedObject].Contains(newStatement))
            {
                _objectStatements[selectedObject].Add(newStatement);
                lstStatements.Items.Add(newStatement);
            }
        }

        private void btnEditStatement_Click(object sender, EventArgs e)
        {
            if (lstObjects.SelectedItem == null || lstStatements.SelectedItem == null) return;

            string selectedObject = lstObjects.SelectedItem.ToString();
            string oldStatement = lstStatements.SelectedItem.ToString();

            string newStatement = PromptInput("Изменить объект", "Введите новый текст объекта:", oldStatement);
            if (!string.IsNullOrWhiteSpace(newStatement) && !_objectStatements[selectedObject].Contains(newStatement))
            {
                int index = _objectStatements[selectedObject].IndexOf(oldStatement);
                _objectStatements[selectedObject][index] = newStatement;

                int lbIndex = lstStatements.SelectedIndex;
                lstStatements.Items[lbIndex] = newStatement;
            }
        }

        private void btnRemoveStatement_Click(object sender, EventArgs e)
        {
            if (lstObjects.SelectedItem == null || lstStatements.SelectedItem == null) return;

            string selectedObject = lstObjects.SelectedItem.ToString();
            string selectedStatement = lstStatements.SelectedItem.ToString();

            _objectStatements[selectedObject].Remove(selectedStatement);
            lstStatements.Items.Remove(selectedStatement);
        }

        // === КНОПКИ НАВИГАЦИИ И СОХРАНЕНИЯ ===

        private void btnBack_Click(object sender, EventArgs e)
        {
            // Закрываем форму или возвращаемся в главное окно
            this.Close();
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            string json = JsonConvert.SerializeObject(_objectStatements);
            System.IO.File.WriteAllText(@"_objectStatements.json", json);

            json = JsonConvert.SerializeObject(_objects);
            System.IO.File.WriteAllText(@"_objects.json", json);

            MessageBox.Show("База знаний сохранена!",
                "Сохранение",
                MessageBoxButtons.OK,
                MessageBoxIcon.Information);
        }

        // === ПРИМЕР ВСПОМОГАТЕЛЬНОГО МЕТОДА ДЛЯ ВВОДА ТЕКСТА ===

        /// <summary>
        /// Пример простого диалога ввода текста. 
        /// В реальном проекте можно создать отдельную форму для ввода.
        /// </summary>
        private string PromptInput(string title, string prompt, string defaultValue = "")
        {
            // Можно реализовать свою форму ввода или воспользоваться Microsoft.VisualBasic.Interaction.InputBox (VB)
            // Ниже упрощённый вариант с использованием статического класса Prompt (если он у вас есть).
            // Здесь заглушка — возвращаем значение defaultValue или новый ввод.
            // Для наглядности оставлен только InputBox из VB (не забудьте ссылку на Microsoft.VisualBasic!).
            return InputBox.Show(title, prompt, defaultValue);
        }

        private void lblStatements_Click(object sender, EventArgs e)
        {

        }
    }

    public static class InputBox
    {
        public static string Show(string title, string prompt, string defaultValue = "")
        {
            Form form = new Form();
            Label label = new Label() { Text = prompt, Left = 10, Top = 10, AutoSize = true };
            TextBox textBox = new TextBox() { Left = 10, Top = 30, Width = 250, Text = defaultValue };
            Button buttonOk = new Button() { Text = "OK", Left = 90, Top = 60, DialogResult = DialogResult.OK };
            Button buttonCancel = new Button() { Text = "Отмена", Left = 170, Top = 60, DialogResult = DialogResult.Cancel };

            form.Text = title;
            form.ClientSize = new System.Drawing.Size(280, 100);
            form.Controls.Add(label);
            form.Controls.Add(textBox);
            form.Controls.Add(buttonOk);
            form.Controls.Add(buttonCancel);
            form.AcceptButton = buttonOk;
            form.CancelButton = buttonCancel;
            form.StartPosition = FormStartPosition.CenterParent;

            return (form.ShowDialog() == DialogResult.OK) ? textBox.Text : defaultValue;
        }
    }
}
