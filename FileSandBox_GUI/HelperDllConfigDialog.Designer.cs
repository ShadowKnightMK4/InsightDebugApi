
namespace FileSandBox_GUI
{
    partial class HelperDllConfigDialog
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
            this.buttonAddToLoadList = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.listBoxPossDlls = new System.Windows.Forms.ListBox();
            this.listBoxLoadTheseDlls = new System.Windows.Forms.ListBox();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.button1 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // buttonAddToLoadList
            // 
            this.buttonAddToLoadList.Location = new System.Drawing.Point(332, 74);
            this.buttonAddToLoadList.Name = "buttonAddToLoadList";
            this.buttonAddToLoadList.Size = new System.Drawing.Size(94, 29);
            this.buttonAddToLoadList.TabIndex = 1;
            this.buttonAddToLoadList.Text = ">>";
            this.buttonAddToLoadList.UseVisualStyleBackColor = true;
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(332, 109);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(94, 29);
            this.button2.TabIndex = 2;
            this.button2.Text = "<<";
            this.button2.UseVisualStyleBackColor = true;
            // 
            // listBoxPossDlls
            // 
            this.listBoxPossDlls.FormattingEnabled = true;
            this.listBoxPossDlls.ItemHeight = 20;
            this.listBoxPossDlls.Location = new System.Drawing.Point(23, 76);
            this.listBoxPossDlls.Name = "listBoxPossDlls";
            this.listBoxPossDlls.Size = new System.Drawing.Size(269, 324);
            this.listBoxPossDlls.TabIndex = 3;
            // 
            // listBoxLoadTheseDlls
            // 
            this.listBoxLoadTheseDlls.FormattingEnabled = true;
            this.listBoxLoadTheseDlls.ItemHeight = 20;
            this.listBoxLoadTheseDlls.Location = new System.Drawing.Point(453, 76);
            this.listBoxLoadTheseDlls.Name = "listBoxLoadTheseDlls";
            this.listBoxLoadTheseDlls.Size = new System.Drawing.Size(335, 324);
            this.listBoxLoadTheseDlls.TabIndex = 4;
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(23, 406);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(756, 68);
            this.textBox1.TabIndex = 5;
            this.textBox1.Text = "Shows a description reported by the helper DLL";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(651, 13);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(94, 29);
            this.button1.TabIndex = 6;
            this.button1.Text = "Cancel";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(494, 12);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(94, 29);
            this.button3.TabIndex = 7;
            this.button3.Text = "Ok";
            this.button3.UseVisualStyleBackColor = true;
            // 
            // HelperDllConfigDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 495);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.listBoxLoadTheseDlls);
            this.Controls.Add(this.listBoxPossDlls);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.buttonAddToLoadList);
            this.Name = "HelperDllConfigDialog";
            this.Text = "HelperDllConfigDialog";
            this.Load += new System.EventHandler(this.HelperDllConfigDialog_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonAddToLoadList;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.ListBox listBoxPossDlls;
        private System.Windows.Forms.ListBox listBoxLoadTheseDlls;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button3;
    }
}