using System;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.IO.Pipes;
using System.Windows.Forms;
using System.Threading;
using System.Linq;

namespace NEA___Chess_Engine
{
    public partial class FormGame : Form
    {
        int colour;
        int time;
        Board board = new Board();
        StreamReader sr;
        Process engine = null;
        bool recievemove = false;
        Button piece;
        NamedPipeServerStream server_pipe;

        public FormGame(int _colour, int _time)
        {
            InitializeComponent();
            Text = "Player: ";
            if (_colour == 1)
            {
                Text += "White";
            }
            else
            {
                Text += "Black";
            }
            Text += " | " + _time.ToString() + ":00";

            colour = _colour;
            time = _time;

            server_pipe = new NamedPipeServerStream("p", PipeDirection.InOut, 1, PipeTransmissionMode.Byte);
            sr = new StreamReader(server_pipe);

            label1.Text = time.ToString() + ":00";
            label2.Text = time.ToString() + ":00";
        }

        private void FormGame_Load(object sender, EventArgs e)
        {

            for (int x = 0; x < 8; x++)
            {
                for (int y = 0; y < 8; y++)
                {
                    Button button = new Button();
                    button.Dock = DockStyle.Fill;
                    button.Margin = new Padding(0);
                    button.FlatStyle = FlatStyle.Flat;
                    button.FlatAppearance.BorderSize = 0;
                    if ((x + y) % 2 == 1)
                    {
                        if (colour == 0)
                            button.BackColor = Color.White;

                        else
                            button.BackColor = Color.LightBlue;
                    }
                    else
                    {
                        if (colour == 1)
                            button.BackColor = Color.White;

                        else
                            button.BackColor = Color.LightBlue;
                    }

                    if (colour == 1)
                    {
                        if (board.boardArray[x, y] != null)
                        {
                            button.Tag = board.boardArray[x, y].tag;
                            if (board.boardArray[x, y].img != null)
                            {
                                button.Image = board.boardArray[x, y].img;
                            }
                        }
                    }

                    else
                    {
                        if (board.boardArray[7 - x, 7 - y] != null)
                        {
                            button.Tag = board.boardArray[7 - x, 7 - y].tag;
                            if (board.boardArray[7 - x, 7 - y].img != null)
                            {
                                button.Image = board.boardArray[7 - x, 7 - y].img;
                            }
                        }
                    }

                    boardTable.Controls.Add(button, y, x);
                    button.Click += Board_Click;
                }
            }
        }

        private void Board_Click(object s, EventArgs e)
        {
            if (board.current_player == colour)
            {
                //MessageBox.Show(board.current_player.ToString(), colour.ToString());
                piece = (Button)s;
                TableLayoutPanelCellPosition p = boardTable.GetCellPosition(piece);

                if (board.selected_piece_tag == 0)
                {
                    if (board.get_colour((int)piece.Tag) == colour)
                    {
                        board.selected_piece_tag = (int)piece.Tag;
                        board.selected_piece_position = new Point(p.Column, p.Row);
                        board.selected_piece_img = piece.Image;
                    }
                }
                else
                {

                    int src_col = board.selected_piece_position.X;
                    int src_row = board.selected_piece_position.Y;
                    int dst_col = p.Column;
                    int dst_row = p.Row;
                    int type = board.selected_piece_tag;

                    if (colour == 0)
                    {
                        src_col = 7 - board.selected_piece_position.X;
                        src_row = 7 - board.selected_piece_position.Y;
                        dst_col = 7 - p.Column;
                        dst_row = 7 - p.Row;
                    }

                    //MessageBox.Show(board.selected_piece_position.X.ToString(), board.selected_piece_position.Y.ToString());
                    //MessageBox.Show(p.Column.ToString(), p.Row.ToString());

                    char[] move_data = new char[5];
                    move_data[0] = (char)src_row;
                    move_data[1] = (char)src_col;
                    move_data[2] = (char)dst_row;
                    move_data[3] = (char)dst_col;
                    move_data[4] = (char)type;

                    StreamWriter sw = new StreamWriter(server_pipe);
                    sw.Write(move_data);
                    sw.Flush();
                    recievemove = true;
                }
            }
        }

