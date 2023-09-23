namespace InsightLogger
{
    partial class ChooseLogFormat
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
            LabelDirections = new System.Windows.Forms.Label();
            comboBox1 = new System.Windows.Forms.ComboBox();
            LabelFormatExampleDirection = new System.Windows.Forms.Label();
            LabelExampleFormat = new System.Windows.Forms.Label();
            ButtonOK = new System.Windows.Forms.Button();
            ButtonCancel = new System.Windows.Forms.Button();
            SuspendLayout();
            // 
            // LabelDirections
            // 
            LabelDirections.AutoSize = true;
            LabelDirections.Location = new System.Drawing.Point(12, 32);
            LabelDirections.Name = "LabelDirections";
            LabelDirections.Size = new System.Drawing.Size(182, 25);
            LabelDirections.TabIndex = 0;
            LabelDirections.Text = "Choose A log Format";
            LabelDirections.Click += label1_Click;
            // 
            // comboBox1
            // 
            comboBox1.FormattingEnabled = true;
            comboBox1.Items.AddRange(new object[] { "Pass Thru", "JSON", "Friendly" });
            comboBox1.Location = new System.Drawing.Point(12, 74);
            comboBox1.Name = "comboBox1";
            comboBox1.Size = new System.Drawing.Size(305, 33);
            comboBox1.TabIndex = 1;
            comboBox1.SelectedIndexChanged += comboBox1_SelectedIndexChanged;
            // 
            // LabelFormatExampleDirection
            // 
            LabelFormatExampleDirection.AutoSize = true;
            LabelFormatExampleDirection.Location = new System.Drawing.Point(12, 144);
            LabelFormatExampleDirection.Name = "LabelFormatExampleDirection";
            LabelFormatExampleDirection.Size = new System.Drawing.Size(336, 25);
            LabelFormatExampleDirection.TabIndex = 2;
            LabelFormatExampleDirection.Text = "This format will appear list like this below";
            // 
            // LabelExampleFormat
            // 
            LabelExampleFormat.AutoSize = true;
            LabelExampleFormat.Location = new System.Drawing.Point(12, 180);
            LabelExampleFormat.Name = "LabelExampleFormat";
            LabelExampleFormat.Size = new System.Drawing.Size(199, 25);
            LabelExampleFormat.TabIndex = 3;
            LabelExampleFormat.Text = "<Example Format Text>";
            LabelExampleFormat.Click += LabelExampleFormat_Click;
            // 
            // ButtonOK
            // 
            ButtonOK.Location = new System.Drawing.Point(374, 27);
            ButtonOK.Name = "ButtonOK";
            ButtonOK.Size = new System.Drawing.Size(112, 34);
            ButtonOK.TabIndex = 4;
            ButtonOK.Text = "OK";
            ButtonOK.UseVisualStyleBackColor = true;
            ButtonOK.Click += ButtonOK_Click;
            // 
            // ButtonCancel
            // 
            ButtonCancel.Location = new System.Drawing.Point(374, 66);
            ButtonCancel.Name = "ButtonCancel";
            ButtonCancel.Size = new System.Drawing.Size(112, 34);
            ButtonCancel.TabIndex = 5;
            ButtonCancel.Text = "Cancel";
            ButtonCancel.UseVisualStyleBackColor = true;
            ButtonCancel.Click += ButtonCancel_Click;
            // 
            // ChooseLogFormat
            // 
            AutoScaleDimensions = new System.Drawing.SizeF(10F, 25F);
            AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            ClientSize = new System.Drawing.Size(545, 455);
            Controls.Add(ButtonCancel);
            Controls.Add(ButtonOK);
            Controls.Add(LabelExampleFormat);
            Controls.Add(LabelFormatExampleDirection);
            Controls.Add(comboBox1);
            Controls.Add(LabelDirections);
            Name = "ChooseLogFormat";
            Text = "Choose a Log Format";
            Load += ChooseLogFormat_Load;
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private System.Windows.Forms.Label LabelDirections;
        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.Label LabelFormatExampleDirection;
        private System.Windows.Forms.Label LabelExampleFormat;
        private System.Windows.Forms.Button ButtonOK;
        private System.Windows.Forms.Button ButtonCancel;
    }
}