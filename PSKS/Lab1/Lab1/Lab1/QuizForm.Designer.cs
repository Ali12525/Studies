namespace Lab1
{
    partial class QuizForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.MainSplitContainer = new System.Windows.Forms.SplitContainer();
            this.QuestionLabel = new System.Windows.Forms.Label();
            this.QuestionNumberLabel = new System.Windows.Forms.Label();
            this.NextButton = new System.Windows.Forms.Button();
            this.FinishButton = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.MainSplitContainer)).BeginInit();
            this.MainSplitContainer.Panel1.SuspendLayout();
            this.MainSplitContainer.Panel2.SuspendLayout();
            this.MainSplitContainer.SuspendLayout();
            this.SuspendLayout();
            // 
            // MainSplitContainer
            // 
            this.MainSplitContainer.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.MainSplitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.MainSplitContainer.IsSplitterFixed = true;
            this.MainSplitContainer.Location = new System.Drawing.Point(0, 0);
            this.MainSplitContainer.Name = "MainSplitContainer";
            this.MainSplitContainer.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // MainSplitContainer.Panel1
            // 
            this.MainSplitContainer.Panel1.Controls.Add(this.QuestionLabel);
            this.MainSplitContainer.Panel1.Controls.Add(this.QuestionNumberLabel);
            // 
            // MainSplitContainer.Panel2
            // 
            this.MainSplitContainer.Panel2.Controls.Add(this.button1);
            this.MainSplitContainer.Panel2.Controls.Add(this.NextButton);
            this.MainSplitContainer.Panel2.Controls.Add(this.FinishButton);
            this.MainSplitContainer.Size = new System.Drawing.Size(705, 395);
            this.MainSplitContainer.SplitterDistance = 320;
            this.MainSplitContainer.TabIndex = 0;
            // 
            // QuestionLabel
            // 
            this.QuestionLabel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.QuestionLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F);
            this.QuestionLabel.Location = new System.Drawing.Point(0, 20);
            this.QuestionLabel.Name = "QuestionLabel";
            this.QuestionLabel.Size = new System.Drawing.Size(703, 298);
            this.QuestionLabel.TabIndex = 1;
            this.QuestionLabel.Text = "??!?!?!?";
            this.QuestionLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // QuestionNumberLabel
            // 
            this.QuestionNumberLabel.BackColor = System.Drawing.SystemColors.Control;
            this.QuestionNumberLabel.Dock = System.Windows.Forms.DockStyle.Top;
            this.QuestionNumberLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F);
            this.QuestionNumberLabel.Location = new System.Drawing.Point(0, 0);
            this.QuestionNumberLabel.Name = "QuestionNumberLabel";
            this.QuestionNumberLabel.Size = new System.Drawing.Size(703, 20);
            this.QuestionNumberLabel.TabIndex = 0;
            this.QuestionNumberLabel.Text = "QuestionNumberLabel";
            this.QuestionNumberLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.QuestionNumberLabel.Click += new System.EventHandler(this.QuestionNumberLabel_Click);
            // 
            // NextButton
            // 
            this.NextButton.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.NextButton.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F);
            this.NextButton.Location = new System.Drawing.Point(472, 15);
            this.NextButton.Name = "NextButton";
            this.NextButton.Size = new System.Drawing.Size(90, 39);
            this.NextButton.TabIndex = 1;
            this.NextButton.Text = "Да";
            this.NextButton.UseVisualStyleBackColor = true;
            this.NextButton.Click += new System.EventHandler(this.NextButton_Click);
            // 
            // FinishButton
            // 
            this.FinishButton.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.FinishButton.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F);
            this.FinishButton.Location = new System.Drawing.Point(192, 15);
            this.FinishButton.Name = "FinishButton";
            this.FinishButton.Size = new System.Drawing.Size(90, 39);
            this.FinishButton.TabIndex = 0;
            this.FinishButton.Text = "Закончить";
            this.FinishButton.UseVisualStyleBackColor = true;
            this.FinishButton.Click += new System.EventHandler(this.FinishButton_Click);
            // 
            // button1
            // 
            this.button1.Anchor = System.Windows.Forms.AnchorStyles.Right;
            this.button1.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F);
            this.button1.Location = new System.Drawing.Point(376, 15);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(90, 39);
            this.button1.TabIndex = 4;
            this.button1.Text = "Нет";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // QuizForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(705, 395);
            this.Controls.Add(this.MainSplitContainer);
            this.Name = "QuizForm";
            this.Text = "Опрос";
            this.MainSplitContainer.Panel1.ResumeLayout(false);
            this.MainSplitContainer.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.MainSplitContainer)).EndInit();
            this.MainSplitContainer.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer MainSplitContainer;
        private System.Windows.Forms.Label QuestionLabel;
        private System.Windows.Forms.Label QuestionNumberLabel;
        private System.Windows.Forms.Button NextButton;
        private System.Windows.Forms.Button FinishButton;
        private System.Windows.Forms.Button button1;
    }
}