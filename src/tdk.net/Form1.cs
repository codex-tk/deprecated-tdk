using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace tdk.net
{
    public partial class Form1 : Form
    {
        tdk.clr.log.file_writer writer = new clr.log.file_writer("test");
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            tdk.clr.log.logger logger = new clr.log.logger( "tdk.clr");
            logger.add_writer( writer );
            logger.debug( "Debug Test {0} {1}" , DateTime.Now.ToString() , 1 );

            tdk.clr.log.logger logger2 = new clr.log.logger( "tdk.clr2");
            logger2.add_writer( writer );
            logger2.debug( "Debug Test {0} {1}" , DateTime.Now.ToString() , 1 );
        }

        private void button2_Click(object sender, EventArgs e)
        {
            tdk.clr.error_code ec = new clr.error_code();
            System.Diagnostics.Trace.WriteLine(ec.message());
        }
    }
}
