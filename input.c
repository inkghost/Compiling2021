int a[3][4];

int main() {
    int i   = 0;
    int cnt = 0;
    while (i <= 3 + 4 - 2) {
        int j = i;
        while (j >= 0) {
            if (j < 4 && i - j < 3) {
                a[i - j][j] = cnt;
                cnt         = cnt + 1;
            }
            j = j - 1;
        }
        i = i + 1;
    }
    i     = 0;
    int j = 0;
    while (i < 3) {
        j = 0;
        while (j < 4) {
            putint(a[i][j]);
            putch(32);
            j = j + 1;
        }
        putch(10);
        i = i + 1;
    }
    return 0;
}