        private void start_engine_and_create_pipe()
        {
            engine = Process.Start("..\\..\\..\\Backend - Engine\\Debug\\Backend - Engine.exe");

            server_pipe.WaitForConnection();
            StreamWriter sw = new StreamWriter(server_pipe);
            sw.Write((byte)colour ^ 1);
            sw.Flush();
        }

        private void FormGame_FormClosing(object sender, FormClosingEventArgs e)
        {

            server_pipe.Dispose();

            if (engine != null)
            {
                if (!engine.HasExited)
                {
                    engine.Kill();
                }
            }
        }

        private void timer1_tick(object sender, EventArgs e)
        {
            if (board.current_player == colour)
            {
                string t = label2.Text;
                int min = int.Parse(t.Substring(0, t.IndexOf(":")));
                int seconds = int.Parse(t.Substring(t.IndexOf(":") + 1));
                //int seconds = int.Parse(t[t.Length - 2].ToString() + t[t.Length - 1].ToString());

                if (seconds == 0)
                {
                    if (min == 0)
                    {
                        timer1.Stop();
                        string msg = "You lose";
                        MessageBox.Show(msg);
                    }
                    t = (min - 1).ToString() + ":59";
                }
                else
                {
                    t = (min).ToString() + ":";
                    if ((seconds - 1).ToString().Length == 1)
                    {
                        t += "0";
                    }
                    t += (seconds - 1).ToString();
                }
                label2.Text = t;
            }
            else
            {
                string t = label1.Text;
                int min = int.Parse(t.Substring(0, t.IndexOf(":")));
                int seconds = int.Parse(t.Substring(t.IndexOf(":") + 1));

                if (seconds == 0)
                {
                    if (min == 0)
                    {
                        timer1.Stop();
                        string msg = "You Win";
                        MessageBox.Show(msg);

                    }
                    t = (min - 1).ToString() + ":59";
                }
                else
                {
                    t = (min).ToString() + ":";
                    if ((seconds - 1).ToString().Length == 1)
                    {
                        t += "0";
                    }
                    t += (seconds - 1).ToString();
                }
                label1.Text = t;
            }
        }

