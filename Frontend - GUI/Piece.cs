using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NEA___Chess_Engine
{
    class Piece
    {
        public int tag;
        public Image img = null;
         
        public Piece(int _t)
        {
            tag = _t;
            getImage();
        }

        void getImage()
        {
            switch (tag)
            {
                case 1:
                    img = Image.FromFile("../../../Images//WK.png");
                    break;

                case 7:
                    img = Image.FromFile("../../../Images//BK.png");
                    break;

                case 2:
                    img = Image.FromFile("../../../Images//WQ.png");
                    break;

                case 8:
                    img = Image.FromFile("../../../Images//BQ.png");
                    break;

                case 3:
                    img = Image.FromFile("../../../Images//WB.png");
                    break;

                case 9:
                    img = Image.FromFile("../../../Images//BB.png");
                    break;

                case 4:
                    img = Image.FromFile("../../../Images//WN.png");
                    break;

                case 10:
                    img = Image.FromFile("../../../Images//BN.png");
                    break;

                case 5:
                    img = Image.FromFile("../../../Images//WR.png");
                    break;

                case 11:
                    img = Image.FromFile("../../../Images//BR.png");
                    break;

                case 6:
                    img = Image.FromFile("../../../Images//WP.png");
                    break;

                case 12:
                    img = Image.FromFile("../../../Images//BP.png");
                    break;
            }
        }
    }
}
