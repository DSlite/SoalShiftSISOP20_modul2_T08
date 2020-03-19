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

**Asumsi Soal:**\
Soal meminta kami untuk membuat program C yang bekerja menyerupai crontab. Dimana prosesnya akan berupa daemon, dan menjalankan file bash yang diinputkan setiap waktu yang diinginkan yang dimasukkan ke dalam *arguments*. Untuk melakukan pengecekan terhadap waktu, kelompok kami menjalankan crontab setiap detik, dimana tiap detik akan dicek apakah waktu **saat ini** sesuai dengan *arguments* yang diinputkan. Jika sesuai maka, file bash yang diinputkan akan dijalankan.

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

#### Kesulitan
Tidak ada.

#### ScreenShot
**Contoh Output Berhasil:**\
![Output Soal 1-1](https://user-images.githubusercontent.com/17781660/76401535-9c8d7200-63b4-11ea-96ba-04edb884761e.png)
*Screenshot* diatas merupakan contoh dari eksekusi program C dengan argumen yang sesuai. Program C tersebut akan menjalankan file `/home/umum/test.sh` setiap detik pada jam 16 menit 21. Jika dilihat akan muncul directory 16:21:00 - 16:21:59, karena shell script yang dijalankan untuk melakukan `mkdir` dengan `date` saat ini. Jika dilihat pada `ps`, program `soal1` masih berjalan dan menunggu sampai jam 16 dan menit 21 berikutnya.

**Isi File /home/umum/test.sh :**\
![Output Soal 1-2](https://user-images.githubusercontent.com/17781660/76400474-f8ef9200-63b2-11ea-8cc9-bd2f1141b83e.png)

**Contoh Output Jika Arguments Salah:**\
![Output Soal 1-3](https://user-images.githubusercontent.com/17781660/76400520-0c026200-63b3-11ea-824a-725c0cb27696.png)
Jika argument jam, menit, atau detik yang diinputkan tidak sesuai, maka program akan nge-*printf* Error dan menghentikan prosesnya.

**Contoh Output Jika File Shell Script Tidak Ada:**\
![Output Soal 1-4](https://user-images.githubusercontent.com/17781660/76400554-17ee2400-63b3-11ea-9505-9270a6715594.png)
Jika file yang diinputkan pada *argument* tidak ada, maka program akan nge-*printf* Error dan menghentikan prosesnya.

---

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

**Asumsi Soal:**\
Kami berasumsi bahwa soal meminta untuk membuat Program C berupa daemon, dimana daemon tersebut akan berjalan setiap 30 detik dan akan melakukan `fork()` untuk melakukan `mkdir` dengan timestamp yang sesuai.

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

**Asumsi Soal:**\
Dari masing-masing folder yang telah dibuat, program akan melakukan `fork()` lagi dimana *child proces* akan melakukan `loop` sebanyak 20 kali. Dimana masing-masing loop akan melakukan `fork()` kembali untuk mendownload file di *https://picsum.photos/*. Agar ukuran sesuai dengan yang diminta, ukuran dapat diinputkan pada url tersebut (e.g. *https://picsum.photos/100* untuk mendownload file dengan ukuran 100x100 pixel). Lalu `loop` tersebut akan melakukan `sleep` selama 5 detik.

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

**Asumsi Soal:**\
Proses ini dilakukan setelah loop untuk mendownload 20 gambar tadi telah selesai. Program akan melakukan `fork()` dan akan melakukan `execv` perintah `zip`.

**Pembahasan:**\
Untuk melakukan zip, maka setelah `for` loop pada soal sebelumnya selesai, *parent process* akan menunggu sampai seluruh ***download*** `wget` selesai. Lalu akan melakukan `fork()` lagi. Dimana *child process* akan melakukan zip terhadap folder tersebut. dan *parent process* akan menunggu *child process* selesai dan menghapus directory folder yang telah di zip.

```c
while(wait(NULL) > 0);
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

**Asumsi Soal:**\
Untuk men-generate program "`killer`", kami membuat file bernama `killer.c` dan menginputkannya dengan program c. Dimana program C tersebut akan melakukan `pkill` terhadap Session ID (**`SID`**) yang telah diset ketika melakukan daemon, dan melakukan `rm` terhadap file `killer`. Lalu Program C tersebut akan dicompile dengan `gcc` dan file `killer.c` akan di `rm`.


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
Kiwa menambahkan bahwa program **utama** bisa dirun dalam dua mode, yaitu MODE_A dan MODE_B. untuk mengaktifkan MODE_A, program harus dijalankan dengan argumen `-a`. Untuk MODE_B, program harus dijalankan dengan argumen `-b`. Ketika dijalankan dalam MODE_A, program utama akan langsung mengehentikan semua operasinya ketika program killer dijalankan. Untuk MODE_B, ketika program killer dijalankan, program utama akan berhenti tapi membiarkan proses di setiap folder yang masih berjalan sampai selesai(semua folder terisi gambar, terzip lalu di delete).

**Asumsi Soal:**\
Untuk membuat mode, maka *arguments*-nya akan dicek terlebih dahulu dan harus sesuai. Prosesnya mirip dengan soal 2.d. Pertama akan membuat file `killer.c`, lalu jika *argument* yang diinputkan merupakan MODE_A, maka kodingan C yang diinputkan sama pada soal 2.d. dan jika *argument* yang diinputkan merupakan MODE_B, maka kodingan C yang diinputkan akan melakukan `kill` terhadap proses utamanya saja. Untuk mendapatkan Process ID (**`PID`**) dari proses utama dapat dengan fungsi `getpid()`, lalu program akan dicompile kembali menggunakan `gcc` dan file `killer.c` akan dihapus

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

#### Kesulitan
Tidak ada.

#### ScreenShot

**Contoh Output Berhasil dengan MODE_A:**\
![Output Soal 2-1](https://user-images.githubusercontent.com/17781660/76580476-ac1ad100-6502-11ea-98dd-eaf83b2b676a.png)
Program akan dijalankan dengan MODE_A (`-a`). Setelah program dijalankan, maka akan muncul file `killer` yang dapat langsung dieksekusi. Ketika dilihat directory yang muncul, didalamnya ada 2 buah file yang telah didownload, dan akan lanjut sampai 20 file. Ketika dilihat proses yang sedang berjalan, maka akan terlihat soal2, dan beberapa *child process* masih berjalan. Ketika `killer` dijalankan, maka proses-proses tersebut akan langsung dibunuh sesuai dengan permintaan soal dengan MODE_A.

**Contoh Output Berhasil dengan MODE_B:**\
![Output Soal 2-2](https://user-images.githubusercontent.com/17781660/76580535-dbc9d900-6502-11ea-89d8-ba3fd71e45da.png)
Program akan dijalankan dengan MODE_B (`-b`). Setelah program dijalankan maka akan muncul file `killer` dan sama seperti MODE_A.
![Output Soal 2-3](https://user-images.githubusercontent.com/17781660/76580500-c05ece00-6502-11ea-929f-1fdfd67ff146.png)
Ketika `killer` dijalankan, maka proses yang dibunuh hanya process utamanya saja sehingga *child process* akan menjadi *orphan process* yang dimana akan dikill oleh `init` ketika processnya telah selesai (tidak menjadi *zombie process*). Process wget, sampai zip directory akan terus berjalan. Ketika seluruh folder telah di-zip, maka process akan selesai dan dibunuh.

**Contoh Output Jika Tidak Memasukkan MODE:**\
![Output Soal 2-4](https://user-images.githubusercontent.com/17781660/76580502-c18ffb00-6502-11ea-907b-2c075b432124.png))
Jika tidak memasukkan mode, maka akan muncul Error.

---

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
Program buatan jaya harus bisa membuat dua direktori di **"/home/[USER]/modul2/"**. Direktori yang pertama diberi nama **"indomie"**, lalu lima detik kemudian membuat direktori yang kedua bernama **"sedaap"**.

**Asumsi Soal:**\
Program ini tidak dijadikan daemon, pertama melakukan `fork()` untuk melakukan `mkdir` directory `/home/umum/modul2/indomie` lalu `sleep()` selama 5 detik dan `fork()` kembali untuk melakukan `mkdir` directory `/home/umum/modul2/sedaap`.

**Pembahasan:**\
Pertama, kami akan melakukan `#include` library yang diperlukan

```c
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <wait.h>
#include <dirent.h>
```

Library yang digunakan untuk soal ini mirip dengan soal sebelumnya. Disini hanya ditambahkan `#include <dirent.h>` yang digunakan untuk membuka isi directory (e.g. `readdir()`).

```c
int main() {

  pid_t child_id = fork();
  if (child_id == 0) {
    char *argv[] = {"mkdir", "-p", "/home/umum/modul2/indomie", NULL};
    execv("/bin/mkdir", argv);
  }
  while(wait(NULL) != child_id);
  sleep(5)

  child_id = fork();
  if (child_id == 0) {
    char *argv[] = {"mkdir", "-p", "/home/umum/modul2/sedaap", NULL};
    execv("/bin/mkdir", argv);
  }
  while(wait(NULL) != child_id);

  ...

}
```

*note: tanda `...` merupakan kode program yang tidak ditampilkan untuk memudahkan pembacaan, untuk lebih detail dapat dilihat pada [soal2.c](https://github.com/DSlite/SoalShiftSISOP20_modul2_T08/blob/master/soal3/soal3.c)*

* *Parent process* akan membuat *child process* dimana dia akan membuat direktori pada `/home/umum/modul2/indomie` menggunakan perintah `mkdir`. Disini *argument* `-p` digunakan agar directory yang belum dibuat (seperti `/home/umum/modul2`) langsung terbuat.
* Lalu *parent process* akan menunggu mkdir selesai dibuat, lalu akan `sleep()` selama 5 detik. Lalu *parent process* akan membuat *child process* lagi untuk membuat direktoripada `/home/umum/modul2/sedaap`. Lalu *parent process* akan menunggu *child process* selesai.

### Soal 3.b.

**Deskripsi:**\
Kemudian program tersebut harus meng-ekstrak file **jpg.zip** di direktori **"/home/[USER]/modul2/"**. Setelah tugas sebelumnya selesai, ternyata tidak hanya itu tugasnya.

**Asumsi Soal:**\
Untuk mengekstrak, dapat dilakukan dengan perintah `unzip`. Maka program akan melakukan `fork()` untuk menjalankan `unzip` tersebut.

**Pembahasan:**\
Untuk meng-ekstrak file **jpg.zip** dapat menggunakan perintah `unzip`.

```c
child_id = fork();
if (child_id == 0) {
  char *argv[] = {"unzip", "-q", "jpg.zip", "-d", "/home/umum/modul2", NULL};
  execv("/usr/bin/unzip", argv);
}
while(wait(NULL) != child_id);
```

* *argument* `-q` digunakan agar process **unzip** tidak mengeluarkan output ke terminal.
* *argument* `-d` untuk menyatakan *directory* output hasil ekstrak.


### Soal 3.c.

**Deskripsi:**\
Diberilah tugas baru yaitu setelah di ekstrak, hasil dari ekstrakan tersebut (di dalam direktori **"/home/[USER]/modul2/jpg/"**) harus dipindahkan sesuai dengan pengelompokan, semua file harus dipindahkan ke **"/home/[USER]/modul2/sedaap/"** dan semua direktori harus dipindahkan ke **"/home[USER]/modul2/indomie/"**.

**Asumsi Soal:**\
Untuk bagian soal ini, seluruh *entry* yang terdapat pada `/home/umum/modul2/jpg/` di list terlebih dahulu. Lalu akan dicek apakah *entry* tersebut merupakan file atau directory, jika file akan dipindahkan ke `/home/umum/modul2/sedaap`, dan jika directory akan dipindahkan ke `/home/umum/modul2/indomie`.

**Pembahasan:**\
Pertama, kami melakukan *entry* terhadap isi directory `/home/umum/modul2/jpg` menggunakan library `<dirent.h>`.

```c
char f_name[256];
struct dirent *de;
DIR *dr = opendir("/home/umum/modul2/jpg");

while((de = readdir(dr)) != NULL) {
  if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) {
    continue;
  }
  sprintf(f_name+strlen(f_name), "%s\n", de->d_name);
}

closedir(dr);
```

* `f_name` akan menyimpan seluruh *entry* yang terdapat dalam folder `/home/umum/modul2/jpg`.
* `de` akan menyimpan *entry* ketika melakukan iterasi.
* `dr` akan sebagai pointer ke directory `/home/umum/modul2/jpg`.
* Lalu iterasi akan dilakukan dengan menggunakan fungsi `readdir()` terhadap directory `dr` dan *entry* diinputkan pada `de`.
* Jika nama *entry* tersebut adalah `"."` atau `".."` maka tidak akan dimasukkan kedalam `f_name`.
* Lalu untuk memasukkan nama *entry* tersebut, kami menggunakan fungsi `sprintf()` dengan menggunakan delimiter `\n`.
* Lalu directory tersebut akan ditutup koneksinya dengan menggunakan fungsi `closedir()`.

```c
char *token = strtok(f_name, "\n");

while (token != NULL) {

  ...

  token = strtok(NULL, "\n");

}
```

*note: tanda `...` merupakan kode program yang tidak ditampilkan untuk memudahkan pembacaan, untuk lebih detail dapat dilihat pada [soal2.c](https://github.com/DSlite/SoalShiftSISOP20_modul2_T08/blob/master/soal3/soal3.c)*

Lalu kami akan menggunakan fungsi `strtok()` untuk melakukan iterasi pada masing-masing nama file dengan menggunakan delimiter yang sudah di-*set* tadi (`\n`).

```c
char location[80];
struct stat location_stat;

sprintf(location, "/home/umum/modul2/jpg/%s", token);
stat(location, &location_stat);

if (S_ISDIR(location_stat.st_mode)) {

  child_id = fork();
  if (child_id == 0) {
    char *argv[] = {"mv", location, "/home/umum/modul2/indomie", NULL};
    execv("/bin/mv", argv);
  }
  while(wait(NULL) != child_id);

  ...

} else if (S_ISREG(location_stat.st_mode)) {

  child_id = fork();
  if (child_id == 0) {
    char *argv[] = {"mv", location, "/home/umum/modul2/sedaap", NULL};
    execv("/bin/mv", argv);
  }
  while(wait(NULL) != child_id);

}
```

*note: tanda `...` merupakan kode program yang tidak ditampilkan untuk memudahkan pembacaan, untuk lebih detail dapat dilihat pada [soal2.c](https://github.com/DSlite/SoalShiftSISOP20_modul2_T08/blob/master/soal3/soal3.c)*

Untuk masing-masing nama *entry*, akan dilakukan operasi sebagai berikut:

* deklarasi `location` untuk lokasi *entry* tersebut, dan `location_stat` yang akan digunakan untuk mengecek apakah dia merupakan file biasa atau sebuah directory.
* `sprintf()` digunakan untuk menginputkan lokasi kedalam variable `location`
* `stat()` digunakan untuk mendapatkan ***stat*** dari *entry* tersebut.
* Lalu *entry* tersebut akan dicek menggunakan **macros** `S_ISDIR()` untuk mengecek apakah *entry* tersebut directory dan `S_ISREG()` untuk mengecek apakah *entry* tersebut file biasa atau bukan.
* Lalu untuk masing masing *entry* tersebut akan dilakukan `mv` untuk memindahkan file / directory tersebut kedalam lokasi yang diminta.

### Soal 3.d.

**Deskripsi:**\
Untuk setiap direktori yang dipindahkan ke **"/home/[USER]/modul2/indomie/"** harus membuat dua file kosong. File yang pertama diberi nama **"coba1.txt"**, lalu 3 detik kemudian membuat file bernama **"coba2.txt"**. (contoh : **"/home/[USER]/modul2/indomie/{nama_folder}/coba1.txt"**).

**Asumsi Soal:**\
Pada soal 3.c. ketika direktori dipindahkan ke `/home/umum/modul2/indomie/`, process akan melakukan `fork()` dimana *child process* akan melakukan `fork()` lagi untuk membuat file `coba1.txt`, lalu `sleep()` selama 3 detik, dan membuat file `coba2.txt`.

**Pembahasan:**\
Proses ini dilakukan setelah *entry* bertipe directory dipindahkan menggunakan `mv`.

```c
char new_location[80];
child_id = fork();
if (child_id == 0) {
  sprintf(new_location, "/home/umum/modul2/indomie/%s/coba1.txt", token);
  char *argv[] = {"touch", new_location, NULL};
  execv("/usr/bin/touch", argv);
}

child_id = fork();
if (child_id == 0) {
  sleep(3);
  sprintf(new_location, "/home/umum/modul2/indomie/%s/coba2.txt", token);
  char *argv[] = {"touch", new_location, NULL};
  execv("/usr/bin/touch", argv);
}
```

Pertama, kami mendeklarasikan variable `new_location` untuk menyimpan alamat directory yang telah dipindahkan. Lalu akan melakukan `fork()` dimana *child process* akan membuat file `coba1.txt` pada `new_location` menggunakan perintah `touch`. Lalu *parent process* akan melakukan `fork()` lagi, tetapi pada *child process* akan melakukan `sleep` selama 3 detik terlebih dahulu, lalu akan membuat file `coba2.txt` pada `new_location`.

#### Kendala
Tidak ada.

#### ScreenShot

**Contoh Output:**\
![Output Soal 3-1](https://user-images.githubusercontent.com/17781660/76583819-d83b4f80-650c-11ea-95a1-5fea99d9da36.png)
Ketika program dijalankan, maka proses `mkdir`, `unzip`, `mv`, dan lain-lain akan langsung dijalankan secara berurutan. Sehingga ketika dilihat pada `/home/umum/modul2` akan langsung terdiri atas 3 directory `indomie`, `jpg`, dan `sedaap`. untuk directory `sedaap` akan terdiri atas file saja, dan directory `indomie` akan terdiri atas directory saja.
![Output Soal 3-2](https://user-images.githubusercontent.com/17781660/76583822-d96c7c80-650c-11ea-883e-94b13f7b0eee.png)
Sebagai contoh, pada directory `/home/umum/modul2/indomie/3577.jpg/` dan `/home/umum/modul2/indomie/3985.txt/` akan terdiri atas 2 file dengan nama `coba1.txt` dan `coba2.txt`.
