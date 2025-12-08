#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <windows.h>
#include <algorithm>
#include <mysql.h>

using namespace std;

// Untuk koneksi ke database mysql
MYSQL *KoneksiDB()
{
    MYSQL *conn;
    conn = mysql_init(0);
    //
    conn = mysql_real_connect(conn, "localhost", "root", "", "takel4", 3306, NULL, 0);
    return conn;
}

void Insertdata(MYSQL *conn, string noPlat, string jenis, time_t waktuMasuk, int slotParkir)
{
    string query = "INSERT INTO main (no_plat, jenis, waktu_masuk) VALUES ('" + noPlat + "', '" + jenis + "', FROM_UNIXTIME(" + to_string(waktuMasuk) + "))";
    mysql_query(conn, query.c_str());
}
// Struktur data untuk kendaraan
struct Kendaraan
{
    string platNomor;
    string jenisKendaraan;
    time_t waktuMasuk;
    int jamMasuk, menitMasuk, detikMasuk;
    int id;
};

// Struktur data untuk transaksi (history)
struct Transaksi
{
    string platNomor;
    string jenisKendaraan;
    string waktuMasuk;
    string waktuKeluar;
    string durasi;
    int biaya;
};

// Class untuk sistem parkir
class SistemParkir
{
private:
    vector<Kendaraan> daftarKendaraan;
    vector<Transaksi> historyTransaksi; // Menyimpan semua transaksi
    string namaOperator;
    string nimOperator;
    const int TARIF_MOTOR = 2000;
    const int TARIF_MOBIL = 5000;
    int idCounter;

public:
    SistemParkir() : idCounter(1) {}

    // Fungsi untuk set console ke UTF-8
    void setupConsole()
    {
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
    }

    // Fungsi untuk clear screen
    void clearScreen()
    {
        system("cls");
    }

    // Fungsi untuk menampilkan garis
    void tampilkanGaris()
    {
        cout << "═══════════════════════════════════════════════════════════════════════════════════" << endl;
    }

    // Fungsi untuk menampilkan header dengan box
    void tampilkanHeader(string judul)
    {
        clearScreen();
        cout << "\n╔═══════════════════════════════════════════════════════════════════════════════╗\n";
        cout << "║ " << left << setw(77) << judul << " ║\n";
        cout << "╚═══════════════════════════════════════════════════════════════════════════════╝\n\n";
    }

    // Fungsi untuk mengubah string ke lowercase
    string toLower(string str)
    {
        transform(str.begin(), str.end(), str.begin(), ::tolower);
        return str;
    }

    // Fungsi login
    void login()
    {
        clearScreen();
        cout << "\n╔═══════════════════════════════════════════════════════════════════════════════╗\n";
        cout << "║                     SELAMAT DATANG DI SISTEM PARKIR                           ║\n";
        cout << "║                        DI SUSUN OLEH KELOMPOK 4                               ║\n";
        cout << "║                            NAMA ANGGOTA :                                     ║\n";
        cout << "║                       IMAM AMRIZAL NIM 250935730051                           ║\n";
        cout << "║                       IMAM AMRIZAL NIM 250935730051                           ║\n";
        cout << "║                       IMAM AMRIZAL NIM 250935730051                           ║\n";
        cout << "║                       IMAM AMRIZAL NIM 250935730051                           ║\n";
        cout << "╚═══════════════════════════════════════════════════════════════════════════════╝\n\n";

        cout << "Masukkan Nama Anda: ";
        getline(cin, namaOperator);
        cout << "Masukkan NIM: ";
        getline(cin, nimOperator);
        cout << "\nLogin berhasil! Tekan Enter untuk melanjutkan...";
        cin.get();
    }

    // Fungsi untuk format durasi waktu
    string formatDurasi(int detik)
    {
        int jam = detik / 3600;
        int menit = (detik % 3600) / 60;
        int sisa = detik % 60;

        char buffer[20];
        sprintf(buffer, "%02d:%02d:%02d", jam, menit, sisa);
        return string(buffer);
    }

