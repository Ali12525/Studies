namespace Lab1
{
    partial class FormChangeData
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
        /// Метод, необходимый для поддержки конструктора Windows Forms.
        /// Не изменяйте его вручную; для изменения используйте конструктор форм.
        /// </summary>
        private void InitializeComponent()
        {
            this.lblObjects = new System.Windows.Forms.Label();
            this.lstObjects = new System.Windows.Forms.ListBox();
            this.btnAddObject = new System.Windows.Forms.Button();
            this.btnEditObject = new System.Windows.Forms.Button();
            this.btnRemoveObject = new System.Windows.Forms.Button();
            this.lblStatements = new System.Windows.Forms.Label();
            this.lstStatements = new System.Windows.Forms.ListBox();
            this.btnAddStatement = new System.Windows.Forms.Button();
            this.btnEditStatement = new System.Windows.Forms.Button();
            this.btnRemoveStatement = new System.Windows.Forms.Button();
            this.btnBack = new System.Windows.Forms.Button();
            this.btnSave = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // lblObjects
            // 
            this.lblObjects.AutoSize = true;
            this.lblObjects.Location = new System.Drawing.Point(12, 9);
            this.lblObjects.Name = "lblObjects";
            this.lblObjects.Size = new System.Drawing.Size(76, 13);
            this.lblObjects.TabIndex = 0;
            this.lblObjects.Text = "Утверждения";
            // 
            // lstObjects
            // 
            this.lstObjects.FormattingEnabled = true;
            this.lstObjects.Location = new System.Drawing.Point(15, 25);
            this.lstObjects.Name = "lstObjects";
            this.lstObjects.Size = new System.Drawing.Size(760, 95);
            this.lstObjects.TabIndex = 1;
            // 
            // btnAddObject
            // 
            this.btnAddObject.Location = new System.Drawing.Point(15, 126);
            this.btnAddObject.Name = "btnAddObject";
            this.btnAddObject.Size = new System.Drawing.Size(75, 23);
            this.btnAddObject.TabIndex = 2;
            this.btnAddObject.Text = "Добавить";
            this.btnAddObject.UseVisualStyleBackColor = true;
            this.btnAddObject.Click += new System.EventHandler(this.btnAddObject_Click);
            // 
            // btnEditObject
            // 
            this.btnEditObject.Location = new System.Drawing.Point(96, 126);
            this.btnEditObject.Name = "btnEditObject";
            this.btnEditObject.Size = new System.Drawing.Size(75, 23);
            this.btnEditObject.TabIndex = 3;
            this.btnEditObject.Text = "Изменить";
            this.btnEditObject.UseVisualStyleBackColor = true;
            this.btnEditObject.Click += new System.EventHandler(this.btnEditObject_Click);
            // 
            // btnRemoveObject
            // 
            this.btnRemoveObject.Location = new System.Drawing.Point(177, 126);
            this.btnRemoveObject.Name = "btnRemoveObject";
            this.btnRemoveObject.Size = new System.Drawing.Size(75, 23);
            this.btnRemoveObject.TabIndex = 4;
            this.btnRemoveObject.Text = "Удалить";
            this.btnRemoveObject.UseVisualStyleBackColor = true;
            this.btnRemoveObject.Click += new System.EventHandler(this.btnRemoveObject_Click);
            // 
            // lblStatements
            // 
            this.lblStatements.AutoSize = true;
            this.lblStatements.Location = new System.Drawing.Point(12, 167);
            this.lblStatements.Name = "lblStatements";
            this.lblStatements.Size = new System.Drawing.Size(53, 13);
            this.lblStatements.TabIndex = 5;
            this.lblStatements.Text = "Объекты";
            this.lblStatements.Click += new System.EventHandler(this.lblStatements_Click);
            // 
            // lstStatements
            // 
            this.lstStatements.FormattingEnabled = true;
            this.lstStatements.Location = new System.Drawing.Point(15, 183);
            this.lstStatements.Name = "lstStatements";
            this.lstStatements.Size = new System.Drawing.Size(760, 95);
            this.lstStatements.TabIndex = 6;
            // 
            // btnAddStatement
            // 
            this.btnAddStatement.Location = new System.Drawing.Point(15, 284);
            this.btnAddStatement.Name = "btnAddStatement";
            this.btnAddStatement.Size = new System.Drawing.Size(75, 23);
            this.btnAddStatement.TabIndex = 7;
            this.btnAddStatement.Text = "Добавить";
            this.btnAddStatement.UseVisualStyleBackColor = true;
            this.btnAddStatement.Click += new System.EventHandler(this.btnAddStatement_Click);
            // 
            // btnEditStatement
            // 
            this.btnEditStatement.Location = new System.Drawing.Point(96, 284);
            this.btnEditStatement.Name = "btnEditStatement";
            this.btnEditStatement.Size = new System.Drawing.Size(75, 23);
            this.btnEditStatement.TabIndex = 8;
            this.btnEditStatement.Text = "Изменить";
            this.btnEditStatement.UseVisualStyleBackColor = true;
            this.btnEditStatement.Click += new System.EventHandler(this.btnEditStatement_Click);
            // 
            // btnRemoveStatement
            // 
            this.btnRemoveStatement.Location = new System.Drawing.Point(177, 284);
            this.btnRemoveStatement.Name = "btnRemoveStatement";
            this.btnRemoveStatement.Size = new System.Drawing.Size(75, 23);
            this.btnRemoveStatement.TabIndex = 9;
            this.btnRemoveStatement.Text = "Удалить";
            this.btnRemoveStatement.UseVisualStyleBackColor = true;
            this.btnRemoveStatement.Click += new System.EventHandler(this.btnRemoveStatement_Click);
            // 
            // btnBack
            // 
            this.btnBack.Location = new System.Drawing.Point(15, 401);
            this.btnBack.Name = "btnBack";
            this.btnBack.Size = new System.Drawing.Size(75, 23);
            this.btnBack.TabIndex = 10;
            this.btnBack.Text = "Назад";
            this.btnBack.UseVisualStyleBackColor = true;
            this.btnBack.Click += new System.EventHandler(this.btnBack_Click);
            // 
            // btnSave
            // 
            this.btnSave.Location = new System.Drawing.Point(700, 401);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(75, 23);
            this.btnSave.TabIndex = 11;
            this.btnSave.Text = "Сохранить";
            this.btnSave.UseVisualStyleBackColor = true;
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // FormChangeData
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(792, 436);
            this.Controls.Add(this.btnSave);
            this.Controls.Add(this.btnBack);
            this.Controls.Add(this.btnRemoveStatement);
            this.Controls.Add(this.btnEditStatement);
            this.Controls.Add(this.btnAddStatement);
            this.Controls.Add(this.lstStatements);
            this.Controls.Add(this.lblStatements);
            this.Controls.Add(this.btnRemoveObject);
            this.Controls.Add(this.btnEditObject);
            this.Controls.Add(this.btnAddObject);
            this.Controls.Add(this.lstObjects);
            this.Controls.Add(this.lblObjects);
            this.Name = "FormChangeData";
            this.Text = "Редактирование базы знаний";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lblObjects;
        private System.Windows.Forms.ListBox lstObjects;
        private System.Windows.Forms.Button btnAddObject;
        private System.Windows.Forms.Button btnEditObject;
        private System.Windows.Forms.Button btnRemoveObject;
        private System.Windows.Forms.Label lblStatements;
        private System.Windows.Forms.ListBox lstStatements;
        private System.Windows.Forms.Button btnAddStatement;
        private System.Windows.Forms.Button btnEditStatement;
        private System.Windows.Forms.Button btnRemoveStatement;
        private System.Windows.Forms.Button btnBack;
        private System.Windows.Forms.Button btnSave;
    }
}
