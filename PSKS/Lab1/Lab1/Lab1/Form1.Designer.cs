namespace Lab1
{
    partial class Form1
    {
        /// <summary>
        /// Обязательная переменная конструктора.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Освободить все используемые ресурсы.
        /// </summary>
        /// <param name="disposing">истинно, если управляемый ресурс должен быть удален; иначе ложно.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Код, автоматически созданный конструктором форм Windows

        /// <summary>
        /// Требуемый метод для поддержки конструктора — не изменяйте 
        /// содержимое этого метода с помощью редактора кода.
        /// </summary>
        private void InitializeComponent()
        {
            this.buttonFillData = new System.Windows.Forms.Button();
            this.buttonChangeData = new System.Windows.Forms.Button();
            this.buttonPlay = new System.Windows.Forms.Button();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // buttonFillData
            // 
            this.buttonFillData.Location = new System.Drawing.Point(11, 644);
            this.buttonFillData.Name = "buttonFillData";
            this.buttonFillData.Size = new System.Drawing.Size(219, 33);
            this.buttonFillData.TabIndex = 0;
            this.buttonFillData.Text = "Загрузить данные";
            this.buttonFillData.UseVisualStyleBackColor = true;
            this.buttonFillData.Click += new System.EventHandler(this.buttonFillData_Click);
            // 
            // buttonChangeData
            // 
            this.buttonChangeData.Location = new System.Drawing.Point(353, 644);
            this.buttonChangeData.Name = "buttonChangeData";
            this.buttonChangeData.Size = new System.Drawing.Size(219, 33);
            this.buttonChangeData.TabIndex = 1;
            this.buttonChangeData.Text = "Создание данных";
            this.buttonChangeData.UseVisualStyleBackColor = true;
            this.buttonChangeData.Click += new System.EventHandler(this.buttonChangeData_Click_1);
            // 
            // buttonPlay
            // 
            this.buttonPlay.Location = new System.Drawing.Point(701, 644);
            this.buttonPlay.Name = "buttonPlay";
            this.buttonPlay.Size = new System.Drawing.Size(219, 33);
            this.buttonPlay.TabIndex = 2;
            this.buttonPlay.Text = "Начать опрос";
            this.buttonPlay.UseVisualStyleBackColor = true;
            this.buttonPlay.Click += new System.EventHandler(this.buttonPlay_Click);
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = global::Lab1.Properties.Resources.Nyukhach;
            this.pictureBox1.Location = new System.Drawing.Point(1, 1);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(924, 626);
            this.pictureBox1.TabIndex = 3;
            this.pictureBox1.TabStop = false;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(932, 689);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.buttonPlay);
            this.Controls.Add(this.buttonChangeData);
            this.Controls.Add(this.buttonFillData);
            this.Name = "Form1";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button buttonFillData;
        private System.Windows.Forms.Button buttonChangeData;
        private System.Windows.Forms.Button buttonPlay;
        private System.Windows.Forms.PictureBox pictureBox1;
    }
}