    // Fungsi untuk menghitung durasi dalam detik
    int hitungDurasiDetik(int jamMasuk, int menitMasuk, int detikMasuk,
                          int jamKeluar, int menitKeluar, int detikKeluar)
    {
        int totalDetikMasuk = jamMasuk * 3600 + menitMasuk * 60 + detikMasuk;
        int totalDetikKeluar = jamKeluar * 3600 + menitKeluar * 60 + detikKeluar;

        int durasi = totalDetikKeluar - totalDetikMasuk;

        // Jika waktu keluar lebih kecil (lewat tengah malam), tambah 24 jam
        if (durasi < 0)
        {
            durasi += 86400; // 24 jam dalam detik
        }

        return durasi;
    }

    // Fungsi untuk menghitung biaya parkir berdasarkan jenis kendaraan
    int hitungBiaya(int detik, string jenisKendaraan)
    {
        int jam = (detik + 3599) / 3600; // Pembulatan ke atas

        // Cek jenis kendaraan (case insensitive)
        string jenis = toLower(jenisKendaraan);

        if (jenis == "motor")
        {
            return jam * TARIF_MOTOR;
        }
        else if (jenis == "mobil")
        {
            return jam * TARIF_MOBIL;
        }
        else
        {
            // Default ke motor jika tidak dikenali
            return jam * TARIF_MOTOR;
        }
    }

    // Fungsi untuk mendapatkan tarif sesuai jenis kendaraan
    int getTarif(string jenisKendaraan)
    {
        string jenis = toLower(jenisKendaraan);
        if (jenis == "motor")
        {
            return TARIF_MOTOR;
        }
        else if (jenis == "mobil")
        {
            return TARIF_MOBIL;
        }
        return TARIF_MOTOR;
    }

    // Menu utama
    void tampilkanMenu()
    {
        clearScreen();
        cout << "\n╔═══════════════════════════════════════════════════════════════════════════════╗\n";
        cout << "║                  SISTEM PARKIR KELOMPOK 4 - MENU UTAMA                        ║\n";
        cout << "╚═══════════════════════════════════════════════════════════════════════════════╝\n\n";

        cout << "Operator: " << namaOperator << " | NIM: " << nimOperator << "\n";
        cout << "Tarif Motor: Rp " << TARIF_MOTOR << "/jam | Tarif Mobil: Rp " << TARIF_MOBIL << "/jam\n";
        cout << "Kendaraan Parkir: " << daftarKendaraan.size() << " | Total Transaksi: " << historyTransaksi.size() << "\n\n";

        tampilkanGaris();
        cout << "1. Masuk Parkir\n";
        cout << "2. Keluar Parkir\n";
        cout << "3. Lihat Status Parkir\n";
        cout << "4. Lihat Laporan Pendapatan\n";
        cout << "5. Lihat Daftar Kendaraan\n";
        cout << "6. Keluar Program\n";
        tampilkanGaris();
        cout << "Pilih menu (1-6): ";
    }

    // Fungsi masuk parkir dengan input waktu manual
    void masukParkir()
    {
        tampilkanHeader("MASUK PARKIR");

        Kendaraan kendaraan;
        kendaraan.id = idCounter++;

        cout << "Masukkan Plat Nomor: ";
        getline(cin, kendaraan.platNomor);

        cout << "Jenis Kendaraan (Motor/Mobil): ";
        getline(cin, kendaraan.jenisKendaraan);

        // Validasi jenis kendaraan
        string jenis = toLower(kendaraan.jenisKendaraan);
        if (jenis != "motor" && jenis != "mobil")
        {
            cout << "\n✗ Jenis kendaraan tidak valid! Harus 'Motor' atau 'Mobil'\n";
            cout << "Tekan Enter untuk kembali ke menu...";
            cin.get();
            return;
        }

        cout << "\n--- INPUT WAKTU MASUK ---\n";
        cout << "Masukkan Jam (0-23): ";
        cin >> kendaraan.jamMasuk;

        cout << "Masukkan Menit (0-59): ";
        cin >> kendaraan.menitMasuk;

        cout << "Masukkan Detik (0-59): ";
        cin >> kendaraan.detikMasuk;
        cin.ignore();

        // Validasi input
        if (kendaraan.jamMasuk < 0 || kendaraan.jamMasuk > 23 ||
            kendaraan.menitMasuk < 0 || kendaraan.menitMasuk > 59 ||
            kendaraan.detikMasuk < 0 || kendaraan.detikMasuk > 59)
        {
            cout << "\n✗ Waktu tidak valid!\n";
            cout << "Tekan Enter untuk kembali ke menu...";
            cin.get();
            return;
        }

        kendaraan.waktuMasuk = time(0);
        daftarKendaraan.push_back(kendaraan);

        char buffer[20];
        sprintf(buffer, "%02d:%02d:%02d", kendaraan.jamMasuk, kendaraan.menitMasuk, kendaraan.detikMasuk);

        cout << "\n✓ Kendaraan " << kendaraan.platNomor << " berhasil masuk parkir!\n";
        cout << "Jenis: " << kendaraan.jenisKendaraan << "\n";
        cout << "Waktu Masuk: " << buffer << "\n";
        cout << "Tarif: Rp " << getTarif(kendaraan.jenisKendaraan) << "/jam\n";

        cout << "\nTekan Enter untuk kembali ke menu...";
        cin.get();
    }