        private void recieve_move()
        {
            while (true)
            {
                if (recievemove == true)
                {
                    if (colour == board.current_player)
                    {
                        char[] output = new char[5];
                        sr.Read(output, 0, 1);
                        if (output[0] == 0)
                        {
                            piece.Tag = board.selected_piece_tag;
                            piece.Image = board.selected_piece_img;

                            Button prev_piece = (Button)boardTable.GetControlFromPosition(board.selected_piece_position.X, board.selected_piece_position.Y);
                            prev_piece.Image = null;
                            prev_piece.Tag = 0;
                            board.switch_player();
                        }
                        if (output[0] == 2)
                        {
                            TableLayoutPanelCellPosition p = boardTable.GetCellPosition(piece);
                            if (board.selected_piece_position.X == 7 || p.Column == 7)
                            {
                                Button rook = (Button)boardTable.GetControlFromPosition(7, p.Row);
                                Button king;
                                if (board.current_player == 1)
                                {
                                    king = (Button)boardTable.GetControlFromPosition(4, p.Row);
                                }
                                else
                                {
                                    king = (Button)boardTable.GetControlFromPosition(3, p.Row);
                                }

                                Button new_rook;
                                if (board.current_player == 1)
                                {
                                    new_rook = (Button)boardTable.GetControlFromPosition(5, p.Row);
                                }
                                else
                                {
                                    new_rook = (Button)boardTable.GetControlFromPosition(4, p.Row);
                                }

                                if (colour == 1)
                                {
                                    new_rook.Tag = 5;
                                    new_rook.Image = Image.FromFile("../../../Images//WR.png");
                                }
                                else
                                {
                                    new_rook.Tag = 11;
                                    new_rook.Image = Image.FromFile("../../../Images//BR.png");
                                }
                                rook.Image = null;
                                rook.Tag = 0;

                                Button new_king;
                                if (board.current_player == 1)
                                {
                                    new_king = (Button)boardTable.GetControlFromPosition(6, p.Row);
                                }
                                else
                                {
                                    new_king = (Button)boardTable.GetControlFromPosition(5, p.Row);
                                }

                                if (board.current_player == 1)
                                {
                                    new_king.Image = Image.FromFile("../../../Images//WK.png");
                                    new_king.Tag = 1;
                                }
                                else
                                {
                                    new_king.Tag = 7;
                                    new_king.Image = Image.FromFile("../../../Images//BK.png");
                                }
                                king.Image = null;
                                king.Tag = 0;
                            }
                            if (board.selected_piece_position.X == 0 || p.Column == 0)
                            {
                                Button rook = (Button)boardTable.GetControlFromPosition(0, p.Row);
                                Button king;
                                if (board.current_player == 1)
                                {
                                    king = (Button)boardTable.GetControlFromPosition(4, p.Row);
                                }
                                else
                                {
                                    king = (Button)boardTable.GetControlFromPosition(3, p.Row);
                                }

                                Button new_rook;
                                if (board.current_player == 1)
                                {
                                    new_rook = (Button)boardTable.GetControlFromPosition(3, p.Row);
                                }
                                else
                                {
                                    new_rook = (Button)boardTable.GetControlFromPosition(2, p.Row);
                                }

                                if (colour == 1)
                                {
                                    new_rook.Tag = 5;
                                    new_rook.Image = Image.FromFile("../../../Images//WR.png");
                                }
                                else
                                {
                                    new_rook.Tag = 11;
                                    new_rook.Image = Image.FromFile("../../../Images//BR.png");
                                }
                                rook.Image = null;
                                rook.Tag = 0;

                                Button new_king;
                                if (board.current_player == 1)
                                {
                                    new_king = (Button)boardTable.GetControlFromPosition(2, p.Row);
                                }
                                else
                                {
                                    new_king = (Button)boardTable.GetControlFromPosition(1, p.Row);
                                }

                                if (board.current_player == 1)
                                {
                                    new_king.Image = Image.FromFile("../../../Images//WK.png");
                                    new_king.Tag = 1;
                                }
                                else
                                {
                                    new_king.Tag = 7;
                                    new_king.Image = Image.FromFile("../../../Images//BK.png");
                                }
                                king.Image = null;
                                king.Tag = 0;
                            }
                            board.switch_player();
                        }
                        board.selected_piece_tag = 0;
                    }
                    if (colour != board.current_player)
                    {
                        //MessageBox.Show(board.current_player.ToString());
                        char[] output = new char[5];
                        //sr = new StreamReader(pipe);
                        sr.Read(output, 0, 5);
                        int src_row = output[0];
                        int src_col = output[1];
                        int dst_row = output[2];
                        int dst_col = output[3];

                        if (src_row == 8)
                        {
                            timer1.Stop();
                            MessageBox.Show("You have won");
                            engine.Close();
                        }
                        else
                        {
                            //MessageBox.Show(src_row.ToString());
                            //MessageBox.Show(src_col.ToString());
                            //MessageBox.Show(dst_row.ToString());
                            //MessageBox.Show(dst_col.ToString());
                            if (colour == 0)
                            {
                                src_col = 7 - src_col;
                                src_row = 7 - src_row;
                                dst_col = 7 - dst_col;
                                dst_row = 7 - dst_row;
                            }

                            Button prev_piece = (Button)boardTable.GetControlFromPosition(src_col, src_row);
                            Button dst_sqaure = (Button)boardTable.GetControlFromPosition(dst_col, dst_row);

                            if (((int)prev_piece.Tag == 1 && (int)dst_sqaure.Tag == 5) || ((int)prev_piece.Tag == 7 && (int)dst_sqaure.Tag == 11) ||
                               ((int)prev_piece.Tag == 5 && (int)dst_sqaure.Tag == 1) || ((int)prev_piece.Tag == 11 && (int)dst_sqaure.Tag == 7))
                            {
                                TableLayoutPanelCellPosition prev = boardTable.GetCellPosition(prev_piece);
                                TableLayoutPanelCellPosition dest = boardTable.GetCellPosition(dst_sqaure);
                                if (prev.Column == 7 || dest.Column == 7)
                                {
                                    Button rook = (Button)boardTable.GetControlFromPosition(7, prev.Row);
                                    Button king;
                                    if (board.current_player == 1)
                                    {
                                        king = (Button)boardTable.GetControlFromPosition(4, prev.Row);
                                    }
                                    else
                                    {
                                        king = (Button)boardTable.GetControlFromPosition(3, prev.Row);
                                    }

                                    Button new_rook;
                                    if (board.current_player == 1)
                                    {
                                        new_rook = (Button)boardTable.GetControlFromPosition(5, prev.Row);
                                    }
                                    else
                                    {
                                        new_rook = (Button)boardTable.GetControlFromPosition(4, prev.Row);
                                    }

                                    if (colour == 1)
                                    {
                                        new_rook.Tag = 5;
                                        new_rook.Image = Image.FromFile("../../../Images//WR.png");
                                    }
                                    else
                                    {
                                        new_rook.Tag = 11;
                                        new_rook.Image = Image.FromFile("../../../Images//BR.png");
                                    }
                                    rook.Image = null;
                                    rook.Tag = 0;
                                    Button new_king;
                                    if (board.current_player == 1)
                                    {
                                        new_king = (Button)boardTable.GetControlFromPosition(6, prev.Row);
                                    }
                                    else
                                    {
                                        new_king = (Button)boardTable.GetControlFromPosition(5, prev.Row);
                                    }

                                    if (board.current_player == 1)
                                    {
                                        new_king.Image = Image.FromFile("../../../Images//WK.png");
                                        new_king.Tag = 1;
                                    }
                                    else
                                    {
                                        new_king.Tag = 7;
                                        new_king.Image = Image.FromFile("../../../Images//BK.png");
                                    }
                                    king.Image = null;
                                    king.Tag = 0;
                                }
                                if (prev.Column == 0 || dest.Column == 0)
                                {
                                    Button rook = (Button)boardTable.GetControlFromPosition(0, prev.Row);
                                    Button king = (Button)boardTable.GetControlFromPosition(3, prev.Row);

                                    Button new_rook = (Button)boardTable.GetControlFromPosition(2, prev.Row);
                                    if (colour == 1)
                                    {
                                        new_rook.Tag = 5;
                                        new_rook.Image = Image.FromFile("../../../Images//WR.png");
                                    }
                                    else
                                    {
                                        new_rook.Tag = 11;
                                        new_rook.Image = Image.FromFile("../../../Images//BR.png");
                                    }
                                    rook.Image = null;
                                    rook.Tag = 0;
                                    Button new_king = (Button)boardTable.GetControlFromPosition(1, prev.Row);
                                    if (colour == 1)
                                    {
                                        new_king.Image = Image.FromFile("../../../Images//WK.png");
                                        new_king.Tag = 1;
                                    }
                                    else
                                    {
                                        new_king.Tag = 7;
                                        new_king.Image = Image.FromFile("../../../Images//BK.png");
                                    }
                                    king.Image = null;
                                    king.Tag = 0;
                                }
                            }
                            else
                            {
                                dst_sqaure.Image = prev_piece.Image;
                                dst_sqaure.Tag = prev_piece.Tag;
                                prev_piece.Image = null;
                                prev_piece.Tag = 0;
                                board.switch_player();
                            }
                        }
                    }
                    recievemove = false;

                }
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            start_engine_and_create_pipe();
            timer1.Enabled = true;
            timer1.Start();
            if (colour == 0)
            {
                recievemove = true;
            }

            Thread t = new Thread(recieve_move);
            t.Start();

            button1.Visible = false;
        }
    }
}