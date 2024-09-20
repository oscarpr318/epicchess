using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NEA___Chess_Engine
{
    class Board
    {
        public Piece[,] boardArray = new Piece[8,8];
        public Point selected_piece_position = new Point();
        public int selected_piece_tag = 0;
        public Image selected_piece_img = null;
        public int current_player = 1; // White, 0 = black

        public Board()
        {
            initBoard();
        }

        private void initBoard()
        {
            int[] black_tags = { 11, 10, 9,8, 7, 9, 10, 11};
            int[] white_tags = { 5, 4, 3, 2, 1, 3, 4, 5 };
            int x;
            for (x = 0; x < 8; x++)
            {
                boardArray[0, x] = new Piece(black_tags[x]);
                boardArray[1, x] = new Piece(12);
                boardArray[6, x] = new Piece(6);
                boardArray[7, x] = new Piece(white_tags[x]);
                boardArray[2, x] = new Piece(0);
                boardArray[3, x] = new Piece(0);
                boardArray[4, x] = new Piece(0);
                boardArray[5, x] = new Piece(0);
            }
        }

        public void switch_player()
        {
            current_player ^= 1;
        }

        public int get_colour(int tag)
        {
            if (tag < 7 && tag > 0)
            {
                return 1;
            }
            return 0;
        }
    }
}