    // Fungsi keluar parkir dengan input waktu manual
    void keluarParkir()
    {
        tampilkanHeader("KELUAR PARKIR");

        if (daftarKendaraan.empty())
        {
            cout << "Tidak ada kendaraan di area parkir!\n";
            cout << "\nTekan Enter untuk kembali ke menu...";
            cin.get();
            return;
        }

        string platNomor;
        cout << "Masukkan Plat Nomor: ";
        getline(cin, platNomor);

        for (int i = 0; i < daftarKendaraan.size(); i++)
        {
            if (daftarKendaraan[i].platNomor == platNomor)
            {
                int jamKeluar, menitKeluar, detikKeluar;

                cout << "\n--- INPUT WAKTU KELUAR ---\n";
                cout << "Masukkan Jam (0-23): ";
                cin >> jamKeluar;

                cout << "Masukkan Menit (0-59): ";
                cin >> menitKeluar;

                cout << "Masukkan Detik (0-59): ";
                cin >> detikKeluar;
                cin.ignore();

                // Validasi input
                if (jamKeluar < 0 || jamKeluar > 23 ||
                    menitKeluar < 0 || menitKeluar > 59 ||
                    detikKeluar < 0 || detikKeluar > 59)
                {
                    cout << "\n✗ Waktu tidak valid!\n";
                    cout << "Tekan Enter untuk kembali ke menu...";
                    cin.get();
                    return;
                }

                int durasi = hitungDurasiDetik(
                    daftarKendaraan[i].jamMasuk,
                    daftarKendaraan[i].menitMasuk,
                    daftarKendaraan[i].detikMasuk,
                    jamKeluar, menitKeluar, detikKeluar);

                int biaya = hitungBiaya(durasi, daftarKendaraan[i].jenisKendaraan);
                int tarif = getTarif(daftarKendaraan[i].jenisKendaraan);

                char bufferMasuk[20], bufferKeluar[20];
                sprintf(bufferMasuk, "%02d:%02d:%02d",
                        daftarKendaraan[i].jamMasuk,
                        daftarKendaraan[i].menitMasuk,
                        daftarKendaraan[i].detikMasuk);
                sprintf(bufferKeluar, "%02d:%02d:%02d",
                        jamKeluar, menitKeluar, detikKeluar);

                cout << "\n";
                tampilkanGaris();
                cout << "                    STRUK PEMBAYARAN PARKIR\n";
                tampilkanGaris();
                cout << "Plat Nomor     : " << daftarKendaraan[i].platNomor << "\n";
                cout << "Jenis Kendaraan: " << daftarKendaraan[i].jenisKendaraan << "\n";
                cout << "Waktu Masuk    : " << bufferMasuk << "\n";
                cout << "Waktu Keluar   : " << bufferKeluar << "\n";
                cout << "Durasi Parkir  : " << formatDurasi(durasi) << "\n";
                cout << "Tarif          : Rp " << tarif << "/jam\n";
                cout << "───────────────────────────────────────────────────────────────────────────────\n";
                cout << "TOTAL BIAYA    : Rp " << biaya << "\n";
                tampilkanGaris();

                cout << "\nProses pembayaran (y/n)? ";
                char konfirmasi;
                cin >> konfirmasi;
                cin.ignore();

                if (konfirmasi == 'y' || konfirmasi == 'Y')
                {
                    // Simpan ke history transaksi
                    Transaksi trans;
                    trans.platNomor = daftarKendaraan[i].platNomor;
                    trans.jenisKendaraan = daftarKendaraan[i].jenisKendaraan;
                    trans.waktuMasuk = string(bufferMasuk);
                    trans.waktuKeluar = string(bufferKeluar);
                    trans.durasi = formatDurasi(durasi);
                    trans.biaya = biaya;
                    historyTransaksi.push_back(trans);

                    // Hapus dari daftar kendaraan aktif
                    daftarKendaraan.erase(daftarKendaraan.begin() + i);
                    cout << "\n✓ Pembayaran berhasil! Terima kasih.\n";
                    cout << "✓ Transaksi telah dicatat dalam laporan pendapatan.\n";
                }
                else
                {
                    cout << "\n✗ Pembayaran dibatalkan.\n";
                }

                cout << "\nTekan Enter untuk kembali ke menu...";
                cin.get();
                return;
            }
        }

        cout << "\n✗ Kendaraan dengan plat nomor " << platNomor << " tidak ditemukan!\n";
        cout << "\nTekan Enter untuk kembali ke menu...";
        cin.get();
    }

