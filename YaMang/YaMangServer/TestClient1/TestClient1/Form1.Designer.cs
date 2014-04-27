namespace TestClient1
{
    partial class Form1
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
            this.lb_connect_server = new System.Windows.Forms.Label();
            this.bu_connect_server = new System.Windows.Forms.Button();
            this.lb_playerID = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.label1 = new System.Windows.Forms.Label();
            this.tb_gameOver_playerId = new System.Windows.Forms.TextBox();
            this.cb_gameOver_isWin = new System.Windows.Forms.CheckBox();
            this.bu_gameOver_send = new System.Windows.Forms.Button();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.tb_roomLeaveNum = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.bu_roomLeave = new System.Windows.Forms.Button();
            this.tb_roomEnterNum = new System.Windows.Forms.TextBox();
            this.bu_roomEnter = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.bu_roomCreate = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.bu_corps_formation = new System.Windows.Forms.Button();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.rb_corps_formation_rush = new System.Windows.Forms.RadioButton();
            this.rb_corps_formation_destroy = new System.Windows.Forms.RadioButton();
            this.rb_corps_formation_defense = new System.Windows.Forms.RadioButton();
            this.bu_corps_stop = new System.Windows.Forms.Button();
            this.bu_corps_move = new System.Windows.Forms.Button();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.rb_unit_sword = new System.Windows.Forms.RadioButton();
            this.rb_unit_pike = new System.Windows.Forms.RadioButton();
            this.rb_unit_knight = new System.Windows.Forms.RadioButton();
            this.rb_unit_arrow = new System.Windows.Forms.RadioButton();
            this.rb_unit_guard = new System.Windows.Forms.RadioButton();
            this.bu_generate_corps = new System.Windows.Forms.Button();
            this.tb_corpsID = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.tb_corps_look_z = new System.Windows.Forms.TextBox();
            this.tb_corps_look_y = new System.Windows.Forms.TextBox();
            this.tb_corps_look_x = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.tb_corps_eye_z = new System.Windows.Forms.TextBox();
            this.tb_corps_eye_y = new System.Windows.Forms.TextBox();
            this.tb_corps_eye_x = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox6.SuspendLayout();
            this.groupBox5.SuspendLayout();
            this.SuspendLayout();
            // 
            // lb_connect_server
            // 
            this.lb_connect_server.AutoSize = true;
            this.lb_connect_server.Location = new System.Drawing.Point(136, 25);
            this.lb_connect_server.Name = "lb_connect_server";
            this.lb_connect_server.Size = new System.Drawing.Size(53, 12);
            this.lb_connect_server.TabIndex = 1;
            this.lb_connect_server.Text = "연결상태";
            // 
            // bu_connect_server
            // 
            this.bu_connect_server.Location = new System.Drawing.Point(6, 20);
            this.bu_connect_server.Name = "bu_connect_server";
            this.bu_connect_server.Size = new System.Drawing.Size(124, 23);
            this.bu_connect_server.TabIndex = 10;
            this.bu_connect_server.Text = "1. connect_server";
            this.bu_connect_server.UseVisualStyleBackColor = true;
            this.bu_connect_server.Click += new System.EventHandler(this.bu_connect_server_Click);
            // 
            // lb_playerID
            // 
            this.lb_playerID.AutoSize = true;
            this.lb_playerID.Location = new System.Drawing.Point(29, 9);
            this.lb_playerID.Name = "lb_playerID";
            this.lb_playerID.Size = new System.Drawing.Size(68, 12);
            this.lb_playerID.TabIndex = 6;
            this.lb_playerID.Text = "PlayerID:-1";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.bu_connect_server);
            this.groupBox1.Controls.Add(this.lb_connect_server);
            this.groupBox1.Location = new System.Drawing.Point(31, 24);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(399, 50);
            this.groupBox1.TabIndex = 7;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "connectServer";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.label1);
            this.groupBox3.Controls.Add(this.tb_gameOver_playerId);
            this.groupBox3.Controls.Add(this.cb_gameOver_isWin);
            this.groupBox3.Controls.Add(this.bu_gameOver_send);
            this.groupBox3.Location = new System.Drawing.Point(436, 26);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(415, 48);
            this.groupBox3.TabIndex = 9;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "GameOverRequest";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(92, 25);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(55, 12);
            this.label1.TabIndex = 3;
            this.label1.Text = "playerID:";
            // 
            // tb_gameOver_playerId
            // 
            this.tb_gameOver_playerId.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.tb_gameOver_playerId.Location = new System.Drawing.Point(151, 20);
            this.tb_gameOver_playerId.Name = "tb_gameOver_playerId";
            this.tb_gameOver_playerId.Size = new System.Drawing.Size(100, 21);
            this.tb_gameOver_playerId.TabIndex = 30;
            // 
            // cb_gameOver_isWin
            // 
            this.cb_gameOver_isWin.AutoSize = true;
            this.cb_gameOver_isWin.Location = new System.Drawing.Point(257, 24);
            this.cb_gameOver_isWin.Name = "cb_gameOver_isWin";
            this.cb_gameOver_isWin.Size = new System.Drawing.Size(54, 16);
            this.cb_gameOver_isWin.TabIndex = 40;
            this.cb_gameOver_isWin.Text = "IsWin";
            this.cb_gameOver_isWin.UseVisualStyleBackColor = true;
            // 
            // bu_gameOver_send
            // 
            this.bu_gameOver_send.Location = new System.Drawing.Point(6, 20);
            this.bu_gameOver_send.Name = "bu_gameOver_send";
            this.bu_gameOver_send.Size = new System.Drawing.Size(75, 23);
            this.bu_gameOver_send.TabIndex = 20;
            this.bu_gameOver_send.Text = "gameover";
            this.bu_gameOver_send.UseVisualStyleBackColor = true;
            this.bu_gameOver_send.Click += new System.EventHandler(this.bu_gameOver_send_Click);
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.tb_roomLeaveNum);
            this.groupBox4.Controls.Add(this.label3);
            this.groupBox4.Controls.Add(this.bu_roomLeave);
            this.groupBox4.Controls.Add(this.tb_roomEnterNum);
            this.groupBox4.Controls.Add(this.bu_roomEnter);
            this.groupBox4.Controls.Add(this.label2);
            this.groupBox4.Controls.Add(this.bu_roomCreate);
            this.groupBox4.Location = new System.Drawing.Point(31, 80);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(820, 47);
            this.groupBox4.TabIndex = 10;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "RoomRequest";
            // 
            // tb_roomLeaveNum
            // 
            this.tb_roomLeaveNum.Location = new System.Drawing.Point(714, 20);
            this.tb_roomLeaveNum.Name = "tb_roomLeaveNum";
            this.tb_roomLeaveNum.Size = new System.Drawing.Size(100, 21);
            this.tb_roomLeaveNum.TabIndex = 80;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(643, 25);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(65, 12);
            this.label3.TabIndex = 15;
            this.label3.Text = "roomNum:";
            // 
            // bu_roomLeave
            // 
            this.bu_roomLeave.Location = new System.Drawing.Point(545, 18);
            this.bu_roomLeave.Name = "bu_roomLeave";
            this.bu_roomLeave.Size = new System.Drawing.Size(92, 23);
            this.bu_roomLeave.TabIndex = 90;
            this.bu_roomLeave.Text = "roomLeave";
            this.bu_roomLeave.UseVisualStyleBackColor = true;
            this.bu_roomLeave.Click += new System.EventHandler(this.bu_roomLeave_Click);
            // 
            // tb_roomEnterNum
            // 
            this.tb_roomEnterNum.Location = new System.Drawing.Point(368, 21);
            this.tb_roomEnterNum.Name = "tb_roomEnterNum";
            this.tb_roomEnterNum.Size = new System.Drawing.Size(100, 21);
            this.tb_roomEnterNum.TabIndex = 60;
            // 
            // bu_roomEnter
            // 
            this.bu_roomEnter.Location = new System.Drawing.Point(215, 18);
            this.bu_roomEnter.Name = "bu_roomEnter";
            this.bu_roomEnter.Size = new System.Drawing.Size(75, 23);
            this.bu_roomEnter.TabIndex = 70;
            this.bu_roomEnter.Text = "roomEnter";
            this.bu_roomEnter.UseVisualStyleBackColor = true;
            this.bu_roomEnter.Click += new System.EventHandler(this.bu_roomEnter_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(296, 25);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(65, 12);
            this.label2.TabIndex = 12;
            this.label2.Text = "roomNum:";
            // 
            // bu_roomCreate
            // 
            this.bu_roomCreate.Location = new System.Drawing.Point(6, 20);
            this.bu_roomCreate.Name = "bu_roomCreate";
            this.bu_roomCreate.Size = new System.Drawing.Size(96, 23);
            this.bu_roomCreate.TabIndex = 50;
            this.bu_roomCreate.Text = "roomCreate";
            this.bu_roomCreate.UseVisualStyleBackColor = true;
            this.bu_roomCreate.Click += new System.EventHandler(this.bu_roomCreate_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.bu_corps_formation);
            this.groupBox2.Controls.Add(this.groupBox6);
            this.groupBox2.Controls.Add(this.bu_corps_stop);
            this.groupBox2.Controls.Add(this.bu_corps_move);
            this.groupBox2.Controls.Add(this.groupBox5);
            this.groupBox2.Controls.Add(this.bu_generate_corps);
            this.groupBox2.Controls.Add(this.tb_corpsID);
            this.groupBox2.Controls.Add(this.label7);
            this.groupBox2.Controls.Add(this.tb_corps_look_z);
            this.groupBox2.Controls.Add(this.tb_corps_look_y);
            this.groupBox2.Controls.Add(this.tb_corps_look_x);
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Controls.Add(this.tb_corps_eye_z);
            this.groupBox2.Controls.Add(this.tb_corps_eye_y);
            this.groupBox2.Controls.Add(this.tb_corps_eye_x);
            this.groupBox2.Controls.Add(this.label5);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Location = new System.Drawing.Point(31, 134);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(380, 221);
            this.groupBox2.TabIndex = 11;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "corps";
            // 
            // bu_corps_formation
            // 
            this.bu_corps_formation.Location = new System.Drawing.Point(217, 187);
            this.bu_corps_formation.Name = "bu_corps_formation";
            this.bu_corps_formation.Size = new System.Drawing.Size(157, 23);
            this.bu_corps_formation.TabIndex = 280;
            this.bu_corps_formation.Text = "change formation";
            this.bu_corps_formation.UseVisualStyleBackColor = true;
            this.bu_corps_formation.Click += new System.EventHandler(this.bu_corps_formation_Click);
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.rb_corps_formation_rush);
            this.groupBox6.Controls.Add(this.rb_corps_formation_destroy);
            this.groupBox6.Controls.Add(this.rb_corps_formation_defense);
            this.groupBox6.Location = new System.Drawing.Point(280, 17);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(71, 84);
            this.groupBox6.TabIndex = 12;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "formation";
            // 
            // rb_corps_formation_rush
            // 
            this.rb_corps_formation_rush.AutoSize = true;
            this.rb_corps_formation_rush.Location = new System.Drawing.Point(6, 64);
            this.rb_corps_formation_rush.Name = "rb_corps_formation_rush";
            this.rb_corps_formation_rush.Size = new System.Drawing.Size(48, 16);
            this.rb_corps_formation_rush.TabIndex = 240;
            this.rb_corps_formation_rush.Text = "rush";
            this.rb_corps_formation_rush.UseVisualStyleBackColor = true;
            // 
            // rb_corps_formation_destroy
            // 
            this.rb_corps_formation_destroy.AutoSize = true;
            this.rb_corps_formation_destroy.Location = new System.Drawing.Point(6, 42);
            this.rb_corps_formation_destroy.Name = "rb_corps_formation_destroy";
            this.rb_corps_formation_destroy.Size = new System.Drawing.Size(65, 16);
            this.rb_corps_formation_destroy.TabIndex = 230;
            this.rb_corps_formation_destroy.Text = "destroy";
            this.rb_corps_formation_destroy.UseVisualStyleBackColor = true;
            // 
            // rb_corps_formation_defense
            // 
            this.rb_corps_formation_defense.AutoSize = true;
            this.rb_corps_formation_defense.Checked = true;
            this.rb_corps_formation_defense.Location = new System.Drawing.Point(6, 20);
            this.rb_corps_formation_defense.Name = "rb_corps_formation_defense";
            this.rb_corps_formation_defense.Size = new System.Drawing.Size(68, 16);
            this.rb_corps_formation_defense.TabIndex = 220;
            this.rb_corps_formation_defense.TabStop = true;
            this.rb_corps_formation_defense.Text = "defense";
            this.rb_corps_formation_defense.UseVisualStyleBackColor = true;
            // 
            // bu_corps_stop
            // 
            this.bu_corps_stop.Location = new System.Drawing.Point(299, 157);
            this.bu_corps_stop.Name = "bu_corps_stop";
            this.bu_corps_stop.Size = new System.Drawing.Size(75, 23);
            this.bu_corps_stop.TabIndex = 270;
            this.bu_corps_stop.Text = "stop";
            this.bu_corps_stop.UseVisualStyleBackColor = true;
            this.bu_corps_stop.Click += new System.EventHandler(this.bu_corps_stop_Click);
            // 
            // bu_corps_move
            // 
            this.bu_corps_move.Location = new System.Drawing.Point(217, 158);
            this.bu_corps_move.Name = "bu_corps_move";
            this.bu_corps_move.Size = new System.Drawing.Size(75, 23);
            this.bu_corps_move.TabIndex = 260;
            this.bu_corps_move.Text = "move";
            this.bu_corps_move.UseVisualStyleBackColor = true;
            this.bu_corps_move.Click += new System.EventHandler(this.bu_corps_move_Click);
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.rb_unit_sword);
            this.groupBox5.Controls.Add(this.rb_unit_pike);
            this.groupBox5.Controls.Add(this.rb_unit_knight);
            this.groupBox5.Controls.Add(this.rb_unit_arrow);
            this.groupBox5.Controls.Add(this.rb_unit_guard);
            this.groupBox5.Location = new System.Drawing.Point(6, 84);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(185, 64);
            this.groupBox5.TabIndex = 14;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "unitType";
            // 
            // rb_unit_sword
            // 
            this.rb_unit_sword.AutoSize = true;
            this.rb_unit_sword.Location = new System.Drawing.Point(63, 42);
            this.rb_unit_sword.Name = "rb_unit_sword";
            this.rb_unit_sword.Size = new System.Drawing.Size(58, 16);
            this.rb_unit_sword.TabIndex = 200;
            this.rb_unit_sword.Text = "sword";
            this.rb_unit_sword.UseVisualStyleBackColor = true;
            // 
            // rb_unit_pike
            // 
            this.rb_unit_pike.AutoSize = true;
            this.rb_unit_pike.Location = new System.Drawing.Point(6, 42);
            this.rb_unit_pike.Name = "rb_unit_pike";
            this.rb_unit_pike.Size = new System.Drawing.Size(46, 16);
            this.rb_unit_pike.TabIndex = 190;
            this.rb_unit_pike.Text = "pike";
            this.rb_unit_pike.UseVisualStyleBackColor = true;
            // 
            // rb_unit_knight
            // 
            this.rb_unit_knight.AutoSize = true;
            this.rb_unit_knight.Location = new System.Drawing.Point(125, 20);
            this.rb_unit_knight.Name = "rb_unit_knight";
            this.rb_unit_knight.Size = new System.Drawing.Size(56, 16);
            this.rb_unit_knight.TabIndex = 180;
            this.rb_unit_knight.Text = "knight";
            this.rb_unit_knight.UseVisualStyleBackColor = true;
            // 
            // rb_unit_arrow
            // 
            this.rb_unit_arrow.AutoSize = true;
            this.rb_unit_arrow.Checked = true;
            this.rb_unit_arrow.Location = new System.Drawing.Point(6, 20);
            this.rb_unit_arrow.Name = "rb_unit_arrow";
            this.rb_unit_arrow.Size = new System.Drawing.Size(55, 16);
            this.rb_unit_arrow.TabIndex = 160;
            this.rb_unit_arrow.TabStop = true;
            this.rb_unit_arrow.Text = "arrow";
            this.rb_unit_arrow.UseVisualStyleBackColor = true;
            // 
            // rb_unit_guard
            // 
            this.rb_unit_guard.AutoSize = true;
            this.rb_unit_guard.Location = new System.Drawing.Point(64, 20);
            this.rb_unit_guard.Name = "rb_unit_guard";
            this.rb_unit_guard.Size = new System.Drawing.Size(55, 16);
            this.rb_unit_guard.TabIndex = 170;
            this.rb_unit_guard.Text = "guard";
            this.rb_unit_guard.UseVisualStyleBackColor = true;
            // 
            // bu_generate_corps
            // 
            this.bu_generate_corps.Location = new System.Drawing.Point(6, 158);
            this.bu_generate_corps.Name = "bu_generate_corps";
            this.bu_generate_corps.Size = new System.Drawing.Size(181, 23);
            this.bu_generate_corps.TabIndex = 210;
            this.bu_generate_corps.Text = "generate";
            this.bu_generate_corps.UseVisualStyleBackColor = true;
            this.bu_generate_corps.Click += new System.EventHandler(this.bu_generate_corps_Click);
            // 
            // tb_corpsID
            // 
            this.tb_corpsID.Location = new System.Drawing.Point(283, 126);
            this.tb_corpsID.Name = "tb_corpsID";
            this.tb_corpsID.Size = new System.Drawing.Size(68, 21);
            this.tb_corpsID.TabIndex = 250;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(225, 129);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(52, 12);
            this.label7.TabIndex = 12;
            this.label7.Text = "corpsID:";
            // 
            // tb_corps_look_z
            // 
            this.tb_corps_look_z.Location = new System.Drawing.Point(207, 57);
            this.tb_corps_look_z.Name = "tb_corps_look_z";
            this.tb_corps_look_z.Size = new System.Drawing.Size(35, 21);
            this.tb_corps_look_z.TabIndex = 150;
            this.tb_corps_look_z.Text = "0";
            // 
            // tb_corps_look_y
            // 
            this.tb_corps_look_y.Location = new System.Drawing.Point(166, 57);
            this.tb_corps_look_y.Name = "tb_corps_look_y";
            this.tb_corps_look_y.Size = new System.Drawing.Size(35, 21);
            this.tb_corps_look_y.TabIndex = 140;
            this.tb_corps_look_y.Text = "0";
            // 
            // tb_corps_look_x
            // 
            this.tb_corps_look_x.Location = new System.Drawing.Point(125, 57);
            this.tb_corps_look_x.Name = "tb_corps_look_x";
            this.tb_corps_look_x.Size = new System.Drawing.Size(35, 21);
            this.tb_corps_look_x.TabIndex = 130;
            this.tb_corps_look_x.Text = "0";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(125, 17);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(102, 12);
            this.label6.TabIndex = 14;
            this.label6.Text = "   x        y        z";
            // 
            // tb_corps_eye_z
            // 
            this.tb_corps_eye_z.Location = new System.Drawing.Point(207, 32);
            this.tb_corps_eye_z.Name = "tb_corps_eye_z";
            this.tb_corps_eye_z.Size = new System.Drawing.Size(35, 21);
            this.tb_corps_eye_z.TabIndex = 120;
            this.tb_corps_eye_z.Text = "0";
            // 
            // tb_corps_eye_y
            // 
            this.tb_corps_eye_y.Location = new System.Drawing.Point(166, 32);
            this.tb_corps_eye_y.Name = "tb_corps_eye_y";
            this.tb_corps_eye_y.Size = new System.Drawing.Size(35, 21);
            this.tb_corps_eye_y.TabIndex = 110;
            this.tb_corps_eye_y.Text = "0";
            // 
            // tb_corps_eye_x
            // 
            this.tb_corps_eye_x.Location = new System.Drawing.Point(125, 32);
            this.tb_corps_eye_x.Name = "tb_corps_eye_x";
            this.tb_corps_eye_x.Size = new System.Drawing.Size(35, 21);
            this.tb_corps_eye_x.TabIndex = 100;
            this.tb_corps_eye_x.Text = "0";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(93, 61);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(28, 12);
            this.label5.TabIndex = 12;
            this.label5.Text = "look";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(93, 35);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(26, 12);
            this.label4.TabIndex = 0;
            this.label4.Text = "eye";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(863, 367);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.lb_playerID);
            this.Name = "Form1";
            this.Text = "Form1";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox6.ResumeLayout(false);
            this.groupBox6.PerformLayout();
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lb_connect_server;
        private System.Windows.Forms.Button bu_connect_server;
        private System.Windows.Forms.Label lb_playerID;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.CheckBox cb_gameOver_isWin;
        private System.Windows.Forms.Button bu_gameOver_send;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox tb_gameOver_playerId;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Button bu_roomCreate;
        private System.Windows.Forms.TextBox tb_roomEnterNum;
        private System.Windows.Forms.Button bu_roomEnter;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox tb_roomLeaveNum;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button bu_roomLeave;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox tb_corps_look_z;
        private System.Windows.Forms.TextBox tb_corps_look_y;
        private System.Windows.Forms.TextBox tb_corps_look_x;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox tb_corps_eye_z;
        private System.Windows.Forms.TextBox tb_corps_eye_y;
        private System.Windows.Forms.TextBox tb_corps_eye_x;
        private System.Windows.Forms.Button bu_generate_corps;
        private System.Windows.Forms.RadioButton rb_unit_arrow;
        private System.Windows.Forms.RadioButton rb_unit_guard;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.RadioButton rb_unit_knight;
        private System.Windows.Forms.RadioButton rb_unit_sword;
        private System.Windows.Forms.RadioButton rb_unit_pike;
        private System.Windows.Forms.Button bu_corps_stop;
        private System.Windows.Forms.Button bu_corps_move;
        private System.Windows.Forms.TextBox tb_corpsID;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.RadioButton rb_corps_formation_rush;
        private System.Windows.Forms.RadioButton rb_corps_formation_destroy;
        private System.Windows.Forms.RadioButton rb_corps_formation_defense;
        private System.Windows.Forms.Button bu_corps_formation;
    }
}

