using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SteamIdleHelper
{
    public partial class Form1 : Form
    {
        public const int WM_SHOWWINDOW = 0x0018;
        private const int SW_SHOWDEFAULT = 10;
        [DllImport("user32.dll")]
        private static extern bool ShowWindowAsync(IntPtr hWnd, int nCmdShow);

        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "TXT|*.txt";
            ofd.ValidateNames = true;
            ofd.CheckPathExists = true;
            ofd.CheckFileExists = true;
            ofd.Title = "ѡ����Ŀ�ļ�";
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                var fs= System.IO.File.OpenRead(ofd.FileName);
                System.IO.StreamReader read = new System.IO.StreamReader(fs, Encoding.Default);
                string Str;
                foreach (Process p in Process.GetProcessesByName("steam-idle"))
                {
                    p.Kill();
                }
                while (!read.EndOfStream&&(Str=read.ReadLine())!="")
                {
                    Process.Start(new ProcessStartInfo("steam-idle.exe", Str+ " 0"));
                }
            }
        }
        private bool EnumChild(IntPtr handle, string num)
        {
            return true;
        }

        private void button4_Click(object sender, EventArgs e)
        {
            foreach (Process p in Process.GetProcessesByName("steam-idle"))
            {
                p.Kill();
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            IntPtr mainWindowHandle = IntPtr.Zero;
            foreach (Process p in Process.GetProcessesByName("steam-idle"))
            {
                ShowWindowAsync(User32API.GetWindowHandle((uint)p.Id), SW_SHOWDEFAULT);
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            foreach (Process p in Process.GetProcessesByName("steam-idle"))
            {
                ShowWindowAsync(p.MainWindowHandle, 0);
            }
        }
    }
}

public class User32API
{
    private static Hashtable processWnd = null;

    public delegate bool WNDENUMPROC(IntPtr hwnd, uint lParam);

    static User32API()
    {
        if (processWnd == null)
        {
            processWnd = new Hashtable();
        }
    }

    [DllImport("user32.dll", EntryPoint = "EnumWindows", SetLastError = true)]
    public static extern bool EnumWindows(WNDENUMPROC lpEnumFunc, uint lParam);

    [DllImport("user32.dll", EntryPoint = "GetParent", SetLastError = true)]
    public static extern IntPtr GetParent(IntPtr hWnd);

    [DllImport("user32.dll", EntryPoint = "GetWindowThreadProcessId")]
    public static extern uint GetWindowThreadProcessId(IntPtr hWnd, ref uint lpdwProcessId);

    [DllImport("user32.dll", EntryPoint = "IsWindow")]
    public static extern bool IsWindow(IntPtr hWnd);

    [DllImport("kernel32.dll", EntryPoint = "SetLastError")]
    public static extern void SetLastError(uint dwErrCode);

    public static IntPtr GetWindowHandle(uint uiPid)
    {
        IntPtr ptrWnd = IntPtr.Zero;
        //uint uiPid = (uint)Process.GetCurrentProcess().Id;  // ��ǰ���� ID
        object objWnd = processWnd[uiPid];

        if (objWnd != null)
        {
            ptrWnd = (IntPtr)objWnd;
            if (ptrWnd != IntPtr.Zero && IsWindow(ptrWnd))  // �ӻ����л�ȡ���
            {
                return ptrWnd;
            }
            else
            {
                ptrWnd = IntPtr.Zero;
            }
        }

        bool bResult = EnumWindows(new WNDENUMPROC(EnumWindowsProc), uiPid);
        // ö�ٴ��ڷ��� false ����û�д����ʱ������ȡ�ɹ�
        if (!bResult && Marshal.GetLastWin32Error() == 0)
        {
            objWnd = processWnd[uiPid];
            if (objWnd != null)
            {
                ptrWnd = (IntPtr)objWnd;
            }
        }

        return ptrWnd;
    }

    private static bool EnumWindowsProc(IntPtr hwnd, uint lParam)
    {
        uint uiPid = 0;

        if (GetParent(hwnd) == IntPtr.Zero)
        {
            GetWindowThreadProcessId(hwnd, ref uiPid);
            if (uiPid == lParam)    // �ҵ����̶�Ӧ�������ھ��
            {
                processWnd[uiPid] = hwnd;   // �Ѿ����������
                SetLastError(0);    // �����޴���
                return false;   // ���� false ����ֹö�ٴ���
            }
        }

        return true;
    }
}