    // Fungsi lihat status parkir
    void lihatStatus()
    {
        tampilkanHeader("STATUS PARKIR");

        if (daftarKendaraan.empty())
        {
            cout << "Tidak ada kendaraan di area parkir.\n";
        }
        else
        {
            for (int i = 0; i < daftarKendaraan.size(); i++)
            {
                char buffer[20];
                sprintf(buffer, "%02d:%02d:%02d",
                        daftarKendaraan[i].jamMasuk,
                        daftarKendaraan[i].menitMasuk,
                        daftarKendaraan[i].detikMasuk);

                int tarif = getTarif(daftarKendaraan[i].jenisKendaraan);

                cout << "╔═══════════════════════════════════════════════════════════════════════════╗\n";
                cout << "║ Plat Nomor  : " << left << setw(60) << daftarKendaraan[i].platNomor << "║\n";
                cout << "║ Jenis       : " << left << setw(60) << daftarKendaraan[i].jenisKendaraan << "║\n";
                cout << "║ Waktu Masuk : " << left << setw(60) << buffer << "║\n";
                cout << "║ Tarif       : Rp " << left << setw(57) << (to_string(tarif) + "/jam") << "║\n";
                cout << "╚═══════════════════════════════════════════════════════════════════════════╝\n\n";
            }
        }

        cout << "\nTekan Enter untuk kembali ke menu...";
        cin.get();
    }

    // Fungsi lihat laporan pendapatan (DENGAN REKAP)
    void lihatPendapatan()
    {
        tampilkanHeader("LAPORAN PENDAPATAN & REKAP TRANSAKSI");

        // Hitung statistik
        int totalPendapatan = 0;
        int jumlahMotor = 0, jumlahMobil = 0;
        int pendapatanMotor = 0, pendapatanMobil = 0;

        for (int i = 0; i < historyTransaksi.size(); i++)
        {
            totalPendapatan += historyTransaksi[i].biaya;

            string jenis = toLower(historyTransaksi[i].jenisKendaraan);
            if (jenis == "motor")
            {
                jumlahMotor++;
                pendapatanMotor += historyTransaksi[i].biaya;
            }
            else if (jenis == "mobil")
            {
                jumlahMobil++;
                pendapatanMobil += historyTransaksi[i].biaya;
            }
        }

        // Tampilkan ringkasan
        cout << "╔════════════════════════════════════════════════════════════════════════════╗\n";
        cout << "║                          RINGKASAN PENDAPATAN                              ║\n";
        cout << "╚════════════════════════════════════════════════════════════════════════════╝\n\n";

        cout << "Total Transaksi Selesai : " << historyTransaksi.size() << " transaksi\n";
        cout << "  - Motor               : " << jumlahMotor << " transaksi\n";
        cout << "  - Mobil               : " << jumlahMobil << " transaksi\n\n";

        cout << "Total Pendapatan        : Rp " << totalPendapatan << "\n";
        cout << "  - Dari Motor          : Rp " << pendapatanMotor << "\n";
        cout << "  - Dari Mobil          : Rp " << pendapatanMobil << "\n\n";

        // Tampilkan kendaraan yang masih parkir
        int jumlahMotorAktif = 0, jumlahMobilAktif = 0;
        for (int i = 0; i < daftarKendaraan.size(); i++)
        {
            string jenis = toLower(daftarKendaraan[i].jenisKendaraan);
            if (jenis == "motor")
                jumlahMotorAktif++;
            else if (jenis == "mobil")
                jumlahMobilAktif++;
        }

        cout << "Kendaraan Aktif (Masih Parkir):\n";
        cout << "  - Motor               : " << jumlahMotorAktif << " kendaraan\n";
        cout << "  - Mobil               : " << jumlahMobilAktif << " kendaraan\n";
        cout << "  - Total               : " << daftarKendaraan.size() << " kendaraan\n\n";

        tampilkanGaris();

        // Tampilkan detail transaksi
        if (historyTransaksi.size() > 0)
        {
            cout << "\n╔════════════════════════════════════════════════════════════════════════════╗\n";
            cout << "║                       DETAIL HISTORY TRANSAKSI                             ║\n";
            cout << "╚════════════════════════════════════════════════════════════════════════════╝\n\n";

            cout << left << setw(5) << "No"
                 << setw(13) << "Plat"
                 << setw(10) << "Jenis"
                 << setw(11) << "Masuk"
                 << setw(11) << "Keluar"
                 << setw(11) << "Durasi"
                 << setw(12) << "Biaya" << "\n";
            tampilkanGaris();

            for (int i = 0; i < historyTransaksi.size(); i++)
            {
                cout << left << setw(5) << (i + 1)
                     << setw(13) << historyTransaksi[i].platNomor
                     << setw(10) << historyTransaksi[i].jenisKendaraan
                     << setw(11) << historyTransaksi[i].waktuMasuk
                     << setw(11) << historyTransaksi[i].waktuKeluar
                     << setw(11) << historyTransaksi[i].durasi
                     << "Rp " << historyTransaksi[i].biaya << "\n";
            }

            tampilkanGaris();
        }
        else
        {
            cout << "\n[!] Belum ada transaksi yang tercatat.\n";
        }

        cout << "\nTekan Enter untuk kembali ke menu...";
        cin.get();
    }

