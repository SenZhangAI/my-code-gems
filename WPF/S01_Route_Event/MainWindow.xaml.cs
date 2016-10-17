﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace WpfApplication1
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {

        protected int eventCounter = 0;

        public MainWindow()
        {
            InitializeComponent();

        }

        private void SomethingClicked(object sender, RoutedEventArgs e)
        {
            eventCounter++;

            string message = "#" + eventCounter.ToString() + ":\r\n" +
                " Sender: " + sender.ToString() + "\r\n" +
                " Source " + e.Source + "\r\n" +
                " Original Source: " + e.OriginalSource;

            lstMessage.Items.Add(message);
            e.Handled = (bool)chkHandle.IsChecked;
        }

        private void cmdClear_Click(object sender, RoutedEventArgs e)
        {
            eventCounter = 0;
            lstMessage.Items.Clear();
        }
    }
}
