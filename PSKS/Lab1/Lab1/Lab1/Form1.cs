using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Newtonsoft.Json;

namespace Lab1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            this.Text = "Главное окно";
            buttonChangeData.Click += ButtonChangeData_Click;

        }

        private void ButtonChangeData_Click(object sender, EventArgs e)
        {
            FormChangeData formChangeData = new FormChangeData();
            formChangeData.ShowDialog(); // Открывает форму модально
        }

        private void buttonFillData_Click(object sender, EventArgs e)
        {
            string json = File.ReadAllText(@"_objects.json");
            List<string> _objects = JsonConvert.DeserializeObject<List<string>>(json);

            json = File.ReadAllText(@"_objectStatements.json");
            Dictionary<string, List<string>> _objectStatements = JsonConvert.DeserializeObject<Dictionary<string, List<string>>>(json);

            FormChangeData formChangeData = new FormChangeData(_objects, _objectStatements);
            formChangeData.ShowDialog(); // Открывает форму модально
        }

        private void buttonChangeData_Click_1(object sender, EventArgs e)
        {

        }

        private void buttonPlay_Click(object sender, EventArgs e)
        {
            string json = File.ReadAllText(@"_objects.json");
            List<string> _objects = JsonConvert.DeserializeObject<List<string>>(json);

            json = File.ReadAllText(@"_objectStatements.json");
            Dictionary<string, List<string>> _objectStatements = JsonConvert.DeserializeObject<Dictionary<string, List<string>>>(json);

            QuizForm formChangeData = new QuizForm(_objectStatements);
            formChangeData.ShowDialog(); // Открывает форму модально
        }
    }
}