    // Fungsi lihat daftar kendaraan
    void lihatDaftarKendaraan()
    {
        tampilkanHeader("DAFTAR KENDARAAN");

        if (daftarKendaraan.empty())
        {
            cout << "Tidak ada kendaraan di area parkir.\n";
        }
        else
        {
            cout << left << setw(5) << "No"
                 << setw(15) << "Plat Nomor"
                 << setw(12) << "Jenis"
                 << setw(15) << "Waktu Masuk"
                 << setw(15) << "Tarif/jam" << "\n";
            tampilkanGaris();

            for (int i = 0; i < daftarKendaraan.size(); i++)
            {
                char buffer[20];
                sprintf(buffer, "%02d:%02d:%02d",
                        daftarKendaraan[i].jamMasuk,
                        daftarKendaraan[i].menitMasuk,
                        daftarKendaraan[i].detikMasuk);

                int tarif = getTarif(daftarKendaraan[i].jenisKendaraan);

                cout << left << setw(5) << (i + 1)
                     << setw(15) << daftarKendaraan[i].platNomor
                     << setw(12) << daftarKendaraan[i].jenisKendaraan
                     << setw(15) << buffer
                     << "Rp " << tarif << "\n";
            }
        }

        cout << "\nTekan Enter untuk kembali ke menu...";
        cin.get();
    }

    // Fungsi utama untuk menjalankan sistem
    void jalankan()
    {
        setupConsole();
        login();

        int pilihan;
        do
        {
            tampilkanMenu();
            cin >> pilihan;
            cin.ignore();

            switch (pilihan)
            {
            case 1:
                masukParkir();
                break;
            case 2:
                keluarParkir();
                break;
            case 3:
                lihatStatus();
                break;
            case 4:
                lihatPendapatan();
                break;
            case 5:
                lihatDaftarKendaraan();
                break;
            case 6:
                clearScreen();
                cout << "\n╔═══════════════════════════════════════════════════════════════════════════╗\n";
                cout << "║        Terima kasih telah menggunakan Sistem Parkir Kelompok 4!           ║\n";
                cout << "╚═══════════════════════════════════════════════════════════════════════════╝\n\n";
                break;
            default:
                cout << "\n✗ Pilihan tidak valid!\n";
                cout << "Tekan Enter untuk melanjutkan...";
                cin.get();
            }
        } while (pilihan != 6);
    }
};

// Fungsi main
int main()
{
    SistemParkir sistem;
    sistem.jalankan();
    return 0;
}
