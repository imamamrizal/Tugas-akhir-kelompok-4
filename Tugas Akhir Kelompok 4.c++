#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>
#include <limits>

using namespace std;

// Struktur data untuk kendaraan
struct Kendaraan {
    string noPlat;
    string jenis; // Motor atau Mobil
    time_t waktuMasuk;
    int slotParkir;
    bool statusKeluar;
};

// Class Sistem Parkir
class SistemParkir {
private:
    vector<Kendaraan> daftarKendaraan;
    int slotMotor;
    int slotMobil;
    int maxSlotMotor;
    int maxSlotMobil;
    int tarifMotor; // per jam
    int tarifMobil; // per jam
    double totalPendapatan;

public:
    // Constructor
    SistemParkir(int maxMotor, int maxMobil, int hargaMotor, int hargaMobil) {
        maxSlotMotor = maxMotor;
        maxSlotMobil = maxMobil;
        slotMotor = 0;
        slotMobil = 0;
        tarifMotor = hargaMotor;
        tarifMobil = hargaMobil;
        totalPendapatan = 0;
    }

    // Fungsi untuk menampilkan menu
    void tampilkanMenu() {
        cout << "\n+----------------------------------------------------+\n";
        cout << "¦ SISTEM PARKIR KELOMPOK 4 - MENU UTAMA ¦\n";
        cout << "+----------------------------------------------------+\n";
        cout << "1. Masuk Parkir\n";
        cout << "2. Keluar Parkir\n";
        cout << "3. Lihat Status Parkir\n";
        cout << "4. Lihat Laporan Pendapatan\n";
        cout << "5. Lihat Daftar Kendaraan\n";
        cout << "6. Keluar Program\n";
        cout << "-------------------------------------------\n";
        cout << "Pilih menu (1-6): ";
    }

    // Fungsi masuk parkir
    void masukParkir() {
        if (slotMotor >= maxSlotMotor && slotMobil >= maxSlotMobil) {
            cout << "\n? Maaf, parkir penuh!\n";
            return;
        }

        Kendaraan kendaraan;
        int pilihan;

        cout << "\n---------------------------------------\n";
        cout << "         MASUK PARKIR\n";
        cout << "---------------------------------------\n";

        cout << "Jenis Kendaraan:\n";
        cout << "1. Motor (Tersisa: " << (maxSlotMotor - slotMotor) << " slot)\n";
        cout << "2. Mobil (Tersisa: " << (maxSlotMobil - slotMobil) << " slot)\n";
        cout << "Pilihan: ";
        cin >> pilihan;

        if (pilihan == 1) {
            if (slotMotor >= maxSlotMotor) {
                cout << "\n? Slot parkir motor penuh!\n";
                return;
            }
            kendaraan.jenis = "Motor";
            slotMotor++;
            kendaraan.slotParkir = slotMotor;
        } else if (pilihan == 2) {
            if (slotMobil >= maxSlotMobil) {
                cout << "\n? Slot parkir mobil penuh!\n";
                return;
            }
            kendaraan.jenis = "Mobil";
            slotMobil++;
            kendaraan.slotParkir = slotMobil;
        } else {
            cout << "\n? Pilihan tidak valid!\n";
            return;
        }

        cin.ignore();
        cout << "Nomor Plat: ";
        getline(cin, kendaraan.noPlat);

        kendaraan.waktuMasuk = time(0);
        kendaraan.statusKeluar = false;

        daftarKendaraan.push_back(kendaraan);

        cout << "\n? Kendaraan berhasil masuk parkir!\n";
        cout << "---------------------------------------\n";
        cout << "Nomor Plat  : " << kendaraan.noPlat << "\n";
        cout << "Jenis       : " << kendaraan.jenis << "\n";
        cout << "Slot        : " << kendaraan.slotParkir << "\n";
        cout << "Waktu Masuk : " << ctime(&kendaraan.waktuMasuk);
        cout << "---------------------------------------\n";
    }

    // Fungsi keluar parkir
    void keluarParkir() {
        string noPlat;
        cin.ignore();

        cout << "\n---------------------------------------\n";
        cout << "         KELUAR PARKIR\n";
        cout << "---------------------------------------\n";
        cout << "Nomor Plat: ";
        getline(cin, noPlat);

        bool ditemukan = false;
        for (size_t i = 0; i < daftarKendaraan.size(); i++) {
            if (daftarKendaraan[i].noPlat == noPlat && !daftarKendaraan[i].statusKeluar) {
                ditemukan = true;

                time_t waktuKeluar = time(0);
                double durasi = difftime(waktuKeluar, daftarKendaraan[i].waktuMasuk) / 3600.0;
                int jam = (int)durasi;
                if (durasi > jam) jam++; // Pembulatan ke atas
                if (jam == 0) jam = 1; // Minimal 1 jam

                int tarif = (daftarKendaraan[i].jenis == "Motor") ? tarifMotor : tarifMobil;
                double biaya = jam * tarif;

                cout << "\n---------------------------------------\n";
                cout << "Nomor Plat   : " << daftarKendaraan[i].noPlat << "\n";
                cout << "Jenis        : " << daftarKendaraan[i].jenis << "\n";
                cout << "Waktu Masuk  : " << ctime(&daftarKendaraan[i].waktuMasuk);
                cout << "Waktu Keluar : " << ctime(&waktuKeluar);
                cout << "Durasi       : " << jam << " jam\n";
                cout << "Tarif        : Rp " << tarif << "/jam\n";
                cout << "---------------------------------------\n";
                cout << "TOTAL BAYAR  : Rp " << fixed << setprecision(0) << biaya << "\n";
                cout << "---------------------------------------\n";

                daftarKendaraan[i].statusKeluar = true;
                totalPendapatan += biaya;

                if (daftarKendaraan[i].jenis == "Motor") {
                    slotMotor--;
                } else {
                    slotMobil--;
                }

                cout << "\n? Pembayaran berhasil! Terima kasih.\n";
                break;
            }
        }

        if (!ditemukan) {
            cout << "\n? Kendaraan dengan nomor plat tersebut tidak ditemukan!\n";
        }
    }

