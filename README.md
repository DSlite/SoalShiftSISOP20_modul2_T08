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
Source Code : [soal1.c](https://github.com/DSlite/SoalShiftSISOP20_modul2_T08/blob/master/soal1/soal1.c)

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
int main(int argc, char *argv[]) {

  ...

}
```

*note: tanda `...` merupakan kode program yang tidak ditampilkan untuk memudahkan pembacaan, untuk lebih detail dapat dilihat pada [soal1.c](https://github.com/DSlite/SoalShiftSISOP20_modul2_T08/blob/master/soal1/soal1.c)*

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
*note: tanda `...` merupakan kode program yang tidak ditampilkan untuk memudahkan pembacaan, untuk lebih detail dapat dilihat pada [soal1.c](https://github.com/DSlite/SoalShiftSISOP20_modul2_T08/blob/master/soal1/soal1.c)*

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
*note: tanda `...` merupakan kode program yang tidak ditampilkan untuk memudahkan pembacaan, untuk lebih detail dapat dilihat pada [soal1.c](https://github.com/DSlite/SoalShiftSISOP20_modul2_T08/blob/master/soal1/soal1.c)*

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
Source Code : [soal2.c](https://github.com/DSlite/SoalShiftSISOP20_modul2_T08/blob/master/soal2/soal2.c)

**Deskripsi:**\
Shisoppu mantappu! itulah yang selalu dikatakan Kiwa setiap hari karena sekarang dia
merasa sudah jago materi sisop. Karena merasa jago, suatu hari Kiwa iseng membuat
sebuah program.

*Catatan:*
* *Tidak boleh memakai `system()`*
* *Program utama harus ter-detach dari terminal*

### Soal 2.a.

**Deskripsi:**\
Pertama-tama, Kiwa membuat sebuah folder khusus, di dalamnya dia membuat sebuah program C yang per 30 detik membuat sebuah folder dengan nama timestamp \[YYYY-mm-dd_HH:ii:ss\].

**Pembahasan:**\
Pertama, kami akan melakukan `#include` library yang diperlukan

```c
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <wait.h>
#include <string.h>
```

Library yang digunakan untuk soal ini mirip dengan soal sebelumnya. Disini hanya ditambahkan `#include <string.h>` yang digunakan untuk manipulasi string (e.g. `strcmp()`). Selanjutnya, agar program ter-*detach* dari terminal, maka pada `int main()`, program tersebut harus dijadikan *daemon*.

```c
pid_t pid, sid;
pid = fork();

if (pid < 0) {
  exit(EXIT_FAILURE);
}

if (pid > 0) {
  exit(EXIT_SUCCESS);
}

sid = setsid();
if (sid < 0) {
  exit(EXIT_FAILURE);
}

...

close(STDIN_FILENO);
close(STDERR_FILENO);
close(STDOUT_FILENO);

while(1) {
  ...
}
```

*note: tanda `...` merupakan kode program yang tidak ditampilkan untuk memudahkan pembacaan, untuk lebih detail dapat dilihat pada [soal2.c](https://github.com/DSlite/SoalShiftSISOP20_modul2_T08/blob/master/soal2/soal2.c)*

Proses diatas sama dengan proses pembuatan daemon pada soal1. Lalu kami akan melakukan pendeklarasian variable awal.

```c
time_t t = time(NULL);
struct tm *tm = localtime(&t);
char now[80];
```

* Variable `t` digunakan untuk menyimpan timestamp dalam format **epoch/unix timestamp**.
* Variable `tm` digunakan untuk menyimpan timestamp yang sudah terstruktur sesuai dengan `localtime`
* Variable `now` akan digunakan sebagai buffer dari string hasil format variable `tm`

```c
strftime(now, 80, "%Y-%m-%d_%H:%M:%S", tm);
```

kode `strftime` diatas digunakan untuk melakukan formatting dari waktu `tm` menjadi string sesuai dengan format yang diinginkan (**[YYYY-mm-dd_HH:ii:ss]**) kedalam buffer `now` sebesar `80`.

```c
pid_t child_id;
child_id = fork();

if (child_id == 0) {
  char *argv[] = {"mkdir", now, NULL};
  execv("/bin/mkdir", argv);
}

...

sleep(30);
```

*note: tanda `...` merupakan kode program yang tidak ditampilkan untuk memudahkan pembacaan, untuk lebih detail dapat dilihat pada [soal2.c](https://github.com/DSlite/SoalShiftSISOP20_modul2_T08/blob/master/soal2/soal2.c)*

Lalu, process akan di `fork()` dan *child process* akan melakukan `execv()` terhadap perintah `mkdir` dengan *argument* `now`. Sementara *parent process* **tidak** akan menunggu child process dan akan langsung `sleep()` selama 30 detik.

## Soal 2.b.

**Deskripsi:**\
Tiap-tiap folder lalu diisi dengan 20 gambar yang di download dari *https://picsum.photos/*, dimana tiap gambar di download setiap 5 detik. Tiap gambar berbentuk persegi dengan ukuran *(`t`%1000)+100* piksel dimana `t` adalah detik Epoch Unix. Gambar tersebut diberi nama dengan format timestamp [YYYY-mm-dd_HH:ii:ss].

**Pembahasan:**\
Untuk menjawab soal 2.b. pertama kami membuat *child process* untuk melakukan download 20 gambar setelah *directory* dibuat pada bagian kode sebelumnya. Namun program utamanya tidak akan menunggu *child process*-nya (download gambar) dan langsung `sleep()` selama 30 detik setelah fungsi `fork()` dijalankan.

```c
child_id = fork();
if (child_id == 0) {

  for (int i = 0; i < 20; i++) {

    ...

    sleep(5);

  }

  ...

}

```

*note: tanda `...` merupakan kode program yang tidak ditampilkan untuk memudahkan pembacaan, untuk lebih detail dapat dilihat pada [soal2.c](https://github.com/DSlite/SoalShiftSISOP20_modul2_T08/blob/master/soal2/soal2.c)*

Pertama, *child process* akan melakukan loop selama 20 kali, dan pada masing-masing loop melakukan `sleep()` selama 5 detik.

```c
child_id = fork();
if (child_id == 0) {

  t = time(NULL);
  tm = localtime(&t);
  char new_now[80], location[160], link[80];
  strftime(new_now, 80, "%Y-%m-%d_%H:%M:%S", tm);
  sprintf(location, "%s/%s", now, new_now);
  sprintf(link, "https://picsum.photos/%ld", ((t%1000)+100));
  char *argv[] = {"wget", "-O", location, link, NULL};
  execv("/usr/bin/wget", argv);

}
```

Lalu, pada masing-masing loop akan dibuat *child process* yang dimana masing-masing *child process* akan mendownload file pada ***picsum.photos*** yang sesuai menggunakan `wget`.

* `t` digunakan untuk mengambil waktu Epoch UNIX yang baru.
* `tm` digunakan untuk mengambil waktu `t` yang baru dalam format yang terstruktur.
* Lalu deklarasi variable `new_now` untuk menyimpan waktu saat ini yang sudah di format menjadi string, `location` untuk menyimpan lokasi output dari wget, dan `link` untuk menyimpan alamat tempat mendownload pada ***picsum.photos***
* `new_now` akan diformat menggunakan `strftime()` berdasarkan nilai `tm` yang baru.
* `location` akan diformat menjadi "`[now]/[new_now]`" agar file yang didownload masuk kedalam folder yang diinginkan.
* `link` akan diformat sesuai dengan `((t%1000)+100)`. Untuk mendapatkan foto dengan ukuran piksel seperti itu hanya perlu menanbahkan nilainya pada bagian belakang alamat `https://picsum.photos/`.
* `wget` akan dijalankan sesuai `location` dan `link` yang sudah diformat.

### Soal 2.c.

**Deskripsi:**\
Agar rapi, setelah sebuah folder telah terisi oleh 20 gambar, folder akan di zip dan folder akan di delete(sehingga hanya menyisakan .zip).

**Pembahasan:**\
Untuk melakukan zip, maka setelah `for` loop pada soal sebelumnya selesai, akan melakukan `fork()` lagi. Dimana *child process* akan melakukan zip terhadap folder tersebut. dan *parent process* akan menunggu *child process* selesai dan menghapus directory folder yang telah di zip.

```c
child_id = fork();
if (child_id == 0) {

  char nama_file[80];
  sprintf(nama_file, "%s.zip", now);
  char *argv[] = {"zip", "-r", nama_file, now, NULL};
  execv("/usr/bin/zip", argv);

}

while(wait(NULL) != child_id);
char *argv[] = {"rm", "-r", now, NULL};
execv("/bin/rm", argv);
```

### Soal 2.d.

**Deskripsi:**\
Karena takut program tersebut lepas kendali, Kiwa ingin program tersebut men-generate sebuah program "`killer`" **yang siap di run(executable)** untuk menterminasi semua operasi program tersebut. Setelah di run, program yang menterminasi ini lalu akan mendelete dirinya sendiri.

**Pembahasan:**\
Untuk men-generate sebuah program yang bisa menterminasi semua operasi program, kami pertama membuat file yang akan kami inputkan dengan kode **C**.

```c
FILE *pFile;
pFile = fopen("killer.c", "w");
```

* `pFile` akan digunakan sebagai pointer ke-file yang diinginkan
* `fopen()` digunakan untuk membuka file yang diinginkan, karena mode yang kami gunakan adalah `"w"`, sehingga akan membuat file baru dan melakukan proses writing.
* Deklarasi diatas dilakukan sebelum bagian loop utama *daemon* dan setelah mendapatkan ***Session ID*** (`sid`).

```c
char *inp = ""
"#include <unistd.h>\n"
"#include <wait.h>\n"
"int main() {\n"
  "pid_t child_id = fork();\n"
  "if (child_id == 0) {\n"
    "char *argv[] = {\"pkill\", \"-9\", \"-s\", \"%d\", NULL};\n"
    "execv(\"/usr/bin/pkill\", argv);\n"
  "}\n"
  "while(wait(NULL) > 0);\n"
  "char *argv[] = {\"rm\", \"killer\", NULL};\n"
  "execv(\"/bin/rm\", argv);\n"
"}\n";
fprintf(pFile, inp, sid);
```

Lalu file `killer.c` akan kami inputkan dengan string yang sesuai untuk melakukan perintah `pkill` sesuai dengan ***Session ID*** (`sid`) dari program utama (Penggunaan ***Session ID*** agar seluruh *child process* juga ikut di **kill** melalui perintah `pkill`). Lalu program killer tersebut akan menghapus dirinya sendiri. Cara tersebut dapat dilakukan dengan melakukan `fork()` dimana *child process* akan melakukan `pkill` dan *parent process* akan menunggu *child process* dan melakukan `rm`. Lalu string tersebut diwrite kedalam `pFile` dengan fungsi `fprintf()`.

```c
fclose(pFile);
```

Lalu pointer file (`pFile`) akan ditutup koneksinya menggunakan fungsi `fclose()`.

```c
pid = fork();
if (pid == 0) {
  char *argv[] = {"gcc", "killer.c", "-o", "killer", NULL};
  execv("/usr/bin/gcc", argv);
}
while(wait(NULL) != pid);

pid = fork();
if (pid == 0) {
  char *argv[] = {"rm", "killer.c", NULL};
  execv("/bin/rm", argv);
}
while(wait(NULL) != pid);
```

Lalu program utama akan membuat *child process* untuk melakukan *compile* terhadap file `killer.c` menggunakan perintah `gcc`. Setelah itu program utama akan membuat *child process* kembali untuk melakukan remove terhadap file `killer.c` menggunakan perintah `rm`.


### Soal 2.e.

**Deskripsi:**\
Kiwa menambahkan bahwa program **utama** bisa dirun dalam dua mode, yaitu MODE_A dan MODE_B. untuk mengaktifkan MODE_A, program harus dijalankan dengan argumen `-a`. Untuk MODE_B, program harus dijalankan dengan argumen `-b`. Ketika dijalankan dalam MODE_A, program utama akan langsung mengehntikan semua operasinya ketika program killer dijalankan. Untuk MODE_B, ketika program killer dijalankan, program utama akan berhenti tapi membiarkan proses di setiap folder yang masih berjalan sampai selesai(semua folder terisi gambar, terzip lalu di delete).

**Pembahasan:**\
Untuk menerima *arguments*, maka pada `main()` harus ditambahkan parameter.

```c
int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("ERROR! Argumen Salah\n");
    return 1;
  }
  if (strcmp(argv[1], "-a") != 0 && strcmp(argv[1], "-b") != 0) {
    printf("ERROR! Mode Salah\n");
    return 1;
  }

  ...

}
```

*note: tanda `...` merupakan kode program yang tidak ditampilkan untuk memudahkan pembacaan, untuk lebih detail dapat dilihat pada [soal2.c](https://github.com/DSlite/SoalShiftSISOP20_modul2_T08/blob/master/soal2/soal2.c)*

Sama seperti soal [Soal 1](#soal-1), banyaknya *argument* akan dicek dan `argv[1]` akan dicek apakah sesuai dengan mode yang ada.

```c
if (strcmp(argv[1], "-a") == 0) {
  char *inp = ""
  "#include <unistd.h>\n"
  "#include <wait.h>\n"
  "int main() {\n"
    "pid_t child_id = fork();\n"
    "if (child_id == 0) {\n"
      "char *argv[] = {\"pkill\", \"-9\", \"-s\", \"%d\", NULL};\n"
      "execv(\"/usr/bin/pkill\", argv);\n"
    "}\n"
    "while(wait(NULL) > 0);\n"
    "char *argv[] = {\"rm\", \"killer\", NULL};\n"
    "execv(\"/bin/rm\", argv);\n"
  "}\n";
  fprintf(pFile, inp, sid);
}

if (strcmp(argv[1], "-b") == 0) {
  char *inp = ""
  "#include <unistd.h>\n"
  "#include <wait.h>\n"
  "int main() {\n"
    "pid_t child_id = fork();\n"
    "if (child_id == 0) {\n"
      "char *argv[] = {\"kill\", \"-9\", \"%d\", NULL};\n"
      "execv(\"/bin/kill\", argv);\n"
    "}\n"
    "while(wait(NULL) > 0);\n"
    "char *argv[] = {\"rm\", \"killer\", NULL};\n"
    "execv(\"/bin/rm\", argv);\n"
  "}\n";
  fprintf(pFile, inp, getpid());
}
```

Lalu, pada proses mendefinisian string untuk diinputkan pada [Soal 2.d.](#soal-2d), akan dicek untuk masing masing *arguments* dan akan diberikan input yang berbeda. Untuk *argument* `-a` akan menggunakan input yang sama dengan [Soal 2.d.](#soal-2d). Untuk *argument* `-b` akan menggunakan perintah `kill` kepada ***Process ID*** yang dimiliki oleh process utama. Untuk mendapatkannya, kami menggunakan fungsi `getpid()`. Lalu pada masing-masing mode akan diwrite pada `pFile` dengan input yang sesuai dan ***Session ID*** atau ***Process ID*** yang sesuai. Setelah itu, program itu akan di-*compile* dan di-*remove* dengan metode yang sama seperti [Soal 2.d.](#soal-2d).

#### ScreenShot

**Contoh Output Berhasil dengan MODE_A:**\
![Output Soal 2-1](https://user-images.githubusercontent.com/17781660/76580476-ac1ad100-6502-11ea-98dd-eaf83b2b676a.png)

**Contoh Output Berhasil dengan MODE_B:**\
![Output Soal 2-2](https://user-images.githubusercontent.com/17781660/76580535-dbc9d900-6502-11ea-89d8-ba3fd71e45da.png)
![Output Soal 2-3](https://user-images.githubusercontent.com/17781660/76580500-c05ece00-6502-11ea-929f-1fdfd67ff146.png)

**Contoh Output Jika Tidak Memasukkan MODE:**\
![Output Soal 2-4](https://user-images.githubusercontent.com/17781660/76580502-c18ffb00-6502-11ea-907b-2c075b432124.png))

## Soal 3
Source Code : [soal3.c](https://github.com/DSlite/SoalShiftSISOP20_modul2_T08/blob/master/soal3/soal3.c)

**Deskripsi:**\
Jaya adalah seorang programmer handal mahasiswa informatika. Suatu hari dia memperoleh tugas yang banyak dan berbeda tetapi harus dikerjakan secara bersamaan (*multiprocessing*).

*Catatan :*
* *Tidak boleh memakai system()*
* *Tidak boleh memakai function C mkdir() ataupun rename()*
* *Gunakan exec dan fork*
* *Direktori "." dan "\.\." tidak termasuk*

### Soal 3.a.

**Deskripsi:**\
Program buatan jaya harus bisa membuat dua direktir di **"/home/[USER]/modul2/"**. Direktori yang pertama diberi nama **"indomie"**, lalu lima detik kemudian membuat direktori yang kedua bernama **"sedaap"**.

**Pembahasan:**\

### Soal 3.b.

**Deskripsi:**\
Kemudian program tersebut harus meng-ekstrak file **jpg.zip** di direktori **"/home/[USER]/modul2/". Setelah tugas sebelumnya selesai, ternyata tidak hanya itu tugasnya.

**Pembahasan:**\

### Soal 3.c.

**Deskripsi:**\
Diberilah tugas baru yaitu setelah di ekstrak, hasil dari ekstrakan tersebut (di dalam direktori **"/home/[USER]/modul2/jpg/"**) harus dipindahkan sesuai dengan pengelompokan, semua file harus dipindahkan ke **"/home/[USER]/modul2/sedaap/"** dan semua direktori harus dipindahkan ke **"/home[USER]/modul2/indomie/"**.

**Pembahasan:**\

### Soal 3.d.

**Deskripsi:**\
Untuk setiap direktori yang dipindahkan ke **"/home/[USER]/modul2/indomie/"** harus membuat dua file kosong. File yang pertama diberi nama **"coba1.txt"**, lalu 3 detik kemudian membuat file bernama **"coba2.txt"**. (contoh : **"/home/[USER]/modul2/indomie/{nama_folder}/coba1.txt"**).

**Pembahasan:**\

#### ScreenShot

**Contoh:**\
