namespace YamangTools
{
    partial class Main
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다.
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
        /// </summary>
        private void InitializeComponent()
        {
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.eunjarimInfoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.button1 = new System.Windows.Forms.Button();
            this.RenderTarget = new System.Windows.Forms.PictureBox();
            this.sideMenu = new System.Windows.Forms.TabControl();
            this.Terrain = new System.Windows.Forms.TabPage();
            this.Texture = new System.Windows.Forms.TabPage();
            this.Object = new System.Windows.Forms.TabPage();
            this.menuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.RenderTarget)).BeginInit();
            this.sideMenu.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1051, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(93, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.eunjarimInfoToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.helpToolStripMenuItem.Text = "Help";
            // 
            // eunjarimInfoToolStripMenuItem
            // 
            this.eunjarimInfoToolStripMenuItem.Name = "eunjarimInfoToolStripMenuItem";
            this.eunjarimInfoToolStripMenuItem.Size = new System.Drawing.Size(146, 22);
            this.eunjarimInfoToolStripMenuItem.Text = "Eunjarim info";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(104, 0);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(122, 23);
            this.button1.TabIndex = 1;
            this.button1.Text = "Import HeightMap";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // RenderTarget
            // 
            this.RenderTarget.Location = new System.Drawing.Point(295, 51);
            this.RenderTarget.Name = "RenderTarget";
            this.RenderTarget.Size = new System.Drawing.Size(729, 588);
            this.RenderTarget.TabIndex = 2;
            this.RenderTarget.TabStop = false;
            // 
            // sideMenu
            // 
            this.sideMenu.Controls.Add(this.Terrain);
            this.sideMenu.Controls.Add(this.Texture);
            this.sideMenu.Controls.Add(this.Object);
            this.sideMenu.Location = new System.Drawing.Point(12, 29);
            this.sideMenu.Name = "sideMenu";
            this.sideMenu.SelectedIndex = 0;
            this.sideMenu.Size = new System.Drawing.Size(277, 614);
            this.sideMenu.TabIndex = 4;
            // 
            // Terrain
            // 
            this.Terrain.Location = new System.Drawing.Point(4, 22);
            this.Terrain.Name = "Terrain";
            this.Terrain.Padding = new System.Windows.Forms.Padding(3);
            this.Terrain.Size = new System.Drawing.Size(269, 588);
            this.Terrain.TabIndex = 0;
            this.Terrain.Text = "Terrain";
            this.Terrain.UseVisualStyleBackColor = true;
            // 
            // Texture
            // 
            this.Texture.Location = new System.Drawing.Point(4, 22);
            this.Texture.Name = "Texture";
            this.Texture.Padding = new System.Windows.Forms.Padding(3);
            this.Texture.Size = new System.Drawing.Size(269, 588);
            this.Texture.TabIndex = 1;
            this.Texture.Text = "Texture";
            this.Texture.UseVisualStyleBackColor = true;
            // 
            // Object
            // 
            this.Object.Location = new System.Drawing.Point(4, 22);
            this.Object.Name = "Object";
            this.Object.Padding = new System.Windows.Forms.Padding(3);
            this.Object.Size = new System.Drawing.Size(269, 588);
            this.Object.TabIndex = 2;
            this.Object.Text = "Object";
            this.Object.UseVisualStyleBackColor = true;
            // 
            // Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1051, 655);
            this.Controls.Add(this.sideMenu);
            this.Controls.Add(this.RenderTarget);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.menuStrip1);
            this.Name = "Main";
            this.Text = "HeightMap Tool";
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.RenderTarget)).EndInit();
            this.sideMenu.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.PictureBox RenderTarget;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem eunjarimInfoToolStripMenuItem;
        private System.Windows.Forms.TabControl sideMenu;
        private System.Windows.Forms.TabPage Terrain;
        private System.Windows.Forms.TabPage Texture;
        private System.Windows.Forms.TabPage Object;
    }
}

