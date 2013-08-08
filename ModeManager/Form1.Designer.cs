namespace ModeManager
{
    partial class Form1
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
            this.btnGetCityNames = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.txtCityA = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.txtCityB = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.txtGametypeLength = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.btnDoTwitterAnalysis = new System.Windows.Forms.Button();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.btnCreateAndDeploy = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.SuspendLayout();
            // 
            // btnGetCityNames
            // 
            this.btnGetCityNames.Location = new System.Drawing.Point(6, 19);
            this.btnGetCityNames.Name = "btnGetCityNames";
            this.btnGetCityNames.Size = new System.Drawing.Size(177, 56);
            this.btnGetCityNames.TabIndex = 1;
            this.btnGetCityNames.Text = "Get City Names";
            this.btnGetCityNames.UseVisualStyleBackColor = true;
            this.btnGetCityNames.Click += new System.EventHandler(this.btnGetCityNames_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(6, 83);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(37, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "City A:";
            // 
            // txtCityA
            // 
            this.txtCityA.BackColor = System.Drawing.Color.White;
            this.txtCityA.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.txtCityA.Location = new System.Drawing.Point(6, 99);
            this.txtCityA.Name = "txtCityA";
            this.txtCityA.ReadOnly = true;
            this.txtCityA.Size = new System.Drawing.Size(176, 20);
            this.txtCityA.TabIndex = 3;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.txtCityB);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.btnGetCityNames);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.txtCityA);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(188, 170);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Grab Cities";
            // 
            // txtCityB
            // 
            this.txtCityB.BackColor = System.Drawing.Color.White;
            this.txtCityB.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.txtCityB.Location = new System.Drawing.Point(6, 143);
            this.txtCityB.Name = "txtCityB";
            this.txtCityB.ReadOnly = true;
            this.txtCityB.Size = new System.Drawing.Size(176, 20);
            this.txtCityB.TabIndex = 5;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 127);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(37, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "City B:";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.txtGametypeLength);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.btnDoTwitterAnalysis);
            this.groupBox2.Location = new System.Drawing.Point(217, 12);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(188, 105);
            this.groupBox2.TabIndex = 5;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Do Twitter Analysis";
            // 
            // txtGametypeLength
            // 
            this.txtGametypeLength.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.txtGametypeLength.Location = new System.Drawing.Point(6, 78);
            this.txtGametypeLength.Name = "txtGametypeLength";
            this.txtGametypeLength.Size = new System.Drawing.Size(176, 20);
            this.txtGametypeLength.TabIndex = 6;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(6, 62);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(137, 13);
            this.label3.TabIndex = 7;
            this.label3.Text = "Gametype Length (Minutes)";
            // 
            // btnDoTwitterAnalysis
            // 
            this.btnDoTwitterAnalysis.Location = new System.Drawing.Point(6, 19);
            this.btnDoTwitterAnalysis.Name = "btnDoTwitterAnalysis";
            this.btnDoTwitterAnalysis.Size = new System.Drawing.Size(176, 33);
            this.btnDoTwitterAnalysis.TabIndex = 0;
            this.btnDoTwitterAnalysis.Text = "Do Twitter Analysis";
            this.btnDoTwitterAnalysis.UseVisualStyleBackColor = true;
            this.btnDoTwitterAnalysis.Click += new System.EventHandler(this.btnDoTwitterAnalysis_Click);
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.btnCreateAndDeploy);
            this.groupBox3.Location = new System.Drawing.Point(423, 12);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(188, 75);
            this.groupBox3.TabIndex = 6;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Create && Deploy Gametype";
            // 
            // btnCreateAndDeploy
            // 
            this.btnCreateAndDeploy.Location = new System.Drawing.Point(6, 19);
            this.btnCreateAndDeploy.Name = "btnCreateAndDeploy";
            this.btnCreateAndDeploy.Size = new System.Drawing.Size(176, 50);
            this.btnCreateAndDeploy.TabIndex = 1;
            this.btnCreateAndDeploy.Text = "Create && Deploy Gametype to Xbox";
            this.btnCreateAndDeploy.UseVisualStyleBackColor = true;
            this.btnCreateAndDeploy.Click += new System.EventHandler(this.btnCreateAndDeploy_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(628, 192);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Form1";
            this.ShowIcon = false;
            this.Text = "Xenon Manager";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnGetCityNames;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox txtCityA;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.TextBox txtCityB;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.TextBox txtGametypeLength;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button btnDoTwitterAnalysis;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Button btnCreateAndDeploy;
    }
}

