namespace InsightLogger
{
    partial class LogRunner_ExceptionSettings
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
            label1 = new System.Windows.Forms.Label();
            checkedListBox2 = new System.Windows.Forms.CheckedListBox();
            label2 = new System.Windows.Forms.Label();
            groupBox1.SuspendLayout();
            SuspendLayout();
            // 
            // groupBox1
            // 
            groupBox1.Controls.Add(label2);
            groupBox1.Controls.Add(checkedListBox2);
            groupBox1.Controls.Add(label1);
            groupBox1.Controls.Add(checkedListBox1);
            groupBox1.Location = new System.Drawing.Point(29, 36);
            groupBox1.Name = "groupBox1";
            groupBox1.Size = new System.Drawing.Size(976, 402);
            groupBox1.TabIndex = 0;
            groupBox1.TabStop = false;
            groupBox1.Text = "Debug Only Log Settings";
            // 
            // checkedListBox1
            // 
            checkedListBox1.FormattingEnabled = true;
            checkedListBox1.Items.AddRange(new object[] { "CREATE_PROCESS_DEBUG_EVENT", "CREATE_THREAD_DEBUG_EVENT", "EXCEPTION_DEBUG_EVENT", "EXIT_PROCESS_DEBUG_EVENT", "EXIT_THREAD_DEBUG_EVENT", "LOAD_DLL_DEBUG_EVENT", "OUTPUT_DEBUG_STRING_EVENT", "RIP_EVENT", "UNLOAD_DLL_DEBUG_EVENT", "ALL" });
            checkedListBox1.Location = new System.Drawing.Point(29, 73);
            checkedListBox1.Name = "checkedListBox1";
            checkedListBox1.Size = new System.Drawing.Size(325, 312);
            checkedListBox1.TabIndex = 0;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new System.Drawing.Point(29, 36);
            label1.Name = "label1";
            label1.Size = new System.Drawing.Size(233, 25);
            label1.TabIndex = 1;
            label1.Text = "Which Debug Events to Log";
            // 
            // checkedListBox2
            // 
            checkedListBox2.FormattingEnabled = true;
            checkedListBox2.Items.AddRange(new object[] { "Close Handle", "Telemetry Exceptions", "Breakpoints", "Invert Handling (checked exceptions are NOT Handled - everything else is)", "All Exceptions" });
            checkedListBox2.Location = new System.Drawing.Point(598, 73);
            checkedListBox2.Name = "checkedListBox2";
            checkedListBox2.Size = new System.Drawing.Size(325, 312);
            checkedListBox2.TabIndex = 2;
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new System.Drawing.Point(598, 36);
            label2.Name = "label2";
            label2.Size = new System.Drawing.Size(338, 25);
            label2.TabIndex = 1;
            label2.Text = "Which Exceptions are marked as Handled";
            // 
            // LoggingSettings
            // 
            AutoScaleDimensions = new System.Drawing.SizeF(10F, 25F);
            AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            ClientSize = new System.Drawing.Size(1050, 450);
            Controls.Add(groupBox1);
            Name = "LoggingSettings";
            Text = "Insight Logger - Log Settings";
            groupBox1.ResumeLayout(false);
            groupBox1.PerformLayout();
            ResumeLayout(false);
        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.CheckedListBox checkedListBox1;
        private System.Windows.Forms.CheckedListBox checkedListBox2;
        private System.Windows.Forms.Label label2;
    }
}