    // Fungsi lihat status parkir
    void lihatStatus() {
        cout << "\n+----------------------------------------+\n";
        cout << "¦          STATUS PARKIR SAAT INI        ¦\n";
        cout << "+----------------------------------------+\n";
        cout << "Motor:\n";
        cout << "  Terisi    : " << slotMotor << "/" << maxSlotMotor << " slot\n";
        cout << "  Tersedia  : " << (maxSlotMotor - slotMotor) << " slot\n";
        cout << "\nMobil:\n";
        cout << "  Terisi    : " << slotMobil << "/" << maxSlotMobil << " slot\n";
        cout << "  Tersedia  : " << (maxSlotMobil - slotMobil) << " slot\n";
        cout << "-------------------------------------------\n";
    }

    // Fungsi laporan pendapatan
    void laporanPendapatan() {
        cout << "\n+----------------------------------------+\n";
        cout << "¦         LAPORAN PENDAPATAN             ¦\n";
        cout << "+----------------------------------------+\n";
        cout << "Total Pendapatan: Rp " << fixed << setprecision(0) << totalPendapatan << "\n";
        cout << "-------------------------------------------\n";
    }

    // Fungsi lihat daftar kendaraan
    void lihatDaftarKendaraan() {
        cout << "\n+----------------------------------------------------------------+\n";
        cout << "¦              DAFTAR KENDARAAN DI PARKIR                        ¦\n";
        cout << "+----------------------------------------------------------------+\n";

        if (slotMotor == 0 && slotMobil == 0) {
            cout << "Tidak ada kendaraan di parkir.\n";
            return;
        }

        cout << left << setw(15) << "No. Plat"
             << setw(10) << "Jenis"
             << setw(8) << "Slot"
             << setw(25) << "Waktu Masuk" << "\n";
        cout << "-----------------------------------------------------------------\n";

        for (size_t i = 0; i < daftarKendaraan.size(); i++) {
            if (!daftarKendaraan[i].statusKeluar) {
                string waktu = ctime(&daftarKendaraan[i].waktuMasuk);
                waktu = waktu.substr(0, waktu.length() - 1);

                cout << left << setw(15) << daftarKendaraan[i].noPlat
                     << setw(10) << daftarKendaraan[i].jenis
                     << setw(8) << daftarKendaraan[i].slotParkir
                     << setw(25) << waktu << "\n";
            }
        }
        cout << "-----------------------------------------------------------------\n";
    }
};

// Fungsi utama
int main() {
    // Inisialisasi sistem parkir
    // (maxMotor, maxMobil, tarifMotor, tarifMobil)
    SistemParkir parkir(20, 15, 2000, 5000);

    int pilihan;

    cout << "+----------------------------------------+\n";
    cout << "¦   SELAMAT DATANG DI SISTEM PARKIR ¦\n";
    cout << "¦   DI SUSUN OLEH KELOMPOK 4¦\n";
    cout << "¦   NAMA ANGGOTA : IMAM AMRIZAL NIM 250935730051¦\n";
    cout << "+----------------------------------------+\n";

    do {
        parkir.tampilkanMenu();
        cin >> pilihan;

        // Validasi input
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\n? Input tidak valid! Masukkan angka 1-6.\n";
            continue;
        }

        switch (pilihan) {
            case 1:
                parkir.masukParkir();
                break;
            case 2:
                parkir.keluarParkir();
                break;
            case 3:
                parkir.lihatStatus();
                break;
            case 4:
                parkir.laporanPendapatan();
                break;
            case 5:
                parkir.lihatDaftarKendaraan();
                break;
            case 6:
                cout << "\n+----------------------------------------+\n";
                cout << "¦  Terima kasih telah menggunakan sistem ¦\n";
                cout << "¦            parkir kami!                ¦\n";
                cout << "+----------------------------------------+\n";
                break;
            default:
                cout << "\n? Pilihan tidak valid! Silakan pilih 1-6.\n";
        }
    } while (pilihan != 6);

    return 0;
}
