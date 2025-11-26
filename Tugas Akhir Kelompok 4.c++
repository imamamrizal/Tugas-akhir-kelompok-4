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
        cout << "¦ SISTEM PARKIR MINI KELOMPOK 4 - MENU UTAMA ¦\n";
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
