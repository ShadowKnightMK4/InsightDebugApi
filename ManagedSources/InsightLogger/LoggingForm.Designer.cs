namespace InsightLogger
{
    partial class LoggingForm
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
            groupBox1 = new System.Windows.Forms.GroupBox();
            checkedListBox1 = new System.Windows.Forms.CheckedListBox();
            groupBox2 = new System.Windows.Forms.GroupBox();
            checkBox1 = new System.Windows.Forms.CheckBox();
            checkBox2 = new System.Windows.Forms.CheckBox();
            groupBox1.SuspendLayout();
            groupBox2.SuspendLayout();
            SuspendLayout();
            // 
            // groupBox1
            // 
            groupBox1.Controls.Add(checkedListBox1);
            groupBox1.Location = new System.Drawing.Point(21, 58);
            groupBox1.Name = "groupBox1";
            groupBox1.Size = new System.Drawing.Size(306, 380);
            groupBox1.TabIndex = 0;
            groupBox1.TabStop = false;
            groupBox1.Text = "Debug Events";
            // 
            // checkedListBox1
            // 
            checkedListBox1.FormattingEnabled = true;
            checkedListBox1.Items.AddRange(new object[] { "All", "Create Process Event", "Create Thread Event", "Exception Events", "Exit Process Event", "Exit Thread Event", "Load Dll Debug Event", "Output Debug String Event", "Unload Dll Debug Event", "Rip Event" });
            checkedListBox1.Location = new System.Drawing.Point(6, 30);
            checkedListBox1.Name = "checkedListBox1";
            checkedListBox1.Size = new System.Drawing.Size(274, 312);
            checkedListBox1.TabIndex = 0;
            // 
            // groupBox2
            // 
            groupBox2.Controls.Add(checkBox2);
            groupBox2.Controls.Add(checkBox1);
            groupBox2.Location = new System.Drawing.Point(333, 69);
            groupBox2.Name = "groupBox2";
            groupBox2.Size = new System.Drawing.Size(300, 273);
            groupBox2.TabIndex = 1;
            groupBox2.TabStop = false;
            groupBox2.Text = "Exception Specific";
            // 
            // checkBox1
            // 
            checkBox1.AutoSize = true;
            checkBox1.Location = new System.Drawing.Point(24, 67);
            checkBox1.Name = "checkBox1";
            checkBox1.Size = new System.Drawing.Size(277, 29);
            checkBox1.TabIndex = 0;
            checkBox1.Text = "Handle Telemetry DLL (Debug)";
            checkBox1.ThreeState = true;
            checkBox1.UseVisualStyleBackColor = true;
            // 
            // checkBox2
            // 
            checkBox2.AutoSize = true;
            checkBox2.Location = new System.Drawing.Point(24, 102);
            checkBox2.Name = "checkBox2";
            checkBox2.Size = new System.Drawing.Size(203, 29);
            checkBox2.TabIndex = 1;
            checkBox2.Text = "Handle Close Handle";
            checkBox2.ThreeState = true;
            checkBox2.UseVisualStyleBackColor = true;
            // 
            // LoggingForm
            // 
            AutoScaleDimensions = new System.Drawing.SizeF(10F, 25F);
            AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            ClientSize = new System.Drawing.Size(800, 450);
            Controls.Add(groupBox2);
            Controls.Add(groupBox1);
            Name = "LoggingForm";
            Text = "Log Settings";
            groupBox1.ResumeLayout(false);
            groupBox2.ResumeLayout(false);
            groupBox2.PerformLayout();
            ResumeLayout(false);
        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.CheckedListBox checkedListBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.CheckBox checkBox1;
        private System.Windows.Forms.CheckBox checkBox2;
    }
}