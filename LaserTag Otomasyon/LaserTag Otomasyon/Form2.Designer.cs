namespace silerim_calis
{
    partial class Form2
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form2));
            this.soloBtn = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.teamBtn = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // soloBtn
            // 
            this.soloBtn.Location = new System.Drawing.Point(72, 175);
            this.soloBtn.Name = "soloBtn";
            this.soloBtn.Size = new System.Drawing.Size(255, 143);
            this.soloBtn.TabIndex = 0;
            this.soloBtn.Text = "Deathmatch";
            this.soloBtn.UseVisualStyleBackColor = true;
            this.soloBtn.Click += new System.EventHandler(this.soloBtn_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(413, 81);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(8, 8);
            this.button2.TabIndex = 1;
            this.button2.Text = "button2";
            this.button2.UseVisualStyleBackColor = true;
            // 
            // teamBtn
            // 
            this.teamBtn.Location = new System.Drawing.Point(72, 364);
            this.teamBtn.Name = "teamBtn";
            this.teamBtn.Size = new System.Drawing.Size(255, 143);
            this.teamBtn.TabIndex = 2;
            this.teamBtn.Text = "Team Deathmatch";
            this.teamBtn.UseVisualStyleBackColor = true;
            this.teamBtn.Click += new System.EventHandler(this.teamBtn_Click);
            // 
            // Form2
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1549, 988);
            this.Controls.Add(this.teamBtn);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.soloBtn);
            this.Name = "Form2";
            this.Text = resources.GetString("$this.Text");
            this.Load += new System.EventHandler(this.Form2_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button soloBtn;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button teamBtn;
    }
}