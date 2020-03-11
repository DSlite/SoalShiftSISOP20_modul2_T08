# Shift 2 SISOP 2020 - T08
Penyelesaian Soal Shift 2 Sistem Operasi 2020\
Kelompok T08
  * I Made Dindra Setyadharma (05311840000008)
  * Muhammad Irsyad Ali (05311840000041)

---
## Table of Contents
* [Soal 1](#soal-1)
* [Soal 2](#soal-2)
  * [Soal 2.a.](#soal-2a)
  * [Soal 2.b.](#soal-2b)
  * [Soal 2.c.](#soal-2c)
  * [Soal 2.d.](#soal-2d)
  * [Soal 2.e.](#soal-2e)
* [Soal 3](#soal-3)
  * [Soal 3.a.](#soal-3a)
  * [Soal 3.b.](#soal-3b)
  * [Soal 3.c.](#soal-3c)
  * [Soal 3.d.](#soal-3d)
---

## Soal 1
Source Code : [source](https://github.com/DSlite/SoalShiftSISOP20_modul2_T08/blob/master/soal1.c)

**Deskripsi:**\
Buatlah program C yang menyerupai crontab untuk menjalankan script bash dengan ketentuan sebagai berikut:
1. Program menerima 4 argumen berupa:
    1. Detik: 0-59 atau * (any value)
    2. Menit: 0-59 atau * (any value)
    3. Jam: 0-23 atau * (any value)
    4. Path file .sh
2. Program akan mengeluarkan pesan error jika argumen yang  diberikan tidak sesuai
3. Program hanya menerima 1 config cron
4. Program berjalan di background (daemon)
5. Tidak boleh menggunakan fungsi system()

Contoh: `./program \* 34 7 /home/somi/test.sh`

**Pembahasan:**\
Untuk membuat program C yang berjalan di background, pertama harus melakukan `#include` terhadap library yang diperlukan

``` c
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <wait.h>
```

* `<sys/types.h>` library tipe data khusus (e.g. `pid_t`)
* `<stdio.h>` library untuk fungsi input-output (e.g. `printf(), sprintf()`)
* `<stdlib.h>` library untuk fungsi umum (e.g. `exit(), atoi()`)
* `<unistd.h>` library untuk melakukan system call kepada kernel linux (e.g. `fork()`)
* `<time.h>` library untuk melakukan manipulasi date dan time (e.g. `time(), strftime()`)
* `<wait.h>` Library untuk melakukan *wait* (e.g. `wait()`)

Setelah itu, karena pada soal program C yang dijalankan akan menerima argumen, sehingga kami harus deklarasi variable `argc` dan `argv` pada fungsi `main()`

```c
int main(int argc, char *argv[])
```

Agar menampilkan pesan error, masing-masing argumen akan dicek apakah sesuai dengan ketentuan yang ada.

```c
if (argc != 5) {
  printf("ERROR! Argumen Salah\n");
  return 1;
}
if (!(*argv[1] == '*' || (atoi(argv[1]) >= 0 && atoi(argv[1]) <= 59))) {
  printf("ERROR! Argumen Salah\n");
  return 1;
}
if (!(*argv[2] == '*' || (atoi(argv[2]) >= 0 && atoi(argv[2]) <= 59))) {
  printf("ERROR! Argumen Salah\n");
  return 1;
}
if (!(*argv[3] == '*' || (atoi(argv[3]) >= 0 && atoi(argv[3]) <= 23))) {
  printf("ERROR! Argumen Salah\n");
  return 1;
}
if (access(argv[4], F_OK) == -1) {
  printf("ERROR! File tidak ada\n");
  return 1;
}
```

* `argc` akan berisi banyaknya *arguments*. Jumlah *arguments* harus 5 (perintah "`./program`" terhitung dalam argumen)
* `argv[1]` akan berisi *argument* detik. Disini kami mengecek apakah nilainya berupa `'*'` atau diantara 0-59 menggunakan fungsi `atoi()` untuk mengubah `string` menjadi `int`
* `argv[2]` akan berisi *argument* menit. Disini kami akan mengecek apakah nilainya berupa `'*'` atau diantara 0-59 menggunakan fungsi `atoi()` untuk mengubah `string` menjadi `int`
* `argv[3]` akan berisi *argument* jam. Disini kami akan mengecek apakah nilainya berupa `'*'` atau diantara 0-23 menggunakan fungsi `atoi()` untuk mengubah `string` menjadi `int`
* `argv[4]` akan berisi `path` ke sebuah file shell script yang akan dijalankan. Disini kami akan mengecek file tersebut ada atau tidak menggunakan fungsi `access()`

Setelah itu, kami harus membuat program menjadi daemon.

```c
pid_t pid, sid;
pid = fork();

if (pid < 0) {
  exit(EXIT_FAILURE);
}

if (pid > 0) {
  exit(EXIT_SUCCESS);
}
```

Pertama, kami akan melakukan `fork()`. sehingga menghasilkan parent process dengan variable `pid` berisi **`PID`** dari child processnya dan child process dengan variable `pid` berisi nilai **`0`**. Lalu parent process akan di keluarkan menggunakan fungsi `exit()` dengan exit statusnya.

```c
sid = setsid();
if (sid < 0) {
  exit(EXIT_FAILURE);
}

close(STDIN_FILENO);
close(STDOUT_FILENO);
close(STDERR_FILENO);

while (1) {
  ...
  sleep(1);
}
```

Selanjutnya child process akan membuat session ID (`sid`) menggunakan fungsi `setsid()`. Lalu jika gagal, process tersebut akan dikeluarkan. Lalu process akan menutup *file descriptor* menggunakan fungsi `close()`. Dan loop utama akan dijalankan dengan setiap **satu** detik dengan fungsi `sleep(1)`.

```c
time_t t = time(NULL);
struct tm *tm = localtime(&t);
```

Dalam fungsi utama, kami akan mengeset waktu saat ini variable `t` dengan fungsi `time(NULL)`. Karena format dari variable `t` masih dalam **epoch/unix timestamp**, sehingga perlu diubah ke bentuk format yang sudah terstandard. Disini kami menggunakan fungsi `localtime` dan memasukkannya kedalam variable `tm`.

```c
if (atoi(argv[1]) == tm->tm_sec || *argv[1] == '*') {
  if (atoi(argv[2]) == tm->tm_min || *argv[2] == '*') {
    if (atoi(argv[3]) == tm->tm_hour || *argv[3] == '*') {
      ...
    }
  }
}
```

Setelah itu, kami akan mengecek apakah detik (`tm->tm_sec`), menit (`tm->tm_min`), dan jam (`tm->tm_hour`) saat ini sudah sesuai dengan *arguments* yang diinputkan, atau *arguments* yang diinputkan merupakan karakter `'*'`.

```c
pid_t child_id;
child_id = fork();

if (child_id == 0) {

  char *argvChild[] = {"bash", argv[4], NULL};
  execv("/bin/bash", argvChild);

}
while(wait(NULL) > 0);
```

Jika kondisi sebelumnya terpenuhi, maka program akan mengeksekusi *shell script* yang diinputkan. Pertama, kita perlu melakukan `fork()` dimana *child process* akan menjalankan *shell script*-nya, dan *parent process* akan menunggu *child process*-nya selesai mengeksekusi *shell script* tersebut.

#### ScreenShot
**Contoh Output Berhasil:**\
![Output Soal 1-1](https://user-images.githubusercontent.com/17781660/76401535-9c8d7200-63b4-11ea-96ba-04edb884761e.png)

**Isi File /home/umum/test.sh :**\
![Output Soal 1-2](https://user-images.githubusercontent.com/17781660/76400474-f8ef9200-63b2-11ea-8cc9-bd2f1141b83e.png)

**Contoh Output Jika Arguments Salah:**\
![Output Soal 1-3](https://user-images.githubusercontent.com/17781660/76400520-0c026200-63b3-11ea-824a-725c0cb27696.png)

**Contoh Output Jika File Shell Script Tidak Ada:**\
![Output Soal 1-4](https://user-images.githubusercontent.com/17781660/76400554-17ee2400-63b3-11ea-9505-9270a6715594.png)

## Soal 2

## Soal 3
